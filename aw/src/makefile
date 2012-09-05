
aw_common.o: aw_common.h aw_common.cpp
	g++ -c aw_common.cpp
	
aw_common_test.o: aw_common.h aw_common_test.cpp
	g++ -c aw_common_test.cpp
	
# this is the executable
aw_common_test: aw_common.o aw_common_test.o
	g++ aw_common_test.cpp aw_common.o -o aw_common_test
	chmod 755 aw_common_test
	
	
	