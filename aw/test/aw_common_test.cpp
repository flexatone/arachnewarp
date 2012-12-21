// g++ -I ../src aw_common_test.cpp ../src/aw_common.cpp -DSTAND_ALONE -l boost_filesystem -l boost_system -l boost_unit_test_framework -Wall -o aw_common_test

// -std=c++0x 


#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE main
#endif

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

// stand-alone way of testing with non-compiled boost
//#define BOOST_TEST_MODULE
//#include <boost/test/included/unit_test.hpp>
//#include <boost/test/included/unit_test_framework.hpp> 

#include <iostream>
#include <string>

#include "aw_common.h"


BOOST_AUTO_TEST_CASE(aw_common_test1) {
	//std::cout << "running aw_common_test 1" << std::endl;

    boost::filesystem::path home(aw::get_fp_home());
    // std::cout << home << std::endl;
        
}

BOOST_AUTO_TEST_CASE(aw_common_test2) {
	//std::cout << "running aw_common_test 2" << std::endl;
	
	aw::EnvironmentShared es = aw::EnvironmentShared(new aw::Environment);
	std::string fp = es->get_fp_plot();

	std::string match("plot.plt");
	// give starting position, in source, length of target, target
	
	BOOST_CHECK_EQUAL(fp.compare(fp.length() - match.length(), 
					match.length(), 
					match), 0);
					
	BOOST_CHECK_EQUAL(es->get_sampling_rate(), 44100);
}










