#ifndef WEBSERV_PARSINGUTILS_HPP
#define WEBSERV_PARSINGUTILS_HPP

#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <set>

# define CLIENT_MAX_BODY_SIZE_AUTHORISED 20971521 // 20 megabytes + 1
# define LOCALHOST_ADDRESS "127.0.0.1"

bool checkDirectiveValues(const std::string &value);

bool responseErrorCodeSupported(const u_int16_t &code);
bool responseRedirectCodeSupported(const u_int16_t &code);

std::string extractVariableName(const std::string &line);

size_t skipVariableName(const std::string &line);

std::vector<std::string> extractValues(const std::string &line);

std::string extractVariableName(const std::string &line);

void removeNewLines(std::string &lineSplit);

std::string::const_iterator findNextSpace(const std::string::const_iterator &start,
										  const std::string::const_iterator &end);

bool isDigitOrDot(char c);

std::string::const_iterator moveToNextWord(const std::string::const_iterator &beginIt,
										   const std::string::const_iterator &end);

bool isStringDigit(const std::string &line);

#endif //WEBSERV_PARSINGUTILS_HPP
