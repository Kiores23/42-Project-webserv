#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <map>
# include <string>
# include <vector>
# include <set>
#include "../logging/Logger.hpp"
#include "parsingConfigFile.hpp"
#include "../utils/utils.hpp"
#include "../utils/LocationDirectives.hpp"
#include "../utils/ServerDirectives.hpp"
#include "checkConfig.hpp"

class LocationConfig
{
  private:
	ServerDirectives 	_heritage;
	LocationDirectives	_directives;
  public:
	LocationConfig();
	explicit LocationConfig(const ServerDirectives& heritage);
	LocationConfig(const LocationConfig& copy);
  LocationConfig& operator=(const LocationConfig& src);
	~LocationConfig();

	const LocationDirectives &getDirectives() const;

	void fillData(std::vector<std::string>::const_iterator& it,
		const std::vector<std::string>::const_iterator& endInfo, const std::string& path);
	void fillHeritageData();
	void setAutoindex(const std::string& value);
	void setClientMaxBodySize(const std::string& value);
	void setDirectiveMultipleParameter(const std::string& variableName, std::vector<std::string>::const_iterator& it,
		const std::vector<std::string>::const_iterator& endInfo);
	void setDirectiveOneParameter(const std::string& variableName, const std::string& line);
	void setDirectiveTwoParameter(const std::string& variableName, const std::string& line);
	void setHttpAcceptedMethods(std::vector<std::string>::const_iterator& it,
		const std::vector<std::string>::const_iterator& endInfo);
	void setHostServerName(const std::string& host);
};

#endif
