#include "ClientRequest.hpp"

ClientRequest::ClientRequest() : ClientData(-1)
{

}

ClientRequest::ClientRequest(int clientSocket) : ClientData(clientSocket)
{
}

ClientRequest::ClientRequest(const ClientRequest &original) : ClientData(-1)
{
	*this = original;
}

ClientRequest &ClientRequest::operator=(const ClientRequest &original)
{
	if (this == &original)
		return *this;
	this->_isAvailable = original._isAvailable;
	this->_clientSocket = original._clientSocket;
	this->_data = original._data;
	return *this;
}

ClientRequest::~ClientRequest()
{
}
