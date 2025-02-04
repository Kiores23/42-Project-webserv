#include "Directives.hpp"

Directives::Directives()
  : _autoindex(false, false),
    _cgiScriptFilenames(),
    _clientMaxBodySize(false, 20),
    _errorPages(),
    _indexFiles(),
    _root(""),
    _uploadFilesPath(""),
    _uploadPath(false) {
}

Directives::Directives(const Directives& copy)
  : _autoindex(copy._autoindex),
    _cgiScriptFilenames(copy._cgiScriptFilenames),
    _clientMaxBodySize(copy._clientMaxBodySize),
    _errorPages(copy._errorPages),
    _indexFiles(copy._indexFiles),
    _root(copy._root),
    _uploadFilesPath(copy._uploadFilesPath),
    _uploadPath(copy._uploadPath) {
}

Directives& Directives::operator=(const Directives& src) {
  if (this != &src) {
    _autoindex = src._autoindex;
    _cgiScriptFilenames = src._cgiScriptFilenames;
    _clientMaxBodySize = src._clientMaxBodySize;
    _errorPages = src._errorPages;
    _indexFiles = src._indexFiles;
    _root = src._root;
    _uploadFilesPath = src._uploadFilesPath;
    _uploadPath = src._uploadPath;
  }
  return (*this);
}

Directives::~Directives() {
}

/*  Getters */

const bool &Directives::getAutoindex() const {
    return (_autoindex.second);
}

const std::pair<bool, bool> &Directives::getAutoindexParsing() const
{
    return (_autoindex);
}

const std::set<std::string> &Directives::getCgiScriptFilenames() const
{
    return (_cgiScriptFilenames);
}

const unsigned int &Directives::getClientMaxBodySize() const {
    return (_clientMaxBodySize.second);
}

const std::pair<bool, unsigned int> &Directives::getClientMaxBodySizeParsing() const {
    return (_clientMaxBodySize);
}

const std::string &Directives::getErrorCodePath(const int& errorCode) const {
    std::map<int, std::string>::const_iterator it = _errorPages.find(errorCode);
    if (it != _errorPages.end())
        return (it->second);
    throw std::runtime_error("500");
}

const std::map<int, std::string> &Directives::getErrorPages() const {
    return (_errorPages);
}

const std::vector<std::string> &Directives::getIndexFiles() const {
    return (_indexFiles);
}

const std::string &Directives::getRoot() const
{
    return (_root);
}

const std::string &Directives::getUploadFilesPath() const {
    return (_uploadFilesPath);
}

const bool &Directives::getUploadPath() const {
    return (_uploadPath);
}

/*  Setters */

void Directives::setAutoindex(const std::pair<bool, bool> &autoindex)
{
    _autoindex = autoindex;
}

void Directives::setCgiScriptFilename(const std::string& cgiScriptFilename)
{
    std::set<std::string> acceptedCgiScripts;
    acceptedCgiScripts.insert("py");
    acceptedCgiScripts.insert("pl");
	acceptedCgiScripts.insert("php");
	if (acceptedCgiScripts.find(cgiScriptFilename) == acceptedCgiScripts.end())
        Logger::throwAndLogRuntimeError("Cgi script not supported");
    _cgiScriptFilenames.insert(cgiScriptFilename);
}

void Directives::setCgiScriptFilenames(const std::set<std::string>& cgiScriptFilenames) {
    _cgiScriptFilenames = cgiScriptFilenames;
}

void Directives::setClientMaxBodySize(const std::pair<bool, unsigned int>& clientMaxBodySize) {
    _clientMaxBodySize = clientMaxBodySize;
}

void Directives::addErrorPage(const std::pair<int, std::string>& errorPage) {
    _errorPages.insert(errorPage);
}

void Directives::setErrorPage(const int& errorCode, const std::string& uri) {
    _errorPages[errorCode] = uri;
}

void Directives::setIndexFile(const std::string& filePath) {
    for (std::vector<std::string>::const_iterator it = _indexFiles.begin(); it != _indexFiles.end(); ++it) {
        if (filePath == *it)
            return ;
    }
    _indexFiles.push_back(filePath);
}

void Directives::setRoot(const std::string& rootPath) {
    _root = rootPath;
}

void Directives::setUploadFilesPath(const std::string& uploadFilesPath) {
    _uploadFilesPath = uploadFilesPath;
    _uploadPath = true;
}

void Directives::setUploadPathBool(const bool& value) {
    _uploadPath = value;
}
