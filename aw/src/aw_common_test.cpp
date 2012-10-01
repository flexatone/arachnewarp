// g++ aw_common_test.cpp aw_common.cpp -DSTAND_ALONE -lboost_unit_test_framework -Wall -o aw_common_test
// -std=c++0x 


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

    // as enum it has an interger value
    BOOST_CHECK_EQUAL(aw::Gen_Name_Constant, 0);
    

}

BOOST_AUTO_TEST_CASE(aw_common_test2) {
	std::cout << "running aw_common_test 2" << std::endl;

    

}

