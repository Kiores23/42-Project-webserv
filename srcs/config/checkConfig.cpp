#include "checkConfig.hpp"
#include "../logging/Logger.hpp"

/**
 * Checks the validity of the client_max_body_size value
 *
 * @param str
 * @throw
 */
void checkClientMaxBodySizeValidity(const std::string& str) {
	int clientMaxBodySizeValidity = 0;
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (!std::isdigit(*it)) {
			if ((it != str.end() - 1) || (*it != 'k' && *it != 'K' && *it != 'm' && *it != 'M')) {
				Logger::throwAndLogRuntimeError("invalid client_max_body_size value");
			}
		}
		else if (clientMaxBodySizeValidity > CLIENT_MAX_BODY_SIZE_AUTHORISED / 10) {
			Logger::throwAndLogRuntimeError("Client_max_body_size above authorised level");
		}
		else
			clientMaxBodySizeValidity = clientMaxBodySizeValidity * 10 + (*it - '0');
	}
}

/**
 * Checks for the client max body size if the value would overflow the unsigned int max value, units are based on kibibytes
 *
 * @param unit: of the client max body size value
 * @param clientMaxBodySize
 */
void checkMultiplyOverflow(const char& unit, const unsigned int& clientMaxBodySize) {
	unsigned int kilobyte = 1024, megabyte = 1024 * 1024;
	switch (unit) {
		case 'k':
		case 'K':
		if (clientMaxBodySize > CLIENT_MAX_BODY_SIZE_AUTHORISED / kilobyte) {
			Logger::throwAndLogRuntimeError("client_max_body_size multiply would overflow");
		}
		break;

		case 'm':
		case 'M':
		if (clientMaxBodySize > CLIENT_MAX_BODY_SIZE_AUTHORISED / megabyte) {
			Logger::throwAndLogRuntimeError("client_max_body_size multiply would overflow");
		}
		break;
	}
}
