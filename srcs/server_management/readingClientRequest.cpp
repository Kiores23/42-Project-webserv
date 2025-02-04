#include "readingClientRequest.hpp"
#include "../socketio/SocketIO.hpp"
#include "generatingResponse.hpp"

void readingClientRequest(int clientSocket)
{
	int readStatus;
	HttpRequestContext *hrc;

	if (SocketIO::getInstance()->needNewRequest(clientSocket))
	{
		hrc = SocketIO::getInstance()->addHttpRequestContext(clientSocket);
		readStatus = SocketIO::readContentFromFd(clientSocket, &(hrc->getClientRequest()), hrc);
		if (readStatus == R_CONTENT_TOO_LARGE || readStatus == R_BAD_REQUEST)
			return;
		if (readStatus == R_ERROR || readStatus == R_CLIENT_DISCONNECTED)
		{
			MotherWebserv::getInstance()->removeClientSocket(clientSocket);
			return;
		}
		if (hrc->getClientRequest().isAvailable())
		{
			startGeneratingResponse(hrc, clientSocket);
		}
		return;
	}
	else
	{
		hrc = SocketIO::getInstance()->getCurrentRequestHRC(clientSocket);
		if (hrc == NULL)
			return;
		readStatus = SocketIO::readContentFromFd(clientSocket, &(hrc->getClientRequest()), hrc);
        if (readStatus == R_CONTENT_TOO_LARGE || readStatus == R_BAD_REQUEST)
			return;
		if (readStatus == R_ERROR || readStatus == R_CLIENT_DISCONNECTED)
		{
			MotherWebserv::getInstance()->removeClientSocket(clientSocket);
			return;
		}
		if (hrc->getClientRequest().isAvailable())
		{
			startGeneratingResponse(hrc, clientSocket);
		}
		return;
	}
}
