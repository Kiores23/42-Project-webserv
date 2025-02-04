#include "remove.hpp"

// remove - index

int protectedRemove(std::string target)
{
	if (std::remove(target.c_str()) != 0)
		throw 403;
	return 204;
}
