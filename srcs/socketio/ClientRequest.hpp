#ifndef WEBSERV_CLIENTREQUEST_HPP
#define WEBSERV_CLIENTREQUEST_HPP

#include "ClientData.hpp"

class ClientRequest : public ClientData {
public:
	ClientRequest();
	explicit ClientRequest(int clientSocket);
	ClientRequest(const ClientRequest &original);
	ClientRequest &operator=(const ClientRequest &original);
    ~ClientRequest();
};

#endif
