#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

# include <cstring>
# include <iostream>
#include <cstring>
# include <unistd.h>
#include <sstream>
#include <cstdlib>
#include <map>

#define MAX_UINT16 65535

template<typename T>
std::string convertToString(T value)
{
	std::ostringstream ss;
	ss << value;
	return ss.str();
}

template std::string convertToString<size_t>(size_t value);
template std::string convertToString<int>(int value);
template std::string convertToString<unsigned int>(unsigned int value);

ssize_t readData(int fd, char *buffer, size_t bufferSize);

bool equalsIgnoreCase(const std::string &str1, const std::string &str2);

std::string toLowerCase(const std::string &str);

char *const *mapToCArray(const std::map<std::string, std::string> &map);

void deleteCStringArray(char *const *array);

bool check_unsigned_integer16(const std::string& str);
std::string decodeURL(const std::string &encodedURL);
#endif //WEBSERV_UTILS_HPP
