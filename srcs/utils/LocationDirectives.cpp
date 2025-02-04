#include "LocationDirectives.hpp"

LocationDirectives::LocationDirectives()
  : ServerDirectives(),
    _httpAcceptedMethods(),
    _path(""),
    _httpRedirection() {
}

LocationDirectives::LocationDirectives(const LocationDirectives& copy)
  : ServerDirectives(copy),
    _httpAcceptedMethods(copy._httpAcceptedMethods),
    _path(copy._path),
    _httpRedirection(copy._httpRedirection) {
}

LocationDirectives& LocationDirectives::operator=(const LocationDirectives& src) {
  if (this != &src) {
    ServerDirectives::operator=(src);
    _httpAcceptedMethods = src._httpAcceptedMethods;
    _path = src._path;
    _httpRedirection = src._httpRedirection;
  }
  return (*this);
}

LocationDirectives::~LocationDirectives() {
}

/*  Getters */

const std::set<std::string> &LocationDirectives::getHttpAcceptedMethods() const {
    return (_httpAcceptedMethods);
}

const std::pair<u_int16_t, std::string> &LocationDirectives::getHttpRedirection() const {
    return (_httpRedirection);
}

const std::string &LocationDirectives::getPath() const {
    return (_path);
}

/*  Setters */


void LocationDirectives::setHttpAcceptedMethod(const std::string& method) {
    _httpAcceptedMethods.insert(method);
}

void LocationDirectives::setHttpAcceptedMethods(const std::set<std::string>& acceptedMethods) {
   _httpAcceptedMethods = acceptedMethods; 
}

void LocationDirectives::setHttpRedirection(const u_int16_t& responseCode, const std::string& uri) {
    _httpRedirection.first = responseCode;
    _httpRedirection.second = uri;
}

void LocationDirectives::setPath(const std::string& path) {
    _path = path;
}
