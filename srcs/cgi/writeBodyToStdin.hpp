#ifndef WEBSERV_WRITEBODYTOSTDIN_HPP
#define WEBSERV_WRITEBODYTOSTDIN_HPP

#include "../http/Request.hpp"

void passBodyToCGIScript(int outputPipeWrite, Request *request);

#endif //WEBSERV_WRITEBODYTOSTDIN_HPP
