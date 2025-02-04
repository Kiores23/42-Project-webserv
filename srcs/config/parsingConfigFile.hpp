#ifndef WEBSERV_PARSINGCONFIGFILE_HPP
#define WEBSERV_PARSINGCONFIGFILE_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "../utils/HostPort.hpp"

# define MIN_PORT 1
# define MAX_PORT 65535

void readConfigFile(const std::string &configFilename);

HostPort extractHostPort(const std::string &line);

#endif //WEBSERV_PARSINGCONFIGFILE_HPP
