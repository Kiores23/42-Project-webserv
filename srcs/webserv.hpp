#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <string>
# include <vector>
# include <sys/wait.h>
# include "networking/Server.hpp"
# include "logging/Logger.hpp"
# include "networking/MotherWebserv.hpp"
# include "server_management/signal.hpp"
# include "config/parsingConfigFile.hpp"
# include "server_management/serverManager.hpp"
# include "utils/TimerMap.hpp"

# define WEBSERV "webserv/1.0"
# define MAX_EVENTS 10

#endif // WEBSERV_HPP
