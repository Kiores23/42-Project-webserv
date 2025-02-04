#include "ClientResponse.hpp"

ClientResponse::ClientResponse()
		: ClientData(-1)
{

}

ClientResponse::ClientResponse(int clientSocket)
		: ClientData(clientSocket)
{
}

ClientResponse::ClientResponse(const ClientResponse &original)
		: ClientData(-1)
{
	*this = original;
}

ClientResponse &ClientResponse::operator=(const ClientResponse &original)
{
	if (this == &original)
		return *this;
	_data = original._data;
	_clientSocket = original._clientSocket;
	_isAvailable = original._isAvailable;
	return *this;
}

ClientResponse::~ClientResponse()
{
}
