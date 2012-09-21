
CC=g++
CFLAGS=-c -Wall
CFLAGS_TEST=-lboost_unit_test_framework -Wall

# had to remove -std=c++0x because cannot get gcc4.4 on macos yet
EXECUTABLE=aw_test 


all: $(EXECUTABLE)

# compiling all modules
aw_common.o: aw_common.h aw_common.cpp
	$(CC) $(CFLAGS) aw_common.cpp

aw_array_map.o: aw_array_map.h aw_array_map.cpp
	$(CC) $(CFLAGS) aw_array_map.cpp
	

# compiling all tests
aw_common_test.o: aw_common.h aw_common_test.cpp
	$(CC) $(CFLAGS) aw_common_test.cpp
	
aw_array_map_test.o: aw_common.h aw_array_map.h aw_array_map_test.cpp
	$(CC) $(CFLAGS) aw_array_map_test.cpp	



# this is the executable
aw_test: aw_common.o aw_array_map.o	aw_common_test.o aw_array_map_test.o
	$(CC) aw_test.cpp aw_array_map_test.o aw_common_test.o aw_array_map.o   aw_common.o $(CFLAGS_TEST) -o $(EXECUTABLE)
	./$(EXECUTABLE)
	
clean:
	rm -rf *o $(EXECUTABLE)
	