LIBS_LINUX=
ADD_LIB=
LIBNAME=
DEPENDS=sudo apt install -y qtbase5-dev cmake doxygen

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)

	ADD_LIB= -l:s21_smart_calc.a
	LIBNAME= s21_smart_calc.a

	Linux_type := $(shell cat /etc/issue | sed -n '1p' | awk '{print $$1}')

	ifeq ($(Linux_type), Arch)
		LIBS_LINUX = -lm
		DEPENDS=sudo pacman -S --needed qt5-base cmake doxygen
	endif

	ifeq ($(Linux_type), Ubuntu)
		LIBS_LINUX = -lsubunit -lm -pthread -lrt
		DEPENDS=sudo apt-get install -y qtbase5-dev cmake doxygen
	endif

	ifeq ($(Linux_type), Debian)
		LIBS_LINUX = -lsubunit -lm -pthread -lrt
		DEPENDS=sudo apt-get install -y qtbase5-dev cmake doxygen
	endif

endif

ifeq ($(UNAME_S), Darwin)
    ADD_LIB= -ls21_smart_calc
	LIBNAME= libs21_smart_calc.a
	DEPENDS=brew install qt cmake doxygen
endif

CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 -g

OBJS=s21*.o
SRCS_OBJ=s21*.c
TESTS_OBJ=*.c

TARGET=s21_smart_calc
TEST_TARG=check_s21_smart_calc

SRCS_DIR=src/calc_logic	
TESTS_DIR=test

SRCS_H =s21*.h

LIBS = -lcheck $(LIBS_LINUX)

ALL_SRC_OBJ = $(shell find $(SRCS_DIR) -type f -name "$(SRCS_OBJ)")
ALL_TESTS_OBJ = $(shell find $(TESTS_DIR) -type f -name "$(TESTS_OBJ)")

ALL_SRC_H = $(shell find $(SRCS_DIR) -type f -name "$(SRCS_H)")
ALL_TESTS_H = $(shell find $(TESTS_DIR) -type f -name "*.h")

all: install test gcov_report

dependencies:
ifeq ($(Linux_type), Arch)
	@which qt5-base cmake doxygen > /dev/null || $(DEPENDS)
endif
ifeq ($(Linux_type), Ubuntu)
	@dpkg-query -l qtbase5-dev cmake doxygen > /dev/null || $(DEPENDS)
endif
ifeq ($(Linux_type), Debian)
	@dpkg-query -l qtbase5-dev cmake doxygen > /dev/null || $(DEPENDS)
endif
ifeq ($(UNAME_S), Darwin)
	@brew list qt cmake doxygen > /dev/null || $(DEPENDS)
endif

s21_smart_calc.a:
	$(CC) $(CFLAGS) $(ALL_SRC_OBJ) -c
	ar rsc $(LIBNAME) $(OBJS)
	ranlib $(LIBNAME)
	rm -f $(OBJS)

calc: dependencies s21_smart_calc.a 
	cd src/UI/calc && cmake -B../../../build 
	cd build && cmake --build ./

install: calc 
	sudo install -m 755 build/calc /usr/local/bin

uninstall:
	sudo rm -f /usr/local/bin/calc

dvi: dependencies
	doxygen Doxyfile
	xdg-open docs/html/index.html

dist: clean_all
	tar -czvf smart_calc.tar.gz *

test: s21_smart_calc.a
	$(CC) $(CFLAGS) $(ALL_TESTS_OBJ) $(LIBS) -L. $(ADD_LIB) -o $(TEST_TARG) 
	./$(TEST_TARG)

test_val: s21_smart_calc.a test
	valgrind --tool=memcheck --leak-check=yes -s ./$(TEST_TARG)

gcov_report: 
	$(CC) --coverage $(CFLAGS) $(ALL_SRC_OBJ) $(ALL_TESTS_OBJ) $(LIBS) -o $(TEST_TARG)
	./$(TEST_TARG)
	lcov -t "$(TEST_TARG)" -o $(TEST_TARG).info -c -d .
	genhtml -o report $(TEST_TARG).info
	rm -f *.gcda *.gcno *.info $(TEST_TARG)

clean_lib:
	rm -f *.[oa]

clean_cov:
	rm -f *.gcno *.gcda *.info *.gcov $(TEST_TARG)

clean_docs:
	rm -rf docs

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem $(ALL_SRC_OBJ)

clean: clean_lib clean_cov clean_docs
	rm -f $(TEST_TARG)
	rm -rf report *.dSYM
	rm -rf build

clean_all: uninstall clean

rebuild: clean all
