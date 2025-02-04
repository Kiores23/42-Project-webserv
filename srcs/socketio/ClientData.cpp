#include "ClientData.hpp"

ClientData::ClientData() : _clientSocket(-1), _isAvailable(false), _headerEnded(false), _contentLength(-1)
{

}

ClientData::ClientData(int clientSocket)
		: _clientSocket(clientSocket), _isAvailable(false), _headerEnded(false), _contentLength(-1)
{
}

ClientData::ClientData(const ClientData &original) :
		_clientSocket(original._clientSocket), _isAvailable(original._isAvailable), _headerEnded(false), _contentLength(-1)
{
	*this = original;
}

ClientData &ClientData::operator=(const ClientData &original)
{
	if (this == &original)
		return *this;
	_isAvailable = original._isAvailable;
	_data = original._data;
	_clientSocket = original._clientSocket;
	_headerEnded = original._headerEnded;
	return *this;
}

ClientData::~ClientData()
{
}

int ClientData::clientSocket() const
{
	return _clientSocket;
}

std::string &ClientData::getData()
{
	return _data;
}

void ClientData::setData(const std::string &data)
{
	_data = data;
}

void ClientData::addData(char *buffer, size_t bytesRead)
{
	_data.append(buffer, bytesRead);
}

bool ClientData::isAvailable() const
{
	return _isAvailable;
}

void ClientData::setIsAvailable(bool isAvailable)
{
	_isAvailable = isAvailable;
}

bool ClientData::isHeaderEnded() const
{
	return _headerEnded;
}

void ClientData::setHeaderEnded(bool hasHeaderEnded)
{
	_headerEnded = hasHeaderEnded;
}

ssize_t ClientData::getContentLength() const
{
	return _contentLength;
}

void ClientData::setContentLength(ssize_t contentLength)
{
	_contentLength = contentLength;
}

