// g++ -I ../src  aw_illustration_test.cpp ../src/aw_illustration.cpp ../src/aw_generator.cpp ../src/aw_common.cpp -DSTAND_ALONE -l boost_filesystem -l boost_system -l boost_unit_test_framework -l boost_unit_test_framework -l sndfile -Wall -o aw_illustration_test


#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE main
#endif
#include <boost/test/unit_test.hpp>


#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>


#include "aw_common.h"
#include "aw_illustration.h"
#include "aw_generator.h"


// provide testCase, name
BOOST_AUTO_TEST_CASE(aw_illustration_test_1) {
	// std::cout << "running aw_illustration_test1 1" << std::endl;

    // as enum it has an interger value
    //BOOST_CHECK_EQUAL(aw::Gen_Name_Constant, 0);
	
	aw::TimeDomainGraph p;
	std::vector<aw::SampleType> v;
	for (int i=-100; i<100; ++i) {
		v.push_back(i);
	}
	aw::OutputCountType d(2);

    // need to get an matrix directory from common
    std::string fp("plot.plt");
    
    BOOST_REQUIRE_THROW(p.draw_vector(v, 0), std::invalid_argument);
    
	p.draw_vector(v, d);
    // p.pipe(); // remove comment to test generated matrix
    //p.print();
    //p.write(fp); 
}

BOOST_AUTO_TEST_CASE(aw_illustration_test_2) {
	std::cout << "running aw_illustration_test_2" << std::endl;
}














