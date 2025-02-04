#include "webserv.hpp"
#include "socketio/SocketIO.hpp"

bool handleArguments(int argc, char *const *argv, std::string &configFile)
{
	if (argc > 2)
	{
		Logger::log(ERROR, "Please give the name of the configuration file or no parameter");
		return false;
	}
	configFile = "./test/configuration-files/default.conf";
	if (argc == 2)
		configFile = static_cast<std::string>(argv[1]);
	return true;
}

bool parseConfig(const std::string &configFile)
{
	try
	{
		readConfigFile(configFile);
	}
	catch (std::runtime_error &e)
	{
		delete MotherWebserv::getInstance();
		return (false);
	}
	return (true);
}

void stopCgiOnTimeout(CGIHandler *cgiHandler)
{
	cgiHandler->killChild(false);
}

void stopCgi(CGIHandler *cgiHandler)
{
	cgiHandler->cleanChild(true);
}

void stopKeepAliveTimeout(int clientSocket)
{
	if (!SocketIO::getInstance()->hasNoPendingRequest(clientSocket))
	{
		MotherWebserv::getInstance()->getKeepAliveTimeExpiredFds().insert(clientSocket);
	}
	else
	{
		MotherWebserv::getInstance()->removeClientSocket(clientSocket);
	}
}

void waitChilds(void)
{
	int childStatus;
	int isChildTerminated;

	std::set<pid_t> &pids = MotherWebserv::getInstance()->getChildsZombies();
	if (pids.empty())
		return;
	std::set<pid_t>::iterator it = pids.begin();
	for (; it != pids.end();)
	{
		isChildTerminated = waitpid(*it, &childStatus, WNOHANG);
		if (isChildTerminated > 0)
		{
			std::set<pid_t>::iterator current = it;
			++it;
			MotherWebserv::getInstance()->getChildsZombies().erase(*current);
		}
		else {
			++it;
		}
	}
}

void webservLoop()
{
	int num_events;
	struct epoll_event events[MAX_EVENTS];
	while (MotherWebserv::getInstance()->webservisAlive())
	{
		num_events = waitForEvents(events);
		if (num_events == -1)
			break;
		treatEvents(num_events, events);
		waitChilds();
		MotherWebserv::getInstance()->getKeepAliveMap().timeOutChecker(stopKeepAliveTimeout);
		MotherWebserv::getInstance()->getCgiTimers().timeOutChecker(stopCgiOnTimeout);
	}
	MotherWebserv::getInstance()->getKeepAliveMap().clean(stopKeepAliveTimeout);
	MotherWebserv::getInstance()->getCgiTimers().clean(stopCgi);
}

int main(int argc, char **argv)
{
	std::string configFile;
	if (!handleArguments(argc, argv, configFile))
		return (EXIT_FAILURE);
	if (!parseConfig(configFile))
		return (EXIT_FAILURE);
	if (!MotherWebserv::getInstance()->createEpollInstance())
		return (EXIT_FAILURE);
	if (!MotherWebserv::getInstance()->setSockets())
	{
		delete MotherWebserv::getInstance();
		return (EXIT_FAILURE);
	}
	handleSignals();
	Logger::log(INFO, "webserv is ready.");
	webservLoop();
	Logger::log(INFO, "Closing webserv...");
	while (waitpid(-1, NULL, 0) > 0);

	std::set<int> clientsSocketConnectionClose = MotherWebserv::getInstance()->getClientsSocket();
		std::set<int>::iterator itClientsSocket = clientsSocketConnectionClose.begin();
	for (; itClientsSocket != clientsSocketConnectionClose.end(); ++itClientsSocket)
		MotherWebserv::getInstance()->removeClientSocket((*itClientsSocket));

	delete SocketIO::getInstance();
	delete MotherWebserv::getInstance();
	Logger::log(INFO, "webserv is now closed.");
	return (0);
}
