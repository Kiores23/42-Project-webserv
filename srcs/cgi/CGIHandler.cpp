#include "CGIHandler.hpp"
#include "getScriptInfos.hpp"
#include "../socketio/SocketIO.hpp"
#include "writeBodyToStdin.hpp"
#include "execCgiScript.hpp"
#include "../http/response.hpp"

CGIHandler::CGIHandler() :
		_request(NULL), _clientResponse(NULL), _clientSocket(-1), _locationConfig(NULL),
		_envp(NULL), _outputPipe(), _childPid(-1), _bodyRequired(false)
{
	_outputPipe[0] = -1;
	_outputPipe[1] = -1;
}

CGIHandler::CGIHandler(const CGIHandler &original) :
		_request(NULL), _clientResponse(NULL), _clientSocket(-1), _locationConfig(NULL),
		_envp(NULL), _outputPipe(), _childPid(-1), _bodyRequired(false)
{
	_outputPipe[0] = -1;
	_outputPipe[1] = -1;
	*this = original;
}

CGIHandler &CGIHandler::operator=(const CGIHandler &original)
{
	if (this == &original)
		return *this;
	_outputPipe[0] = original._outputPipe[0];
	_outputPipe[1] = original._outputPipe[1];
	_childPid = original._childPid;
	_clientSocket = original._clientSocket;

	_request = original._request;
	_clientResponse = original._clientResponse;
	_locationConfig = original._locationConfig;

	_env = original._env;
	if (not _env.empty())
		_envp = mapToCArray(_env);
	else
		_envp = NULL;
	return *this;
}

CGIHandler::~CGIHandler()
{
	if (_envp != NULL)
		delete[] _envp;
}

/**
 * @brief Execute a request to the CGI passed in constructor
 * with the informations from the request passed in the constructor.
 * @return The result of executing the CGI script as a string.
 */
void CGIHandler::startCGIScriptCommunication(HttpRequestContext *hrc)
{
	if (pipe(_outputPipe) == -1)
		throw logForError500("CGI: Cannot create pipe");
	_childPid = fork();
	if (_childPid == -1)
	{
		close(_outputPipe[0]);
		close(_outputPipe[1]);
		throw logForError500("CGI: Cannot create fork");
	}
	else if (_childPid == 0)
	{
		CGIEnvironment cgiEnv(this);
		cgiEnv.setEnvForCurrentRequest();
		close(_outputPipe[0]);
		childProcess();
	}
	close(_outputPipe[1]);
	MotherWebserv::getInstance()->addPipe(_outputPipe[0], hrc);
	MotherWebserv::getInstance()->getCgiTimers().add(this, CGI_TIMEOUT);
}

void CGIHandler::childProcess()
{
	passBodyToCGIScript(_outputPipe[1], _request);
	dup2(_outputPipe[1], STDOUT_FILENO);
	close(_outputPipe[1]);

    this->_envp = mapToCArray(_env);

    char * const *ptrEnvp = _envp;
    _envp = NULL;
	
    executeCGIScript(_relativeScriptPath, ptrEnvp);
    _exit(errno);
}

/**
 * @throw logForError500
 */
void CGIHandler::readCGIResponse(HttpRequestContext *hrc)
{
	int isChildAlive;

	if (SocketIO::getInstance()->hasNoPendingRequest(_clientSocket))
	{
		killChild(false);
		return;
	}
	isChildAlive = checkChildFinished(false);
	if (isChildAlive == CHILD_FAIL || _clientResponse->isAvailable())
	{
		MotherWebserv::getInstance()->getCgiTimers().erase(this);
		return;
	}
	int readStatus = SocketIO::readContentFromFd(_outputPipe[0], _clientResponse, NULL);
	if (readStatus == R_ERROR)
	{
		MotherWebserv::getInstance()->removeClientSocket(_outputPipe[0]);
		MotherWebserv::getInstance()->getCgiTimers().erase(this);
		return;
	}
	if (readStatus == R_FINISHED_READING || readStatus == R_CLIENT_DISCONNECTED)
	{
		if (readStatus == R_CLIENT_DISCONNECTED && isChildAlive == CHILD_CONTINUE
			&& _clientResponse->getContentLength() != -1)
		{
			return;
		}
		MotherWebserv::getInstance()->removeClientSocket(_outputPipe[0]);
		MotherWebserv::getInstance()->getCgiTimers().erase(this);
		if (SocketIO::getInstance()->hasNoPendingRequest(_clientSocket))
			return;
		try
		{
			_clientResponse->setData(generateResponseCGI(_clientResponse->getData(), 200, hrc->getRequest()));
		} catch (int code)
		{
			_clientResponse->setData(generateResponse(*_locationConfig, code, _bodyRequired));
		}
		_clientResponse->setIsAvailable(true);
	}
}

int CGIHandler::checkChildFinished(bool isClientDisconnected)
{
	int childStatus;
	int childCode;
	int isChildTerminated;

	isChildTerminated = waitpid(_childPid, &childStatus, WNOHANG);
	if (isChildTerminated > 0)
	{
		if (WIFEXITED(childStatus))
		{
			childCode = WEXITSTATUS(childStatus);
			if (childCode >= EXIT_FAILURE)
			{
				MotherWebserv::getInstance()->removeClientSocket(_outputPipe[0]);
				if (SocketIO::getInstance()->hasNoPendingRequest(_clientSocket) || isClientDisconnected)
					return CHILD_FAIL;
				if (childCode == EACCES)
					_clientResponse->setData(generateResponse(*_locationConfig, 403, _bodyRequired));
				else
					_clientResponse->setData(generateResponse(*_locationConfig, 500, _bodyRequired));
				_clientResponse->setIsAvailable(true);
				return CHILD_FAIL;
			}
		}
		return CHILD_TERMINATED;
	}
	return CHILD_CONTINUE;
}

bool CGIHandler::isSupportedScript(const std::string &targetFile, LocationConfig *locationConfig)
{
	return (isSupportedScriptExtension(targetFile, locationConfig));
}

void CGIHandler::setRelativeScriptPath(const std::string &relativeScriptPath)
{
	_relativeScriptPath = extractScriptPath(relativeScriptPath, *_locationConfig);
}

void CGIHandler::killChild(bool isClientDisconnected)
{
	int childStatus;
	int readPipe = _outputPipe[0];

	if (_childPid <= 0)
		return;
	if (checkChildFinished(isClientDisconnected) == CHILD_CONTINUE)
	{
		if (!terminateChild(childStatus))
			MotherWebserv::getInstance()->getChildsZombies().insert(_childPid);
		bool isPipeFdClosed = SocketIO::getInstance()->isPipeFdFromDeadChild(readPipe);
		DataToEpoll *dataToEpoll = MotherWebserv::getInstance()->getDataToEpoll(readPipe);
		SocketIO::getInstance()->getDataDeadChilds().push_back(dataToEpoll);
		if (dataToEpoll != NULL && not isPipeFdClosed)
		{
			MotherWebserv::getInstance()->removeClientSocket(readPipe);
		}
		if (SocketIO::getInstance()->hasNoPendingRequest(_clientSocket) || isClientDisconnected)
			return;
		_clientResponse->setData(generateResponse(*_locationConfig, 408, _bodyRequired));
		_clientResponse->setIsAvailable(true);
	}
}

void CGIHandler::cleanChild(bool isClientDisconnected)
{
	int childStatus;
	int readPipe = _outputPipe[0];

	if (_childPid <= 0)
		return;
	if (checkChildFinished(isClientDisconnected) == CHILD_CONTINUE)
	{
		terminateChild(childStatus);
	}
	bool isPipeFdClosed = SocketIO::getInstance()->isPipeFdFromDeadChild(readPipe);
	DataToEpoll *dataToEpoll = MotherWebserv::getInstance()->getDataToEpoll(readPipe);
	SocketIO::getInstance()->getDataDeadChilds().push_back(dataToEpoll);
	if (dataToEpoll != NULL && not isPipeFdClosed)
	{
		MotherWebserv::getInstance()->removeClientSocket(readPipe);
	}
}

bool CGIHandler::terminateChild(int &childStatus) const
{
	int isChildTerminated;

	kill(_childPid, SIGTERM);
	isChildTerminated = waitpid(_childPid, &childStatus, WNOHANG);
	if (isChildTerminated <= 0)
	{
		kill(_childPid, SIGKILL);
		isChildTerminated = waitpid(_childPid, &childStatus, WNOHANG);
	}
	return isChildTerminated > 0;
}

const std::string &CGIHandler::getScriptPathInfo() const
{
	return _scriptPathInfo;
}

void CGIHandler::setScriptPathInfo(const std::string &scriptPathInfo)
{
	_scriptPathInfo = scriptPathInfo;
}

const std::string &CGIHandler::getRelativeScriptPath() const
{
	return _relativeScriptPath;
}

const Request &CGIHandler::getRequest() const
{
	return *_request;
}

void CGIHandler::setRequest(Request *request)
{
	_request = request;
}

void CGIHandler::setClientResponse(ClientResponse *clientResponse)
{
	_clientResponse = clientResponse;
}

int CGIHandler::getClientSocket() const
{
	return _clientSocket;
}

void CGIHandler::setClientSocket(int clientSocket)
{
	_clientSocket = clientSocket;
}

void CGIHandler::setLocationConfig(LocationConfig *locationConfig)
{
	_locationConfig = locationConfig;
}

std::map<std::string, std::string> &CGIHandler::getEnv()
{
	return _env;
}

void CGIHandler::setBodyRequired(bool bodyRequired)
{
	_bodyRequired = bodyRequired;
}
