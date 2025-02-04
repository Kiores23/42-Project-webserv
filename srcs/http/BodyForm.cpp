#include "BodyForm.hpp"

BodyForm::BodyForm()
{
}

BodyForm::~BodyForm()
{
}

void	BodyForm::parseBodyForm(std::string body)
{
	std::string key;
	std::string value;

	int i = 0;
	int j = i;
	if (!body[i])
		return ;
	if (body[i] == '&')
		i++;
	for (; body[i] && body[i]!='='; i++);
	if (body[i] != '=')
		throw logForError400("malformed body: " + body);
	key = body.substr(j, i - j);
	i++;
	j = i;
	for (; body[i] && body[i]!='&'; i++);
	value = body.substr(j, i - j);
	_bodyForm[key] = value;
	if (!body[i])
		return ;
	parseBodyForm(&body[i + 1]);
}

std::string	BodyForm::getValue(const std::string & key)
{
	if (_bodyForm.find(key) != _bodyForm.end())
		return _bodyForm[key];
	return "";
}