#include "Server.hpp"

Server::Server(const Directives &heritage) {
	_config.setHeritage(heritage);
}

Server::Server(const Server& copy) : _config(copy._config) {
}

Server& Server::operator=(const Server& src) {
  if (this != &src) {
    _config = src._config;
  }
  return (*this);
}

Server::~Server() {
}

ServerConfig &Server::getConfig() {
	return (_config);
}
