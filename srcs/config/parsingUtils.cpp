#include "parsingUtils.hpp"

bool checkDirectiveValues(const std::string &value) {
	for (std::string::const_iterator it = value.begin(); it != value.end(); ++it) {
		if (not std::isalnum(*it) && *it != '/' && *it != '.' && *it != '-' && *it != '_' && *it != '~')
			return (false);
	}
	return (true);
}

bool responseErrorCodeSupported(const u_int16_t &code)
{
    std::set<u_int16_t> errorCodes;
    errorCodes.insert(400);
    errorCodes.insert(403);
    errorCodes.insert(404);
    errorCodes.insert(405);
    errorCodes.insert(408);
    errorCodes.insert(500);
	return (errorCodes.find(code) != errorCodes.end());
}

bool responseRedirectCodeSupported(const u_int16_t &code)
{
    std::set<u_int16_t> httpCodes;
    httpCodes.insert(301);
    httpCodes.insert(308);
	return (httpCodes.find(code) != httpCodes.end());
}

static size_t firstSpacePosition(const std::string &string)
{
	return (string.find(' ', 0));
}

/**
 * Takes the line and return the index to next char after the variable name
 *
 * @param line
 *
 * @return prev: index to next char after the variable name
*/
size_t skipVariableName(const std::string &line)
{
	size_t pos = firstSpacePosition(line);
	return ((pos != std::string::npos) ? (pos + 1) : (std::string::npos));
}

/**
 * Extracts values after the variable name of the line
 *
 * @param line
 *
 * @return vectorValues
 */
std::vector<std::string> extractValues(const std::string &line)
{
	std::vector<std::string> vectorValues;
	size_t prev = skipVariableName(line), pos;

	while ((pos = line.find(' ', prev)) != std::string::npos)
	{
		std::string lineSplit = line.substr(prev, pos - prev);
		if (pos > prev && lineSplit != " " && !lineSplit.empty())
			vectorValues.push_back(lineSplit);
		prev = pos + 1;
	}
	if (prev < line.length())
		vectorValues.push_back(line.substr(prev));
	return (vectorValues);
}

/**
 * Takes a line from the vector strings and returns the variable name
 *
 * @param line
 *
 * @return variableName
 */
std::string extractVariableName(const std::string &line)
{
	size_t pos = firstSpacePosition(line);
	std::string variableName;

	if (pos != std::string::npos)
		variableName = line.substr(0, pos);
	else
		variableName = line;
	return (variableName);
}

/**
 * Removes first and last spaces and all '\\n' from the string
 *
 * @param lineSplit
 */
void removeNewLines(std::string &lineSplit)
{
	std::string result;
	size_t start = lineSplit.find_first_not_of(" \n");
	if (start == std::string::npos)
	{
		lineSplit.clear();
		return;
	}
	for (size_t i = start; i < lineSplit.length(); i++)
		if (lineSplit[i] != '\n')
			result += lineSplit[i];
	result.erase(result.find_last_not_of(' ') + 1);
	lineSplit = result;
}

std::string::const_iterator
findNextSpace(const std::string::const_iterator &start, const std::string::const_iterator &end)
{
	return std::find(start, end, ' ');
}

bool isDigitOrDot(char c)
{
	return std::isdigit(static_cast<unsigned char>(c)) || c == '.';
}

/**
 * Gives a string iterator to the next word
 *
 * @param beginIt
 * @param end
 *
 * @return nextWord or end of the string
 */
std::string::const_iterator moveToNextWord(const std::string::const_iterator &beginIt,
										   const std::string::const_iterator &end)
{
	for (std::string::const_iterator nextWord = beginIt; nextWord != end; ++nextWord)
		if (*nextWord != ' ')
			return (nextWord);
	return (end);
}

/**
 * Checks if the string is completely digits
 *
 * @param line
 */
bool isStringDigit(const std::string &line)
{
	for (std::string::const_iterator it = line.begin(); it != line.end(); ++it)
		if (!std::isdigit(static_cast<unsigned char>(*it)))
			return (false);
	return (true);
}

