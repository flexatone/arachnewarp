# this is the abs directory for this file 
# TOP := $(dir $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

# store the path shift to the src dir
PATH_TO_SRC = ../src/
PATH_TO_BIN = ../bin/
PATH_TO_TEST = 

UNAME := $(shell uname)

# the -c option only compiles, producing object files
# this is for basic compiling to .o files

ifeq ($(UNAME), Darwin)
	CC=g++-4.7 -std=c++11
	CFLAGS=-I $(PATH_TO_SRC) -c -Wall 
	CFLAGS_TEST=-I $(PATH_TO_SRC) -DSTAND_ALONE -Wall -g 
endif
ifeq ($(UNAME), Linux)
	# need: % export LD_LIBRARY_PATH=/usr/local/lib
	CC=LD_LIBRARY_PATH=/usr/local/lib g++-4.7 -std=c++11
	# add -L/usr/local/lib to get most-recent boost headers
	CFLAGS=-I $(PATH_TO_SRC) -L /usr/local/lib -c -Wall 	
	CFLAGS_TEST=-I $(PATH_TO_SRC) -DSTAND_ALONE -L /usr/local/lib -Wall -g 
endif

# must follow -o on ubuntu
CFLAGS_LIBS_TEST = -l boost_filesystem -l boost_system -l boost_unit_test_framework -l sndfile


$(PATH_TO_BIN)aw_common.o: $(PATH_TO_SRC)aw_common.h $(PATH_TO_SRC)aw_common.cpp
	$(CC) $(CFLAGS) $(PATH_TO_SRC)aw_common.cpp -o $(PATH_TO_BIN)aw_common.o

$(PATH_TO_BIN)aw_timer.o: $(PATH_TO_SRC)aw_timer.h $(PATH_TO_SRC)aw_timer.cpp
	$(CC) $(CFLAGS) $(PATH_TO_SRC)aw_timer.cpp -o $(PATH_TO_BIN)aw_timer.o

$(PATH_TO_BIN)aw_generator.o: $(PATH_TO_SRC)aw_common.h $(PATH_TO_SRC)aw_common.cpp $(PATH_TO_SRC)aw_generator.h $(PATH_TO_SRC)aw_generator.cpp
	$(CC) $(CFLAGS) $(PATH_TO_SRC)aw_generator.cpp -o $(PATH_TO_BIN)aw_generator.o

$(PATH_TO_BIN)aw_illustration.o: $(PATH_TO_SRC)aw_illustration.h $(PATH_TO_SRC)aw_illustration.cpp $(PATH_TO_SRC)aw_common.cpp $(PATH_TO_SRC)aw_generator.cpp
	$(CC) $(CFLAGS) $(PATH_TO_SRC)aw_illustration.cpp -o $(PATH_TO_BIN)aw_illustration.o



aw_common_test.o: $(PATH_TO_BIN)aw_common.o $(PATH_TO_TEST)aw_common_test.cpp
	$(CC) $(CFLAGS) $(PATH_TO_TEST)aw_common_test.cpp

aw_timer_test.o: $(PATH_TO_BIN)aw_timer.o $(PATH_TO_TEST)aw_timer_test.cpp
	$(CC) $(CFLAGS) $(PATH_TO_TEST)aw_timer_test.cpp

aw_illustration_test.o: $(PATH_TO_BIN)aw_illustration.o $(PATH_TO_TEST)aw_illustration_test.cpp
	$(CC) $(CFLAGS) $(PATH_TO_TEST)aw_illustration_test.cpp

aw_generator_test.o: $(PATH_TO_BIN)aw_common.o $(PATH_TO_BIN)aw_generator.o $(PATH_TO_BIN)aw_illustration.o $(PATH_TO_TEST)aw_generator_test.cpp
	$(CC) $(CFLAGS) $(PATH_TO_TEST)aw_generator_test.cpp



# compiling all tests
# must make object files the dependency, as this will check other dependencies

aw_common_test: $(PATH_TO_BIN)aw_common.o aw_common_test.cpp
	$(CC) $(CFLAGS_TEST) aw_common_test.cpp $(PATH_TO_BIN)aw_common.o -o aw_common_test $(CFLAGS_LIBS_TEST)

aw_timer_test: $(PATH_TO_BIN)aw_timer.o aw_timer_test.cpp
	$(CC) $(CFLAGS_TEST) aw_timer_test.cpp $(PATH_TO_BIN)aw_timer.o -o aw_timer_test $(CFLAGS_LIBS_TEST)

aw_generator_test: $(PATH_TO_BIN)aw_common.o $(PATH_TO_BIN)aw_generator.o $(PATH_TO_BIN)aw_illustration.o aw_generator_test.cpp
	$(CC) $(CFLAGS_TEST) aw_generator_test.cpp $(PATH_TO_BIN)aw_generator.o $(PATH_TO_BIN)aw_illustration.o $(PATH_TO_BIN)aw_common.o -o aw_generator_test $(CFLAGS_LIBS_TEST)

aw_generator_assert: $(PATH_TO_BIN)aw_common.o $(PATH_TO_BIN)aw_generator.o $(PATH_TO_BIN)aw_illustration.o aw_generator_assert.cpp
	$(CC) $(CFLAGS_TEST) aw_generator_assert.cpp $(PATH_TO_BIN)aw_generator.o $(PATH_TO_BIN)aw_illustration.o $(PATH_TO_BIN)aw_common.o -o aw_generator_assert $(CFLAGS_LIBS_TEST)

aw_illustration_test: $(PATH_TO_BIN)aw_common.o $(PATH_TO_BIN)aw_generator.o $(PATH_TO_BIN)aw_illustration.o aw_illustration_test.cpp 
	$(CC) $(CFLAGS_TEST) aw_illustration_test.cpp $(PATH_TO_BIN)aw_generator.o $(PATH_TO_BIN)aw_illustration.o $(PATH_TO_BIN)aw_common.o -o aw_illustration_test $(CFLAGS_LIBS_TEST)

EXE_TEST=aw_test 

# testing command
aw_test: aw_common_test.o  aw_timer_test.o  aw_generator_test.o  aw_illustration_test.o
	$(CC) $(PATH_TO_TEST)aw_test.cpp  $(PATH_TO_BIN)aw_common.o  $(PATH_TO_BIN)aw_timer.o  $(PATH_TO_BIN)aw_generator.o  $(PATH_TO_BIN)aw_illustration.o $(PATH_TO_TEST)aw_common_test.o  $(PATH_TO_TEST)aw_timer_test.o  $(PATH_TO_TEST)aw_generator_test.o  $(PATH_TO_TEST)aw_illustration_test.o $(CFLAGS_TEST) -o $(EXE_TEST) $(CFLAGS_LIBS_TEST)


# testing command
# aw_test: aw_common_test.o  aw_timer_test.o  aw_generator_test.o  aw_illustration_test.o

# 	$(CC) $(PATH_TO_TEST)aw_test.cpp  aw_common.o  aw_timer.o  aw_generator.o  aw_illustration.o aw_common_test.o  aw_timer_test.o  aw_generator_test.o  aw_illustration_test.o $(CFLAGS_TEST) -o $(EXE_TEST)


# all: aw_test aw_lib aw_shared

# clean:
# 	rm -rf *o $(EXE_TEST) $(STATIC_LIB) $(DYN_LIB)
	
	
	
	
	