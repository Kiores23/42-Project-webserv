# include "Socket.hpp"
# include "../http/response.hpp"

Socket::Socket() : _socketFd(-1), _localPort(-1), _remotePort(-1)
{
}

Socket::Socket(const HostPort& hostPort) : _socketFd(-1), _localPort(hostPort.getPort()), _remotePort(-1)
{
	if (!hostPort.getHost().empty())
		_localIp = hostPort.getHost();
	if (!socketInit(_socketFd))
		throw std::exception();
	if (!socketBind(_localIp, _localPort, _socketFd))
		throw std::exception();
	if (!socketListen(_socketFd))
		throw std::exception();
}

Socket::Socket(const Socket &other) : _socketFd(-1), _localPort(-1), _remotePort(-1)
{
	*this = other;
}

Socket::~Socket()
{
}

Socket &Socket::operator=(const Socket &other)
{
	if (this != &other)
	{
		_localPort = other._localPort;
		_socketFd = other._socketFd;
		_localIp = other._localIp;
	}
	return *this;
}

int Socket::getPort() const
{
	return _localPort;
}

std::string Socket::getIP()
{
	return _localIp;
}

int Socket::getSocketFD() const
{
	return _socketFd;
}

/**
 * Initializes and configures a TCP (SOCK_STREAM) socket for IPv4 (AF_INET).
 *
 * SO_REUSEADDR is enabled to allow the socket to bind to a port
 * which might still be in TCP TIME-WAIT state from a previous connection.
 * @return If an error occurs, returns false.
 */
bool Socket::socketInit(int &socketFd) const
{
	socketFd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
	if (socketFd == -1)
	{
		Logger::log(ERROR, "Error while creating the server socket");
		return false;
	}
	const int enable = 1;
	int sockoptReturn = setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
	if (sockoptReturn == -1)
	{
		Logger::log(ERROR, "Error while creating the server socket");
		return false;
	}
	return true;
}

/**
 * @brief Binds the socket to a local IPv4 address and port.
 *
 * @return If an error occurs, closes the socket and returns false.
 * @note The local IP address, provided as a ip within _localIp, is transformed into its
 *       binary representation using the inet_addr function. Meanwhile, the port number, an integer
 *       stored in _localPort, undergoes conversion to network byte order through the htons function.
 *       See https://www.gnu.org/software/libc/manual/html_node/Byte-Order.html
 */
bool Socket::socketBind(const std::string &ip, int port, int socketFd) const
{
	sockaddr_in serverAddress = {};
	serverAddress.sin_family = AF_INET;
	if (!setSocketAddress(serverAddress, ip))
		return false;
	serverAddress.sin_port = htons(static_cast<u_int16_t>(port));
	if (bind(socketFd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1)
	{
		Logger::logErrno("Error while binding socket");
		close(socketFd);
		return false;
	}
	return true;
}

/**
 * @brief Sets the IP address in sockaddr_in for socket binding.
 * @param serverAddress Reference to sockaddr_in structure to configure.
 *
 * Configures serverAddress with an IP address determined by _localIp class member.
 * If _localIp is not empty, it's used as the bind address. Otherwise, binds to all
 * interfaces using INADDR_ANY.
 * @return If an error occurs, closes the socket and returns false.
 * @note See https://www.gnu.org/software/libc/manual/html_node/Byte-Order.html
 */
bool Socket::setSocketAddress(sockaddr_in &serverAddress, const std::string &ip) const
{
	if (ip.empty() || ip == "0.0.0.0")
		serverAddress.sin_addr.s_addr = INADDR_ANY;
	else
	{
		serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
		if (serverAddress.sin_addr.s_addr == INADDR_NONE)
		{
			close(_socketFd);
			return false;
		}
	}
	return true;
}

/**
 * Server socket waits for at least one client to connect
 * There is a queue of MAX_CLIENTS possible before being accepted
 * Any client trying to connect when the queue is full can receive an error of ECONNREFUSED
 * Next step after a client comes to connect is accept()
 */
bool Socket::socketListen(int socketFd) const
{
	if (listen(socketFd, MAX_CLIENTS) == -1)
	{
		Logger::logErrno("Error when listening client");
		close(socketFd);
		return false;
	}
	return true;
}

std::string Socket::toString() const
{
	return getLocalSocketInfos(_socketFd);
}

/**
 * Return local informations on a socket with it's fd.
 * @param fd the socket's fd.
 * @return the string of informations.
 */
std::string Socket::getLocalSocketInfos(int fd)
{
	std::stringstream socketContent;
	try
	{
		socketContent << "Local IP address: " << getLocalIpAddress(fd) << '\n';
		socketContent << "Local port: " << getLocalPort(fd);
	}
	catch (int e)
	{
		socketContent << "";
	}
	return socketContent.str();
}

/**
 * Return local and remote informations on a socket with it's fd.
 * @param fd the socket's fd.
 * @return the string of informations.
 */
std::string Socket::getConnectedSocketInfos(int fd)
{
	std::stringstream socketContent;

	try
	{
		socketContent << "Socket fd: " << fd << '\n';
		socketContent << getLocalSocketInfos(fd) << '\n';
		socketContent << "Remote IP Address: " << getRemoteIpAddress(fd) << '\n'
					  << "Remote port: " << getRemotePort(fd);
	}
	catch (int e)
	{
		socketContent << "";
	}
	return socketContent.str();
}

/**
 *
 * @return
 * @throw
 */
std::string Socket::getLocalIpAddress()
{
	if (!_localIp.empty())
		return _localIp;

	_localIp = getLocalIpAddress(_socketFd);
	return _localIp;
}

/**
 *
 * @param socketFd
 * @return
 * @throw
 */
std::string Socket::getLocalIpAddress(int socketFd)
{
	std::stringstream socketContent;
	struct sockaddr_in local_addr = {};
	socklen_t local_len = sizeof(local_addr);
	if (getsockname(socketFd, reinterpret_cast<struct sockaddr *>(&local_addr), &local_len) == 0)
		return inet_ntoa(local_addr.sin_addr);
	throw logForError500("Cannot get local IP address of socket " + convertToString(socketFd));
}

/**
 *
 * @return
 * @throw
 */
int Socket::getLocalPort()
{
	if (_localPort != -1)
		return _localPort;

	_localPort = getLocalPort(_socketFd);
	return _localPort;
}

/**
 *
 * @param socketFd
 * @return
 * @throw
 */
int Socket::getLocalPort(int socketFd)
{
	std::stringstream socketContent;
	struct sockaddr_in local_addr = {};
	socklen_t local_len = sizeof(local_addr);
	if (getsockname(socketFd, reinterpret_cast<struct sockaddr *>(&local_addr), &local_len) == 0)
		return ntohs(local_addr.sin_port);
	throw logForError500("Cannot get local port of socket " + convertToString(socketFd));
}

/**
 *
 * @return
 * @throw
 */
std::string Socket::getRemoteIpAddress()
{
	if (!_remoteIp.empty())
		return _remoteIp;
	_remoteIp = getRemoteIpAddress(_socketFd);
	return _remoteIp;
}

/**
 *
 * @param socketFd
 * @return
 * @throw
 */
std::string Socket::getRemoteIpAddress(int socketFd)
{
	struct sockaddr_in remote_addr = {};
	socklen_t remote_len = sizeof(remote_addr);
	if (getpeername(socketFd, reinterpret_cast<struct sockaddr *>(&remote_addr), &remote_len) == 0)
		return inet_ntoa(remote_addr.sin_addr);
	throw logForError500("Cannot get remote IP address of socket " + convertToString(socketFd));
}

int Socket::getRemotePort()
{
	if (_remotePort != -1)
		return _remotePort;
	_remotePort = getRemotePort(_socketFd);
	return _remotePort;
}

/**
 *
 * @param socketFd
 * @return
 * @throw
 */
int Socket::getRemotePort(int socketFd)
{
	struct sockaddr_in remote_addr = {};
	socklen_t remote_len = sizeof(remote_addr);
	if (getpeername(socketFd, reinterpret_cast<struct sockaddr *>(&remote_addr), &remote_len) == 0)
		return ntohs(remote_addr.sin_port);
	throw logForError500("Cannot get remote port of socket " + convertToString(socketFd));
}

std::ostream &operator<<(std::ostream &out, const Socket &socket)
{
	out << socket.toString();
	return out;
}
