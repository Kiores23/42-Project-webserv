#include "ServerConfig.hpp"
#include "../networking/MotherWebserv.hpp"

ServerConfig::ServerConfig() {
	_directives.setAutoindex(std::pair<bool, bool>(false, false));
	_directives.setClientMaxBodySize(std::pair<bool, bool>(false, CLIENT_MAX_BODY_SIZE_AUTHORISED));
//	_directives.setServerName("");
	_directives.setRoot("");
	_directives.setUploadFilesPath("");
	_directives.setUploadPathBool(false);
}

ServerConfig::ServerConfig(const ServerConfig& copy) {
	*this = copy;
}

ServerConfig& ServerConfig::operator=(const ServerConfig& original) {
  if (this == &original)
	  return *this;
    _heritage = original._heritage;
    _directives = original._directives;
	if (!_location.empty())
	{
		for (std::vector<LocationConfig *>::iterator it = _location.begin(); it != _location.end(); ++it)
		{
			delete *it;
		}
		_location.clear();
	}
	if (!original._location.empty())
	{
		for (std::vector<LocationConfig *>::const_iterator it = original._location.begin(); it != original._location.end(); it++)
		{
			this->_location.push_back(new LocationConfig(**it));
		}
	}
    _hostPorts = original._hostPorts;
  return (*this);
}

ServerConfig::~ServerConfig()
{
	for (std::vector<LocationConfig*>::iterator it = _location.begin(); it != _location.end(); ++it) {
		delete *it;
	}
	_location.clear();
}

/*  Getters */

const ServerDirectives &ServerConfig::getDirectives() const {
    return (_directives);
}

const std::vector<LocationConfig*> &ServerConfig::getLocations() const {
    return (_location);
}

const std::set<HostPort> &ServerConfig::getHostPorts() const {
	return (_hostPorts);
}

/*  Checker */

bool ServerConfig::checkValidServerDirective(const std::string& variableName) {
	std::set<std::string> acceptedDirectives;
	acceptedDirectives.insert("{");
	acceptedDirectives.insert("}");
	acceptedDirectives.insert("autoindex");
	acceptedDirectives.insert("cgi_param");
	acceptedDirectives.insert("client_max_body_size");
	acceptedDirectives.insert("error_page");
	acceptedDirectives.insert("index");
	acceptedDirectives.insert("listen");
	acceptedDirectives.insert("location");
	acceptedDirectives.insert("root");
	acceptedDirectives.insert("server_name");
	acceptedDirectives.insert("upload");
	if (acceptedDirectives.find(variableName) == acceptedDirectives.end())
		return (false);
	return (true);
}

/*  Setters */

/**
 * Creates a new location context inside the existing server context
 * 
 * @param it: iterator on the vector of strings starting from the new location context
 * @param endInfo: iterator on the end of the vector of strings
 */
void ServerConfig::newLocation(std::vector<std::string>::const_iterator& it,
	const std::vector<std::string>::const_iterator& endInfo, std::set<std::string> &pathLocations) {
	std::vector<std::string> values = extractValues(*it);
	if (values.size() != 1) {
		Logger::log(ERROR, (values.size() == 0 ? "Too little" : "Too many"), " arguments in server directive: location");
		throw (std::runtime_error("Too little/many arguments in server directive: location"));
	}
	if (pathLocations.find(values[0]) != pathLocations.end()) {
		Logger::log(ERROR, "path of the location already exists, would create unintended behavior, program stoped");
		throw (std::runtime_error("path of the location already exists, would create unintended behavior, program stoped"));
	}
	else if (not checkDirectiveValues(values[0])) {
		Logger::log(ERROR, "Invalid path (Alphanumerical, '/', '.', '-'. '_' or '~' only) on server directive: location");
		throw (std::runtime_error("Invalid path (Alphanumerical, '/', '.', '-'. '_' or '~' only) on server directive location"));
	}
	else
		pathLocations.insert(values[0]);
	LocationConfig*	newLocation = new LocationConfig(_directives);

	_location.push_back(newLocation);
	(*newLocation).fillData(it, endInfo, values[0]);
}

void ServerConfig::newBasicLocation() {
	LocationConfig* newLocation = new LocationConfig(_directives);

	_location.push_back(newLocation);
	std::vector<std::string> basicLocation;
	basicLocation.push_back("{");
	basicLocation.push_back("}");
	std::vector<std::string>::const_iterator begin = basicLocation.begin(), end = basicLocation.end();
	(*newLocation).fillData(begin, end, "/");
}

/**
 * Fills the Server data, listen is a particular directive, it creates
 * a new socket for it
 * 
 * @param it: const_iterator on the string vector containing the config file from the start of the server context
 * @param endInfo: const_iterator on the end of the string vector
 */
void ServerConfig::fillData(std::vector<std::string>::const_iterator& it,
	const std::vector<std::string>::const_iterator& endInfo) {
	std::vector<std::string>::const_iterator it3 = it;
	std::string variableName;
	std::vector<std::string> serverContent;
	int	openedBracket = 0;

	//	Picks all the content for the server only
	do {
		if (it3 < endInfo)
			it3 += 1;
		if (*it3 == "{")
			openedBracket += 1;
		else if (*it3 == "}")
			openedBracket -= 1;
		variableName = extractVariableName(*it3);

		if (variableName == "location") {
			do {
				if (it3 < endInfo)
					it3 += 1;
				if (*it3 == "{")
					openedBracket += 1;
				else if (*it3 == "}")
					openedBracket -= 1;
			} while (it3 < endInfo && openedBracket > 1);
		}
		if (!checkValidServerDirective(variableName)) {
			Logger::log(ERROR, "Invalid server directive: ", variableName);
			throw (std::runtime_error("Invalid server directive"));
		}
		else
			serverContent.push_back(*it3);
	} while (it3 < endInfo && openedBracket > 0);

	//	Simple Directives
	bool listenExists = false;
	for (std::vector<std::string>::const_iterator it2 = serverContent.begin(); it2 != serverContent.end(); ++it2) {
		variableName = extractVariableName(*it2);
		if (variableName == "listen") {
			setHostPort(extractHostPort(*it2));
			listenExists = true;
		}
		else if (variableName == "error_page" || variableName == "index" || variableName == "server_name")
			setDirectiveMultipleParameter(variableName, *it2);
		else if (variableName == "autoindex" || variableName == "client_max_body_size" || variableName == "root" || variableName == "upload")
			setDirectiveOneParameter(variableName, *it2);
		else if (variableName == "cgi_param")
			setDirectiveTwoParameter(variableName, *it2);
	}
	if (not listenExists) {
		HostPort defaultHostPort;
		setHostPort(defaultHostPort);
	}

	//	Fills the data from the heritage directives
	fillHeritageData();

	//	Object Directives
	openedBracket = 0;
	std::set<std::string> pathLocations;
	do {
		if (it < endInfo)
			it += 1;
		if (*it == "{")
			openedBracket += 1;
		else if (*it == "}")
			openedBracket -= 1;
		else {
			variableName = extractVariableName(*it);
			if (variableName == "location")
				newLocation(it, endInfo, pathLocations);
		}
	} while (it < endInfo && openedBracket > 0);
	
	//	Check if any basic Location
	bool basicLocation = false;
	for (std::vector<LocationConfig*>::const_iterator it2 = getLocations().begin(); it2 != getLocations().end(); ++it2) {
		if ((*it2)->getDirectives().getPath() == "/") {
			basicLocation = true;
			break;
		}
	}
	if (basicLocation == false)
		newBasicLocation();
}

void ServerConfig::fillHeritageData() {
	if (_directives.getAutoindexParsing().first == false)
		_directives.setAutoindex(_heritage.getAutoindexParsing());
	if (_directives.getCgiScriptFilenames().empty())
		_directives.setCgiScriptFilenames(_heritage.getCgiScriptFilenames());
	if (_directives.getClientMaxBodySizeParsing().first == false)
		_directives.setClientMaxBodySize(_heritage.getClientMaxBodySizeParsing());
	std::map<int, std::string> errorPages = _heritage.getErrorPages();
	for (std::map<int, std::string>::const_iterator it = errorPages.begin(); it != errorPages.end(); ++it) {
		_directives.addErrorPage(*it);
	}
	if (_directives.getIndexFiles().empty()) {
		std::vector<std::string> indexFiles = _heritage.getIndexFiles();
		for (std::vector<std::string>::const_iterator it = indexFiles.begin(); it != indexFiles.end(); ++it) {
			_directives.setIndexFile(*it);
		}
	}
	if (_directives.getRoot().empty())
		_directives.setRoot(_heritage.getRoot());
	if (_directives.getUploadFilesPath().empty() && _heritage.getUploadPath() == true)
		_directives.setUploadFilesPath(_heritage.getUploadFilesPath());
}

/**
 * Checks the validity of the autoindex value and turns the directory listing "on" or "off"
 * inside the server context
 * 
 * @param value
 */
void ServerConfig::setAutoindex(const std::string& value) {
	if (value == "on")
		_directives.setAutoindex(std::pair<bool, bool>(true, true));
	else if (value == "off")
		_directives.setAutoindex(std::pair<bool, bool>(true, false));
	else {
		Logger::log(ERROR, "invalid argument on server directive autoindex, should only be \"on\" or \"off\"");
		throw (std::runtime_error("invalid argument on server directive autoindex"));
	}
} 

/**
 * Sets the client max body size of the server context
 * 
 * @param values: containing the values of the line, this function only utilizes the first one
 * and it should be the only one existing
 */
void ServerConfig::setClientMaxBodySize(const std::string& value) {
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
	_directives.setClientMaxBodySize(std::pair<bool, unsigned int>(true, clientMaxBodySize));
}

void ServerConfig::setDirectiveMultipleParameter(const std::string& variableName, const std::string& line) {
	std::vector<std::string> values = extractValues(line);
	//	Checks there is at least one parameter for index or at least two parameters for error_page
	if ((variableName == "error_page" && values.size() < 2) || ((variableName == "index" || variableName == "server_name") && values.size() < 1)) {
		Logger::log(ERROR, "no argument on server directive: ", variableName);
		throw (std::runtime_error("no argument on server directive: error_page or index or server_name"));
	}

	if (variableName == "error_page") {
		std::string uri = values.back();
		if (not checkDirectiveValues(uri)) {
			Logger::log(ERROR, "Invalid path (Alphanumerical, '/', '.', '-'. '_' or '~' only) on server directive: error_page");
			throw (std::runtime_error("Invalid path (Alphanumerical, '/', '.', '-'. '_' or '~' only) on server directive error_page"));
		}

		for (std::vector<std::string>::const_iterator it = values.begin(); it != (values.end() - 1); ++it) {
			if (!check_unsigned_integer16(*it)) {
				Logger::log(ERROR, "Invalid error code, should be an unsigned integer_16 on server directive: error_page");
				throw (std::runtime_error("Invalid error code, should be an unsigned integer_16 on server directive: error_page"));
			}
			std::stringstream ss(*it);
			u_int16_t errorPage = 0;
			ss >> errorPage;
			if (responseErrorCodeSupported(errorPage))
				_directives.setErrorPage(errorPage, uri);
			else {
				Logger::log(ERROR, "Unsupported response error code on server directive: error_page");
				throw (std::runtime_error("Unsupported response error code on server directive: error_page"));
			}
		}
	}
	else if (variableName == "index") {
		for (std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it) {
			if (not checkDirectiveValues(*it)) {
				Logger::log(ERROR, "Invalid path (Alphanumerical, '/', '.', '-'. '_' or '~' only) on server directive: index");
				throw (std::runtime_error("Invalid path (Alphanumerical, '/', '.', '-'. '_' or '~' only) on server directive index"));
			}
			_directives.setIndexFile(*it);
		}
	}
	else {
		for (std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it) {
			if (not checkDirectiveValues(*it)) {
				Logger::log(ERROR, "Invalid path (Alphanumerical, '/', '.', '-'. '_' or '~' only) on server directive: server_name");
				throw (std::runtime_error("Invalid path (Alphanumerical, '/', '.', '-'. '_' or '~' only) on server directive server_name"));
			}
			_directives.setServerName(*it, 0);
		}
	}
}

void ServerConfig::setDirectiveOneParameter(const std::string& variableName, const std::string& line) {
	std::vector<std::string> values = extractValues(line);
	//	Checks number of arguments
	if (values.size() != 1) {
		Logger::log(ERROR, (values.size() == 0 ? "no " : "too many "), "argument on server directive: ", variableName);
		throw (std::runtime_error("no argument on server directive autoindex or client_max_body_size or root or upload"));
	}

	//	Checks if the global directive already exists
	if ((variableName == "autoindex" && _directives.getAutoindexParsing().first == true) ||
		(variableName == "client_max_body_size" && _directives.getClientMaxBodySizeParsing().first == true))
	{
		Logger::log(ERROR, "duplicate on server directive ", variableName);
		throw (std::runtime_error("server directive autoindex or client_max_body_size is duplicate"));
	}

	//	Global directive does not exist
	if (variableName == "autoindex")
		setAutoindex(values[0]);
	else if (variableName == "client_max_body_size")
		setClientMaxBodySize(values[0]);
	else {
		if (not checkDirectiveValues(values[0])) {
			Logger::log(ERROR, "Invalid argument (Alphanumerical, '/', '.', '-'. '_' or '~' only) on server directive: ", variableName);
			throw (std::runtime_error("Invalid argument (Alphanumerical, '/', '.', '-'. '_' or '~' only) on server directive root or upload"));
		}
		if (variableName == "root")
			_directives.setRoot(values[0]);
		else
			_directives.setUploadFilesPath(values[0]);
	}
}

void ServerConfig::setDirectiveTwoParameter(const std::string& variableName, const std::string& line) {
	std::vector<std::string> values = extractValues(line);
	if (values.size() != 2) {
		Logger::log(ERROR, (values.size() == 0 ? "no argument" : (values.size() == 1 ? "too little argument" : "too many arguments")), " on server directive: ", variableName);
		throw (std::runtime_error("no argument or too little/many arguments on server directive cgi_param"));
	}
    if (values[0] == "SCRIPT_FILENAME")
        _directives.setCgiScriptFilename(values[1]);
    else {
        Logger::log(ERROR, "cgi_param not supported");
        throw (std::runtime_error("cgi_param not supported"));}
}

void ServerConfig::setHeritage(const Directives& heritage) {
    _heritage = heritage;
}

void ServerConfig::setHostPort(const HostPort& hostPort) {
    std::vector<Server *> servers = MotherWebserv::getInstance()->getConfig().getServers();
    for (std::vector<Server *>::const_iterator it = servers.begin(); it != servers.end(); ++it) {
        std::set<HostPort> hostPorts = (*it)->getConfig().getHostPorts();
        for (std::set<HostPort>::const_iterator it2 = hostPorts.begin(); it2 != hostPorts.end(); ++it2) {
            if (hostPort.getHost() == it2->getHost() && hostPort.getPort() == it2->getPort()) {
                HostPort copy = hostPort;
                std::string errorMsg = copy.getHost() + ":" + convertToString(copy.getPort());
                Logger::log(ERROR, "Duplicate listen in ", errorMsg);
                throw (std::runtime_error("Duplicate listen in Server"));
            }
        }
    }
	_hostPorts.insert(hostPort);
}

void ServerConfig::setHostServerName(const std::string& host) {
	_directives.setServerName(host, 1);
}
