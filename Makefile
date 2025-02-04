NAME		= 	webserv

# ---------------------------------------------------
# --------------------  SOURCES  --------------------
# ---------------------------------------------------

SRCS		=	\
				./srcs/cgi/CGIEnvironment.cpp \
				./srcs/cgi/CGIHandler.cpp \
				./srcs/cgi/execCgiScript.cpp \
				./srcs/cgi/getScriptInfos.cpp \
				./srcs/cgi/writeBodyToStdin.cpp \
				\
				./srcs/config/checkConfig.cpp \
				./srcs/config/LocationConfig.cpp \
				./srcs/config/MotherWebservConfig.cpp \
				./srcs/config/parsingConfigFile.cpp \
				./srcs/config/parsingUtils.cpp \
				./srcs/config/ServerConfig.cpp \
				\
				./srcs/http/response.cpp \
				./srcs/http/response_utils.cpp \
				./srcs/http/header.cpp \
				./srcs/http/header_utils.cpp \
				./srcs/http/Request.cpp \
				./srcs/http/Body.cpp \
				./srcs/http/BodyForm.cpp \
				\
				./srcs/execution/execution.cpp \
				./srcs/execution/upload.cpp \
				./srcs/execution/remove.cpp \
				\
				./srcs/networking/MotherWebserv.cpp \
				./srcs/networking/Server.cpp \
				./srcs/networking/Socket.cpp \
				\
				./srcs/logging/Logger.cpp \
				\
				./srcs/server_management/cgiProcess.cpp \
				./srcs/server_management/generatingResponse.cpp \
				./srcs/server_management/readingClientRequest.cpp \
				./srcs/server_management/sendingResponse.cpp \
				./srcs/server_management/serverManager.cpp \
				./srcs/server_management/signal.cpp \
				\
				./srcs/socketio/ClientData.cpp \
				./srcs/socketio/ClientRequest.cpp \
				./srcs/socketio/ClientResponse.cpp \
				./srcs/socketio/HttpRequestContext.cpp \
				./srcs/socketio/SocketIO.cpp \
				\
				./srcs/utils/utils.cpp \
				./srcs/utils/Directives.cpp \
				./srcs/utils/HostPort.cpp \
				./srcs/utils/LocationDirectives.cpp \
				./srcs/utils/ServerDirectives.cpp \
				\
				./srcs/webserv.cpp \

# ---------------------------------------------------
# --------------------  COMPILATION  ----------------
# ---------------------------------------------------

COMPILER	=	g++
FLAGS		+=	-Wall -Wextra -Werror -std=c++98 -Wpedantic -Wshadow -Wconversion
#FLAGS		+=	-Wnewline-eof -Wsign-conversion # clang++ flags
FLAGS		+=	-g				# debug flag for g++
#FLAGS		+=	-gdwarf-4		# debug flag for valgrind to work with clang++

OBJS 		= $(SRCS:.cpp=.o)

%.o 		:	%.cpp
	@$(COMPILER) $(FLAGS) -c $< -o $@

all			:	$(NAME)

$(NAME)		:	$(OBJS)
	@$(COMPILER) $(FLAGS) $(OBJS) -o $(NAME)
	@echo ${BLUE} "> 📚 Compilation finished - $(NAME)" ${END}

# ---------------------------------------------------
# ------------------- OTHER RULES -------------------
# ---------------------------------------------------

RM			=	rm -f
BLUE		:=	"\033[1;34m"
YELLOW		:=	"\033[1;33m"
PURPLE		:=	"\033[1;35m"
END			:=	"\033[0m"

clean	:
	@$(RM) $(OBJS)
	@echo ${PURPLE} "> 🧹 *.o files removed - $(NAME)" ${END}

fclean	:	clean
	@$(RM) $(NAME)
	@echo ${PURPLE} "> 🧹 Binaries removed - $(NAME)" ${END}

re		:	fclean all
	@echo ${YELLOW} "> 📚 Remake done - $(NAME)" ${END}

start	:
	@./webserv tests/configuration-files/tester.conf

run_tester:
	@cd tests/testers && !(./tester.sh)


.PHONY	:	all clean fclean re start run_tester