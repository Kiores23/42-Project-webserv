#include "Request.hpp"

using std::string;
using std::map;
using std::list;

Request::Request() : _keepAlive(0), _timeKeepAlive(0)
{
}

Request::Request(const Request &original) : _keepAlive(0), _timeKeepAlive(0)
{
	*this = original;
}

Request &Request::operator=(const Request &original)
{
	if (this == &original)
		return *this;
	if (!_body.empty())
	{
		for (std::list<Body *>::iterator it = _body.begin(); it != _body.end(); ++it)
		{
			delete *it;
		}
		_body.clear();
	}
	if (!original._body.empty())
	{
		for (std::list<Body *>::const_iterator it = original._body.begin(); it != original._body.end(); it++)
		{
			this->_body.push_back(new Body(**it));
		}
	}
	_header = original._header;
	_target = original._target;
	_method = original._method;
	_httpVersion = original._httpVersion;
	_keepAlive = original._keepAlive;
	return *this;
}

Request::~Request()
{
	if (!_body.empty())
	{
		std::list<Body *>::iterator it;
		for (it = _body.begin(); it != _body.end(); it++)
		{
			if (*it)
				delete *it;
		}
	}
}

void Request::parsingHTTP(std::string requestStr, LocationConfig **locationConfig, ServerConfig &serverConfig)
{
	parsStartLine(requestStr);
	*locationConfig = MotherWebserv::getInstance()->getLocationConfigFromSocket(serverConfig, _target);
	parsHeader(requestStr);
	parsKeepAlive();
	requestStr = &requestStr[2];
	parsBody(requestStr);
}

void Request::parsStartLine(std::string &requestStr)
{
	size_t i;
	size_t j;
	for (j = 0; requestStr[j] && requestStr[j] != ' ' && requestStr[j] != '\r'; j++);
	_method = requestStr.substr(0, j);
	if (!requestStr[j] || requestStr[j] == '\r')
		throw logForError400("Wrong client request: method: \"" + _method + "\"" + "\n" + requestStr);
	i = j + 1;
	for (j = i; requestStr[j] && requestStr[j] != ' ' && requestStr[j] != '\r'; j++);
	_target = decodeURL(requestStr.substr(i, j - i));
	if (!requestStr[j] || requestStr[j] == '\r' || _target.size() == 0)
		throw logForError400("Wrong client request: target: \"" + _target + "\"");
	i = j + 1;
	for (j = i; requestStr[j] && requestStr[j] != ' ' && requestStr[j] != '\r'; j++);
	_httpVersion = requestStr.substr(i, j - i);
	if (!requestStr[j] || requestStr[j] == ' ' || requestStr[j] != '\r' || _httpVersion.size() == 0)
		throw logForError400("Wrong client request: version http");
	requestStr = &requestStr[j + 2];
}

void Request::parsHeader(std::string &requestStr)
{
	std::string key;
	std::string value;
	size_t i;
	size_t j;
	if (!requestStr[0] || requestStr[1] == '\n')
		return;
	for (j = 0; requestStr[j] && requestStr[j] != ':' && requestStr[j] != '\r'; j++);
	key = requestStr.substr(0, j);
	if (requestStr[j] != ':' || requestStr[j + 1] != ' ')
		throw logForError400("Wrong client request: header key");
	i = j + 2;
    for (j = i; requestStr[j] && requestStr[j] != '\r'; j++) {}
	value = requestStr.substr(i, j - i);
	if (value[0] == ' ')
		_header[key].push_back(value.substr(1, value.size() - 1));
	else
		_header[key].push_back(value);
	if (i - j == 0)
		throw logForError400("Wrong client request: header value");
	requestStr = &requestStr[j + 2];
	parsHeader(requestStr);
}

void Request::parsKeepAlive()
{
	if (getHeaderBack("Connection") == "keep-alive")
	{
		_keepAlive = KEEP_ALIVE;
		if (getHeaderBack("Keep-Alive").substr(0, 8) == "timeout=")
		{
			for (size_t i = 8; getHeaderBack("Keep-Alive")[i]; i++)
			{
				if (!std::isdigit(getHeaderBack("Keep-Alive")[i]))
					throw logForError400("Keep-Alive: " + getHeaderBack("Keep-Alive"));
			}
			_timeKeepAlive = std::atoi(getHeaderBack("Keep-Alive").substr(8, getHeaderBack("Keep-Alive").size() - 8).c_str());
			if (_timeKeepAlive == 0)
				_keepAlive = CLOSE;
		}
	}
	else if (getHeaderBack("Connection") == "close")
		_keepAlive = CLOSE;
}

void Request::parsBody(string &requestStr)
{
	map<string, std::list<std::string> >::const_iterator it;
	if (_method == "POST")
	{
		it = _header.find("Content-Type");
		if (it != _header.end() && !it->second.empty() && it->second.back().size() > 19
			&& it->second.back().substr(0, 19) == "multipart/form-data")
		{
			string boundaryStr = &it->second.back()[19];
			string boundary;
			size_t i;
			size_t j;

			for (i = 0; boundaryStr[i] && boundaryStr[i] != '='; i++);
			if (boundaryStr[i++] != '=')
				throw logForError400("Wrong client request: Content-Type");
			for (j = 0; boundaryStr[i + j]; j++);
			boundary = boundaryStr.substr(i, j);
			parsMultipart(requestStr, boundary);
			return;
		}
	}
	Body *body = new Body(requestStr);
	_body.push_back(body);
	if (getHeaderBack("Content-Type") == "application/x-www-form-urlencoded")
		_bodyForm.parseBodyForm(getBody(0)->getValue());
}

void Request::parsMultipart(string &requestStr, const string &boundary)
{
	if (requestStr.substr(0, 2 + boundary.size() + 2) != "--" + boundary + "\r\n")
		throw logForError400(
				"Wrong client request: Body content (multipart) 1: \"" + requestStr.substr(0, 2 + boundary.size() + 1) +
				"\"");
	requestStr = &requestStr[4 + boundary.size()];

	Body *body = new Body();
	_body.push_back(body);
	parsBodyParams(requestStr);
	requestStr = &requestStr[1];
	for (size_t i = 0; requestStr[i]; i++)
	{
		if (requestStr[i] == '\n' && requestStr[i + 1] == '-' && requestStr[i + 2] == '-')
		{
			i++;
			if (requestStr.substr(i, 2 + boundary.size() + 2) == "--" + boundary + "\r\n")
			{
				body->setValue(requestStr.substr(0, i - 1));
				if (body->getParams("Content-Type") == "application/x-www-form-urlencoded")
					_bodyForm.parseBodyForm(getBody(0)->getValue());
				requestStr = &requestStr[i];
				parsMultipart(requestStr, boundary);
				return;
			}
			else if (requestStr.substr(i, 2 + boundary.size() + 2) == "--" + boundary + "--")
			{
				body->setValue(requestStr.substr(0, i - 1));
				if (body->getParams("Content-Type") == "application/x-www-form-urlencoded")
					_bodyForm.parseBodyForm(getBody(0)->getValue());
				return;
			}
		}
	}
	throw logForError400("Wrong client request: Body content (multipart) 4");
}

#include <cstring>

void Request::parsBodyParams(std::string &requestStr)
{
	std::string key;
	size_t i;
	size_t j;
	if (!requestStr[0] || requestStr[0] == '\n')
		return;
	for (j = 0; requestStr[j] && requestStr[j] != ':' && requestStr[j] != '\n'; j++);
	if (requestStr[j] != ':')
		throw logForError400("Wrong client request: Body content (multipart) 2");
	key = requestStr.substr(0, j);
	if (requestStr[j] == ' ')
		j++;
	i = j + 1;
	for (j = i; requestStr[j] && requestStr[j] != '\n'; j++);
	_body.back()->addParams(key, requestStr.substr(i, j - i));
	if (i - j == 0)
		throw logForError400("Wrong client request: Body content (multipart) 3");
	requestStr = &requestStr[j + 2];
	if (key == "Content-Disposition")
	{
		for (i = 0; _body.back()->getParams(key).size() > 10 && _body.back()->getParams(key)[i + 10]
					&& std::strncmp(&_body.back()->getParams(key)[i], "filename=\"", 10); i++);
		if (_body.back()->getParams(key).size() > 10 && _body.back()->getParams(key)[i + 10])
		{
			_body.back()->setFileName(
					_body.back()->getParams(key).substr(i + 10, _body.back()->getParams(key).size() - i - 12));
		}
	}
	parsBodyParams(requestStr);
}

// Get

string Request::getMethod() const
{
	return _method;
}

string Request::getTarget() const
{
	return _target;
}

string Request::getHttVersion() const
{
	return _httpVersion;
}

string Request::getHeaderBack(const string &key) const
{
	map<string, std::list<std::string> >::const_iterator it;
	it = _header.find(key);
	if (it == _header.end())
		return ("");
	return it->second.back();
}

std::list<std::string> Request::getHeader(const string &key) const
{
	map<string, std::list<std::string> >::const_iterator it;
	it = _header.find(key);
	if (it == _header.end())
		return std::list<std::string>();
	return it->second;
}

/**
 *
 * @param i
 * @return The intended body line or NULL if not found
 */
Body *Request::getBody(int i) const
{
	int j = 0;
	list<Body *>::const_iterator it;
	for (it = _body.begin(); it != _body.end() && j <= i; it++)
	{
		if (i == j)
			return *it;
		j++;
	}
	return NULL;
}

size_t Request::getBodySize() const
{
	size_t bodySize = 0;
	int i = 0;
	Body *body;

	body = getBody(i);
	while (body != NULL)
	{
		bodySize += body->getValue().size();
		++i;
		body = getBody(i);
	}
	return bodySize;
}

int Request::getKeepAlive() const
{
	return _keepAlive;
}

int Request::getTimeKeepAlive() const
{
	return _timeKeepAlive;
}

//Is

std::string Request::getAuthScheme() const
{
	std::string authHeader;

	authHeader = getHeaderBack("Authorization");
	size_t spacePos = authHeader.find(' ');
	if (spacePos != std::string::npos)
		return authHeader.substr(0, spacePos);
	return "";
}

/**
 * https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Authorization
 * https://datatracker.ietf.org/doc/html/rfc3875#section-4.1.11
 */
std::string Request::getAuthUserId() const
{
	std::string authHeader;
	string authMethod;

	authMethod = getAuthScheme();
	authHeader = getHeaderBack("Authorization");
	size_t spacePos = authHeader.find(' ');
	if (spacePos != std::string::npos)
	{
		if (equalsIgnoreCase(authMethod, "Basic"))
			return authHeader.substr(spacePos, authHeader.size());
	}
	return "";
}

int logForError400(const std::string &logMessage)
{
	Logger::log(ERROR, "400 Bad Request: " + logMessage);
	return 400;
}

std::string &Request::getRelativeTarget()
{
	return _relativeTarget;
}

void	Request::setRelativeTarget(const std::string &relativeTarget)
{
	_relativeTarget = relativeTarget;
}

BodyForm	*Request::getBodyForm()
{
	return &_bodyForm;
}
