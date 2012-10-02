// g++ aw_timer_test.cpp aw_timer.cpp aw_common.cpp -DSTAND_ALONE -lboost_unit_test_framework  -Wall -o aw_timer_test

// -std=c++0x

#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE main
#endif
#include <boost/test/unit_test.hpp>


#include <iostream>
#include <list>

#include "aw_timer.h"
#include "aw_common.h"



BOOST_AUTO_TEST_CASE(aw_timer_test_1) {

//    BOOST_CHECK_EQUAL(0, 0);
    
    std::cout << "clocks: " << double(std::clock()) << " CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << std::endl;
    
    aw::Timer t("Pushing back on a list");
    t.start();
    
    std::list<int> l;

    for (unsigned long i=0; i<1000000; ++i) {
        l.push_back(i);
    }
    t.stop();

    std::cout << t << std::endl;
    std::cout << "clocks: " << double(std::clock()) << std::endl;
	// on ubuntu/lonovo this returns 60 msec
}


BOOST_AUTO_TEST_CASE(aw_timer_test_2) {

    aw::Timer t("printing without stopping");
    t.start();

    int x(2);
    for (unsigned long i=0; i<1000000; ++i) {
        x+=x;
        if (i % 100000 == 0) std::cout << t << std::endl;
    }
    
    t.stop();
    std::cout << "end time: " << t << std::endl;

	
}

