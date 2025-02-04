#include "response.hpp"

const std::string	listingDirectory(const Request &request, const std::string &target)
{
	std::string body;
	body.append("<!DOCTYPE HTML><html><head><title>Index of ");
	body.append(request.getTarget());
	body.append("</title></head><body><h1>Index of ");
	body.append(request.getTarget());
	body.append("</h1><hr>");

	struct dirent *dirFile;
	body.append("<pre><a href=\"");
    body.append(request.getTarget());
    body.append("\">.</a></pre>");
	body.append("<pre><a href=\"");
    body.append(request.getTarget());
	if (request.getTarget()[request.getTarget().size() - 1] != '/')
        body.append("/");
    body.append("..\">..</a></pre>");
	DIR *directory = opendir(target.c_str());
	while ((dirFile = readdir(directory)) != NULL)
	{
		if ((dirFile->d_name[0] != '.' || dirFile->d_name[1] != '\0')
			&& (dirFile->d_name[0] != '.' || dirFile->d_name[1] != '.' || dirFile->d_name[2] != '\0'))
		{
        	body.append("<pre><a href=\"");
        	body.append(request.getTarget());
			if (request.getTarget()[request.getTarget().size() - 1] != '/')
        		body.append("/");
        	body.append(dirFile->d_name);
        	body.append("\">");
        	body.append(dirFile->d_name);
        	body.append("</a></pre>");
		}
	}
	closedir(directory);
	body.append("<hr><address>");
	body.append(WEBSERV);
	body.append("</address></body></html>\r\n");
	return body;
}

void checkMethodAllowed(LocationConfig &locationConfig, const Request &request)
{
	std::set<std::string>::const_iterator it = locationConfig.getDirectives().getHttpAcceptedMethods().begin();
	std::set<std::string>::const_iterator ite = locationConfig.getDirectives().getHttpAcceptedMethods().end();
	for (; request.getMethod() != *it && it != ite; it++);
	if (it == ite)
		throw 405;
}

void checkServerNames(LocationConfig &locationConfig, const Request &request)
{
	std::set<std::string>::const_iterator	it = locationConfig.getDirectives().getServerNames().begin();
	std::set<std::string>::const_iterator	ite = locationConfig.getDirectives().getServerNames().end();
	std::string								host = request.getHeaderBack("Host");
	size_t									i = 0;
	if (host[0] == ':')
		throw logForError400("Wrong host");
	for (; host[i] && host[i] != ':'; i++);
	host = host.substr(0, i);
	if (host == "localhost")
		host = LOCALHOST_ADDRESS;
    for (; it != ite && host != *it; it++) {}
	if (it == ite)
		throw 404;
}

void checkBodyForm(BodyForm * bodyForm)
{
	if (!bodyForm->getValue("infomessage").empty())
		Logger::log(INFO, bodyForm->getValue("infomessage"));
	if (!bodyForm->getValue("debugmessage").empty())
		Logger::log(DEBUG, bodyForm->getValue("debugmessage"));
	if (!bodyForm->getValue("warningmessage").empty())
		Logger::log(DEBUG, bodyForm->getValue("warningmessage"));
	if (!bodyForm->getValue("errormessage").empty())
		Logger::log(ERROR, bodyForm->getValue("errormessage"));
	if (!bodyForm->getValue("testcode").empty())
		throw std::atoi(bodyForm->getValue("testcode").c_str());
}

// Error 500

int	logForError500(const std::string & logMessage)
{
	Logger::log(ERROR, "500 Internal Server Error: " + logMessage);
	return 500;
}

std::string	error500Page()
{
	std::string page500;
	page500.assign("<html>\n<head>\n<title>Error 500 - Internal Server Error</title>\n</head>\n<body>\n");
	page500.append("<h1>Error 500 - Internal Server Error</h1>\n");
	page500.append("<p>Sorry, something went wrong on our end. We are working to fix it.</p>\n");
	page500.append("</body>\n</html>\n");
	return page500;
}
