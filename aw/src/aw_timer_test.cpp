// g++ aw_timer_test.cpp aw_timer.cpp aw_common.cpp -DSTAND_ALONE -lboost_unit_test_framework  -Wall -o aw_timer_test

// -std=c++0x

#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE main
#endif
#include <boost/test/unit_test.hpp>


#include <iostream>

#include "aw_timer.h"
#include "aw_common.h"


BOOST_AUTO_TEST_CASE(aw_generator_test_1) {

    BOOST_CHECK_EQUAL(0, 0);
    
    std::cout << double(std::clock()) << " CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << std::endl;
    
    aw::Timer t("basic");
    t.start();

    int x(2);
    for (int i=0; i<10234923; ++i) {
        x+=x;
    }
    
    t.end();

    std::cout << t << std::endl;

    std::cout << double(std::clock()) << std::endl;
	
}