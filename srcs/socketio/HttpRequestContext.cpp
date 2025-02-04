#include "HttpRequestContext.hpp"

HttpRequestContext::HttpRequestContext() :
		_clientSocket(-1),
		_clientRequest(_clientSocket),
		_clientResponse(_clientSocket),
		_request(new Request),
		_locationConfig(new LocationConfig),
		_serverConfig(new ServerConfig),
		_cgiHandler(new CGIHandler),
		_isCGI(false, false),
		_cgiPid(-1), _cgiFd(-1),
		_bodyRequired(false)
{

}

HttpRequestContext::HttpRequestContext(int clientSocket) :
		_clientSocket(clientSocket),
		_clientRequest(clientSocket),
		_clientResponse(clientSocket),
		_request(new Request),
		_locationConfig(new LocationConfig),
		_serverConfig(new ServerConfig),
		_cgiHandler(new CGIHandler),
		_isCGI(false, false),
		_cgiPid(-1), _cgiFd(-1),
		_bodyRequired(false)
{
}

HttpRequestContext::HttpRequestContext(const HttpRequestContext &original) :
		_clientSocket(original._clientSocket),
		_clientRequest(original._clientSocket),
		_clientResponse(original._clientSocket),
		_request(new Request),
		_locationConfig(new LocationConfig),
		_serverConfig(new ServerConfig),
		_cgiHandler(new CGIHandler),
		_isCGI(false, false),
		_cgiPid(-1), _cgiFd(-1),
		_bodyRequired(false)
{
	*this = original;
}

HttpRequestContext &HttpRequestContext::operator=(const HttpRequestContext &original)
{
	if (this == &original)
		return *this;
	_clientSocket = original._clientSocket;
	_clientRequest = original._clientRequest;
	_clientResponse = original._clientResponse;

	*_request = *original._request;
	*_cgiHandler = *original._cgiHandler;
	*_serverConfig = *original._serverConfig;
	*_locationConfig = *original._locationConfig;

	_isCGI.first = original._isCGI.first;
	_isCGI.second = original._isCGI.second;
	_cgiPid = original._cgiPid;
	_cgiFd = original._cgiFd;
	_bodyRequired = original._bodyRequired;
	return *this;
}

HttpRequestContext::~HttpRequestContext()
{
	delete _request;
	delete _cgiHandler;
	delete _serverConfig;
	delete _locationConfig;
}

bool HttpRequestContext::isCgiChecked() const
{
	return _isCGI.first;
}

/**
 * Call only if isCgiChecked returns true
 * @return
 */
bool HttpRequestContext::isRequestForCgi() const
{
	return (_isCGI.second);
}

void HttpRequestContext::setIsCgi(std::pair<bool, bool> isCgi)
{
	_isCGI.first = isCgi.first;
	_isCGI.second = isCgi.second;
}

int HttpRequestContext::getCgiPid() const
{
	return _cgiPid;
}

void HttpRequestContext::setCgiPid(int cgiPid)
{
	_cgiPid = cgiPid;
}

int HttpRequestContext::getCgiFd() const
{
	return _cgiFd;
}

void HttpRequestContext::setCgiFd(int cgiFd)
{
	_cgiFd = cgiFd;
}

int HttpRequestContext::getClientSocket() const
{
	return _clientSocket;
}

ClientRequest &HttpRequestContext::getClientRequest()
{
	return _clientRequest;
}

ClientResponse &HttpRequestContext::getClientResponse()
{
	return _clientResponse;
}

Request *HttpRequestContext::getRequest() const
{
	return _request;
}

LocationConfig *HttpRequestContext::getLocationConfig() const
{
	return _locationConfig;
}

void HttpRequestContext::setLocationConfig(LocationConfig *locationConfig)
{
	*_locationConfig = *locationConfig;
}

ServerConfig *HttpRequestContext::getServerConfig() const
{
	return _serverConfig;
}

void HttpRequestContext::setServerConfig(ServerConfig *serverConfig)
{
	*_serverConfig = *serverConfig;
}

const std::string &HttpRequestContext::getRequestTarget() const
{
	return _requestTarget;
}

void HttpRequestContext::setRequestTarget(const std::string &requestTarget)
{
	_requestTarget = requestTarget;
}

CGIHandler &HttpRequestContext::getCgiHandler()
{
	return *_cgiHandler;
}

bool HttpRequestContext::isBodyRequired() const
{
	return _bodyRequired;
}

void HttpRequestContext::setBodyRequired(bool bodyRequired)
{
	HttpRequestContext::_bodyRequired = bodyRequired;
}
