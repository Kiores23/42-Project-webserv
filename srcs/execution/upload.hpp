#ifndef UPLOAD_HPP
# define UPLOAD_HPP

# include "iostream"
# include "../http/Request.hpp"
# include "../networking/Server.hpp"
# include "../utils/utils.hpp"

int upload(const Request & request, LocationConfig & locationConfig, std::string target);

#endif
