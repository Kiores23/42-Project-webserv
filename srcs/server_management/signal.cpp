#include "signal.hpp"
#include "../socketio/SocketIO.hpp"
#include <csignal>

void	signalSigint(int sig)
{
	(void)sig;
	MotherWebserv::getInstance()->killWebserv();
}

void	signalSigpipe(int sig)
{
	(void)sig;
	SocketIO::setIsDeconnected(true);
}

void handleSignals()
{
	signal(SIGINT, signalSigint);
	signal(SIGPIPE, signalSigpipe);
}
