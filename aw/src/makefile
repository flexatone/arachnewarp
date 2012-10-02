
CC=g++

# the -c option only compiles, producing ojbect files
CFLAGS=-c -Wall 
CFLAGS_TEST=-lboost_unit_test_framework -Wall

# had to remove -std=c++0x because cannot get gcc4.4 on macos yet
EXECUTABLE=aw_test 


all: $(EXECUTABLE)

# compiling all modules
aw_common.o:
	$(CC) $(CFLAGS) aw_common.cpp

#aw_generator.o: aw_common.h aw_common.cpp aw_generator.h
	#$(CC) $(CFLAGS) aw_generator.cpp

aw_timer.o: 
	$(CC) $(CFLAGS) aw_timer.cpp

# compiling all tests
aw_common_test.o: aw_common.h aw_common_test.cpp
	$(CC) $(CFLAGS) aw_common_test.cpp



# this is the executable; dependent on all object files; c
aw_test: aw_common.o aw_common_test.o aw_timer.o
	$(CC) aw_test.cpp aw_common_test.o aw_common.o aw_timer.o $(CFLAGS_TEST) -o $(EXECUTABLE)
	# after building we run the tests
	./$(EXECUTABLE)
	
clean:
	rm -rf *o $(EXECUTABLE)
	