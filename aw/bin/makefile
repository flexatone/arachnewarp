
CC=g++-4.7 -std=c++11

# this is the abs directory for this file 
TOP := $(dir $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

# store the operating system name
UNAME := $(shell uname)


# store the path shift to the src dir
PATH_TO_SRC = ../src/
PATH_TO_TEST = ../test/

# the -c option only compiles, producing object files


# had to remove -std=c++0x because cannot get gcc4.4 on macos yet

# dyn lib name is platform dependent
ifeq ($(UNAME), Darwin)
	CFLAGS=-I $(PATH_TO_SRC) -c -Wall 
	CFLAGS_SHARED=-shared -l boost_filesystem -l boost_system -l sndfile -Wall -O3
    DYN_LIB=libarachnewarp.dylib
endif
ifeq ($(UNAME), Linux)
	# % export LD_LIBRARY_PATH=/usr/local/lib
	# add -L/usr/local/lib to get most-recent boost headers
	# add fPIC to get position independent code
	# -l boost_filesystem -l boost_system -l sndfile
	CFLAGS=-I $(PATH_TO_SRC) -I /usr/local/include/boost -I /usr/include -L /usr/local/lib -c -fPIC -Wall 	
	CFLAGS_SHARED=-shared -L /usr/local/lib -fPIC -Wall -O3
    DYN_LIB=libarachnewarp.so    
endif


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



# this is the executable; dependent on all object files;
# do not need to include non-test objects as dependencies, because tests will already mandate that these are up to date? Do need to include all with command line options.

# these are the names of the products, not the make dependency name
# EXE_TEST=aw_test 
STATIC_LIB=libarachnewarp.a


# shared static library; needs to be moved to /usr/local/lib, etc
# will be used with -larachnewarp
aw_lib: aw_common.o  aw_timer.o  aw_generator.o  aw_illustration.o
	ar -rcs $(STATIC_LIB)  aw_common.o  aw_timer.o  aw_generator.o aw_illustration.o


# dynamic library; may need to create .o files with -fPIC
# .so on linux, .dylib on macm
# not sure what it means to link other shared libraries in with this shared library
aw_shared: aw_common.o  aw_timer.o  aw_generator.o  aw_illustration.o 
	$(CC) $(CFLAGS_SHARED) aw_common.o  aw_timer.o  aw_generator.o  aw_illustration.o -o $(DYN_LIB)


all: aw_lib aw_shared


clean:
	rm -rf *o $(STATIC_LIB) $(DYN_LIB)
	
	
	
	
	