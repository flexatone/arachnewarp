CC=g++

# this is the abs directory for this file 
# TOP := $(dir $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

# # store the operating system name
# UNAME := $(shell uname)

# store the path shift to the src dir
PATH_TO_SRC = ../src/

# the -c option only compiles, producing object files
# this is for basic compiling to .o files
CFLAGS=-I $(PATH_TO_SRC) -c -Wall 

# this is the text ececutable
CFLAGS_TEST=-I $(PATH_TO_SRC) -DSTAND_ALONE -l boost_filesystem -l boost_system -l boost_unit_test_framework -l sndfile -Wall -g

# note that we need to place .cpp and .h files in the dependecies, as these are checked for changes before processing 

# compiling all modules

aw_common.o: $(PATH_TO_SRC)aw_common.h $(PATH_TO_SRC)aw_common.cpp
	$(CC) $(CFLAGS) $(PATH_TO_SRC)aw_common.cpp

aw_timer.o: $(PATH_TO_SRC)aw_timer.h $(PATH_TO_SRC)aw_timer.cpp
	$(CC) $(CFLAGS) $(PATH_TO_SRC)aw_timer.cpp

aw_generator.o: $(PATH_TO_SRC)aw_common.h $(PATH_TO_SRC)aw_common.cpp $(PATH_TO_SRC)aw_generator.h $(PATH_TO_SRC)aw_generator.cpp
	$(CC) $(CFLAGS) $(PATH_TO_SRC)aw_generator.cpp

aw_illustration.o: $(PATH_TO_SRC)aw_illustration.h $(PATH_TO_SRC)aw_illustration.cpp $(PATH_TO_SRC)aw_common.cpp $(PATH_TO_SRC)aw_generator.h
	$(CC) $(CFLAGS) $(PATH_TO_SRC)aw_illustration.cpp


# compiling all tests
# must make object files the dependency, as this will check other dependnecies

aw_common_test: aw_common.o aw_common_test.cpp
	$(CC) $(CFLAGS_TEST) aw_common_test.cpp aw_common.o -o aw_common_test

aw_timer_test: aw_timer.o aw_timer_test.cpp
	$(CC) $(CFLAGS_TEST) aw_timer_test.cpp aw_timer.o -o aw_timer_test

aw_illustration_test: aw_illustration.o aw_illustration_test.cpp
	$(CC) $(CFLAGS_TEST) aw_illustration_test.cpp -o aw_illustration_test

aw_generator_test: aw_common.o aw_generator.o aw_illustration.o aw_generator_test.cpp
	$(CC) $(CFLAGS_TEST) aw_generator_test.cpp -o aw_generator_test


# testing command
# aw_test: aw_common_test.o  aw_timer_test.o  aw_generator_test.o  aw_illustration_test.o

# 	$(CC) $(PATH_TO_TEST)aw_test.cpp  aw_common.o  aw_timer.o  aw_generator.o  aw_illustration.o aw_common_test.o  aw_timer_test.o  aw_generator_test.o  aw_illustration_test.o $(CFLAGS_TEST) -o $(EXE_TEST)


# all: aw_test aw_lib aw_shared

# clean:
# 	rm -rf *o $(EXE_TEST) $(STATIC_LIB) $(DYN_LIB)
	
	
	
	
	