#include "writeBodyToStdin.hpp"
#include <cstdlib>
#include <sys/epoll.h>
#include <ctime>
# include <string>
# include <cstring>
# include <iostream>
# include <unistd.h>

static bool configureEpoll(const int *stdinPipe, epoll_event *events, int &cgiEpollFd)
{
	cgiEpollFd = epoll_create1(0);
	if (cgiEpollFd == -1)
		return false;
	events->events = EPOLLIN | EPOLLOUT;
	events->data.fd = stdinPipe[1];
	if (epoll_ctl(cgiEpollFd, EPOLL_CTL_ADD, stdinPipe[1], events) == -1)
		return false;
	return true;
}

static bool writeBodyToStdin(int *stdinPipe, std::string bodyString)
{
	struct epoll_event events[MAX_EVENTS];
	int cgiEpollFd;
	size_t bytesSent = 0;

	if (!configureEpoll(stdinPipe, events, cgiEpollFd))
		return (false);

	const std::time_t startTime = time(NULL);
	if (startTime == -1)
		return (false);
	int sender = stdinPipe[1];
	while (true)
	{
		size_t toSendSize = bodyString.size();
		const char *body = bodyString.c_str();
		int num_events = epoll_wait(cgiEpollFd, events, 1, -1);
		if (num_events == -1 || events->events & EPOLLERR || events->events & EPOLLHUP)
		{
			epoll_ctl(cgiEpollFd, EPOLL_CTL_DEL, sender, events);
			return (false);
		}
		if (events->events & EPOLLOUT)
		{
			ssize_t bytesRead = write(sender, body, toSendSize);
			if (bytesRead == 0)
			{
				if (epoll_ctl(cgiEpollFd, EPOLL_CTL_DEL, sender, events) == -1)
					return (false);
				break;
			}
			if (bytesRead == -1)
			{
				epoll_ctl(cgiEpollFd, EPOLL_CTL_DEL, sender, events);
				return (false);
			}
			bytesSent += static_cast<size_t>(bytesRead);
			bodyString = bodyString.substr(bytesSent);
			if (bytesSent < bodyString.size())
				continue;
			break;
		}
	}
	return (true);
}

/**
 * @throw logForError500
 */
void passBodyToCGIScript(int outputPipeWrite, Request *request)
{
	int stdinPipe[2];

	if (pipe(stdinPipe) == -1)
	{
		close(outputPipeWrite);
		_exit(EXIT_FAILURE);
	}
	dup2(stdinPipe[0], STDIN_FILENO);
	close(stdinPipe[0]);
	const Body *bodyObj = request->getBody(0);
	const std::string bodyString = bodyObj->getValue();
	if (bodyString.empty())
	{
		close(stdinPipe[1]);
		return;
	}

	int hasWriteStdin = writeBodyToStdin(stdinPipe, bodyString);
	close(stdinPipe[1]);
	if (not hasWriteStdin)
	{
		close(outputPipeWrite);
		_exit(EXIT_FAILURE);
	}
}
