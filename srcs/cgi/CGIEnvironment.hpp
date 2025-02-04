#ifndef WEBSERV_CGIENVIRONMENT_HPP
#define WEBSERV_CGIENVIRONMENT_HPP

#include <string>
#include <map>
#include "CGIHandler.hpp"
#include "../webserv.hpp"
#include "../networking/Socket.hpp"
#include "../http/Request.hpp"

class CGIHandler;
class Request;

class CGIEnvironment
{
public:
	explicit CGIEnvironment(CGIHandler *cgiHandler);
	void setEnvForCurrentRequest();
private:
	std::map<std::string, std::string> &_env;
	int _clientSocket;
	const Request &_request;
	std::string _scriptPath;
	std::string _scriptPathInfo;
	std::string getServerSoftware() const;
	std::string getServerProtocol() const;
	std::string getServerPort() const;
	std::string getServerName() const;
	std::string getScriptName() const;
	std::string getRequestMethod() const;
	std::string getRemoteUser() const;
	std::string getRemoteHost() const;
	std::string getRemoteAddr() const;
	std::string getQueryString() const;
	std::string getPathTranslated() const;
	std::string getPathInfo() const;
	std::string getGatewayInterface() const;
	std::string getContentType() const;
	std::string getContentLength() const;
	std::string getAuthType() const;
	std::string getRedirectStatus() const;
	std::string getHttpCookie() const;
};


#endif //WEBSERV_CGIENVIRONMENT_HPP
