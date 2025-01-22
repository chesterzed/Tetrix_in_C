MAIN_TARGET = a

LCOV = lcov

#utils
DOXYGEN=doxygen
conf_name=brickgame_prop

DIST_FOLD=../dist

INSTALL_FOLD = ../my_tetris
APP_NAME = tetris

BRICK_FOLDER = brick_game/tetris
GUI_FOLDER = gui/cli

DATE = $(shell date -R)
CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11
LIBS = -lncurses

OBJ_FOLDER = obj
SOURCES = $(wildcard $(BRICK_FOLDER)/*.c $(GUI_FOLDER)/*.c)
OBJECTS_BG = $(patsubst $(BRICK_FOLDER)/%.c, $(OBJ_FOLDER)/%.o, $(SOURCES))
OBJECTS_GUI = $(patsubst $(GUI_FOLDER)/%.c, $(OBJ_FOLDER)/%.o, $(SOURCES))

TEST = s21_test
TEST_FOLDER = tests
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

OS := $(shell uname)
UBUNTU := $(shell hostnamectl | grep -o 'Ubuntu')
MAC_X86 := $(shell uname -a | grep -o _X86_64)
MAC_M1 := $(shell uname -a | grep -o _ARM64)

ifeq ($(OS), Linux)
	TEST_LIBS = -lcheck -pthread -lrt -lm -lsubunit
	OPEN = xdg-open
else ifeq ($(UBUNTU), Ubuntu)
	TEST_LIBS=-lpthread -lcheck -Wno-nonnull -Wno-memset-transposed-args -lm -lsubunit -lncurses
	OPEN = open
endif

ifeq ($(MAC_M1), _ARM64)
	TEST_LIBS := $(shell pkg-config --libs --cflags) -lcheck
	#-I/opt/homebrew/opt/check/include -L/opt/homebrew/opt/check/lib -lcheck
	OPEN = open
endif

ifeq ($(MAC_X86), _X86_64)
	TEST_LIBS = $(shell pkg-config --libs --cflags) -lm -lcheck
	OPEN = open
endif
TEST_LIBS=-lpthread -lcheck -Wno-nonnull -Wno-memset-transposed-args -lm -lncurses

all: clean uninstall install dist

install: uninstall
	mkdir -p $(INSTALL_FOLD)
	$(CC) $(FLAGS) $(SOURCES) $(LIBS) -o $(INSTALL_FOLD)/$(APP_NAME)
	chmod +x $(INSTALL_FOLD)/$(APP_NAME)

uninstall:
	rm -rf $(INSTALL_FOLD)

dvi: clean
	$(DOXYGEN) $(conf_name)
	$(OPEN) ./dvi/html/index.html
	
dist:
	mkdir -p $(DIST_FOLD)
	tar cvf $(DIST_FOLD)/tetris.tar ../src/

repo:
	mkdir -p $(OBJ_FOLDER)

test: clean reset
	rm -rf ./saves.txt
	$(CC) $(TEST_FOLDER)/*.c $(BRICK_FOLDER)/fsm.c $(BRICK_FOLDER)/backend.c $(GUI_FOLDER)/*.c $(TEST_LIBS) $(LIBS) $(GCOV_FLAGS) -o $(TEST)
	./$(TEST)

gcov_report: clean reset test
	$(LCOV) -t "gcovreport" -o gcovreport.info -c -d .
	genhtml -o report gcovreport.info
	rm -rf gcovreport gcovreport.info *.gcda *.gcno 
	$(OPEN) ./report/index.html


leak: test
	leaks -atExit -- ./$(TEST) | grep LEAK

brew: reset
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	reset
	brew install lcov
	brew install check
	brew install doxygen
	sleep 1
	reset

apt: reset
	sudo apt install doxygen
	sudo apt install lcov
	sudo apt install check
	sleep 1
	reset

fix: reset
	cp ../materials/linters/.clang-format ./$(BRICK_FOLDER)/.clang-format
	cp ../materials/linters/.clang-format ./$(GUI_FOLDER)/.clang-format
	cd ./$(BRICK_FOLDER)/
	clang-format -n *.c *.h
	clang-format -i *.c *.h
	cd ./$(GUI_FOLDER)/
	clang-format -n *.c *.h
	clang-format -i *.c *.h
	sleep 1
	reset

clean:
	rm -rf $(TEST) *.o *.a *.gcno *.gcda *.gcov *.info a.out *.dSYM obj *.ccss *.html *.tar
	rm -rf dvi html latex report_gcovr report_lcov $(INSTALL_FOLD) $(DIST_FOLD) report

reset:
	reset | bash

git:
	git add .
	git commit -m "$(DATE) fin"
	git push origin develop
