#ifndef BODY_HPP
# define BODY_HPP

# include <iostream>
# include <map>

class	Body
{
	private:
		std::string							_body;
		std::map<std::string, std::string>	_params;
		std::string							_fileName;

	public:
		Body();
		Body(std::string & body);
		Body(const Body& original);
		~Body();
		const std::string &	getValue() const;
		const std::string &	getFileName() const;
		void				setValue(std::string body);
		void				setFileName(std::string fileName);
		std::string 		getParams(const std::string & key) const;
		void				addParams(const std::string & key, const std::string & value);
};

#endif
