#ifndef WEBSERV_CHECKCONFIG_HPP
#define WEBSERV_CHECKCONFIG_HPP

#include <string>
#include "parsingUtils.hpp"

void checkClientMaxBodySizeValidity(const std::string& str);

void checkMultiplyOverflow(const char& unit, const unsigned int& clientMaxBodySize);

#endif //WEBSERV_CHECKCONFIG_HPP
