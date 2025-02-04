#include "MotherWebservConfig.hpp"
#include "../networking/Server.hpp"
#include "../networking/MotherWebserv.hpp"

MotherWebservConfig::MotherWebservConfig() {
	_globalDirectives.setAutoindex(std::pair<bool, bool>(false, false));
	_globalDirectives.setClientMaxBodySize(std::pair<bool, unsigned int>(false, CLIENT_MAX_BODY_SIZE_AUTHORISED));
	_globalDirectives.setErrorPage(400, "/error_pages/400.html");
	_globalDirectives.setErrorPage(403, "/error_pages/403.html");
	_globalDirectives.setErrorPage(404, "/error_pages/404.html");
	_globalDirectives.setErrorPage(405, "/error_pages/405.html");
	_globalDirectives.setErrorPage(408, "/error_pages/408.html");
	_globalDirectives.setErrorPage(413, "/error_pages/413.html");
	_globalDirectives.setErrorPage(500, "/error_pages/500.html");
	_globalDirectives.setIndexFile("index.html");
	_globalDirectives.setRoot("");
	_globalDirectives.setUploadFilesPath("");
	_globalDirectives.setUploadPathBool(false);
}

MotherWebservConfig::MotherWebservConfig(const MotherWebservConfig& copy)
  : _globalDirectives(copy._globalDirectives),
    _globalMapSocket(copy._globalMapSocket),
    _servers(copy._servers) {
}

MotherWebservConfig& MotherWebservConfig::operator=(const MotherWebservConfig& src) {
  if (this != &src) {
    _globalDirectives = src._globalDirectives;
    _globalMapSocket = src._globalMapSocket;
    _servers = src._servers;
  }
  return (*this);
}

MotherWebservConfig::~MotherWebservConfig() {
	for (std::vector<Server *>::iterator serverIt = _servers.begin(); serverIt != _servers.end(); ++serverIt) {
		delete *serverIt;
	}
	_servers.clear();
}

static bool checkValidMotherWebservDirective(const std::string& variableName) {
	std::set<std::string> acceptedDirectives;
	acceptedDirectives.insert("{");
	acceptedDirectives.insert("}");
	acceptedDirectives.insert("autoindex");
	acceptedDirectives.insert("cgi_param");
	acceptedDirectives.insert("client_max_body_size");
	acceptedDirectives.insert("error_page");
	acceptedDirectives.insert("index");
	acceptedDirectives.insert("root");
	acceptedDirectives.insert("server");
	acceptedDirectives.insert("upload");
	if (acceptedDirectives.find(variableName) == acceptedDirectives.end())
		return (false);
	return (true);
}

/*  Getters */

const Directives &MotherWebservConfig::getGlobalDirectives() const {
    return (_globalDirectives);
}

std::map<int, Socket *> &MotherWebservConfig::getMotherSocketsMap() {
    return (_globalMapSocket);
}

const std::vector<Server*> &MotherWebservConfig::getServers() const {
    return (_servers);
}

/*  Setters */

int	MotherWebservConfig::createServer(std::vector<std::string>::const_iterator &startInfo,
	const std::vector<std::string>::const_iterator &endInfo) {
	std::vector<std::string> values = extractValues(*startInfo);
	if (values.size() > 1) {
		Logger::log(ERROR, "invalid number of arguments on server directive");
		throw (std::runtime_error("no argument on server directive index or root or client_max_body_size"));
	}
	Server*	newServer = new Server(_globalDirectives);
	_servers.push_back(newServer);
    newServer->getConfig().fillData(startInfo, endInfo);
	return (0);
}

/**
 * Takes the vector of strings and assign each string to the correct data
 * structure for each block and directive
 * 
 * @param vectorSplit: vector of strings containing the config file content
 */
void MotherWebservConfig::fillData(const std::vector<std::string>& vectorSplit) {
	std::vector<std::string>::const_iterator end = vectorSplit.end();
	std::vector<std::string> httpContent;
	std::string	variableName = "";
	u_int16_t openedBracket = 0;

	//	Picks all the content for the webserv only
	for (std::vector<std::string>::const_iterator it = vectorSplit.begin(); it != vectorSplit.end(); ++it) {
		variableName = extractVariableName(*it);

		if (variableName == "server") {
			std::vector<std::string> values = extractValues(*it);
			if (values.size() > 0)
				Logger::throwAndLogRuntimeError("No argument accepted in directive: server");
			do {
				if (it < end)
					it += 1;
				if (*it == "{")
					openedBracket += 1;
				else if (*it == "}")
					openedBracket -= 1;
			} while (it < end && openedBracket > 0);
		}
		if (!checkValidMotherWebservDirective(variableName)) {
			Logger::log(ERROR, "Invalid http directive: ", variableName);
			throw (std::runtime_error("Invalid http directive"));
		}
		else
			httpContent.push_back(*it);
	}

	//	Simple Directives
	for (std::vector<std::string>::const_iterator it = httpContent.begin(); it != httpContent.end(); ++it) {
		variableName = extractVariableName(*it);
		if (variableName == "error_page" || variableName == "index")
			setGlobalDirectiveMultipleParameters(variableName, *it);
		else if (variableName == "autoindex" || variableName == "client_max_body_size" || variableName == "root" || variableName == "upload")
			setGlobalDirectiveOneParameter(variableName, *it);
		else if (variableName == "cgi_param")
			setCgiParam(*it);
	}

	//	Object Directives
	for (std::vector<std::string>::const_iterator it = vectorSplit.begin(); it != vectorSplit.end(); ++it) {
		variableName = extractVariableName(*it);
		if (variableName == "server")
			createServer(it, end);
	}
}



void MotherWebservConfig::setAutoindex(const std::string& value) {
	if (value == "on")
		_globalDirectives.setAutoindex(std::pair<bool, bool>(true, true));
	else if (value == "off")
		_globalDirectives.setAutoindex(std::pair<bool, bool>(true, false));
	else {
		Logger::log(ERROR, "invalid argument on http directive autoindex, should only be \"on\" or \"off\"");
		throw (std::runtime_error("invalid argument on http directive autoindex"));
	}
}

void MotherWebservConfig::setCgiParam(const std::string& line) {
	std::vector<std::string> values = extractValues(line);
	if (values.size() != 2) {
		Logger::log(ERROR, (values.size() == 0 ? "no argument" : (values.size() == 1 ? "too little argument" : "too many arguments")), " on http directive cgi_param");
		throw (std::runtime_error("no argument or too little/many arguments on http directive cgi_param"));
	}
	if (values[0] == "SCRIPT_FILENAME")
		_globalDirectives.setCgiScriptFilename(values[1]);
	else {
		Logger::log(ERROR, "cgi_param not supported");
		throw (std::runtime_error("cgi_param not supported"));
	}
}

void MotherWebservConfig::setClientMaxBodySize(const std::string& value) {
	checkClientMaxBodySizeValidity(value);

	unsigned int clientMaxBodySize = 0, kilobyte = 1024;
	unsigned int megabyte = 1024 * kilobyte;
	std::stringstream ss(value);
	ss >> clientMaxBodySize;
	char unit = *(value.rbegin());
	if (unit == 'k' || unit == 'K' || unit == 'm' || unit == 'M') {
		checkMultiplyOverflow(unit, clientMaxBodySize);
		switch (unit) {
			case 'k':
			case 'K':
			clientMaxBodySize *= kilobyte;
			break ;

			case 'm':
			case 'M':
			clientMaxBodySize *= megabyte;
			break ;
		}
	}
	_globalDirectives.setClientMaxBodySize(std::pair<bool, unsigned int>(true, clientMaxBodySize));
}

void MotherWebservConfig::setGlobalDirectiveMultipleParameters(const std::string& variableName, const std::string& line) {
	std::vector<std::string> values = extractValues(line);
	//	Checks there is at least one parameter for index or at least two parameters for error_page
	if ((variableName == "error_page" && values.size() < 2) || (variableName == "index" && values.size() < 1)) {
		Logger::log(ERROR, "no argument on http directive: ", variableName);
		throw (std::runtime_error("no argument on http directive: error_page or index"));
	}

	if (variableName == "error_page") {
		std::string uri = values.back();
		if (not checkDirectiveValues(uri)) {
			Logger::log(ERROR, "Invalid path (Alphanumerical, '/', '.', '-'. '_' or '~' only) on http directive: error_page");
			throw (std::runtime_error("Invalid path (Alphanumerical, '/', '.', '-'. '_' or '~' only) on http directive error_page"));
		}

		for (std::vector<std::string>::const_iterator it = values.begin(); it != (values.end() - 1); ++it) {
			if (!check_unsigned_integer16(*it)) {
				Logger::log(ERROR, "Invalid error code, should be an unsigned integer_16 on http directive: error_page");
				throw (std::runtime_error("Invalid error code, should be an unsigned integer_16 on http directive: error_page"));
			}
			std::stringstream ss(*it);
			u_int16_t errorPage = 0;
			ss >> errorPage;
			if (responseErrorCodeSupported(errorPage))
				_globalDirectives.setErrorPage(errorPage, uri);
			else {
				Logger::log(ERROR, "Unsupported response error code on http directive: error_page");
				throw (std::runtime_error("Unsupported response error code on http directive: error_page"));
			}
		}
	}
	else {
		for (std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it) {
			if (not checkDirectiveValues(*it)) {
				Logger::log(ERROR, "Invalid argument (Alphanumerical, '/', '.', '-'. '_' or '~' only) on http directive: index");
				throw (std::runtime_error("Invalid argument (Alphanumerical, '/', '.', '-'. '_' or '~' only) on http directive index"));
			}
			_globalDirectives.setIndexFile(*it);
		}
	}
}

/**
 * Checks if the global directive (autoindex or client_max_body_size) already exists,
 * then sets it (root gets replaced)
 * 
 * @param variableName
 * @param line: to extract the values from
 */
void MotherWebservConfig::setGlobalDirectiveOneParameter(const std::string& variableName, const std::string& line) {
	std::vector<std::string> values = extractValues(line);
	//	Checks number of arguments
	if (values.size() != 1) {
		Logger::log(ERROR, (values.size() == 0 ? "no " : "too many "), "argument on http directive: ", variableName);
		throw (std::runtime_error("no argument on http directive autoindex or client_max_body_size or root or upload"));
	}

	//	Checks if the global directive already exists
	if ((variableName == "autoindex" && _globalDirectives.getAutoindexParsing().first == true) ||
		(variableName == "client_max_body_size" && _globalDirectives.getClientMaxBodySizeParsing().first == true))
	{
		Logger::log(ERROR, "duplicate on http directive ", variableName);
		throw (std::runtime_error("http directive autoindex or client_max_body_size is duplicate"));
	}

	if (variableName == "autoindex")
		setAutoindex(values[0]);
	else if (variableName == "client_max_body_size")
			setClientMaxBodySize(values[0]);
	else {
		if (not checkDirectiveValues(values[0])) {
			Logger::log(ERROR, "Invalid argument (Alphanumerical, '/', '.', '-'. '_' or '~' only) on http directive: ", variableName);
			throw (std::runtime_error("Invalid argument (Alphanumerical, '/', '.', '-'. '_' or '~' only) on http directive root or upload"));
		}
		else if (variableName == "root")
			_globalDirectives.setRoot(values[0]);
		else
			_globalDirectives.setUploadFilesPath(values[0]);
	}
}



void MotherWebservConfig::setSocket(Socket *socket) {
    _globalMapSocket[socket->getSocketFD()] = socket;
}
