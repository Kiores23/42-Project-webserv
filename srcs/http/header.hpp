#ifndef HEADER_HPP
# define HEADER_HPP

# include "response.hpp"
# include <ctime>
# include <vector>
# include <map>

std::vector<std::string>	getResponseHeader(const std::string & body);
std::vector<std::string>	getResponseHeader(const LocationConfig & locationConfig, const int code, const std::string & body);
void						commonHeader(std::vector<std::string> & header, const std::string & body);
void						parsHeaderCgi(std::string &cgiResponse, std::map<std::string, std::list<std::string> > & header);

// UTILS

void		setDayTab(std::vector<std::string> & dayTab);
void		setMonthTab(std::vector<std::string> & monthTab);
std::string	getUtcDate();

#endif
