#include "utils.hpp"
#include "../logging/Logger.hpp"
#include <sys/socket.h>
#include <cstdio>

ssize_t readData(int fd, char *buffer, size_t bufferSize)
{
	ssize_t bytesRead = read(fd, buffer, bufferSize - 1);
	if (bytesRead >= 0)
		buffer[bytesRead] = 0;
	else
		buffer[0] = 0;
	return bytesRead;
}

std::string toLowerCase(const std::string &str)
{
	std::string lowerCaseStr;

	for (size_t i = 0; i < str.length(); ++i)
		lowerCaseStr.push_back(char(std::tolower(str[i])));
	return lowerCaseStr;
}

bool equalsIgnoreCase(const std::string &str1, const std::string &str2)
{
	return toLowerCase(str1) == toLowerCase(str2);
}

char *const *mapToCArray(const std::map<std::string, std::string> &map)
{
	size_t size = map.size() + 1;
	char **cArray = new char *[size];

	std::stringstream ss;
	size_t i = 0;
	for (std::map<std::string, std::string>::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		ss.str(std::string());
		ss << it->first << "=" << it->second;
		std::string mapVar = ss.str();
		if (not mapVar.empty())
		{
			cArray[i] = new char[mapVar.length() + 1];
			std::strcpy(cArray[i], mapVar.c_str());
			++i;
		}
	}
	cArray[i] = NULL;
	return cArray;
}

void deleteCStringArray(char *const *array)
{
	int i = 0;

	while (array[i])
	{
		delete[] array[i];
		++i;
	}
	delete[] array;
}

bool check_unsigned_integer16(const std::string &str)
{
	if (str.empty())
		return (false);
	u_int32_t u_integer16 = 0;
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		if (!isdigit(*it))
			return (false);

		u_int32_t digit = static_cast<u_int32_t>(*it - '0');
		u_integer16 = u_integer16 * 10 + digit;

		if (u_integer16 > MAX_UINT16)
			return (false);
	}
	return (true);
}

std::string decodeURL(const std::string &encodedURL)
{
	std::string result;
	char ch;
	size_t i;
	unsigned int intChar;

	for (i = 0; i < encodedURL.length(); i++)
	{
		if (int(encodedURL[i]) == '%')
		{
			sscanf(encodedURL.substr(i + 1, 2).c_str(), "%x", &intChar);
			ch = static_cast<char>(intChar);
			result += ch;
			i = i + 2;
		}
		else if (encodedURL[i] == '+')
			result += ' ';
		else
			result += encodedURL[i];
	}
	return result;
}
