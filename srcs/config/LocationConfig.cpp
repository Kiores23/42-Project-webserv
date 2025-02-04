#include "LocationConfig.hpp"

LocationConfig::LocationConfig() : _heritage()
{
	_directives.setAutoindex(std::pair<bool, bool>(false, false));
	_directives.setClientMaxBodySize(std::pair<bool, bool>(false, CLIENT_MAX_BODY_SIZE_AUTHORISED));
	_directives.setPath("");
	_directives.setRoot("");
	_directives.setUploadFilesPath("");
	_directives.setUploadPathBool(false);
}

LocationConfig::LocationConfig(const ServerDirectives& heritage) : _heritage(heritage)
{
	_directives.setAutoindex(std::pair<bool, bool>(false, false));
	_directives.setClientMaxBodySize(std::pair<bool, bool>(false, CLIENT_MAX_BODY_SIZE_AUTHORISED));
	_directives.setPath("");
	_directives.setRoot("");
	_directives.setUploadFilesPath("");
	_directives.setUploadPathBool(false);
}

LocationConfig::LocationConfig(const LocationConfig& copy)
  : _heritage(copy._heritage),
    _directives(copy._directives) {
}

LocationConfig& LocationConfig::operator=(const LocationConfig& src) {
  if (this != &src) {
    _heritage = src._heritage;
    _directives = src._directives;
  }
  return (*this);
}

LocationConfig::~LocationConfig() {
}

static bool checkValidLocationDirective(const std::string& variableName) {

	std::set<std::string> acceptedDirectives;
	acceptedDirectives.insert("{");
	acceptedDirectives.insert("}");
	acceptedDirectives.insert("autoindex");
	acceptedDirectives.insert("cgi_param");
	acceptedDirectives.insert("client_max_body_size");
	acceptedDirectives.insert("deny");
	acceptedDirectives.insert("error_page");
	acceptedDirectives.insert("index");
	acceptedDirectives.insert("limit_except");
	acceptedDirectives.insert("listen");
	acceptedDirectives.insert("return");
	acceptedDirectives.insert("root");
	acceptedDirectives.insert("upload");
	if (acceptedDirectives.find(variableName) == acceptedDirectives.end())
		return (false);
	return (true);
}

static bool validHttpMethod(const std::string& value) {
	std::set<std::string> validHttpMethod;
	validHttpMethod.insert("GET");
	validHttpMethod.insert("HEAD");
	validHttpMethod.insert("POST");
	validHttpMethod.insert("DELETE");
	if (validHttpMethod.find(value) == validHttpMethod.end())
		return (false);
	return (true);
}

const LocationDirectives &LocationConfig::getDirectives() const {
	return (_directives);
}

/**
 * Fills the Location context path and data
 *
 * @param it: const_iterator on the string vector containing the config file from the start of the location context
 * @param endInfo: const_iterator on the end of the string vector
 */
void LocationConfig::fillData(std::vector<std::string>::const_iterator &it,
							  const std::vector<std::string>::const_iterator &endInfo, const std::string &path)
{
	std::string variableName;
	std::vector<std::string> locationContent;
	int	openedBracket = 0;

	_directives.setPath(path);

	//	Picks all the content for the server only
	do {
		if (it < endInfo)
			it += 1;
		if (*it == "{")
			openedBracket += 1;
		else if (*it == "}")
			openedBracket -= 1;
		variableName = extractVariableName(*it);
		if (!checkValidLocationDirective(variableName)) {
			Logger::log(ERROR, "Invalid location directive: ", variableName);
			throw (std::runtime_error("Invalid location directive"));
		}
		else
			locationContent.push_back(*it);
	} while (it < endInfo && openedBracket > 0);

	//	Simple Directives
	for (std::vector<std::string>::const_iterator it2 = locationContent.begin(); it2 != locationContent.end(); ++it2) {
		variableName = extractVariableName(*it2);
		if (variableName == "error_page" || variableName == "index" || variableName == "limit_except")
			setDirectiveMultipleParameter(variableName, it2, locationContent.end());
		else if (variableName == "autoindex" || variableName == "client_max_body_size" || variableName == "root" || variableName == "upload")
			setDirectiveOneParameter(variableName, *it2);
		else if (variableName == "cgi_param" || variableName == "return")
			setDirectiveTwoParameter(variableName, *it2);
		else if (variableName == "deny") {
			Logger::log(ERROR, "Invalid location directive: deny, must be inside a limit_except context");
			throw (std::runtime_error("Invalid location directive: deny, must be inside a limit_except context"));
		}
	}

	//	Fills the data from the heritage directives
	fillHeritageData();

}

void LocationConfig::fillHeritageData() {
	if (_directives.getHttpAcceptedMethods().empty()) {
		_directives.setHttpAcceptedMethod("GET");
		_directives.setHttpAcceptedMethod("HEAD");
		_directives.setHttpAcceptedMethod("POST");
		_directives.setHttpAcceptedMethod("DELETE");
	}
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
    for (std::set<std::string>::const_iterator it = _heritage.getServerNames().begin(); it != _heritage.getServerNames().end(); ++it) {
        setHostServerName(*it);
    }
}


/**
 * Checks the validity of the autoindex value and turns the directory listing "on" or "off"
 * inside the location context
 *
 * @param value
 * @throw std::runtime_error
 */
void LocationConfig::setAutoindex(const std::string &value)
{
	if (value == "on")
		_directives.setAutoindex(std::pair<bool, bool>(true, true));
	else if (value == "off")
		_directives.setAutoindex(std::pair<bool, bool>(true, false));
	else
	{
		Logger::log(ERROR, "invalid argument on location directive autoindex, should only be \"on\" or \"off\"");
		throw(std::runtime_error("invalid argument on location directive autoindex"));
	}
}

/**
 * Checks the validity of the client max body size value and sets it
 * in the instance
 *
 * @param value
 */
void LocationConfig::setClientMaxBodySize(const std::string& value)
{
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

void LocationConfig::setDirectiveMultipleParameter(const std::string& variableName, std::vector<std::string>::const_iterator& it,
	const std::vector<std::string>::const_iterator& endInfo) {
	std::vector<std::string> values = extractValues(*it);
	//	Checks there is at least one parameter for index or at least two parameters for error_page
	if ((variableName == "error_page" && values.size() < 2) || ((variableName == "index" || variableName == "server_name") && values.size() < 1)) {
		Logger::log(ERROR, "no argument on location directive: ", variableName);
		throw (std::runtime_error("no argument on location directive: error_page or index or limit_except"));
	}

	if (variableName == "error_page") {
		std::string uri = values.back();
		if (not checkDirectiveValues(uri)) {
			Logger::log(ERROR, "Invalid path (Alphanumerical, '/', '.', '-'. '_' or '~' only) on location directive: error_page");
			throw (std::runtime_error("Invalid path (Alphanumerical, '/', '.', '-'. '_' or '~' only) on location directive error_page"));
		}

		for (std::vector<std::string>::const_iterator it2 = values.begin(); it2 != (values.end() - 1); ++it2) {
			if (!check_unsigned_integer16(*it2)) {
				Logger::log(ERROR, "Invalid error code, should be an unsigned integer_16 on location directive: error_page");
				throw (std::runtime_error("Invalid error code, should be an unsigned integer_16 on location directive: error_page"));
			}
			std::stringstream ss(*it2);
			u_int16_t errorPage = 0;
			ss >> errorPage;
			if (responseErrorCodeSupported(errorPage))
				_directives.setErrorPage(errorPage, uri);
			else {
				Logger::log(ERROR, "Unsupported _response error code on location directive: error_page");
				throw (std::runtime_error("Unsupported _response error code on location directive: error_page"));
			}
		}
	}
	else if (variableName == "index") {
		for (std::vector<std::string>::const_iterator it2 = values.begin(); it2 != values.end(); ++it2) {
			if (not checkDirectiveValues(*it2)) {
				Logger::log(ERROR, "Invalid path (Alphanumerical, '/', '.', '-'. '_' or '~' only) on location directive: index");
				throw (std::runtime_error("Invalid path (Alphanumerical, '/', '.', '-'. '_' or '~' only) on location directive index"));
			}
			_directives.setIndexFile(*it2);
		}
	}
	else
		setHttpAcceptedMethods(it, endInfo);
}

void LocationConfig::setDirectiveOneParameter(const std::string& variableName, const std::string& line) {
	std::vector<std::string> values = extractValues(line);
	//	Checks number of arguments
	if (values.size() != 1) {
		Logger::log(ERROR, (values.size() == 0 ? "no " : "too many "), "argument on location directive: ", variableName);
		throw (std::runtime_error("no argument on location directive autoindex or client_max_body_size or root or upload"));
	}

	//	Checks if the global directive already exists
	if ((variableName == "autoindex" && _directives.getAutoindexParsing().first == true) ||
		(variableName == "client_max_body_size" && _directives.getClientMaxBodySizeParsing().first == true))
	{
		Logger::log(ERROR, "duplicate on location directive ", variableName);
		throw (std::runtime_error("location directive autoindex or client_max_body_size is duplicate"));
	}

	//	Global directive does not exist
	if (variableName == "autoindex")
		setAutoindex(values[0]);
	else if (variableName == "client_max_body_size")
		setClientMaxBodySize(values[0]);
	else {
		if (not checkDirectiveValues(values[0])) {
			Logger::log(ERROR, "Invalid argument (Alphanumerical, '/', '.', '-'. '_' or '~' only) on location directive: ", variableName);
			throw (std::runtime_error("Invalid argument (Alphanumerical, '/', '.', '-'. '_' or '~' only) on location directive root or upload"));
		}
		if (variableName == "root")
			_directives.setRoot(values[0]);
		else
			_directives.setUploadFilesPath(values[0]);
	}
}

void LocationConfig::setDirectiveTwoParameter(const std::string& variableName, const std::string& line) {
	std::vector<std::string> values = extractValues(line);
	if (values.size() != 2) {
		Logger::log(ERROR, (values.size() == 0 ? "no argument" : (values.size() == 1 ? "too little argument" : "too many arguments")), " on location directive: ", variableName);
		throw (std::runtime_error("no argument or too little/many arguments on location directive cgi_param or return"));
	}
	if (variableName == "cgi_param") {
		if (values[0] == "SCRIPT_FILENAME")
			_directives.setCgiScriptFilename(values[1]);
		else {
			Logger::log(ERROR, "cgi_param not supported");
			throw (std::runtime_error("cgi_param not supported"));
		}
	}
	else {
		if (!check_unsigned_integer16(values[0])) {
			Logger::log(ERROR, "Invalid error code, should be an unsigned integer_16 on location directive: return");
			throw (std::runtime_error("Invalid error code, should be an unsigned integer_16 on location directive: return"));
		}
		std::stringstream ss(values[0]);
		u_int16_t responseCode = 0;
		ss >> responseCode;
		if (responseRedirectCodeSupported(responseCode))
			_directives.setHttpRedirection(responseCode, values[1]);
		else {
			Logger::throwAndLogRuntimeError("Unsupported _response error code on location directive: return");
		}
	}
}

void LocationConfig::setHttpAcceptedMethods(std::vector<std::string>::const_iterator &it,
											const std::vector<std::string>::const_iterator &endInfo)
{
	std::vector<std::string> values = extractValues(*it);
	if (values.size() == 0)
	{
		Logger::throwAndLogRuntimeError("no argument on location directive limit_except");
	}
	for (std::vector<std::string>::const_iterator it2 = values.begin(); it2 != values.end(); ++it2)
	{
		if (not validHttpMethod(*it2))
		{
			Logger::log(ERROR, "location directive limit_except has invalid value ", *it2);
			throw(std::runtime_error("location directive limit_except has invalid values"));
		}
	}
	unsigned int openedBracket = 0;

	do
	{
		if (it < endInfo)
			it += 1;
		if (*it == "{")
			openedBracket += 1;
		else if (*it == "}")
		{
			openedBracket -= 1;
			if (*(it - 1) == "{")
			{
				Logger::throwAndLogRuntimeError("limit_except context has no argument");
			}
		}
		else
		{
			std::string variableName = extractVariableName(*it);
			if (variableName == "deny")
			{
				std::vector<std::string> denyValues = extractValues(*it);
				if (denyValues.size() == 1 && denyValues[0] == "all")
				{
					std::set<std::string> acceptedMethods(values.begin(), values.end());
					_directives.setHttpAcceptedMethods(acceptedMethods);
				}
				else
				{
					Logger::log(ERROR, "location directive deny has ", (denyValues.size() == 0 ? "no argument" : "invalid argument"));
					throw(std::runtime_error("location directive deny has invalid argument or no argument"));
				}
			}
			else
			{
				Logger::log(ERROR, "location directive limit_except has invalid directive ", variableName);
				throw(std::runtime_error("location directive limit_except has invalid directive "));
			}
		}
	} while (it < endInfo && openedBracket > 0);
}

void LocationConfig::setHostServerName(const std::string& host) {
	_directives.setServerName(host, 1);
}
