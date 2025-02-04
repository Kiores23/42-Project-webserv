#ifndef WEBSERV_LOCATIONDIRECTIVES_HPP
# define WEBSERV_LOCATIONDIRECTIVES_HPP

# include "ServerDirectives.hpp"

class LocationDirectives : public ServerDirectives
{
    private:
        std::set<std::string> _httpAcceptedMethods;
    std::string _path;
    std::pair<u_int16_t, std::string> _httpRedirection;

    public:
        LocationDirectives();
        LocationDirectives(const LocationDirectives& copy);
        LocationDirectives& operator=(const LocationDirectives& src);
        ~LocationDirectives();

        const std::set<std::string> &getHttpAcceptedMethods() const;
        const std::pair<u_int16_t, std::string> &getHttpRedirection() const;
        const std::string &getPath() const;

	    void setHttpAcceptedMethod(const std::string& method);
	    void setHttpAcceptedMethods(const std::set<std::string>& acceptedMethods);
        void setHttpRedirection(const u_int16_t& responseCode, const std::string& uri);
	    void setPath(const std::string& path);
};

#endif
