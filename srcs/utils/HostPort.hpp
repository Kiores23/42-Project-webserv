#ifndef WEBSERV_HOSTPORT_HPP
#define WEBSERV_HOSTPORT_HPP

#include <string>
#include <cmath>
#include "../config/parsingUtils.hpp"

class HostPort
{
	private:
		std::string _host;
		int _port;
	
	public:
		HostPort();
    HostPort(const HostPort& copy);
    HostPort& operator=(const HostPort& src);
		~HostPort();

		const std::string& getHost() const;
		const int& getPort() const;

		void setHost(const std::string& host);
		void setPort(const int& port);

		bool operator==(const HostPort& value) const;
		bool operator<(const HostPort& value) const;

};


#endif //WEBSERV_HOSTPORT_HPP
