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

	std::string fp = es->get_fp_temp("test.txt");
	std::string match("test.txt");
	// give starting position, in source, length of target, target
	
	BOOST_CHECK_EQUAL(fp.compare(fp.length() - match.length(), 
					match.length(), 
					match), 0);
					
	BOOST_CHECK_EQUAL(es->get_sampling_rate(), 44100);
}


BOOST_AUTO_TEST_CASE(aw_common_test3) {
	//std::cout << "running aw_common_test 1" << std::endl;

    std::string in("<a test {34}>");

    aw::escape(in, "<>{}", "\\");

    BOOST_CHECK_EQUAL(in, "\\<a test \\{34\\}\\>");

        
}


BOOST_AUTO_TEST_CASE(aw_common_test_inline_1) {

    BOOST_CHECK_EQUAL(aw::double_limiter(.5, 0, 1), .5);
    BOOST_CHECK_EQUAL(aw::double_limiter(1.5, 0, 1), 1);
    BOOST_CHECK_EQUAL(aw::double_limiter(-.5, 0, 1), 0);

}

BOOST_AUTO_TEST_CASE(aw_test_true_min_max) {
    aw::SampleType min(-1);
    aw::SampleType max(-1);
    aw::true_min_max(100, 4, &min, &max);

    BOOST_CHECK_EQUAL(min, 4);
    BOOST_CHECK_EQUAL(max, 100);
    
    min = -1;
    max = -1;
    aw::true_min_max(4, 100, &min, &max);
    BOOST_CHECK_EQUAL(min, 4);
    BOOST_CHECK_EQUAL(max, 100);

    min = -1;
    max = -1;
    aw::true_min_max(3000, -12, &min, &max);
    BOOST_CHECK_EQUAL(min, -12);
    BOOST_CHECK_EQUAL(max, 3000);

}



BOOST_AUTO_TEST_CASE(aw_test_mtof) {

    aw::SampleType x;
    x = aw::mtof(69);
    BOOST_CHECK_CLOSE(x, 440.0, .0001);

    x = aw::mtof(69+12);
    BOOST_CHECK_CLOSE(x, 880.0, .0001);

}

BOOST_AUTO_TEST_CASE(aw_test_environment_1) {

    aw::EnvironmentShared e1 = aw::Environment::make();
    BOOST_CHECK_EQUAL(e1->get_common_frame_size(), 64);

    aw::EnvironmentShared e2 = aw::Environment::make_with_frame_size(128);
    BOOST_CHECK_EQUAL(e2->get_common_frame_size(), 128);

    
}





















