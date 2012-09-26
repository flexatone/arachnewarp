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

	aw::Generator g1;
	g1.print_output();
	g1.render(1); // can call directly on object
	g1.print_output();
	g1.render(3); // will render twice, moving to 3
	g1.print_output();

	aw::GeneratorShared g2 = aw::GeneratorShared(new aw::Generator);
    BOOST_CHECK_CLOSE(g2->output[0], 0, .0000001);
	
	g2->render(4);
	g2->print_output();
	// check default


}

BOOST_AUTO_TEST_CASE(aw_parameter_type_test_2) {
	// testing creation
	aw::ParameterTypeShared pt1 = aw::ParameterTypeShared(new 
                                    aw::ParameterType);

}


BOOST_AUTO_TEST_CASE(aw_generator_constant_test_1) {
	// can creat the table subclass
	aw::Constant g3;
    g3.print_inputs();

    BOOST_CHECK_EQUAL(g3.get_parameter_count(), 1);
    BOOST_CHECK_EQUAL(g3.get_parameter_index_from_name("Constant numerical value"), 0);
	
	// set a constant value
	g3.set_parameter_by_index(0, 29);
	g3.print_output();
	g3.render(4);
	g3.print_output();
	

    BOOST_CHECK_CLOSE(g3.output[0], 29, .0000001);

	g3.add_parameter_by_index(0, 1);
	g3.render(4);
	g3.print_inputs();
	// this is the sum 
    BOOST_CHECK_CLOSE(g3.output[0], 30, .0000001);
}






BOOST_AUTO_TEST_CASE(aw_generator_add_1) {
	// note: these are all being made of different type
	// is this correct?
	aw::GeneratorShared g1 = aw::ConstantShared(new aw::Constant);
	aw::GeneratorShared g2 = aw::ConstantShared(new aw::Constant);
	aw::GeneratorShared g3 = aw::AddShared(new aw::Add);

	g1->set_parameter_by_index(0, 2);
	g2->set_parameter_by_index(0, 3);
	
	// need to set first, so as to clear out the old one
	g3->add_parameter_by_index(0, g1);
	g3->add_parameter_by_index(0, g2);
	g3->print_inputs();

}



























