#include "ServerDirectives.hpp"

ServerDirectives::ServerDirectives()
  : Directives(),
    _serverNames() {
}

ServerDirectives::ServerDirectives(const ServerDirectives& copy)
  : Directives(copy),
    _serverNames(copy._serverNames) {
}

ServerDirectives& ServerDirectives::operator=(const ServerDirectives& src) {
  if (this != &src) {
    Directives::operator=(src);
    _serverNames = src._serverNames;
  }
  return (*this);
}

ServerDirectives::~ServerDirectives() {
}

/*  Getters */

const std::set<std::string> &ServerDirectives::getServerNames() const {
    return (_serverNames);
}


/*  Setters */

void ServerDirectives::setServerName(const std::string& serverName, size_t mode) {
	std::pair<std::set<std::string>::iterator, bool> result = _serverNames.insert(serverName);
	if (result.second == false && mode == 0)
		Logger::log(WARNING, "Server name already taken in account, ignored");
}

