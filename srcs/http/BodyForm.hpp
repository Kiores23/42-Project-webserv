#ifndef BODYFORM_HPP
# define BODYFORM_HPP

# include <iostream>
# include <map>
# include "../logging/Logger.hpp"

int   logForError400(const std::string & logMessage);

class BodyForm
{
	private:
		std::map<std::string, std::string> _bodyForm;
	public:
		BodyForm();
		~BodyForm();

		void		parseValue(std::string & value);
		void		parseBodyForm(std::string body);
		std::string	getValue(const std::string & key);
};

#endif