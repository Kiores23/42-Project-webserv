#ifndef MOTHERWEBSERV_HPP
# define MOTHERWEBSERV_HPP

# include "../config/MotherWebservConfig.hpp"
# include "../utils/utils.hpp"
# include "../socketio/ClientRequest.hpp"
# include "../socketio/ClientResponse.hpp"
# include "../socketio/HttpRequestContext.hpp"
# include "../utils/TimerMap.hpp"
# include <map>
# include <string>
# include <vector>
# include <sys/epoll.h>
# include <queue>

class HttpRequestContext;

class Socket;

class ServerConfig;

class LocationConfig;

struct DataToEpoll
{
	int fd;
	void *ptrHRC;
	bool isCgi;
};

class MotherWebserv
{
private:
	int _epoll_fd;
	bool	_webservAlive;
	bool	_isChild;
	std::set<int> _clientsSocket;
	std::set<pid_t> _childsZombies;
	TimerMap<CGIHandler *> _cgiTimers;
	std::map<int, DataToEpoll *> _fdDataToEpollMap;
	TimerMap<int>	_keepAliveMap;
	std::set<int>	_keepAliveTimeExpiredFds;
	MotherWebservConfig _config;
	static MotherWebserv *_instance;

	bool removeFdFromEpoll(int fd);

	bool addSocketToEpoll(int socketFd);
public:
	MotherWebserv();

	~MotherWebserv();

	bool createEpollInstance();

	MotherWebservConfig &getConfig();

	int getEpollFd() const;

	static MotherWebserv *getInstance();

	ServerConfig &getServerConfigFromSocket(const int clientSocket) const;

	LocationConfig *getLocationConfigFromSocket(const ServerConfig &serverConfig, const std::string &target) const;

	bool addClientSocket(int clientSocketFd);

	bool addServerSocket(int serverSocketFd);

	bool setSockets();

	void removeClientSocket(int clientSocket);

	void removeServerSocket(int serverSocketFd);
	bool addPipe(int pipeFd, HttpRequestContext *pContext);
	TimerMap<CGIHandler *> &getCgiTimers();
	std::map<int, DataToEpoll *> &getFdDataToEpollMap();
	void removeFdDataToEpoll(int fd);
	TimerMap<int> &getKeepAliveMap();

	std::set<int> &getKeepAliveTimeExpiredFds();
	void killWebserv();
	bool webservisAlive();
	std::set<pid_t> &getChildsZombies();
	const std::set<int> &getClientsSocket() const;
	void childCloseFds();
	DataToEpoll *getDataToEpoll(int fd);
};

#endif
