#ifndef WEBSERV_SERVER_HPP
# define WEBSERV_SERVER_HPP

# include "../config/ServerConfig.hpp"

class Server
{
private:
	ServerConfig _config;

public:
	Server(const Directives &heritage);
  Server(const Server& copy);
  Server& operator=(const Server& src);
	~Server();

	ServerConfig &getConfig();
};

#endif // WEBSERV_SERVER_HPP
