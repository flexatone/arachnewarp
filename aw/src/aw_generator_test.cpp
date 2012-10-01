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
    g1.init();
	g1.print_output();
	g1.render(1); // can call directly on object
	g1.print_output();
	g1.render(3); // will render twice, moving to 3
	g1.print_output();

	aw::GeneratorShared g2 = aw::GeneratorShared(new aw::Generator);
    g2->init();
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
    g3.init();
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
    g1->init();
	aw::GeneratorShared g2 = aw::ConstantShared(new aw::Constant);
    g2->init();
	aw::GeneratorShared g3 = aw::AddShared(new aw::Add);
    g3->init();

	g1->set_parameter_by_index(0, 2);
	g1->print_output();
	g2->set_parameter_by_index(0, 3);
	
	// need to set first, so as to clear out the old one
	g3->add_parameter_by_index(0, g1);
	g3->add_parameter_by_index(0, g2);
	g3->print_inputs();
	
	g3->render(1);
	g3->print_output();
	g3->render(8);
	g3->print_output();
	g1->print_output();

	g3->print_inputs(true);
	
    BOOST_CHECK_CLOSE(g1->output[0], 2, .0000001);	
    BOOST_CHECK_CLOSE(g2->output[0], 3, .0000001);	
    BOOST_CHECK_CLOSE(g3->output[0], 5, .0000001);

}


BOOST_AUTO_TEST_CASE(aw_generator_add_2) {
    // do not need to init() when called with make()
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Constant);
	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Constant);
	aw::GeneratorShared g3 = aw::Generator::make(aw::Generator::ID_Add);

	aw::GeneratorShared g4 = aw::Generator::make(aw::Generator::ID_Constant);
	aw::GeneratorShared g5 = aw::Generator::make(aw::Generator::ID_Constant);
	aw::GeneratorShared g6 = aw::Generator::make(aw::Generator::ID_Add);

//	aw::GeneratorShared g1 = aw::ConstantShared(new aw::Constant);
//	aw::GeneratorShared g2 = aw::ConstantShared(new aw::Constant);
//	aw::GeneratorShared g3 = aw::AddShared(new aw::Add);
//
//	aw::GeneratorShared g4 = aw::ConstantShared(new aw::Constant);
//	aw::GeneratorShared g5 = aw::ConstantShared(new aw::Constant);
//	aw::GeneratorShared g6 = aw::AddShared(new aw::Add);


	g4->set_parameter_by_index(0, 11);
	g5->set_parameter_by_index(0, 20);
	g6->add_parameter_by_index(0, g4);
	g6->add_parameter_by_index(0, g5);

	g1->set_parameter_by_index(0, 2);
//	g2->set_parameter_by_index(0, 3);
    g3->add_parameter_by_index(0, g1);
	g3->add_parameter_by_index(0, g6);
	g3->print_inputs();
	
	g3->render(1);
	g3->print_output();
	g3->render(8);
	g3->print_output();

	g3->print_inputs(true);
	
    BOOST_CHECK_CLOSE(g3->output[0], 33, .0000001);

}




BOOST_AUTO_TEST_CASE(aw_generator_add_3) {
	// test auto constant creation when adding a sample type
    
	aw::GeneratorShared g3 = aw::Generator::make(aw::Generator::ID_Add);
    
    // this will automatically create constant Generators
	g3->add_parameter_by_index(0, 11);
	g3->add_parameter_by_index(0, 20);
	
	g3->render(200);
	g3->print_output();
	g3->print_inputs(true);
	
    BOOST_CHECK_CLOSE(g3->output[0], 31, .0000001);

}


BOOST_AUTO_TEST_CASE(aw_generator_make_1) {
	// test auto constant creation when adding a sample type
    
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Add);

    // this will automatically create constant Generators
	g1->add_parameter_by_index(0, 1.5);
	g1->add_parameter_by_index(0, 1.2);
	
    BOOST_CHECK_EQUAL(g1->get_class_name(), "Add");
    
	g1->render(50);
	g1->print_output();
	g1->print_inputs(true);
	
    BOOST_CHECK_CLOSE(g1->output[0], 2.7, .0000001);

}




BOOST_AUTO_TEST_CASE(aw_generator_resize_1) {
	// test auto constant creation when adding a sample type
    
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Add);

    // this will automatically create constant Generators
	g1->add_parameter_by_index(0, 3.5);
	g1->add_parameter_by_index(0, 9.2);
	
    BOOST_CHECK_EQUAL(g1->get_class_name(), "Add");
    
	g1->render(20);
	g1->print_output();
	
    BOOST_CHECK_CLOSE(g1->output[0], 12.7, .0000001);

    g1->set_dimension(4);
	g1->render(20);
	g1->print_output();
	g1->print_inputs();
    
    BOOST_CHECK_EQUAL(g1->get_dimension(), 4);

}













