t: clean libmatrix.a libalgorithms.a
	g++ -g --std=c++17 main.cc -I include -I algorithms -L ./ -lmatrix -lalgorithms



MAIN_PROJ_NAME=Parallels
TEST_DIR=tests
ALGORITHMS_DIR=algorithms
CLI_DIR=utility_cli
INCLUDE_DIR=include
ALGORITHMS_BUILD_FILES=$(ALGORITHMS_DIR)$(SEP)sle_algorithm$(SEP)*.cc \
					   $(ALGORITHMS_DIR)$(SEP)aco_algorithm$(SEP)*.cc \
					   $(ALGORITHMS_DIR)$(SEP)winograd_algorithm$(SEP)*.cc
TESTBUILD_FILES=$(TEST_DIR)$(SEP)$(MAIN_PROJ_NAME)_tests.cc
CLI_BULD_FILES=$(CLI_DIR)$(SEP)utility_cli.cc
TEST_EXECUTABLE=$(MAIN_PROJ_NAME)_test.out
CLI_EXECUTABLE=prl
COMPILER=g++
STD=--std=c++17
CODE_DIAG_FLAGS=-Wall -Wextra -Werror -Wpedantic
TEST_FLAGS=-lgtest -pthread -lgtest_main
CLANG_FILE=.clang-format
CLANG_FILE_WAY=..$(SEP)materials$(SEP)linters$(SEP)$(CLANG_FILE)
CHECK_FILES=$(ALGORITHMS_BUILD_FILES:.cc=.h) \
			$(ALGORITHMS_BUILD_FILES) \
			$(CLI_DIR)$(SEP)* \
			main.cc
CPPCH_SETUP=--enable=all --suppress=missingInclude --suppress=unusedFunction -v --language=c++ $(STD)
BUILD_DIR=..$(SEP)build
TO_DELETE_FILES=*.o *.out *.dSYM *.gch *.gcda *.gcno .DS_Store $(TEST_EXECUTABLE) $(CLI_EXECUTABLE) $(CLANG_FILE) \
				*.info libmatrix.a libalgorithms.a
TO_DELETE_FOLDERS=$(BUILD_DIR) report *.dSYM

#Crossplatform specs
SEP='\'
MAKEDIR=md
DELETE_FOLDER=RD /S/Q
DELETE_FILE=DEL /F
COPY=copy
OPEN=start
OS=$(shell uname)

ifeq ($(OS), Linux)
	OPEN=xdg-open
	MAKEDIR=mkdir -p
	SEP=/
	DELETE_FOLDER=rm -rf
	DELETE_FILE=rm -f
	COPY=cp
endif

ifeq ($(OS), Darwin)
	OPEN=open
	MAKEDIR=mkdir -p
	SEP=/
	DELETE_FOLDER=rm -rf
	DELETE_FILE=rm -f
	COPY=cp
endif

.PHONY: all check tests clean

all: clean tests prl_cli check

libmatrix.a:
	$(COMPILER) -O3 -c $(INCLUDE_DIR)$(SEP)matrix$(SEP)*.cc
	ar rc libmatrix.a matrix.o
	ranlib libmatrix.a

libalgorithms.a:
	$(COMPILER) -O3 -c $(ALGORITHMS_BUILD_FILES) -I include
	ar rc libalgorithms.a sle.o winograd.o aco.o
	ranlib libalgorithms.a

check:
	cppcheck $(CPPCH_SETUP) $(CHECK_FILES)
	$(COPY) $(CLANG_FILE_WAY) $(CLANG_FILE)
	clang-format -i --style=Google $(CHECK_FILES)
	clang-format -n --style=Google $(CHECK_FILES)

tests: libmatrix.a libalgorithms.a
	# $(COMPILER) $(STD) $(TESTBUILD_FILES) $(CLI_BULD_FILES) $(TEST_FLAGS) \
	# -I $(INCLUDE_DIR) $(LIBS_TO_COMPILE) -o $(TEST_EXECUTABLE)
	# .$(SEP)$(TEST_EXECUTABLE)

gcov_report: gcov_build
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info

gcov_build: libmatrix.a libalgorithms.a
	# $(COMPILER) $(STD) $(TESTBUILD_FILES) $(CLI_BULD_FILES) $(GRAPH_BUILD_FILES) \
	# $(ALGORITHMS_BUILD_FILES) -I $(INCLUDE_DIR) -L. -l$(DOT_LIB) $(TEST_FLAGS) \
	# -o $(TEST_EXECUTABLE) --coverage
	# .$(SEP)$(TEST_EXECUTABLE)

clean:
	$(DELETE_FOLDER) $(TO_DELETE_FOLDERS)
	$(DELETE_FILE) $(TO_DELETE_FILES)

prl_cli:
	