
CC=g++

# the -c option only compiles, producing object files

# this is for basic compiling
CFLAGS=-c -Wall 

# this is the text ececutable
CFLAGS_TEST=-lboost_filesystem -lboost_system -lboost_unit_test_framework -lsndfile -Wall

# had to remove -std=c++0x because cannot get gcc4.4 on macos yet

EXECUTABLE=aw_test 


all: $(EXECUTABLE)

# note that we need to place .cpp and .h files in the dependecies, as these are checked for changes before processing 

# compiling all modules

aw_common.o: aw_common.h aw_common.cpp
	$(CC) $(CFLAGS) aw_common.cpp

aw_timer.o: aw_timer.h aw_timer.cpp
	$(CC) $(CFLAGS) aw_timer.cpp

aw_generator.o: aw_common.h aw_common.cpp aw_generator.h aw_generator.cpp
	$(CC) $(CFLAGS) aw_generator.cpp

aw_plotter.o: aw_plotter.h aw_plotter.cpp aw_common.cpp
	$(CC) $(CFLAGS) aw_plotter.cpp





# compiling all tests
# kust make object files the dependency, as this will check other dependnecies

aw_common_test.o: aw_common.o aw_common_test.cpp
	$(CC) $(CFLAGS) aw_common_test.cpp

aw_timer_test.o: aw_timer.o aw_timer_test.cpp
	$(CC) $(CFLAGS) aw_timer_test.cpp

aw_plotter_test.o: aw_plotter.o aw_plotter_test.cpp
	$(CC) $(CFLAGS) aw_plotter_test.cpp

aw_generator_test.o: aw_common.o aw_generator.o aw_generator_test.cpp
	$(CC) $(CFLAGS) aw_generator_test.cpp


# this is the executable; dependent on all object files;
# do not need to include non-test objects as dependencies, because tests will already mandate that these are up to date? Do need to include all with command line options.

aw_test: aw_common_test.o  aw_timer_test.o  aw_generator_test.o  aw_plotter_test.o
	$(CC) aw_test.cpp  aw_common.o  aw_timer.o  aw_generator.o  aw_plotter.o aw_common_test.o  aw_timer_test.o  aw_generator_test.o  aw_plotter_test.o $(CFLAGS_TEST) -o $(EXECUTABLE)

#./$(EXECUTABLE)
clean:
	rm -rf *o $(EXECUTABLE)
	
	
	
	
	