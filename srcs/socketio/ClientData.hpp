#ifndef WEBSERV_CLIENTDATA_HPP
#define WEBSERV_CLIENTDATA_HPP

#include <string>
#include <cstdio>

class ClientData
{
protected:
	int _clientSocket;  /* Client socket's file descriptor */
	std::string _data;          /* The data to handle */
	bool _isAvailable;   /* Is the data ready to be handled */
	bool _headerEnded;
	ssize_t _contentLength;

public:
	ClientData();
	explicit ClientData(int clientSocket);
	ClientData(const ClientData &original);
	ClientData &operator=(const ClientData &original);
	~ClientData();

	int clientSocket() const;
	std::string &getData();
	void setData(const std::string &data);
	void addData(char *buffer, size_t bytesRead);

	bool isAvailable() const;

	void setIsAvailable(bool isAvailable);
	bool isHeaderEnded() const;
	void setHeaderEnded(bool hasHeaderEnded);
	ssize_t getContentLength() const;
	void setContentLength(ssize_t contentLength);
};

#endif
