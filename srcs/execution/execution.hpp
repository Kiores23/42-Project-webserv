#ifndef EXECUTION_HPP
# define EXECUTION_HPP

# include <iostream>
# include <sys/stat.h>
# include "../http/Request.hpp"
# include "../networking/Server.hpp"
# include "../utils/utils.hpp"
# include "upload.hpp"
# include "remove.hpp"

int execution(const Request &request, LocationConfig &locationConfig, std::string &target);
void verifAccess(LocationConfig & locationConfig, std::string & target, std::string method, bool isCGI);
void setIndexRoot(LocationConfig & locationConfig, std::string & target);

#endif
