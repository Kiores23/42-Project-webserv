#include "SocketIO.hpp"
#include "../server_management/generatingResponse.hpp"
#include "../server_management/sendingResponse.hpp"
#include "../http/response.hpp"
#include <sys/socket.h>

SocketIO *SocketIO::_instance = NULL;
bool SocketIO::_isDeconnected = false;

SocketIO::SocketIO()
{
}

SocketIO *SocketIO::getInstance()
{
	if (_instance == NULL)
		_instance = new SocketIO();
	return _instance;
}

HttpRequestContext *SocketIO::addHttpRequestContext(int clientSocket)
{
	HttpRequestContext newHRC(clientSocket);
	_clientHttpRequestContexts[clientSocket].push_back(newHRC);
	HttpRequestContext &addedHRC = _clientHttpRequestContexts[clientSocket].back();
	HttpRequestContext *ptr = &addedHRC;
	return ptr;
}

bool SocketIO::removeCurrentResponse(int clientSocket)
{
	if (_clientHttpRequestContexts.empty())
		return false;
	if (_clientHttpRequestContexts.find(clientSocket) != _clientHttpRequestContexts.end())
	{
		_clientHttpRequestContexts[clientSocket].pop_front();
		return true;
	}
	return false;
}

bool SocketIO::checkIsPresent(int clientSocket, HttpRequestContext *hrc)
{
	if (hrc == NULL)
		return false;
	if (_clientHttpRequestContexts.empty())
		return false;
	std::map<int, std::list<HttpRequestContext> >::iterator mapIt = _clientHttpRequestContexts.begin();
	for (; mapIt != _clientHttpRequestContexts.end(); mapIt++)
	{
		std::list<HttpRequestContext>::iterator hrcIt = mapIt->second.begin();
		for (; hrcIt != mapIt->second.end(); hrcIt++)
		{
			if (hrcIt->getCgiFd() != -1 && hrcIt->getCgiFd() == clientSocket)
				return true;
		}
	}
	return false;
}

bool SocketIO::hasNoPendingRequest(int clientSocket)
{
	if (_clientHttpRequestContexts.empty())
		return (true);
	if (_clientHttpRequestContexts.find(clientSocket) == _clientHttpRequestContexts.end())
		return (true);
	return (_clientHttpRequestContexts[clientSocket].empty());
}

int SocketIO::sendResponse(int clientSocket, ClientResponse *clientResponse)
{
	std::string *responseString = &(clientResponse->getData());

	const char *toSend = responseString->c_str();
	size_t toSendSize = responseString->size();
	ssize_t sent = send(clientSocket, toSend, toSendSize, MSG_DONTWAIT);
	if (sent == -1)
	{
		if (SocketIO::isDeconnected())
		{
			SocketIO::setIsDeconnected(false);
		}
		return S_ERROR;
	}
	if (sent == 0)
	{
		SocketIO::setIsDeconnected(true);
		return S_CLIENT_DISCONNECTED;
	}
	else
	{
		size_t posSent = static_cast<size_t>(sent);
		clientResponse->setData(responseString->substr(posSent));
		if (responseString->empty())
		{
			return S_FINISHED_SENDING;
		}
	}
	return S_CONTINUE;
}

bool SocketIO::needNewRequest(int clientSocket)
{
	if (_clientHttpRequestContexts[clientSocket].empty()
		|| _clientHttpRequestContexts.find(clientSocket) == _clientHttpRequestContexts.end())
	{
		return true;
	}
	ClientRequest *clientRequest = &(_clientHttpRequestContexts[clientSocket].back().getClientRequest());
	return clientRequest->isAvailable();
}

HttpRequestContext *SocketIO::getCurrentRequestHRC(int clientSocket)
{
	if (_clientHttpRequestContexts[clientSocket].empty()
		|| _clientHttpRequestContexts.find(clientSocket) == _clientHttpRequestContexts.end())
	{
		return NULL;
	}
	HttpRequestContext *hrc = &(_clientHttpRequestContexts[clientSocket].back());
	return hrc;
}

HttpRequestContext *SocketIO::getCurrentResponseHRC(int clientSocket)
{
	if (_clientHttpRequestContexts.empty()
		|| _clientHttpRequestContexts.find(clientSocket) == _clientHttpRequestContexts.end()
		|| _clientHttpRequestContexts[clientSocket].empty())
	{
		return NULL;
	}
	HttpRequestContext *hrc = &(_clientHttpRequestContexts[clientSocket].front());
	return hrc;
}

void SocketIO::removeSocketRequests(int clientSocket)
{
	_clientHttpRequestContexts.erase(clientSocket);
}

ssize_t parseContentLength(const std::string &requestStr, bool isCgi)
{
	ssize_t contentLength = -1;
	size_t pos = requestStr.find("Content-Length: ");
	if (pos != std::string::npos)
	{
		size_t start = pos + strlen("Content-Length: ");
		size_t end = requestStr.find("\r\n", start);
		std::istringstream iss(requestStr.substr(start, end - start));
		iss >> contentLength;
	}
	else if (not isCgi)
		contentLength = 0;
	return contentLength;
}

bool isEndBody(const std::string &requestStr, size_t contentLength)
{
	std::string endHeader = "\r\n\r\n";
	return (requestStr.length() >= (contentLength + requestStr.find(endHeader) + endHeader.size()));
}

int SocketIO::readContentFromFd(int fd, ClientData *clientData, HttpRequestContext *hrc)
{
	bool isCgi = (hrc == NULL);

	if (clientData == NULL)
		return false;
	char buffer[BUFSIZ];
	ssize_t bytesRead;
	bytesRead = readData(fd, buffer, sizeof(buffer));
	if (bytesRead == 0)
	{
		if (isCgi)
			clientData->setIsAvailable(true);
		else
			Logger::log(INFO, "Client ", convertToString(fd), " disconnected.");
		return R_CLIENT_DISCONNECTED;
	}
	if (bytesRead <= -1)
	{
		Logger::log(ERROR, "Error when reading from client ", convertToString(fd), ".");
		return R_ERROR;
	}
	clientData->addData(buffer, static_cast<size_t>(bytesRead));
	if (!clientData->isHeaderEnded() && clientData->getData().find("\r\n\r\n") != std::string::npos)
	{
		clientData->setHeaderEnded(true);
		ssize_t contentLength = parseContentLength(clientData->getData(), isCgi);
		clientData->setContentLength(contentLength);
		if (contentLength == -1)
			return R_CONTINUE;
		ssize_t clientMaxBodySize = checkClientMaxBodySize(fd, clientData, hrc, contentLength);
		if (clientMaxBodySize == -1)
			return R_BAD_REQUEST;
		if (!isCgi && clientMaxBodySize < contentLength)
			return R_CONTENT_TOO_LARGE;
	}
	if (clientData->getContentLength() == -1)
		return R_CONTINUE;
	if (clientData->isHeaderEnded()
		&& isEndBody(clientData->getData(), static_cast<size_t>(clientData->getContentLength())))
	{
		clientData->setIsAvailable(true);
		return R_FINISHED_READING;
	}
	return R_CONTINUE;
}

ssize_t
SocketIO::checkClientMaxBodySize(int fd, ClientData *clientData, HttpRequestContext *hrc, ssize_t contentLength)
{
	unsigned int clientMaxBodySize;
	if (hrc == NULL)
		return 0;
	ServerConfig *serverConfig = NULL;
	LocationConfig *locationConfig = NULL;
	try
	{
		Request tempRequest;
		MotherWebserv *mw = MotherWebserv::getInstance();
		serverConfig = (&(mw->getServerConfigFromSocket(fd)));
		std::string requestStr = clientData->getData();
		tempRequest.parsStartLine(requestStr);
		std::string target = tempRequest.getTarget();
		locationConfig = mw->getLocationConfigFromSocket(*serverConfig, target);
		if (locationConfig != NULL)
			clientMaxBodySize = locationConfig->getDirectives().getClientMaxBodySize();
		else
			clientMaxBodySize = serverConfig->getDirectives().getClientMaxBodySize();
	} catch (const std::exception &e)
	{
		std::string response = generateError500(true);
		switchToSendingResponse(hrc, response);
		return -1;
	} catch (int code)
	{
		std::string response = generateErrorResponse(locationConfig, serverConfig, true, code);
		switchToSendingResponse(hrc, response);
		return -1;
	}
	if (clientMaxBodySize < contentLength)
	{
		std::string response = generateErrorResponse(locationConfig, serverConfig, true, 413);
		switchToSendingResponse(hrc, response);
	}
	return clientMaxBodySize;
}

bool SocketIO::isDeconnected()
{
	return _isDeconnected;
}

void SocketIO::setIsDeconnected(bool isDeconnected)
{
	_isDeconnected = isDeconnected;
}

std::map<int, std::list<HttpRequestContext> > &SocketIO::getClientHttpRequestContexts()
{
	return _clientHttpRequestContexts;
}

std::vector<DataToEpoll *> &SocketIO::getDataDeadChilds()
{
	return _dataDeadChilds;
}

bool SocketIO::isPipeFdFromDeadChild(int fd)
{
	DataToEpoll *dataToEpoll = MotherWebserv::getInstance()->getDataToEpoll(fd);
	return isDataToEpollFreed(dataToEpoll);
}

bool SocketIO::isDataToEpollFreed(DataToEpoll *dataToEpoll)
{
	std::vector<DataToEpoll *> &dataDeadChilds = _dataDeadChilds;
	return (std::find(dataDeadChilds.begin(), dataDeadChilds.end(), dataToEpoll) != dataDeadChilds.end());
}
