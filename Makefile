NAME = ircserv


SRC_DIR = src
SRC = main.cpp


OBJ_DIR = obj
OBJ = ${addprefix ${OBJ_DIR}/, ${SRC:.cpp=.o}}


HDR_DIR = hdr
HDR = 
INCLUDE = ${addprefix ${HDR_DIR}/, ${HDR}}

TPL_DIR = tpl
TPL = 
TEMPLATES = ${addprefix ${TPL_DIR}/, ${TPL}}


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
