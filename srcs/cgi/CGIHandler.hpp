#ifndef WEBSERV_CGIHANDLER_HPP
# define WEBSERV_CGIHANDLER_HPP
# include <string>
# include <cstdlib>
# include <cstring>
# include <iostream>
# include <sys/wait.h>
# include <unistd.h>

class Request;
class HttpRequestContext;

#include "../http/Request.hpp"
#include "CGIEnvironment.hpp"
#include "../socketio/HttpRequestContext.hpp"
#include <map>

#define CGI_TIMEOUT 5
#define CHILD_FAIL (-1)
#define CHILD_TERMINATED 0
#define CHILD_CONTINUE 1

class CGIHandler
{
private:
	std::string _scriptPathInfo;
	std::string _relativeScriptPath;

	Request *_request;
	ClientResponse *_clientResponse;
	int _clientSocket;
	LocationConfig *_locationConfig;
	std::map<std::string, std::string> _env;
	char *const *_envp;
	int _outputPipe[2];
	pid_t _childPid;
	bool _bodyRequired;
	void childProcess();
public:
	CGIHandler();
	CGIHandler(const CGIHandler& original);
	CGIHandler &operator=(const CGIHandler& original);
	~CGIHandler();
	void readCGIResponse(HttpRequestContext *hrc);
	static bool isSupportedScript(const std::string &targetFile, LocationConfig *locationConfig);
	void startCGIScriptCommunication(HttpRequestContext *hrc);
	int checkChildFinished(bool isClientDisconnected);
	void setRelativeScriptPath(const std::string &relativeScriptPath);
	const std::string &getScriptPathInfo() const;
	void setScriptPathInfo(const std::string &scriptPathInfo);
	const std::string &getRelativeScriptPath() const;
	const Request &getRequest() const;
	void setRequest(Request *request);
	void setClientResponse(ClientResponse *clientResponse);
	int getClientSocket() const;
	void setClientSocket(int clientSocket);
	void setLocationConfig(LocationConfig *locationConfig);
	std::map<std::string, std::string> &getEnv();
	void setBodyRequired(bool bodyRequired);
	void killChild(bool isClientDisconnected);
	bool terminateChild(int &childStatus) const;
	void cleanChild(bool isClientDisconnected);
};

#endif // WEBSERV_CGIHANDLER_HPP
