#include "generatingResponse.hpp"
#include "sendingResponse.hpp"
#include "cgiProcess.hpp"
#include "../http/response.hpp"

static std::string setConfigTarget(HttpRequestContext *hrc, bool &bodyRequired)
{
	Request tempRequest;
	std::string requestStr = hrc->getClientRequest().getData();
	tempRequest.parsStartLine(requestStr);
	bodyRequired = (tempRequest.getMethod() != "HEAD" && tempRequest.getMethod() != "DELETE");
	std::string target = tempRequest.getTarget();
	hrc->setRequestTarget(target);
	return target;
}

static void getConfigOptions(HttpRequestContext *hrc)
{
	MotherWebserv *mw = MotherWebserv::getInstance();
	std::string target = hrc->getRequestTarget();
	int clientSocket = hrc->getClientSocket();
	ServerConfig &serverConfig = mw->getServerConfigFromSocket(clientSocket);
	hrc->setServerConfig(&serverConfig);
	LocationConfig *locationConfig = mw->getLocationConfigFromSocket(serverConfig, target);
	hrc->setLocationConfig(locationConfig);
}

static std::string findRelativeTarget(const HttpRequestContext *hrc, LocationConfig *locationConfig)
{
	const std::string &target = hrc->getRequestTarget();
	std::string cgiTarget = locationConfig->getDirectives().getRoot() + target;
	std::size_t pos = cgiTarget.find('?');
	if (pos != std::string::npos)
		cgiTarget = cgiTarget.substr(0, pos);
	return cgiTarget;
}

static bool checkIsCGI(HttpRequestContext *hrc)
{
	LocationConfig *locationConfig = hrc->getLocationConfig();
	std::string cgiTarget = findRelativeTarget(hrc, locationConfig);
	setIndexRoot(*locationConfig, cgiTarget);
	hrc->getRequest()->setRelativeTarget(cgiTarget);
	if (CGIHandler::isSupportedScript(cgiTarget, hrc->getLocationConfig()))
		hrc->setIsCgi(std::pair<bool, bool>(true, true));
	else
		hrc->setIsCgi(std::pair<bool, bool>(true, false));
	return true;
}

std::string generateErrorResponse(LocationConfig *locationConfig, ServerConfig *serverConfig, bool bodyRequired, int e)
{
    std::string response;

    if (locationConfig != NULL) {
        response = generateResponse(*locationConfig, e, bodyRequired);
    }
    else if (serverConfig != NULL) {
        response = generateResponse(*serverConfig, e, bodyRequired);
    }
    else {
        response = generateError500(true);
    }
    return response;
}

void generateStandardResponse(HttpRequestContext &hrc)
{
	std::string requestStr = hrc.getClientRequest().getData();
	Request *request = hrc.getRequest();
	if (requestStr.empty())
		return;

	std::string response;

	LocationConfig *locationConfig = hrc.getLocationConfig();
	ServerConfig *serverConfig = hrc.getServerConfig();
	try
	{
		int redir = locationConfig->getDirectives().getHttpRedirection().first;
		if (redir / 100 == 3)
			throw redir;
		response = generateResponse(request, *locationConfig, hrc.isBodyRequired());
	}
	catch (const int e)
	{
		response = generateErrorResponse(locationConfig, serverConfig, hrc.isBodyRequired(), e);
	}
	switchToSendingResponse(&hrc, response);
}

void switchToGeneratingResponse(HttpRequestContext *hrc)
{
	bool bodyRequired = false;
	setConfigTarget(hrc, bodyRequired);
	hrc->setBodyRequired(bodyRequired);
	getConfigOptions(hrc);
	checkIsCGI(hrc);
}

void setKeepAliveState(const HttpRequestContext *hrc, int clientSocket)
{
	if (hrc->getRequest()->getKeepAlive() == KEEP_ALIVE)
	{
		MotherWebserv::getInstance()->getKeepAliveMap().add(clientSocket, hrc->getRequest()->getTimeKeepAlive());
	}
	else if (hrc->getRequest()->getKeepAlive() != CLOSE)
		MotherWebserv::getInstance()->getKeepAliveMap().add(clientSocket, 3);
}

void startGeneratingResponse(HttpRequestContext *hrc, int clientSocket)
{
	LocationConfig *locationConfig;
	ServerConfig *serverConfig;
	try
	{
		switchToGeneratingResponse(hrc);
		std::string requestStr = hrc->getClientRequest().getData();
		locationConfig = hrc->getLocationConfig();
		serverConfig = hrc->getServerConfig();
		hrc->getRequest()->parsingHTTP(requestStr, &locationConfig, *serverConfig);
		setKeepAliveState(hrc, clientSocket);

        if (locationConfig->getDirectives().getHttpRedirection().first == 0)
		    verifAccess(*locationConfig, hrc->getRequest()->getRelativeTarget(), hrc->getRequest()->getMethod(), hrc->isRequestForCgi());
	} catch (int e)
	{
		std::string response = generateErrorResponse(locationConfig, serverConfig, hrc->isBodyRequired(), e);
		switchToSendingResponse(hrc, response);
		return;
	}
	const std::string& method = hrc->getRequest()->getMethod();
	if (hrc->isRequestForCgi() && method != "DELETE")
		startCGIProcess(hrc);
	else
		generateStandardResponse(*hrc);
}
