#ifndef WEBSERV_HTTPREQUESTCONTEXT_HPP
#define WEBSERV_HTTPREQUESTCONTEXT_HPP

class CGIHandler;

#include "ClientRequest.hpp"
#include "ClientResponse.hpp"
#include "../config/ServerConfig.hpp"
#include "../config/LocationConfig.hpp"
#include "../http/Request.hpp"
#include "../cgi/CGIHandler.hpp"

class HttpRequestContext
{
private:
	int _clientSocket;
	ClientRequest _clientRequest;
	ClientResponse _clientResponse;

	Request *_request;
	LocationConfig *_locationConfig;
	ServerConfig *_serverConfig;
	CGIHandler *_cgiHandler;

	std::string _requestTarget;
	std::pair<bool, bool> _isCGI;
	int _cgiPid;
	int _cgiFd;
	bool _bodyRequired;
public:
	HttpRequestContext();
	explicit HttpRequestContext(int clientSocket);
	HttpRequestContext(const HttpRequestContext &original);
	HttpRequestContext &operator=(const HttpRequestContext &original);
	~HttpRequestContext();

	bool isCgiChecked() const;
	bool isRequestForCgi() const;
	void setIsCgi(std::pair<bool, bool> isCgi);

	int getCgiPid() const;
	void setCgiPid(int cgiPid);
	int getClientSocket() const;
	ClientRequest &getClientRequest();
	ClientResponse &getClientResponse();
	Request *getRequest() const;
	LocationConfig *getLocationConfig() const;
	void setLocationConfig(LocationConfig *locationConfig);
	ServerConfig *getServerConfig() const;
	void setServerConfig(ServerConfig *serverConfig);
	const std::string &getRequestTarget() const;
	void setRequestTarget(const std::string &requestTarget);
	CGIHandler &getCgiHandler();
	bool isBodyRequired() const;
	void setBodyRequired(bool bodyRequired);
	int getCgiFd() const;
	void setCgiFd(int cgiFd);
};


#endif //WEBSERV_HTTPREQUESTCONTEXT_HPP
