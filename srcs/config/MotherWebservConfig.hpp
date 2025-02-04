#ifndef WEBSERV_MOTHERWEBSERVCONFIG_HPP
# define WEBSERV_MOTHERWEBSERVCONFIG_HPP

# include "../utils/Directives.hpp"

class Server;
class Socket;
class Directives;

class MotherWebservConfig
{
    private:
		Directives _globalDirectives;
		std::map<int, Socket *> _globalMapSocket;
		std::vector<Server*> _servers;

    public:
        MotherWebservConfig();
        MotherWebservConfig(const MotherWebservConfig& copy);
        MotherWebservConfig& operator=(const MotherWebservConfig& src);
        ~MotherWebservConfig();

        const Directives &getGlobalDirectives() const;
		std::map<int, Socket *> &getMotherSocketsMap();
        const std::vector<Server*> &getServers() const;

		int	createServer(std::vector<std::string>::const_iterator &startInfo,
			const std::vector<std::string>::const_iterator &endInfo);
		void fillData(const std::vector<std::string>& vectorSplit);
		void setAutoindex(const std::string& value);
		void setCgiParam(const std::string& line);
		void setClientMaxBodySize(const std::string& value);
		void setGlobalDirectiveMultipleParameters(const std::string& variableName, const std::string& line);
		void setGlobalDirectiveOneParameter(const std::string& variableName, const std::string& line);
        void setSocket(Socket *socket);

};

#endif
