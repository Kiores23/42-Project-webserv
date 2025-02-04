#ifndef WEBSERV_Response_HPP
# define WEBSERV_Response_HPP

# include "Request.hpp"
# include "header.hpp"
# include "../execution/upload.hpp"
# include "../execution/execution.hpp"
# include "../logging/Logger.hpp"
# include "../networking/Server.hpp"
# include "../networking/MotherWebserv.hpp"
# include "../cgi/CGIHandler.hpp"
# include "../http/BodyForm.hpp"
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include <map>

# define HTTPv "HTTP/1.1"
# define C100 "100 Continue"
# define C200 "200 OK"
# define C201 "201 Created"
# define C202 "202 Accepted"
# define C204 "204 No content"
# define C301 "301 Moved Permanently"
# define C308 "308 Moved Permanently"
# define C400 "400 Bad Request"
# define C403 "403 Forbidden"
# define C404 "404 Not Found"
# define C405 "405 Not Allowed"
# define C408 "408 Request Timeout"
# define C413 "413 Payload Too Large"
# define C500 "500 Internal Server Error"

std::string generateResponse(const LocationConfig &locationConfig, const int code, const bool bodyRequired);
std::string generateResponse(const ServerConfig &serverConfig, const int errorCode, const bool bodyRequired);
std::string generateResponse(Request *request, LocationConfig &locationConfig, const bool bodyRequired);
std::string	generateResponseCGI(std::string & cgiResponse, int code, Request * request);

// UTILS

const std::string		listingDirectory(const Request &request, const std::string &target);
void					checkMethodAllowed(LocationConfig &locationConfig, const Request &request);
void					checkServerNames(LocationConfig &locationConfig, const Request &request);
void                    checkBodyForm(BodyForm * bodyForm);
int						logForError500(const std::string &logMessage);
std::string				error500Page();
std::string             generateError500(const bool bodyRequired);

#endif //WEBSERV_Response_HPP
