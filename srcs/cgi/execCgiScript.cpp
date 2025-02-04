#include <cerrno>
#include <csignal>
#include "execCgiScript.hpp"
#include "../socketio/SocketIO.hpp"

void executeCGIScript(std::string relativeScriptPath, char * const *envp)
{
	MotherWebserv::getInstance()->childCloseFds();
    char *argv[] = {const_cast<char *>(relativeScriptPath.c_str()), NULL};
    execve(relativeScriptPath.c_str(), argv, envp);
    deleteCStringArray(envp);
	_exit(errno);
}
