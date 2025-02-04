#ifndef WEBSERV_CGIPROCESS_HPP
#define WEBSERV_CGIPROCESS_HPP

#include "../socketio/HttpRequestContext.hpp"

void startCGIProcess(HttpRequestContext *hrc);
void readingCGIResponse(HttpRequestContext *hrc);

#endif //WEBSERV_CGIPROCESS_HPP
