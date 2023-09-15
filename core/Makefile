##
## EPITECH PROJECT, 2021
## Makefile
## File description:
## No file there , just an epitech header example
##

CC = gcc
src =	src/main.cpp \
	src/libdl/Archive.cpp \
	src/Core.cpp \
	src/menu/Menu.cpp \
	src/context/RenderContext.cpp \
	src/score/Score.cpp

NAME = ../arcade
OBJ = ${src:.cpp=.o}
CRIT_FLAGS = --coverage -lcriterion -I include
CRIT_FILES = src/*.c
CRIT_TEST = tests/*.c
CRIT_FILES_TOTAL = $(CRIT_FILES)
CRIT_TEST_TOTAL = $(CRIT_TEST)

.c.o:
	${CC} -c $< -o ${<:.cpp=.o}

${NAME}: ${OBJ}
	g++ -o ${NAME} ${OBJ} -g3
all : ${NAME}
clean :
	rm -rf ${OBJ}
fclean : clean
	rm -rf ${NAME}
check :
	make -s re
CI:
	$(foreach src, $(src), bash .CI.sh $(src) && ) true

clean_test:
	rm -f *.gcno
	rm -f *.gcda
	rm -f unit_tests

test_crit: clean_test
	$(CC) -o unit_tests $(CRIT_FILES_TOTAL) $(CRIT_TEST_TOTAL) $(CRIT_FLAGS)

tests_run: test_crit
	./unit_tests

re : fclean all

.PHONY: sus re tests_run test_crit clean_test CI check fclean clean all
