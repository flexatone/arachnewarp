// g++ aw_array_map_test.cpp aw_array_map.cpp aw_common.cpp -Wall -o aw_array_map_test

// g++ -c aw_common.cpp -Wall 

#define BOOST_TEST_MODULE


#include <boost/test/included/unit_test.hpp>
#include <boost/test/included/unit_test_framework.hpp> 

#include "aw_common.h"
#include "aw_array_map.h"


// provide testCase, name
BOOST_AUTO_TEST_CASE(Basic) {

	// this has to be const
	aw::AST const size = 12;
	aw::SVT in1[size] = {0};
	aw::SVT out1[size] = {0};
	
	aw::denormalize(in1, out1, size);
	aw::print(out1, size);
    BOOST_CHECK_EQUAL(true, true);


}