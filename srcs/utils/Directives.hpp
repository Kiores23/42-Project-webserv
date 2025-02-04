#ifndef WEBSERV_DIRECTIVES_HPP
#define WEBSERV_DIRECTIVES_HPP


#include "../logging/Logger.hpp"
#include "../config/parsingConfigFile.hpp"
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

class Directives
{
    protected:
        std::pair<bool, bool> _autoindex;
        std::set<std::string> _cgiScriptFilenames;
        std::pair<bool, unsigned int> _clientMaxBodySize;
        std::map<int, std::string> _errorPages;
        std::vector<std::string> _indexFiles;
        std::string _root;
        std::string _uploadFilesPath;
        bool _uploadPath;

    public:
        Directives();
        Directives(const Directives& copy);
        Directives& operator=(const Directives& src);
        virtual ~Directives();

        const bool &getAutoindex() const;
        const std::pair<bool, bool> &getAutoindexParsing() const;
        const std::set<std::string> &getCgiScriptFilenames() const;
        const unsigned int &getClientMaxBodySize() const;
        const std::pair<bool, unsigned int> &getClientMaxBodySizeParsing() const;
        const std::string &getErrorCodePath(const int& errorCode) const;
        const std::map<int, std::string> &getErrorPages() const;
        const std::vector<std::string> &getIndexFiles() const;
        const std::string &getRoot() const;
        const std::string &getUploadFilesPath() const;
        const bool &getUploadPath() const;

        void setAutoindex(const std::pair<bool, bool> &autoindex);
        void setCgiScriptFilename(const std::string& cgiScriptFilename);
        void setCgiScriptFilenames(const std::set<std::string>& cgiScriptFilenames);
        void setClientMaxBodySize(const std::pair<bool, unsigned int>& clientMaxBodySize);
        void addErrorPage(const std::pair<int, std::string>& errorPage);
        void setErrorPage(const int& errorCode, const std::string& uri);
        void setIndexFile(const std::string& filePath);
        void setRoot(const std::string& rootPath);
        void setUploadFilesPath(const std::string& uploadFilesPath);
        void setUploadPathBool(const bool& value);
};

#endif
