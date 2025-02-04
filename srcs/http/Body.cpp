#include "Body.hpp"

Body::Body()
{
}


Body::Body(std::string &body) : _body(body)
{
}

Body::Body(const Body &original)
		: _body(original._body), _params(original._params), _fileName(original._fileName)
{
}

Body::~Body()
{
}

const std::string &Body::getValue() const
{
	return _body;
}

const std::string &Body::getFileName() const
{
	return _fileName;
}

void Body::setValue(std::string body)
{
	_body = body;
}

void Body::setFileName(std::string fileName)
{
	_fileName = fileName;
}

std::string Body::getParams(const std::string &key) const
{
	std::map<std::string, std::string>::const_iterator f;
	f = _params.find(key);
	if (f == _params.end())
		return "";
	return f->second;
}

void Body::addParams(const std::string &key, const std::string &value)
{
	if (!key.empty() && !value.empty())
		_params[key] = value;
}
