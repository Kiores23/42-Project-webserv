#include "header.hpp"

std::vector<std::string>	getResponseHeader(const std::string & body)
{
	std::vector<std::string>	header;

	commonHeader(header, body);
	return header;
}

std::vector<std::string>	getResponseHeader(const LocationConfig & locationConfig, const int code, const std::string & body)
{
	std::vector<std::string>	header;

	commonHeader(header, body);
	if (code / 100 == 3)
	{
		header.push_back("Location: " + locationConfig.getDirectives().getHttpRedirection().second);
	}
	return header;
}

void	commonHeader(std::vector<std::string> & header, const std::string & body)
{
	header.push_back("Server: " + convertToString(WEBSERV));
	header.push_back("Date: " + getUtcDate());
	header.push_back("Content-Length: " + convertToString(body.size()));
}

void	parsHeaderCgi(std::string &cgiResponse, std::map<std::string, std::list<std::string> > & header)
{
	std::string key;
	std::string	value;
	size_t i;
	size_t j;
	if (!cgiResponse[0] || cgiResponse[0] == '\n' || (cgiResponse[0] == '\r' && cgiResponse[1] == '\n'))
		return;
	for (j = 0; cgiResponse[j] && cgiResponse[j] != ':' && (cgiResponse[j] != '\r' && cgiResponse[j] != '\n'); j++);
	key = cgiResponse.substr(0, j);
	if (cgiResponse[j] != ':' || cgiResponse[j + 1] != ' ')
		throw logForError500("Wrong CGI response: header key: " + cgiResponse);
	if (cgiResponse[j] == '\n')
		i = j + 1;
	else
		i = j + 2;
	for (j = i; cgiResponse[j] && (cgiResponse[j] != '\r' && cgiResponse[j] != '\n'); j++);
	value = cgiResponse.substr(i, j - i);
	if (value[0] == ' ')
		header[key].push_back(value.substr(1, value.size() - 1));
	else
		header[key].push_back(value);
	if (i - j == 0)
		throw logForError500("Wrong CGI response: header value");
	if (cgiResponse[j] == '\n')
		cgiResponse = &cgiResponse[j + 1];
	else
		cgiResponse = &cgiResponse[j + 2];
	parsHeaderCgi(cgiResponse, header);
}

//

std::string	getUtcDate()
{
	std::time_t currentTime = std::time(NULL);
    std::tm utc_tm = *std::gmtime(&currentTime);

	std::vector<std::string> dayNames, monthNames;
	setDayTab(dayNames);
	setMonthTab(monthNames);
	std::string	dayName, day, monthName, year, hour, minute, second;
	dayName = dayNames[static_cast<size_t>(utc_tm.tm_wday)];
	day = convertToString(utc_tm.tm_mday);
	monthName = monthNames[static_cast<size_t>(utc_tm.tm_mon)];
	year = convertToString(utc_tm.tm_year + 1900);
	hour = convertToString(utc_tm.tm_hour);
	minute = convertToString(utc_tm.tm_min);
	second = convertToString(utc_tm.tm_sec);
	const std::string utcDate = \
		dayName + ", " + day + " " + monthName + " " + year + " "\
		+ hour + ":" + minute + ":" + second + " UTC";
	return utcDate;
}
