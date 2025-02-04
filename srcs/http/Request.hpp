#ifndef WEBSERV_REQUEST_HPP
# define WEBSERV_REQUEST_HPP

# include "Body.hpp"
# include <iostream>
# include <cstring>
# include <unistd.h>
# include <algorithm>
# include <map>
# include <vector>
# include <list>
# include "../config/ServerConfig.hpp"
# include "../networking/MotherWebserv.hpp"
# include "BodyForm.hpp"

# define KEEP_ALIVE 1
# define CLOSE 2

int   logForError400(const std::string & logMessage);
int   logForError500(const std::string & logMessage);

class	Request
{
private:
	std::string										_method;
	std::string										_target;
	std::string										_relativeTarget;
	std::string										_httpVersion;
	std::map<std::string, std::list<std::string> >	_header;
	std::list<Body *>								_body;
	BodyForm										_bodyForm;
	int												_keepAlive;
	int												_timeKeepAlive;

	void	parsHeader(std::string & requestStr);
	void	parsBody(std::string & requestStr);
	void	parsMultipart(std::string & requestStr, const std::string & boundary);
	void	parsBodyParams(std::string & requestStr);

public:
	Request();
	Request(const Request& original);
	Request &operator=(const Request& original);
	~Request();

	std::string				getMethod() const;
	std::string				getTarget() const;
	std::string				getHttVersion() const;
	std::string 			getHeaderBack(const std::string &key) const;
	std::list<std::string>	getHeader(const std::string& key) const;
	Body					*getBody(int i) const;
	std::string 			getAuthScheme() const;
	size_t 					getBodySize() const;
	std::string 			getAuthUserId() const;
	int						getKeepAlive() const;
	int						getTimeKeepAlive() const;

    void		parsingHTTP(std::string requestStr, LocationConfig **locationConfig, ServerConfig &serverConfig);
    void		parsKeepAlive();
    void 		parsStartLine(std::string &requestStr);
    void		setRelativeTarget(const std::string &relativeTarget);
	BodyForm	*getBodyForm();
    std::string	&getRelativeTarget();
};

#endif // WEBSERV_REQUEST_HPP
