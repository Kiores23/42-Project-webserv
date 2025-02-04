#include "getScriptInfos.hpp"

bool isSupportedScriptExtension(const std::string &targetFile, LocationConfig *pConfig)
{
	std::set<std::string> validScriptExtensions = pConfig->getDirectives().getCgiScriptFilenames();

	std::set<std::string>::iterator it;
	for (it = validScriptExtensions.begin(); it != validScriptExtensions.end(); ++it)
	{
		const std::string &extension = *it;
		size_t lastPos = targetFile.rfind(extension);
		if (lastPos != std::string::npos)
		{
			size_t expectedEndPos = lastPos + extension.length();
			if (expectedEndPos == targetFile.length() ||
				(expectedEndPos < targetFile.length() && targetFile[expectedEndPos] == '/'))
			{
				return true;
			}
		}
	}
	return false;
}

std::string extractScriptPath(const std::string &targetFile, const LocationConfig &pConfig)
{
	std::set<std::string> validScriptExtensions = pConfig.getDirectives().getCgiScriptFilenames();

	std::set<std::string>::iterator it;
	for (it = validScriptExtensions.begin(); it != validScriptExtensions.end(); ++it)
	{
		const std::string &extension = *it;
		size_t pos = targetFile.find("." + extension + "/");
		if (pos != std::string::npos)
		{
			return targetFile.substr(0, pos + extension.length());
		}
		else
		{
			pos = targetFile.rfind(extension);
			if (pos != std::string::npos && (pos + extension.length()) == targetFile.length())
			{
				return targetFile;
			}
		}
	}
	return targetFile;
}
