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

CURSES_NAME = ncurses
CURSES_SRC = src/NCurses.cpp
CURSES_OBJ = $(CURSES_SRC:.cpp=.o)
CURSES_DEST = $(PREFIX)$(CURSES_NAME).so
CURSES_FLAGS = -lncurses

SFML_NAME = sfml
SFML_SRC = src/SFML.cpp
SFML_OBJ = $(SFML_SRC:.cpp=.o)
SFML_DEST = $(PREFIX)$(SFML_NAME).so
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SDL_NAME = sdl2
SDL_SRC = src/SDL.cpp
SDL_OBJ = $(SDL_SRC:.cpp=.o)
SDL_DEST = $(PREFIX)$(SDL_NAME).so
SDL_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

all: $(CURSES_NAME) $(SFML_NAME) $(SDL_NAME)

$(CURSES_NAME): $(CURSES_OBJ)
	$(CXX) -o $(CURSES_DEST) $(CURSES_OBJ) $(CXXFLAGS) $(CURSES_FLAGS)

$(SFML_NAME): $(SFML_OBJ)
	$(CXX) -o $(SFML_DEST) $(SFML_OBJ) $(CXXFLAGS) $(SFML_FLAGS)

$(SDL_NAME): $(SDL_OBJ)
	$(CXX) -o $(SDL_DEST) $(SDL_OBJ) $(CXXFLAGS) $(SDL_FLAGS)

clean:
	rm -f $(OBJ) $(CURSES_OBJ) $(SFML_OBJ) $(SDL_OBJ)

fclean: clean
	rm -f $(CURSES_DEST) $(SFML_DEST) $(SDL_DEST)

re: fclean all

.PHONY: all clean fclean re
