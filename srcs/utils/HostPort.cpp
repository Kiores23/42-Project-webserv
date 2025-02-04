#include "HostPort.hpp"

HostPort::HostPort() : _host(LOCALHOST_ADDRESS), _port(8082) {
}

HostPort::HostPort(const HostPort& copy) : _host(copy._host), _port(copy._port) {
}

HostPort& HostPort::operator=(const HostPort& src) {
  if (this != &src) {
    _host = src._host;
    _port = src._port;
  }
  return (*this);
}

HostPort::~HostPort() {
}

/*  Getters */

const std::string& HostPort::getHost() const {
    return (_host);
}

const int& HostPort::getPort() const {
    return (_port);
}

/*  Setters */

void HostPort::setHost(const std::string& host) {
    _host = host;
}

void HostPort::setPort(const int& port) {
    _port = port;
}

/*  Operators */

bool HostPort::operator==(const HostPort& value) const {
    return (_host == value._host && _port == value._port);
}

bool HostPort::operator<(const HostPort& value) const {
    if (_host != value._host)
        return (_host < value._host);
    return (_port < value._port);
}
