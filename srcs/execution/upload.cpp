#include "upload.hpp"

std::string	getCurrentDir(std::string & target);
std::string	getFinalFileName(std::string & target, const std::string & fileName);

// upload - index

int upload(const Request & request, LocationConfig & locationConfig, std::string target)
{
	int	r = 200;
	target = getCurrentDir(target) + locationConfig.getDirectives().getUploadFilesPath() + "/";
	std::string fileName;
	for (int i = 0; request.getBody(i); i++)
	{
		if (!request.getBody(i)->getFileName().empty())
		{
			fileName = getFinalFileName(target, request.getBody(i)->getFileName());
			r = 201;
			if(access(target.c_str(), F_OK) == -1)
				throw 404;
			std::ofstream file(fileName.c_str());
			if (file.is_open()) {
				file << request.getBody(0)->getValue();
				file.close();
			}
			else
				throw logForError500("open:" + fileName);
		}
	}
	return r;
}

// upload - utils

std::string	getCurrentDir(std::string & target)
{
	size_t		i;
	for (i = target.size(); i != 0 && target[i] != '/'; i--);
	return target.substr(0, i);
}


std::string	getFinalFileName(std::string & target, const std::string & fileName)
{
	std::string	withoutExtension;
	std::string	extension;
	size_t 		i;
	for (i = fileName.size() - 1; i != 0 && fileName[i] != '.'; i--);
	if (i != 0 && i != fileName.size() - 1)
	{
		withoutExtension = target + fileName.substr(0, i);
		extension = fileName.substr(i, fileName.size());
	}
	else
		withoutExtension = target + fileName;
	std::string	finalFileName = withoutExtension + extension;
	for (i = 1; access(finalFileName.c_str(), F_OK) != -1; i++)
	{
		finalFileName = withoutExtension + "(" + convertToString(i) + ")" + extension;
	}
	return finalFileName;
}
