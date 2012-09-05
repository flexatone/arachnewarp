// g++ aw_common_test.cpp aw_common.cpp -Wall -o aw_common_test
// g++ -c aw_common.cpp -Wall 

#define BOOST_TEST_MODULE


#include <boost/test/included/unit_test.hpp>
#include <boost/test/included/unit_test_framework.hpp> 

#include "aw_common.h"


// provide testCase, name
BOOST_AUTO_TEST_CASE(Basic) {

	double val; 
	val = aw::denormalize(0.5, 0, .5);
    BOOST_CHECK_EQUAL(val, 0.25);

	val = aw::denormalize(0.5, 0, 1);
    BOOST_CHECK_EQUAL(val, 0.5);
    
	val = aw::denormalize(0.5, 0, 2);
    BOOST_CHECK_EQUAL(val, 1);    

}