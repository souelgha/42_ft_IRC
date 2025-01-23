NAME = ircserv


SRC_DIR = src
SRC = main.cpp \
	Server.cpp \
	Client.cpp \
	Channel.cpp \
	join.cpp \
	kick.cpp \
	invite.cpp \
	topic.cpp \
	mode.cpp \
	replies.cpp

OBJ_DIR = obj
OBJ = ${addprefix ${OBJ_DIR}/, ${SRC:.cpp=.o}}


HDR_DIR = hdr
HDR = Server.hpp \
	Client.hpp \
	Channel.hpp
INCLUDE = ${addprefix ${HDR_DIR}/, ${HDR}}


CC = c++
CFLAGS = -Wall -Wextra -Werror -I${HDR_DIR} -I${TPL_DIR} -std=c++98



all: ${NAME}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp ${INCLUDE}
	@mkdir -p ${dir $@}
	${CC} ${CFLAGS} -c $< -o $@

${NAME}: ${OBJ}
	${CC} ${CFLAGS} ${OBJ} -o $@

clean:
	rm -rf ${OBJ_DIR}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
