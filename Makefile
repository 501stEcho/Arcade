##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## No file there , just an epitech header example
##

all :
	@make -C core
	@make -C display
	@make -C game

core :
	@make -C core

games :
	@make -C game

graphicals :
	@make -C display

clean :
	@make clean -C core
	@make clean -C game
	@make clean -C display

fclean :
	@make fclean -C core
	@make fclean -C game
	@make fclean -C display

CI:
	@make CI -C core
	@make CI -C game
	@make CI -C display


clean_test:
	@make clean_test -C core
	@make clean_test -C game
	@make clean_test -C display

test_crit: clean_test
	@make test_crit -C core
	@make test_crit -C game
	@make test_crit -C display

tests_run: test_crit
	@make tests_run -C core
	@make tests_run -C game
	@make tests_run -C display

re : fclean all

.PHONY : game core display all clean fclean CI clean_test test_crit tests_run re
