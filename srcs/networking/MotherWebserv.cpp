#include "MotherWebserv.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "../socketio/SocketIO.hpp"
#include "../cgi/execCgiScript.hpp"

MotherWebserv *MotherWebserv::_instance = NULL;

MotherWebserv::MotherWebserv() : _epoll_fd(-1), _webservAlive(true), _isChild(false)
{
}

MotherWebserv::~MotherWebserv()
{
	if (!_isChild)
	{
		std::map<int, Socket *> *socketMap = &(_config.getMotherSocketsMap());
		std::map<int, Socket *>::iterator socketIt;
		for (socketIt = socketMap->begin(); socketIt != socketMap->end(); ++socketIt)
		{
			MotherWebserv::getInstance()->removeServerSocket(socketIt->second->getSocketFD());
			delete socketIt->second;
		}
		socketMap->clear();
		if (_epoll_fd != -1)
			close(_epoll_fd);
	}
}

void childCloseKeepAlive(int clientSocket)
{
    std::map<int, DataToEpoll *> &fdDataToEpollMap = MotherWebserv::getInstance()->getFdDataToEpollMap();
	std::map<int, DataToEpoll *>::iterator it = fdDataToEpollMap.find(clientSocket);
	if (it != fdDataToEpollMap.end())
	{
		DataToEpoll *dataToEpoll = fdDataToEpollMap[clientSocket];
		fdDataToEpollMap.erase(clientSocket);
		delete dataToEpoll;
	}
	close(clientSocket);
}

void MotherWebserv::childCloseFds()
{
	_isChild = true;

	MotherWebserv::getInstance()->getKeepAliveMap().clean(childCloseKeepAlive);

	delete SocketIO::getInstance();

	std::map<int, Socket *> *socketMap = &(_config.getMotherSocketsMap());
	std::map<int, Socket *>::iterator socketIt;
	for (socketIt = socketMap->begin(); socketIt != socketMap->end(); ++socketIt)
	{
		removeFdDataToEpoll(socketIt->second->getSocketFD());
		close(socketIt->second->getSocketFD());
		delete socketIt->second;
	}
	socketMap->clear();
	if (_epoll_fd != -1)
		close(_epoll_fd);
	delete this;
}


MotherWebservConfig &MotherWebserv::getConfig()
{
	return (_config);
}

int MotherWebserv::getEpollFd() const
{
	return (_epoll_fd);
}

MotherWebserv *MotherWebserv::getInstance()
{
	if (_instance == NULL)
		_instance = new MotherWebserv();
	return _instance;
}

/**
 * Creates the instance of epoll of the server: _epoll_fd
 */
bool MotherWebserv::createEpollInstance()
{
	_epoll_fd = epoll_create1(0);
	if (_epoll_fd == -1)
	{
		Logger::logErrno("Error while creating epoll instance");
		return false;
	}
	return true;
}

/**
 * Adds the sockets from all servers
 */
bool MotherWebserv::setSockets()
{
	std::vector<Server *> servers = getConfig().getServers();
	for (std::vector<Server *>::const_iterator it = servers.begin(); it != servers.end(); ++it)
	{
		std::set<HostPort> hostPorts = (*it)->getConfig().getHostPorts();
		for (std::set<HostPort>::const_iterator it2 = hostPorts.begin(); it2 != hostPorts.end(); ++it2)
		{
			Socket *socket = NULL;
			try
			{
				socket = new Socket(*it2);

				if (!addServerSocket(socket->getSocketFD()))
					throw std::exception();
				getConfig().setSocket(socket);
				socket = NULL;
				std::string host = it2->getHost();
				(*it)->getConfig().setHostServerName(host);
				for (std::vector<LocationConfig *>::const_iterator it3 = (*it)->getConfig().getLocations().begin();
					 it3 != (*it)->getConfig().getLocations().end(); ++it3)
				{
					(*it3)->setHostServerName(host);
				}
			} catch (const std::exception &e)
			{
				delete socket;
				return false;
			}
		}
	}
	return true;
}

/**
 * Gets the server config from the server the client wants to connect to
 *
 * @return a reference to the serverConfig the client wants to connect to
 */
ServerConfig &MotherWebserv::getServerConfigFromSocket(const int clientSocket) const
{
	HostPort hostPort;
	try
	{
		hostPort.setHost(Socket::getLocalIpAddress(clientSocket));
		hostPort.setPort(Socket::getLocalPort(clientSocket));
	} catch (const std::exception &e)
	{
		throw std::runtime_error("500");
	}
	std::vector<Server *> servers = MotherWebserv::getInstance()->getConfig().getServers();
	for (std::vector<Server *>::const_iterator it = servers.begin(); it != servers.end(); ++it)
	{
		std::set<HostPort> hostPorts = (*it)->getConfig().getHostPorts();
		if (hostPorts.find(hostPort) != hostPorts.end())
		{
			return (*it)->getConfig();
		}
	}
	throw std::runtime_error("500");
}

/**
 * Add server _clientSocket to epoll instance
 * https://man7.org/linux/man-pages/man2/epoll_ctl.2.html
 * EPOLLIN: The associated file is available for read(2) operations.
 * @param socketFd
 */
bool MotherWebserv::addSocketToEpoll(int socketFd)
{
	epoll_event epollEvent = {};
	epollEvent.events = EPOLLIN | EPOLLOUT;
	DataToEpoll *dataToEpoll = new DataToEpoll;
	dataToEpoll->isCgi = false;
	dataToEpoll->fd = socketFd;
	dataToEpoll->ptrHRC = NULL;
	epollEvent.data.ptr = dataToEpoll;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, socketFd, &epollEvent) == -1)
	{
		std::stringstream errorMessage;
		errorMessage << "Error when adding clientSocket " << socketFd << " to epoll.";
		Logger::logErrno(errorMessage.str());
		close(socketFd);
		delete dataToEpoll;
		return false;
	}
	_fdDataToEpollMap[socketFd] = dataToEpoll;
	return true;
}

bool MotherWebserv::addPipe(int pipeFd, HttpRequestContext *hrc)
{
	epoll_event epollEvent = {};
	epollEvent.events = EPOLLIN | EPOLLOUT;
	DataToEpoll *dataToEpoll = new DataToEpoll;
	dataToEpoll->isCgi = true;
	dataToEpoll->fd = pipeFd;
	dataToEpoll->ptrHRC = hrc;
	epollEvent.data.ptr = dataToEpoll;
	hrc->setCgiFd(pipeFd);
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, pipeFd, &epollEvent) == -1)
	{
		std::stringstream errorMessage;
		errorMessage << "Error when adding pipe " << pipeFd << " to epoll.";
		Logger::logErrno(errorMessage.str());
		close(pipeFd);
		delete dataToEpoll;
		return false;
	}
	_fdDataToEpollMap[pipeFd] = dataToEpoll;
	return true;
}

/**
 * Add client socket to web server
 * https://man7.org/linux/man-pages/man2/epoll_ctl.2.html
 * EPOLLIN: The associated file is available for read(2) operations.
 * @param clientSocketFd
 */
bool MotherWebserv::addClientSocket(int clientSocketFd)
{
	bool isAdded = addSocketToEpoll(clientSocketFd);
	if (isAdded)
		_clientsSocket.insert(clientSocketFd);
	return (isAdded);
}

/**
 * Add server socket to web server
 * https://man7.org/linux/man-pages/man2/epoll_ctl.2.html
 * EPOLLIN: The associated file is available for read(2) operations.
 * @param socket
 */
bool MotherWebserv::addServerSocket(int serverSocketFd)
{
	return (addSocketToEpoll(serverSocketFd));
}

DataToEpoll *MotherWebserv::getDataToEpoll(int fd)
{
	if (_fdDataToEpollMap.empty())
		return NULL;
	std::map<int, DataToEpoll *>::iterator it = _fdDataToEpollMap.find(fd);
	if (it != _fdDataToEpollMap.end())
	{
		DataToEpoll *dataToEpoll = _fdDataToEpollMap[fd];
		return dataToEpoll;
	}
	return NULL;
}

void MotherWebserv::removeFdDataToEpoll(int fd)
{
	if (_fdDataToEpollMap.empty())
		return;
	std::map<int, DataToEpoll *>::iterator it = _fdDataToEpollMap.find(fd);
	if (it != _fdDataToEpollMap.end())
	{
		DataToEpoll *dataToEpoll = _fdDataToEpollMap[fd];
		_fdDataToEpollMap.erase(fd);
		delete dataToEpoll;
	}
}

bool MotherWebserv::removeFdFromEpoll(int fd)
{
	removeFdDataToEpoll(fd);
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1)
	{
		std::stringstream errorMessage;
		errorMessage << "Error when deleting fd " << fd << " from epoll.";
		Logger::logErrno(errorMessage.str());
		return false;
	}
	return true;
}

/**
 * Remove client socket from web server
 * @param clientSocket
 */
void MotherWebserv::removeClientSocket(int clientSocket)
{
	if (!SocketIO::getInstance()->hasNoPendingRequest(clientSocket))
	{
		std::map<int, std::list<HttpRequestContext> > *map = &(SocketIO::getInstance()->getClientHttpRequestContexts());
		std::list<HttpRequestContext> *hrcs = (&(*map)[clientSocket]);
		std::list<HttpRequestContext>::iterator it = hrcs->begin();
		for (; it != hrcs->end(); it++)
		{
			it->getCgiHandler().cleanChild(true);
			MotherWebserv::getInstance()->getCgiTimers().erase(&it->getCgiHandler());
		}
	}
	SocketIO::getInstance()->removeSocketRequests(clientSocket);
	MotherWebserv::getKeepAliveMap().erase(clientSocket);
	_clientsSocket.erase(clientSocket);
	removeFdFromEpoll(clientSocket);
	close(clientSocket);
}

/**
 * Remove server socket from web server
 * @param socket
 */
void MotherWebserv::removeServerSocket(int serverSocketFd)
{
	removeFdFromEpoll(serverSocketFd);
	close(serverSocketFd);
}

/**
 * Compares the target with the new path and the closest saved path
 *
 * @param nextPath: copy of the new path to compare
 * @param savedPath: copy of the closest saved path
 * @param copyTarget: copy of the request's target
 *
 * @return true if the closest saved path is still the closest, false if the new path is the closest
 */
static bool stillClosestPath(std::string nextPath, std::string savedPath, std::string copyTarget)
{
	if (nextPath[nextPath.size() - 1] != '/')
		nextPath += "/";
    if (savedPath[savedPath.size() - 1] != '/')
		savedPath += "/";
	if (copyTarget[copyTarget.size() - 1] != '/')
		copyTarget += "/";
	size_t foundNextPath = copyTarget.find(nextPath);
	if (foundNextPath != std::string::npos && nextPath.size() >= savedPath.size())
		return (false);
	return (true);
}

/**
 * Checks all the location configs of the server to return the one with the closest path
 *
 * @param serverConfig
 * @param target: the request's target
 *
 * @return NULL if there is no locationConfig or a pointer to the locationConfig with the closest path
 */
LocationConfig *
MotherWebserv::getLocationConfigFromSocket(const ServerConfig &serverConfig, const std::string &target) const
{
	LocationConfig *locationConfig = NULL;
    std::string closestPath = "/";
	std::vector<LocationConfig *> locations = serverConfig.getLocations();
	for (std::vector<LocationConfig *>::const_iterator it = locations.begin(); it != locations.end(); ++it)
	{
		std::string newPath = (*it)->getDirectives().getPath();
		if (not stillClosestPath(newPath, closestPath, target))
		{
			locationConfig = *it;
			closestPath = newPath;
		}
	}
	return (locationConfig);
}

TimerMap<CGIHandler *> &MotherWebserv::getCgiTimers()
{
	return _cgiTimers;
}

std::map<int, DataToEpoll *> &MotherWebserv::getFdDataToEpollMap()
{
	return _fdDataToEpollMap;
}

TimerMap<int> &MotherWebserv::getKeepAliveMap()
{
	return _keepAliveMap;
}

void MotherWebserv::killWebserv()
{
	_webservAlive = false;
}

bool MotherWebserv::webservisAlive()
{
	return _webservAlive;
}

std::set<int> &MotherWebserv::getKeepAliveTimeExpiredFds()
{
	return _keepAliveTimeExpiredFds;
}

std::set<pid_t> &MotherWebserv::getChildsZombies()
{
	return _childsZombies;
}

const std::set<int> &MotherWebserv::getClientsSocket() const
{
	return _clientsSocket;
}
