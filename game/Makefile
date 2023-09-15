##
## EPITECH PROJECT, 2021
## Makefile
## File description:
## No file there , just an epitech header example
##

OBJ = $(SRC:.cpp=.o)
PREFIX = ../lib/arcade_
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -fPIC -shared

SNAKE_NAME = snake
SNAKE_SRC = src/Snake.cpp
SNAKE_OBJ = $(SNAKE_SRC:.cpp=.o)
SNAKE_DEST = $(PREFIX)$(SNAKE_NAME).so

NIBBLER_NAME = nibbler
NIBBLER_SRC = src/Nibbler.cpp
NIBBLER_OBJ = $(NIBBLER_SRC:.cpp=.o)
NIBBLER_DEST = $(PREFIX)$(NIBBLER_NAME).so

SOLARFOX_NAME = solarfox
SOLARFOX_SRC = src/SolarFox.cpp
SOLARFOX_OBJ = $(SOLARFOX_SRC:.cpp=.o)
SOLARFOX_DEST = $(PREFIX)$(SOLARFOX_NAME).so

PACMAN_NAME = pacman
PACMAN_SRC = src/Pacman.cpp
PACMAN_OBJ = $(PACMAN_SRC:.cpp=.o)
PACMAN_DEST = $(PREFIX)$(PACMAN_NAME).so

all: $(SNAKE_NAME) $(NIBBLER_NAME) $(SOLARFOX_NAME) $(PACMAN_NAME)

$(SNAKE_NAME): $(SNAKE_OBJ)
	$(CXX) -o $(SNAKE_DEST) $(SNAKE_OBJ) $(CXXFLAGS)

$(NIBBLER_NAME): $(NIBBLER_OBJ)
	$(CXX) -o $(NIBBLER_DEST) $(NIBBLER_OBJ) $(CXXFLAGS)

$(SOLARFOX_NAME): $(SOLARFOX_OBJ)
	$(CXX) -o $(SOLARFOX_DEST) $(SOLARFOX_OBJ) $(CXXFLAGS)

$(PACMAN_NAME): $(PACMAN_OBJ)
	$(CXX) -o $(PACMAN_DEST) $(PACMAN_OBJ) $(CXXFLAGS)

clean:
	rm -f $(OBJ) $(SNAKE_OBJ) $(NIBBLER_OBJ) $(SOLARFOX_OBJ) $(PACMAN_OBJ)

fclean: clean
	rm -f $(SNAKE_DEST) $(NIBBLER_DEST) $(SOLARFOX_DEST) $(PACMAN_DEST)

re: fclean all

main:
	$(CXX) -o ../game_main $(SNAKE_SRC) $(NIBBLER_SRC) $(SOLARFOX_SRC) $(PACMAN_SRC) main.cpp -g3

.PHONY: all clean fclean re
