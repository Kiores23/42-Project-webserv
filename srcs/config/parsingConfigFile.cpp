# include "parsingConfigFile.hpp"
# include "../networking/MotherWebserv.hpp"
# include "parsingUtils.hpp"

/**
 * Extracts and returns the host value
 * 
 * @param line: the function extracts data from it
 * @param hostIt: iterator on the host's first char
 * @param portIt: iterator on the first char after the host
 * 
 * @return host
 */
static std::string extractHost(const std::string& line, const std::string::const_iterator& hostIt,
	const std::string::const_iterator& portIt) {
	std::ptrdiff_t	startIndex = std::distance(line.begin(), hostIt);
	std::ptrdiff_t	extractLength = std::distance(hostIt, portIt);
	std::string	host = line.substr(static_cast<size_t>(startIndex), static_cast<size_t>(extractLength));
	return (host);
}

/**
 * Extracts and returns the port value
 * 
 * @param line: the function extracts data from it
 * @param portIt: iterator on the port's first char
 * @param optionIt: iterator on the first char after the port
 * 
 * @return port
 */
static int extractPort(const std::string& line, const std::string::const_iterator& portIt,
	const std::string::const_iterator& optionIt) {
	std::ptrdiff_t	startIndex = std::distance(line.begin(), portIt);
	std::ptrdiff_t	extractLength = std::distance(portIt, optionIt);
	std::string	portString = line.substr(static_cast<size_t>(startIndex), static_cast<size_t>(extractLength));
	std::stringstream	SS(portString);
	int	port = 0;
	SS >> port;
	if (port < MIN_PORT || port > MAX_PORT) {
		Logger::log(ERROR, "Port to listen to is below 1 or above 65535");
		throw (std::runtime_error("Port to listen to is below 1 or above 65535"));
	}
	return (port);
}

/**
 * Takes a line that contains "listen" and extracts its host and/or port value, also checks
 * for the host and port values validity
 * Default value of the port: 80
 * 
 * @param line: to extract the port value from
 * 
 * @return hostPort: structure with host and port
 */
HostPort extractHostPort(const std::string& line) {
	std::vector<std::string> values = extractValues(line);
	if (values.size() != 1)
		Logger::throwAndLogRuntimeError("Invalid number of arguments in server directive: listen");
	std::string::const_iterator	endIt = line.end();
	std::string::const_iterator hostIt = findNextSpace(line.begin(), endIt);
	int	dotsCountInHost = 0, colon = 1;
	HostPort	hostPort;

	if (hostIt != endIt) {
		hostIt = moveToNextWord(hostIt, endIt);
		std::string::const_iterator	optionIt = findNextSpace(hostIt, endIt);

		//	Checks the syntax validity of the host and then extracts it
		std::ptrdiff_t	hostStart = std::distance(line.begin(), hostIt);
		std::string::const_iterator portIt = std::find(hostIt, optionIt, ':');

		//	checks if there is points in the host
		std::ptrdiff_t	hostLength = std::distance(hostIt, portIt);
		if (portIt == line.end()) {
			hostLength = std::distance(hostIt, optionIt);
			portIt = optionIt;
			colon = 0;
		}
		std::string checkHost = line.substr(static_cast<size_t>(hostStart), static_cast<size_t>(hostLength));
		if (checkHost != "localhost") {
			for (std::string::const_iterator checkHostIt = hostIt; checkHostIt != portIt; ++checkHostIt) {
				if (!isDigitOrDot(*checkHostIt)) {
					Logger::throwAndLogRuntimeError("Only digit numbers and '.' or \"localhost\" are valid values for the host");
				}
				if (*checkHostIt == '.')
					dotsCountInHost += 1;
			}
			if (dotsCountInHost == 0)
				portIt = hostIt;
			else if (dotsCountInHost == 3)
				hostPort.setHost(extractHost(line, hostIt, portIt));
			else
				Logger::throwAndLogRuntimeError("Impossible syntax on the host");
		}
		
		//	Checks the syntax validity of the port and then extracts it
		if ((dotsCountInHost != 0 || checkHost == "localhost") && colon == 1) {
			portIt += 1;
			for (std::string::const_iterator checkPortIt = portIt; checkPortIt != optionIt; ++checkPortIt) {
				if (!std::isdigit(static_cast<unsigned char>(*checkPortIt))) {
					Logger::throwAndLogRuntimeError("Only digit numbers are valid values for the port");
				}
			}
		}
		if (portIt != optionIt)
			hostPort.setPort(extractPort(line, portIt, optionIt));
	}
	return (hostPort);
}

/**
 * Splits the config file to strings representing directives and curly brackets
 * inside the vectorSplit
 * 
 * @param configFileContent
 * @param vectorSplit the vector containing the strings
 */
static void splitingConfigFile(std::stringstream& configFileContent,
	std::vector<std::string>& vectorSplit)
{
	std::string	line;
	size_t		openBracketCount = 0, closingBracketCount = 0;

	//	Removes all comments
	while (std::getline(configFileContent, line, '#')) {
		size_t	prev = 0, pos = 0;

		//	Splits with the "{};" delimiters
		while ((pos = line.find_first_of("{};", prev)) != std::string::npos) {
			if (line[pos] == '{')
				openBracketCount += 1;
			else if (line[pos] == '}')
				closingBracketCount += 1;

			std::string	lineSplit = line.substr(prev, pos - prev);
			removeNewLines(lineSplit);

			if (pos > prev && !lineSplit.empty())
				vectorSplit.push_back(lineSplit);
			if (line[pos] != ';')
				vectorSplit.push_back(std::string(1, line[pos]));
			prev = pos + 1;
		}
		if (pos == std::string::npos) {
			std::string lineSplit = line.substr(prev, line.size() - prev);
			removeNewLines(lineSplit);
			for (std::string::iterator it = lineSplit.begin(); it != lineSplit.end(); ++it) {
				if (std::isprint(*it))
					Logger::throwAndLogRuntimeError("Wrong config file directives");
			}
		}

		//	Gets to the end of the line
		if (prev < line.length() || line.empty())
			std::getline(configFileContent, line);
	}
	if (openBracketCount != closingBracketCount) {
		Logger::throwAndLogRuntimeError("config file: numbers of open brackets and closing brackets are not equal");
	}
	for (std::vector<std::string>::const_iterator it = vectorSplit.begin(); it != vectorSplit.end(); ++it) {
		std::string openingBracket = extractVariableName(*it);
		if (openingBracket == "{") {
			it--;
			std::string variableName = extractVariableName(*it);
			if (variableName != "location" && variableName != "server" && variableName != "limit_except")
				Logger::throwAndLogRuntimeError("config file: incorrect variable name before opening bracket, should be \"location\", \"server\" or \"limit_except\"");
			it++;
		}
	}
}

/**
 * Parses the config file
 * 
 * @param configFileContent
 */
static void parsingConfigFile(std::stringstream& configFileContent) {
	std::vector<std::string>	vectorSplit;

	splitingConfigFile(configFileContent, vectorSplit);
	MotherWebserv::getInstance()->getConfig().fillData(vectorSplit);
}

/**
 * Checks the extension name of the config file is correct
 * 
 * @param configFilename
 * 
 */
static void checkExtensionName(const std::string& configFilename) {
	size_t	lastPos = configFilename.rfind(".conf");

	//	check if ".conf" is found at the end
	if ((lastPos == std::string::npos) || (lastPos + 5 != configFilename.length())) {
		Logger::throwAndLogRuntimeError("Wrong config file's extension name, it should end with \".conf\"");
	}
}

/**
 * Reads and parses the config file contents if the program can open the
 * correct config file
 * 
 * @param configFilename
 */
void readConfigFile(const std::string& configFilename) {
	std::ifstream	infile;

	checkExtensionName(configFilename);
	infile.open(configFilename.c_str());
	if (!infile.is_open()) {
		Logger::throwAndLogRuntimeError("when opening config file");
	}
	//	read file contents
	std::stringstream	buffer;
	buffer << infile.rdbuf();
	infile.close();

	parsingConfigFile(buffer);
}
