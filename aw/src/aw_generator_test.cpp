// g++ aw_prototype_a_test.cpp aw_prototype_a.cpp  aw_common.cpp -DSTAND_ALONE -lboost_unit_test_framework -std=c++0x -Wall -o aw_prototype_a


#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE main
#endif
#include <boost/test/unit_test.hpp>


#include "aw_generator.h"
#include "aw_common.h"


BOOST_AUTO_TEST_CASE(aw_prototype_test_1) {
	std::cout << "running aw_prototype_a 1" << std::endl;

	aw::Generator g1;
	g1.print_output();
	
	aw::GeneratorShared g2 = aw::GeneratorShared(new aw::Generator);

}




