// g++ -I ../src aw_plotter_test.cpp ../src/aw_plotter.cpp -DSTAND_ALONE -l boost_filesystem -l boost_system -l boost_unit_test_framework -lboost_unit_test_framework -Wall -o aw_plotter_test


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
#include <vector>
#include <string>
#include <stdexcept>


#include "aw_common.h"
#include "aw_plotter.h"


// provide testCase, name
BOOST_AUTO_TEST_CASE(aw_plot_test1) {
	std::cout << "running aw_plot_test 1" << std::endl;

    // as enum it has an interger value
    //BOOST_CHECK_EQUAL(aw::Gen_Name_Constant, 0);
	
	aw::Plotter p;
	std::vector<aw::SampleType> v;
	for (int i=-100; i<100; ++i) {
		v.push_back(i);
	}
	aw::OutputCountType d(2);

    // need to get an output directory from common
    std::string fp("plot.plt");
    
    BOOST_REQUIRE_THROW(p.plot(v, 0), std::invalid_argument);
    BOOST_REQUIRE_THROW(p.plot(v, -1), std::invalid_argument);
    
	p.plot(v, d);
    //p.pipe(); // remove comment to test generated output
    //p.print();
    //p.write(fp); 
}

BOOST_AUTO_TEST_CASE(aw_plot_test2) {
	std::cout << "running aw_plot_test 2" << std::endl;
}














