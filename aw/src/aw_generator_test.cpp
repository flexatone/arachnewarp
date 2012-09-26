// g++ aw_generator_test.cpp aw_generator.cpp aw_common.cpp -DSTAND_ALONE -lboost_unit_test_framework  -Wall -o aw_generator_test
// -std=c++0x

#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE main
#endif
#include <boost/test/unit_test.hpp>


#include "aw_generator.h"
#include "aw_common.h"


BOOST_AUTO_TEST_CASE(aw_generator_test_1) {
	std::cout << "running aw_generator 1" << std::endl;

	aw::Generator g1;
	g1.print_output();
	g1.render(1); // can call directly on object
	g1.print_output();
	g1.render(3); // will render twice, moving to 3
	g1.print_output();

	aw::GeneratorShared g2 = aw::GeneratorShared(new aw::Generator);
	g2->render(4);
	g2->print_output();


	// can creat the table subclass
	aw::Constant g3;
    g3.print_inputs();

    BOOST_CHECK_EQUAL(g3.get_parameter_count(), 1);
    BOOST_CHECK_EQUAL(g3.get_parameter_index_from_name("Constant numerical value"), 0);

	

}

BOOST_AUTO_TEST_CASE(aw_parameter_type_test_2) {
	std::cout << "running aw_generator 2" << std::endl;

	// testing creationg
	aw::ParameterTypeShared pt1 = aw::ParameterTypeShared(new 
                                    aw::ParameterType);

}



