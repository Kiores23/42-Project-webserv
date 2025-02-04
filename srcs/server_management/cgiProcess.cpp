#include "cgiProcess.hpp"
#include "sendingResponse.hpp"
#include "generatingResponse.hpp"
#include "../http/response.hpp"

void setCGIRelativePath(const HttpRequestContext *hrc, CGIHandler &cgiHandler)
{
	LocationConfig *	locationConfig = hrc->getLocationConfig();
	std::string			target = locationConfig->getDirectives().getRoot() + hrc->getRequestTarget();
	std::size_t pos = target.find('?');
	if (pos != std::string::npos)
		target = target.substr(0, pos);
	verifAccess(*locationConfig, target, hrc->getRequest()->getMethod(), true);
	cgiHandler.setRelativeScriptPath(target);
}

void startCGIProcess(HttpRequestContext *hrc)
{
    try
    {
        checkServerNames(*hrc->getLocationConfig(), *hrc->getRequest());
        checkMethodAllowed(*hrc->getLocationConfig(), *hrc->getRequest());
    } catch (int e)
    {
        std::string response = generateErrorResponse(
                hrc->getLocationConfig(), hrc->getServerConfig(), hrc->isBodyRequired(), e);
        switchToSendingResponse(hrc, response);
        return;
    }
    CGIHandler &cgiHandler = hrc->getCgiHandler();
	cgiHandler.setClientResponse(&(hrc->getClientResponse()));
	cgiHandler.setClientSocket(hrc->getClientSocket());
	cgiHandler.setRequest(hrc->getRequest());
	cgiHandler.setLocationConfig(hrc->getLocationConfig());
	cgiHandler.setBodyRequired(hrc->isBodyRequired());
	setCGIRelativePath(hrc, cgiHandler);
	cgiHandler.setScriptPathInfo(hrc->getRequest()->getTarget());

	try
	{
		cgiHandler.startCGIScriptCommunication(hrc);
	} catch (int e)
	{
		std::string response = generateErrorResponse(
				hrc->getLocationConfig(), hrc->getServerConfig(), hrc->isBodyRequired(), e);
		switchToSendingResponse(hrc, response);
	}
}

void readingCGIResponse(HttpRequestContext *hrc)
{
	CGIHandler &cgiHandler = hrc->getCgiHandler();
	cgiHandler.readCGIResponse(hrc);
}
