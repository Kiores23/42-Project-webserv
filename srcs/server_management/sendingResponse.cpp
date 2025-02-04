#include "sendingResponse.hpp"
#include "../socketio/SocketIO.hpp"


int sendingResponse(HttpRequestContext *hrc, int clientSocket)
{
	ClientResponse &clientResponse = hrc->getClientResponse();
	return SocketIO::getInstance()->sendResponse(clientSocket, &clientResponse);
}

void switchToSendingResponse(HttpRequestContext *hrc, const std::string &response)
{
	hrc->getClientResponse().setData(response);
	hrc->getClientResponse().setIsAvailable(true);
}
