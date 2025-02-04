#ifndef WEBSERV_SERVERCONFIG_HPP
# define WEBSERV_SERVERCONFIG_HPP

# include "../utils/HostPort.hpp"
# include "../networking/Socket.hpp"
# include "../utils/ServerDirectives.hpp"
# include "LocationConfig.hpp"
# include "parsingUtils.hpp"

class LocationConfig;
class Socket;

class ServerConfig
{
    private:
	    Directives _heritage;
	    ServerDirectives _directives;
	    std::vector<LocationConfig *> _location;
        std::set<HostPort> _hostPorts;

    public:
        ServerConfig();
        ServerConfig(const ServerConfig& copy);
        ServerConfig& operator=(const ServerConfig& src);
        ~ServerConfig();

        const ServerDirectives &getDirectives() const;
        const std::vector<LocationConfig*> &getLocations() const;
        const std::set<HostPort> &getHostPorts() const;

	    bool checkValidServerDirective(const std::string& variableName);
	    void newBasicLocation();
        void newLocation(std::vector<std::string>::const_iterator &it, const std::vector<std::string>::const_iterator &endInfo, std::set<std::string> &pathLocations);
	    void fillData(std::vector<std::string>::const_iterator &it,
		            const std::vector<std::string>::const_iterator &endInfo);
    	void fillHeritageData();
        void setAutoindex(const std::string &value);
        void setClientMaxBodySize(const std::string &value);
        void setDirectiveMultipleParameter(const std::string &variableName, const std::string &line);
        void setDirectiveOneParameter(const std::string &variableName, const std::string &line);
        void setDirectiveTwoParameter(const std::string &variableName, const std::string &line);
        void setHeritage(const Directives& heritage);
        void setHostPort(const HostPort& hostPort);
        void setHostServerName(const std::string& host);
};

#endif
