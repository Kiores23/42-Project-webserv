#ifndef WEBSERV_SERVERDIRECTIVES_HPP
# define WEBSERV_SERVERDIRECTIVES_HPP

# include "Directives.hpp"

class ServerDirectives : public Directives
{
    private:
	    std::set<std::string> _serverNames;

    public:
        ServerDirectives();
        ServerDirectives(const ServerDirectives& copy);
        ServerDirectives& operator=(const ServerDirectives& src);
        ~ServerDirectives();

        const std::set<std::string> &getServerNames() const;

        void setServerName(const std::string& serverName, size_t mode);
};

#endif
