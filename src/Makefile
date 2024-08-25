CC = gcc
FLAGS = -Wall -Wextra -Werror

PROJECT_NAME = tetris
LIB_NAME = tetris
LIB_SRC = $(wildcard brick_game/tetris/*.c)
GUI_SRC = $(wildcard gui/cli/*.c)

TEST = s21_test
RM_EXTS := o a out gcno gcda gcov info html css

GCOV_FLAGS=-fprofile-arcs -ftest-coverage -fPIC
CFILES=tetris.a
CEXE=s21_test


OS := $(shell uname)
UBUNTU := $(shell hostnamectl | grep -o 'Ubuntu')
MAC_X86 := $(shell uname -a | grep -o _X86_64)
MAC_M1 := $(shell uname -a | grep -o _ARM64)

ifeq ($(OS), Linux)
	TEST_LIBS = -lcheck -pthread -lrt -lm -lsubunit
	MATH = -lm
	OPEN = xdg-open
else ifeq ($(UBUNTU), Ubuntu)
	TEST_FLAGS=-lpthread -lcheck  -Wno-nonnull -Wno-memset-transposed-args -lm -lsubunit
	OPEN = open
endif

ifeq ($(MAC_M1), _ARM64)
	TEST_LIBS := -I/opt/homebrew/opt/check/include -L/opt/homebrew/opt/check/lib -lcheck
	OPEN = open
endif
ifeq ($(MAC_X86), _X86_64)
	TEST_LIBS = $(shell pkg-config --libs --cflags check) -lm
	OPEN = open
endif

UNAME:=$(shell uname -s)
ifeq ($(UNAME),Linux)
	LDFLAGS=-lcheck -lsubunit -lm -lrt -lpthread
	OPEN_CM=xdg-open
endif
ifeq ($(UNAME),Darwin)
	LDFLAGS=-lcheck -lm
	OPEN_CM=open
endif


all: run

run: install
	build/$(PROJECT_NAME)

build: all

install: tetris.a gui.o main.o
	mkdir -p build/
	$(CC) *.o -lncurses -o build/$(PROJECT_NAME)
	rm -rf *.o

uninstall:
	rm -rf build

tetris.a: $(LIB_NAME).o
	ar rcs $(LIB_NAME).a *.o
	ranlib $(LIB_NAME).a

gcov_s21_test.a: clean
	${CC} ${FLAGS} -c brick_game/tetris/*.c gui/cli/*.c ${GCOV_FLAGS}
	ar rc s21_test.a *.o
	ranlib s21_test.a
	rm -f *.o

$(LIB_NAME).o:
	$(CC) $(FLAGS) -c $(LIB_SRC) $(DEBUG_FLAGS)

gui.o:
	$(CC) $(FLAGS) -c $(GUI_SRC)

main.o:
	$(CC) $(FLAGS) -c main.c

dvi:
	doxygen Doxyfile
	open html/index.html

dist: clean uninstall
	mkdir -p dist
	rsync -av --exclude='dist' . dist/
	cd dist && tar -czvf dist.tar.gz ./*

test:  tetris.a 
	$(CC) $(FLAGS) test/*.c $(LIB_SRC) tetris.a $(TEST_LIBS) -o s21_test
	./$(TEST)

gcov_report: clean gcov_s21_test.a
	$(CC) ${CFLAGS} test/*.c s21_test.a -o ${CEXE} ${LDFLAGS} ${GCOV_FLAGS}
	mkdir report
	./${CEXE} >> report/s21_report.txt
	lcov -c -d . -o coverage.info
	genhtml coverage.info --output-directory report
	rm -rf *.gcda *.gcno *.info
	open report/index.html


vg: clean tetris.a
	$(CC) ${FLAGS} test/*.c  ${CFILES} -o ${CEXE} ${LDFLAGS}
	chmod a+x ${CEXE}
	valgrind --tool=memcheck --leak-check=yes --leak-check=full ./${CEXE}
	rm -rf *.gcno


clean:
	rm -rf $(TEST) *.o *.a *.gcno *.gcda *.gcov *.info report a.out *.dSYM obj s21_test
	@for ext in $(RM_EXTS); do \
	find ./ -type f -name "*.$$ext" -exec rm {} \;; \
	done
	@rm -rf ./report
	@rm -rf ./obj
	rm -rf dist
	rm -rf html

clang:
	clang-format -i *.c
	cd brick_game/tetris && clang-format -i *.c *.h
	cd gui/cli && clang-format -i *.c *.h
	cd test && clang-format -i *.c
	clang-format -i *.c
	cd brick_game/tetris && clang-format -n *.c *.h
	cd gui/cli && clang-format -n *.c *.h
	cd test && clang-format -n *.c