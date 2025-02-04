#ifndef WEBSERV_CLIENTRESPONSE_HPP
#define WEBSERV_CLIENTRESPONSE_HPP

#include "ClientData.hpp"

class ClientResponse : public ClientData
{
public:
	ClientResponse();
	explicit ClientResponse(int clientSocket);
	ClientResponse(const ClientResponse &original);
	ClientResponse &operator=(const ClientResponse &original);
	~ClientResponse();
};

#endif
