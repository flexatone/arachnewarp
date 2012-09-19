// g++ aw_common_test.cpp aw_common.cpp -DSTAND_ALONE -lboost_unit_test_framework -std=c++0x -Wall -o aw_common_test



#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE main
#endif
#include <boost/test/unit_test.hpp>

// stand-alone way of testing with non-compiled boost
//#define BOOST_TEST_MODULE
//#include <boost/test/included/unit_test.hpp>
//#include <boost/test/included/unit_test_framework.hpp> 

#include <iostream>

#include "aw_common.h"


// provide testCase, name
BOOST_AUTO_TEST_CASE(aw_common_test1) {
	std::cout << "running aw_common_test 1" << std::endl;
	double val; 
	val = aw::denormalize_unit(0.5, 0, .5);
    BOOST_CHECK_EQUAL(val, 0.25);

	val = aw::denormalize_unit(0.5, 0, 1);
    BOOST_CHECK_EQUAL(val, 0.5);
    
	val = aw::denormalize_unit(0.5, 0, 2);
    BOOST_CHECK_EQUAL(val, 1);    

}

BOOST_AUTO_TEST_CASE(aw_common_test2) {
	std::cout << "running aw_common_test 2" << std::endl;
	double val; 

	val = aw::denormalize_unit(0.5, 0, 2);
    BOOST_CHECK_EQUAL(val, 1);
    

}

