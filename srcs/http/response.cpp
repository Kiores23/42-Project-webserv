#include "response.hpp"

// generateResponse - include

static std::string				getResponseStartLine(const int ResponseStatusCode);
static std::string 				generateBody(const Request &request, const std::string &pagePath, const bool bodyRequired);
static std::string				getPageContent(const std::string & pagePath);
template<typename T>
static std::string 				joinHeader(const std::vector<T>& vec, const std::string delimiter);

// generateResponse - index

std::string generateError500(const bool bodyRequired)
{
	const std::string				startLine = getResponseStartLine(500);
	std::string			            body;
	if (bodyRequired)
		body = error500Page();
	const std::vector<std::string>	header = getResponseHeader(body);

	const std::string	Response = startLine + "\r\n" + joinHeader(header, "\r\n") + "\r\n" + body;
	return Response;
}

std::string	generateResponse(Request *request, LocationConfig & locationConfig, const bool bodyRequired)
{
	checkServerNames(locationConfig, *request);
    checkMethodAllowed(locationConfig, *request);
	const int						r = execution(*request, locationConfig, request->getRelativeTarget());
	const std::string				startLine = getResponseStartLine(r);
	const std::string				body = generateBody(*request, request->getRelativeTarget(), bodyRequired);
	std::vector<std::string>		header = getResponseHeader(body);
	if (request->getHeaderBack("Content-Type") == "application/x-www-form-urlencoded")
		checkBodyForm(request->getBodyForm());
	const std::string	Response = startLine + "\r\n" + joinHeader(header, "\r\n") + "\r\n" + body;
	return Response;
}

/**
 *
 * @param cgiResponse Response content from CGI script, with headers or not.
 * @param code HTTP response code from CGI script.
 * @return Formatted HTTP response to server's client.
 */
std::string	generateResponseCGI(std::string & cgiResponse, int code, Request * request)
{
	const std::string			startLine = getResponseStartLine(code);
	std::map<std::string, std::list<std::string> >		headerMap;
	parsHeaderCgi(cgiResponse, headerMap);
	std::vector<std::string>	header;
	std::vector<std::string>	headerFinal;
	commonHeader(header, cgiResponse);
	for (std::vector<std::string>::iterator it = header.begin(); it != header.end(); it++)
		headerFinal.push_back(*it);
	for (std::map<std::string, std::list<std::string> >::iterator it = headerMap.begin(); it != headerMap.end(); it++)
	{
		for (std::list<std::string>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
			headerFinal.push_back(it->first + ": " + *it2);
	}
	if (request->getHeaderBack("Content-Type") == "application/x-www-form-urlencoded")
		checkBodyForm(request->getBodyForm());
	const std::string	Response = startLine + "\r\n" + joinHeader(headerFinal, "\r\n") + "\r\n" + cgiResponse;
	return Response;
}

std::string	generateResponse(const LocationConfig & locationConfig, const int code, const bool bodyRequired)
{
	try {
		const std::string				startLine = getResponseStartLine(code);
        std::string			            body;
        if (bodyRequired && code / 100 != 3)
		    body = getPageContent(locationConfig.getDirectives().getRoot() \
									+ locationConfig.getDirectives().getErrorCodePath(code));
		const std::vector<std::string>	header = getResponseHeader(locationConfig, code, body);

		const std::string	Response = startLine + "\r\n" + joinHeader(header, "\r\n") + "\r\n" + body;
		return Response;
	}
	catch (const std::exception& ex)
	{
		return generateError500(bodyRequired);

	}
	catch (const int e)
	{
		return generateError500(bodyRequired);
	}
}

std::string	generateResponse(const ServerConfig & serverConfig, const int errorCode, const bool bodyRequired)
{
	try {
		const std::string				target = serverConfig.getDirectives().getRoot() \
													+ serverConfig.getDirectives().getErrorCodePath(errorCode);
		const std::string				startLine = getResponseStartLine(errorCode);
		std::string			            body;
        if (bodyRequired)
		    body = getPageContent(target);
		const std::vector<std::string>	header = getResponseHeader(body);

		const std::string	Response = startLine + "\r\n" + joinHeader(header, "\r\n") + "\r\n" + body;
		return Response;
	}
	catch (const std::exception& ex)
	{
		return generateError500(bodyRequired);
	}
	catch (const int e)
	{
		return generateError500(bodyRequired);
	}
}
//

static std::string	getResponseStartLine(const int ResponseStatusCode)
{
	std::string	rsl = HTTPv;
	rsl.append(" ");

	switch (ResponseStatusCode)
	{
		case 100: rsl += C100; break;
		case 200: rsl += C200; break;
		case 201: rsl += C201; break;
		case 202: rsl += C202; break;
		case 204: rsl += C204; break;
		case 301: rsl += C301; break;
		case 308: rsl += C308; break;
		case 400: rsl += C400; break;
		case 403: rsl += C403; break;
		case 404: rsl += C404; break;
		case 405: rsl += C405; break;
		case 408: rsl += C408; break;
		case 413: rsl += C413; break;
		case 500: rsl += C500; break;
		default: {
			throw logForError500("Non supported HTTP code in response, sending 500");
		}
	}
	return rsl;
}

static std::string generateBody(const Request & request, const std::string & target, const bool bodyRequired)
{
	struct stat	st = {};
    int fileStat = stat(target.c_str(), &st);
	if (request.getMethod() != "DELETE" && fileStat != 0)
		throw logForError500("stat: " + target);
	else if (bodyRequired && S_ISDIR(st.st_mode))
	{
        return listingDirectory(request, target);
    }
	else if (bodyRequired)
		return getPageContent(target);
	return "";
}

static std::string getPageContent(const std::string & pagePath)
{
	std::ifstream pageFile;
	std::stringstream content;
	std::string line;

	pageFile.open(pagePath.c_str());
	if (!pageFile.is_open())
		throw logForError500("Cannot open file: " + pagePath);
	while (getline(pageFile, line))
		content << line << "\n";
	pageFile.close();
	return (content.str());
}

template<typename T>
std::string joinHeader(const std::vector<T>& vec, const std::string delimiter) {
    std::ostringstream oss;

    if (!vec.empty()) {
        oss << vec[0] << delimiter;
        for (size_t i = 1; i < vec.size(); ++i) {
            oss << vec[i] << delimiter;
		}
    }
    return oss.str();
}
