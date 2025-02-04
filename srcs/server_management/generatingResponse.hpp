#ifndef WEBSERV_GENERATINGRESPONSE_HPP
#define WEBSERV_GENERATINGRESPONSE_HPP

#include "../socketio/HttpRequestContext.hpp"

void switchToGeneratingResponse(HttpRequestContext *hrc);
void startGeneratingResponse(HttpRequestContext *hrc, int clientSocket);
std::string generateErrorResponse(LocationConfig *locationConfig, ServerConfig *serverConfig, bool bodyRequired, int e);

#endif //WEBSERV_GENERATINGRESPONSE_HPP
