#include "execution.hpp"

// execution - index

int execution(const Request & request, LocationConfig & locationConfig, std::string & target)
{
	const std::string	& m = request.getMethod();
	int					r;

    if (m == "POST" && request.getBody(0)->getValue().empty())
		r = 204;
	else if (m == "POST" && locationConfig.getDirectives().getUploadPath())
		r = upload(request, locationConfig, target);
	else if (m == "DELETE")
		r = protectedRemove(target);
	else
		r = 200;
	return r;
}

// execution - utils

void verifAccess(LocationConfig & locationConfig, std::string & target, std::string method, bool isCGI)
{
	if(access(target.c_str(), F_OK) == -1)
		throw 404;
	struct stat	st;
	if (stat(target.c_str(), &st) != 0)
		throw logForError500("stat: " + target);
	setIndexRoot(locationConfig, target);

	if (method == "DELETE" && access(target.c_str(), W_OK) != -1)
		return;
	if (method == "DELETE" && access(target.c_str(), W_OK) == -1)
		throw 403;
	if (not isCGI && access(target.c_str(), R_OK) != -1)
		return;
	if (isCGI && access(target.c_str(), X_OK) != -1)
		return;
	throw 403;
}

void setIndexRoot(LocationConfig & locationConfig, std::string & target)
{
	if(access(target.c_str(), F_OK) == -1)
		return ;
	struct stat	st;
	if (stat(target.c_str(), &st) != 0)
		return ;
	if (S_ISDIR(st.st_mode))
	{
		target += "/";
		std::vector<std::string>::const_iterator	it = locationConfig.getDirectives().getIndexFiles().begin();
		std::vector<std::string>::const_iterator	ite = locationConfig.getDirectives().getIndexFiles().end();
		std::string	tmpTarget = target + *it;
		while (it != ite && (access(tmpTarget.c_str(), F_OK) == -1 || access(tmpTarget.c_str(), R_OK) == -1))
		{
			it++;
			if (it != ite)
				tmpTarget = target + *it;
			else
				break;
		}
		if (it == ite && !locationConfig.getDirectives().getAutoindex())
			return ;
		else if (it != ite)
			target = tmpTarget;
	}
}
