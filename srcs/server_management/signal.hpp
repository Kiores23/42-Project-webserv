#ifndef WEBSERV_SIGNAL_HPP
#define WEBSERV_SIGNAL_HPP

void    signalSigint(int sig);
void	signalSigpipe(int sig);
void	handleSignals();

#endif //WEBSERV_SIGNAL_HPP
