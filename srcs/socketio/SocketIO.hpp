#ifndef WEBSERV_SOCKETIO_H
#define WEBSERV_SOCKETIO_H

#include "HttpRequestContext.hpp"
# include "ClientRequest.hpp"
# include "ClientResponse.hpp"
# include <map>
# include <string>
# include <list>

// Read return values
# define R_ERROR (-1)
# define R_CONTINUE 0
# define R_FINISHED_READING 1
# define R_CLIENT_DISCONNECTED 2
# define R_CONTENT_TOO_LARGE 3
# define R_BAD_REQUEST 4

// Send return values
# define S_ERROR (-1)
# define S_CONTINUE 0
# define S_FINISHED_SENDING 1
# define S_CLIENT_DISCONNECTED 2

class SocketIO
{
private:
	static SocketIO *_instance;
	static bool _isDeconnected;
	std::map<int, std::list<HttpRequestContext> > _clientHttpRequestContexts;
	std::vector<DataToEpoll *> _dataDeadChilds;

public:
	SocketIO();
	static SocketIO *getInstance();

	void removeSocketRequests(int clientSocket);
	HttpRequestContext *addHttpRequestContext(int clientSocket);
	bool removeCurrentResponse(int clientSocket);

	static bool isDeconnected();
	static void setIsDeconnected(bool isDeconnected);

    bool checkIsPresent(int clientSocket, HttpRequestContext *hrc);
    int sendResponse(int clientSocket, ClientResponse *clientResponse);
	bool needNewRequest(int clientSocket);
	HttpRequestContext *getCurrentRequestHRC(int clientSocket);
	HttpRequestContext *getCurrentResponseHRC(int clientSocket);
	static int readContentFromFd(int fd, ClientData *clientData, HttpRequestContext *hrc);
	std::map<int, std::list<HttpRequestContext> > &getClientHttpRequestContexts();
	bool hasNoPendingRequest(int clientSocket);
	static ssize_t checkClientMaxBodySize(int fd, ClientData *clientData, HttpRequestContext *hrc, ssize_t contentLength);
	std::vector<DataToEpoll *> &getDataDeadChilds();
	bool isPipeFdFromDeadChild(int fd);
	bool isDataToEpollFreed(DataToEpoll *dataToEpoll);
};


#endif //WEBSERV_SOCKETIO_H
