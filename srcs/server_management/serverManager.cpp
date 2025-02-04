#include "serverManager.hpp"
#include "../socketio/SocketIO.hpp"
#include "cgiProcess.hpp"
#include "readingClientRequest.hpp"
#include "sendingResponse.hpp"

void checkCloseKeepAlive(int clientSocket);

int waitForEvents(epoll_event *events)
{
	int num_events = epoll_wait(
			MotherWebserv::getInstance()->getEpollFd(), events, MAX_EVENTS, TIMEOUT_MS);
	if (num_events == -1)
		Logger::logErrno("Error while waiting epoll events");
	return num_events;
}

bool hasEpollError(epoll_event &epoll_event, int clientSocket)
{
	if (!(epoll_event.events & EPOLLERR))
		return false;
	MotherWebserv::getInstance()->removeClientSocket(clientSocket);
	return true;
}

/**
 * accept4() extracts the first connection request from the queue of pending connections.
 * @param socketServer The server's fd receiving a new connection.
 * @return int The new client fd, or -1 if an error occurs.
 */
int acceptClientConnection(int socketServer)
{
	int client_socket;
	std::stringstream ss;

	client_socket = accept4(socketServer, NULL, NULL, SOCK_NONBLOCK);
	if (client_socket == -1)
	{
		if (not(errno == EAGAIN || errno == EWOULDBLOCK))
			Logger::logErrno("Error when accepting new client connection.");
		return -1;
	}
	return (client_socket);
}

int connectWithNewClient(int socketServer)
{
	int clientSocket;

	clientSocket = acceptClientConnection(socketServer);
	if (clientSocket == -1)
		return false;
	if (!MotherWebserv::getInstance()->addClientSocket(clientSocket))
		return false;
	Logger::log(INFO, "A new client ", convertToString(clientSocket), " has connected to the server.");
	return true;
}

/**
 * Event on an existing client socket
 * @param clientSocket
 */
void handleEventFromAlreadyConnectedClient(epoll_event epoll_event)
{
	DataToEpoll *dataToEpoll = static_cast<DataToEpoll *>(epoll_event.data.ptr);
	HttpRequestContext *hrc = static_cast<HttpRequestContext *>(dataToEpoll->ptrHRC);
	int clientSocket = dataToEpoll->fd;

	if (hasEpollError(epoll_event, clientSocket))
		return;
	if (epoll_event.events & EPOLLIN || epoll_event.events & EPOLLHUP)
	{
		if (hrc == NULL || (not dataToEpoll->isCgi))
		{
			readingClientRequest(clientSocket);
			return;
		}
		if (dataToEpoll->isCgi && SocketIO::getInstance()->checkIsPresent(clientSocket, hrc))
		{
			readingCGIResponse(hrc);
		}
	}
	if (epoll_event.events & EPOLLOUT)
	{
		hrc = SocketIO::getInstance()->getCurrentResponseHRC(clientSocket);
		if (hrc == NULL)
			return;
		if (hrc->getClientResponse().isAvailable())
		{
			int status = sendingResponse(hrc, clientSocket);
			if (status == S_ERROR || status == S_CLIENT_DISCONNECTED)
			{
				MotherWebserv::getInstance()->getKeepAliveMap().erase(clientSocket);
				MotherWebserv::getInstance()->removeClientSocket(clientSocket);
				return;
			}
			if (status == S_FINISHED_SENDING)
			{
				if (hrc->getRequest()->getKeepAlive() == CLOSE)
				{
					SocketIO::getInstance()->removeCurrentResponse(clientSocket);
					MotherWebserv::getInstance()->getKeepAliveMap().erase(clientSocket);
                    MotherWebserv::getInstance()->removeClientSocket(clientSocket);
				}
				else
				{
					checkCloseKeepAlive(clientSocket);
					SocketIO::getInstance()->removeCurrentResponse(clientSocket);
				}
			}
		}
	}
}

void checkCloseKeepAlive(int clientSocket)
{
	std::set<int> &fds = MotherWebserv::getInstance()->getKeepAliveTimeExpiredFds();
	if (fds.find(clientSocket) != fds.end() && SocketIO::getInstance()->hasNoPendingRequest(clientSocket))
	{
		MotherWebserv::getInstance()->removeClientSocket(clientSocket);
		fds.erase(clientSocket);
	}
}

/**
 * Event Management
 */
void treatEvents(int num_events, epoll_event *events)
{
	std::map<int, Socket *> serverSocketsMap = MotherWebserv::getInstance()->getConfig().getMotherSocketsMap();
	std::map<int, Socket *>::iterator currentSocket;

	DataToEpoll *dataToEpoll;
	int clientSocket;

	for (int i = 0; i < num_events; ++i)
	{
		if (not MotherWebserv::getInstance()->webservisAlive())
			break;
		dataToEpoll = static_cast<DataToEpoll *>(events[i].data.ptr);
		if (SocketIO::getInstance()->isDataToEpollFreed(dataToEpoll))
			continue;
		clientSocket = dataToEpoll->fd;
		if (SocketIO::getInstance()->isPipeFdFromDeadChild(clientSocket))
			continue;
		currentSocket = serverSocketsMap.find(clientSocket);
        if (currentSocket != serverSocketsMap.end())
        {
            connectWithNewClient(currentSocket->first);
            return;
        }
        handleEventFromAlreadyConnectedClient(events[i]);
	}
	SocketIO::getInstance()->getDataDeadChilds().clear();
}
