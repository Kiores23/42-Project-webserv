#include "CGIEnvironment.hpp"

CGIEnvironment::CGIEnvironment(CGIHandler *cgiHandler)
		: _env(cgiHandler->getEnv()), _request(cgiHandler->getRequest())
{
	_clientSocket = cgiHandler->getClientSocket();
	_scriptPath = cgiHandler->getRelativeScriptPath();
	_scriptPathInfo = cgiHandler->getScriptPathInfo();
}

/**
 * https://datatracker.ietf.org/doc/html/rfc3875#section-4
 */
void CGIEnvironment::setEnvForCurrentRequest()
{
	_env["AUTH_TYPE"] = getAuthType();
	_env["CONTENT_LENGTH"] = getContentLength();
	_env["CONTENT_TYPE"] = getContentType();
	_env["GATEWAY_INTERFACE"] = getGatewayInterface();
	_env["PATH_INFO"] = getPathInfo();
	_env["PATH_TRANSLATED"] = getPathTranslated();
	_env["QUERY_STRING"] = getQueryString();
	_env["REMOTE_ADDR"] = getRemoteAddr();
	_env["REMOTE_HOST"] = getRemoteHost();
	_env["REMOTE_USER"] = getRemoteUser();
	_env["REQUEST_METHOD"] = getRequestMethod();
	_env["SCRIPT_NAME"] = getScriptName();
	_env["SCRIPT_FILENAME"] = getScriptName();
	_env["SERVER_NAME"] = getServerName();
	_env["SERVER_PORT"] = getServerPort();
	_env["SERVER_PROTOCOL"] = getServerProtocol();
	_env["SERVER_SOFTWARE"] = getServerSoftware();
	_env["REDIRECT_STATUS"] = getRedirectStatus();
	_env["HTTP_COOKIE"] = getHttpCookie();
}

std::string CGIEnvironment::getRedirectStatus() const
{
	return convertToString(200);
}

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Server
std::string CGIEnvironment::getServerSoftware() const
{
	std::string server = WEBSERV;
	return server;
}

std::string CGIEnvironment::getServerProtocol() const
{
	return "HTTP/1.1";
}

std::string CGIEnvironment::getServerPort() const
{
	std::string port;
	try
	{
		port = convertToString(Socket::getLocalPort(_clientSocket));
	} catch (int e)
	{
		port = "";
	}
	return (port);
}

std::string CGIEnvironment::getServerName() const
{
	std::string address;
	try
	{
		address = Socket::getLocalIpAddress(_clientSocket);
	} catch (int e)
	{
		address = "";
	}
	return (address);
}

std::string CGIEnvironment::getScriptName() const
{
	return _scriptPath;
}

std::string CGIEnvironment::getRequestMethod() const
{
	return _request.getMethod();
}

std::string CGIEnvironment::getRemoteUser() const
{
	return _request.getAuthUserId();
}

std::string CGIEnvironment::getRemoteHost() const
{
	std::string remoteHost;
	try
	{
		remoteHost = Socket::getRemoteIpAddress(_clientSocket);
	} catch (const std::exception &e)
	{
		remoteHost = std::string();
	}
	return remoteHost;
}

/**
 * Get the network address of the client sending the request to the server
 */
std::string CGIEnvironment::getRemoteAddr() const
{
	std::string remoteAddress;
	try
	{
		remoteAddress = Socket::getLocalIpAddress(_clientSocket);
	} catch (const std::exception &e)
	{
		remoteAddress = "";
	}
	return remoteAddress;
}

std::string CGIEnvironment::getQueryString() const
{
	if (_request.getMethod() == "GET")
	{
		std::size_t queryStart = _scriptPathInfo.find('?');

		if (queryStart != std::string::npos)
		{
			return (_scriptPathInfo.substr(queryStart + 1));
		}
	}
	if (_request.getMethod() == "POST")
	{
		Body *body = _request.getBody(0);
		if (body == NULL)
			return "";
		return body->getValue();
	}
	return "";
}

std::string CGIEnvironment::getPathTranslated() const
{
	return _scriptPathInfo;
}

std::string CGIEnvironment::getPathInfo() const
{
	return _scriptPathInfo;
}

std::string CGIEnvironment::getGatewayInterface() const
{
	return "CGI/1.1";
}

std::string CGIEnvironment::getContentType() const
{
	return _request.getHeaderBack("Content-Type");
}

std::string CGIEnvironment::getContentLength() const
{
	return _request.getHeaderBack("Content-Length");
}

std::string CGIEnvironment::getAuthType() const
{
	return _request.getAuthScheme();
}

std::string CGIEnvironment::getHttpCookie() const
{
	return _request.getHeaderBack("Cookie");
}
