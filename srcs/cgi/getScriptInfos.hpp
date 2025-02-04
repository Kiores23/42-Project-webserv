#ifndef WEBSERV_GETSCRIPTINFOS_HPP
# define WEBSERV_GETSCRIPTINFOS_HPP

# include <string>
# include <set>
#include "../config/LocationConfig.hpp"

bool isSupportedScriptExtension(const std::string &targetFile, LocationConfig *pConfig);
std::string extractScriptPath(const std::string &targetFile, const LocationConfig &pConfig);

#endif // WEBSERV_CGIHANDLER_HPP
