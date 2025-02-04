#ifndef WEBSERV_SOCKET_HPP
# define WEBSERV_SOCKET_HPP

# include "../logging/Logger.hpp"
#include "../utils/HostPort.hpp"
# include <arpa/inet.h>
# include <cstdlib>
# include <cstring>
# include <iostream>
# include <sys/socket.h>
# include <unistd.h>
#include <string>

class HostPort;

class Socket
{
private:
	int _socketFd;
	std::string _localIp;
	std::string _remoteIp;
	int _localPort;
	int _remotePort;
	static const int DEFAULT_PROTOCOL = 0;
	static const int MAX_CLIENTS = SOMAXCONN;
	Socket();
	bool socketInit(int &socketFd) const;
	bool socketBind(const std::string &ip, int port, int socketFd) const;
	bool socketListen(int socketFd) const;
public:
	explicit Socket(const HostPort& hostPort);
	Socket(const Socket &other);
	~Socket();
	Socket &operator=(const Socket &other);
	int getPort() const;
	std::string getIP();
	int getSocketFD() const;
	std::string toString() const;
	static std::string getLocalSocketInfos(int fd);
	static std::string getConnectedSocketInfos(int fd);
	std::string getLocalIpAddress();
	std::string getRemoteIpAddress();
	int getLocalPort();
	int getRemotePort();
	static std::string getLocalIpAddress(int socketFd);
	static std::string getRemoteIpAddress(int socketFd);
	static int getLocalPort(int socketFd);
	static int getRemotePort(int socketFd);
	bool setSocketAddress(sockaddr_in &serverAddress, const std::string& ip) const;
};

std::ostream &operator<<(std::ostream &out, const Socket &socket);

#endif // WEBSERV_SOCKET_HPP
