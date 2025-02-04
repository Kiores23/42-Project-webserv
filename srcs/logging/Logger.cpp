#include <cerrno>
#include "Logger.hpp"
#include "../utils/utils.hpp"

Logger::Logger()
{
}

Logger &Logger::getInstance()
{
	static Logger instance;
	return instance;
}

void Logger::logMessage(LogLevel level, const std::string &message)
{
	char timestampFormatted[20];
	struct std::tm localTime = {};
	std::stringstream formatMessage;

	std::time_t timeEpoch = std::time(NULL);
	localtime_r(&timeEpoch, &localTime);
	std::strftime(timestampFormatted, sizeof(timestampFormatted), "%Y-%m-%d %H:%M:%S", &localTime);

	formatMessage << "[" << timestampFormatted << "] [" << levelToString(level) << "] " << message;
	if (level == ERROR)
		std::cerr << formatMessage.str() << std::endl;
	else
		std::cout << formatMessage.str() << std::endl;
}

void Logger::log(LogLevel level, const std::string &message)
{
	Logger::getInstance().logMessage(level, message);
}

void Logger::logErrno(const std::string &message)
{
	std::string errnoString = convertToString(errno);
	const char *errorMessage = strerror(errno);
	std::ostringstream stringstream;

	stringstream << message << ": error code " << errnoString << ": " << errorMessage;
	Logger::log(ERROR, stringstream);
}

void Logger::log(LogLevel level, const std::string &msg1, const std::string &msg2)
{
	std::ostringstream stringstream;

	stringstream << msg1 << msg2;
	Logger::log(level, stringstream);
}

void Logger::log(LogLevel level, const std::string &msg1, const std::string &msg2, const std::string &msg3)
{
	std::ostringstream stringstream;

	stringstream << msg1 << msg2 << msg3;
	Logger::log(level, stringstream);
}

void Logger::log(LogLevel level, const std::ostringstream &stream)
{
	Logger::getInstance().logMessage(level, stream.str());
}

void Logger::log(LogUser logUser, LogLevel level, const std::string &message)
{
	if (logUser == false)
		return;
	Logger::getInstance().logMessage(level, message);
}

void Logger::log(LogUser logUser, LogLevel level, const std::string &msg1, const std::string &msg2)
{
	if (logUser == false)
		return;
	std::ostringstream stringstream;

	stringstream << msg1 << msg2;
	Logger::log(level, stringstream);
}

void
Logger::log(LogUser logUser, LogLevel level, const std::string &msg1, const std::string &msg2, const std::string &msg3)
{
	if (logUser == false)
		return;
	std::ostringstream stringstream;

	stringstream << msg1 << msg2 << msg3;
	Logger::log(level, stringstream);
}

void Logger::log(LogUser logUser, LogLevel level, const std::ostringstream &stream)
{
	if (logUser == false)
		return;
	Logger::getInstance().logMessage(level, stream.str());
}

std::string Logger::levelToString(LogLevel level)
{
	std::string string;
	switch (level)
	{
		case DEBUG: return string.append(GREEN).append("DEBUG").append(RESET);
		case INFO: return string.append(BLUE).append("INFO").append(RESET);
		case WARNING: return string.append(YELLOW).append("WARNING").append(RESET);
		case ERROR: return string.append(RED).append("ERROR").append(RESET);
		default: return levelToString(INFO);
	}
}

/**
 * Log and throw a runtime_error, using the current string as the cause to display.
 * @param string the cause message.
 */
void Logger::throwAndLogRuntimeError(const std::string &string)
{
	Logger::log(ERROR, string);
	throw (std::runtime_error(string));
}
