#ifndef WEBSERV_SENDINGRESPONSE_HPP
#define WEBSERV_SENDINGRESPONSE_HPP

#include "../socketio/HttpRequestContext.hpp"

int sendingResponse(HttpRequestContext *hrc, int clientSocket);
void switchToSendingResponse(HttpRequestContext *hrc, const std::string &response);

#endif //WEBSERV_SENDINGRESPONSE_HPP
