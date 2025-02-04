#ifndef WEBSERV_EXECCGISCRIPT_HPP
#define WEBSERV_EXECCGISCRIPT_HPP

#include <string>
#include "CGIHandler.hpp"

void stopCgiOnTimeout(CGIHandler *cgiHandler);
void stopKeepAliveTimeout(int clientSocket);
void executeCGIScript(std::string relativeScriptPath, char * const *envp);

#endif //WEBSERV_EXECCGISCRIPT_HPP
