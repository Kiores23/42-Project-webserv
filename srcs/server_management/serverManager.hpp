#ifndef WEBSERV_SERVERMANAGER_HPP
#define WEBSERV_SERVERMANAGER_HPP

#include <sys/epoll.h>
#include "../utils/TimerMap.hpp"

# define MAX_EVENTS 10		/* Max events received in one call of epoll_ctl() */
# define TIMEOUT_MS 1000	/* epoll_wait() timeout is milliseconds */
# define KEEP_ALIVE_PERSISTENT 0

int waitForEvents(epoll_event *events);
void treatEvents(int num_events, epoll_event *events);
bool hasEpollError(epoll_event &epoll_event, int clientSocket);

#endif // WEBSERV_SERVERMANAGER_HPP
