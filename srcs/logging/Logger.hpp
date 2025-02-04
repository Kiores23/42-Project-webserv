#ifndef WEBSERV_LOGGER_HPP
# define WEBSERV_LOGGER_HPP

# include <string>
# include <iostream>
# include <sstream>
#include <iomanip>
# include <ctime>
#include <map>

# define RESET "\033[0m"
# define RED "\033[31m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define GREEN "\033[32m"

enum LogLevel
{
	DEBUG,
	INFO,
	WARNING,
	ERROR
};

enum LogUser
{
	AMERY = false,
	AVAN = false,
	ELOMBARD = false,
	PRINT_LINE = false
};

class Logger
{
public:
	static Logger &getInstance();
	void logMessage(LogLevel level, const std::string &message);
	static void log(LogLevel level, const std::string &message);
	static void log(LogLevel level, const std::ostringstream &stream);
	static void log(LogLevel level, const std::string &msg1, const std::string &msg2);
	static void log(LogLevel level, const std::string &msg1, const std::string &msg2, const std::string &msg3);
	static void log(LogUser logUser, LogLevel level, const std::ostringstream &stream);
	static void
	log(LogUser logUser, LogLevel level, const std::string &msg1, const std::string &msg2, const std::string &msg3);
	static void log(LogUser logUser, LogLevel level, const std::string &msg1, const std::string &msg2);
	static void log(LogUser logUser, LogLevel level, const std::string &message);
	static void logErrno(const std::string &message);
	static void throwAndLogRuntimeError(const std::string &string);
private:
	Logger();
	Logger(const Logger &);
	Logger &operator=(const Logger &);
	std::string levelToString(LogLevel level);
};

#endif // WEBSERV_LOGGER_HPP
