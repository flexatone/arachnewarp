// g++ aw_generator_test.cpp aw_generator.cpp aw_common.cpp aw_plotter.cpp -DSTAND_ALONE -lboost_unit_test_framework -l boost_filesystem -l boost_system -l sndfile -Wall -o aw_generator_test

// -std=c++0x

#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE main
#endif
#include <boost/test/unit_test.hpp>

#include <stdexcept>


#include "aw_generator.h"
#include "aw_common.h"



BOOST_AUTO_TEST_CASE(aw_generator_test_1) {
	aw::GeneratorConfigShared gc1 = aw::GeneratorConfig::make_with_dimension(1);    

	aw::Generator g1(gc1);
    g1.init();
	g1.print_output();
	g1.render(1); // can call directly on object
	g1.print_output();
	g1.render(3); // will render twice, moving to 3
	g1.print_output();
    
    // this has no parameters so should raise exception on trying to set or add
    BOOST_REQUIRE_THROW(g1.set_parameter_by_index(0, 30), std::invalid_argument);

	aw::GeneratorShared g2 = aw::GeneratorShared(new aw::Generator(gc1));
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
	aw::GeneratorConfigShared gc1 = aw::GeneratorConfig::make_with_dimension(1);    

	aw::Constant g3(gc1);
    g3.init();
    g3.print_inputs();

    BOOST_CHECK_EQUAL(g3.get_parameter_count(), 1);
    BOOST_CHECK_EQUAL(g3.get_parameter_index_from_name("Constant numerical value"), 0);
    // check index out of range
    BOOST_REQUIRE_THROW(g3.set_parameter_by_index(2, 30), std::invalid_argument);
    BOOST_REQUIRE_THROW(g3.set_parameter_by_index(1, 23), std::invalid_argument);
	
    
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
	aw::GeneratorConfigShared gc1 = aw::GeneratorConfig::make_with_dimension(1);    
    
	aw::GeneratorShared g1 = aw::ConstantShared(new aw::Constant(gc1));
    g1->init();
	aw::GeneratorShared g2 = aw::ConstantShared(new aw::Constant(gc1));
    g2->init();
	aw::GeneratorShared g3 = aw::AddShared(new aw::Add(gc1));
    g3->init();

	g1->set_parameter_by_index(0, 2);
	g1->print_output();
	g2->set_parameter_by_index(0, 3);
	
	// need to set first, so as to clear out the old one
	g3->add_parameter_by_index(0, g1);
	g3->add_parameter_by_index(0, g2);
    BOOST_REQUIRE_THROW(g3->set_parameter_by_index(1, 23), std::invalid_argument);
    BOOST_REQUIRE_THROW(g3->set_parameter_by_index(-1, 23), std::invalid_argument);
    
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


BOOST_AUTO_TEST_CASE(aw_generator_add_4) {
	// test auto constant creation when adding a sample type
    
	aw::GeneratorShared g1 = aw::Generator::make_with_dimension(
							aw::Generator::ID_Constant, 2);
	g1->set_parameter_by_index(0, 2); 							
	aw::GeneratorShared g2 = aw::Generator::make_with_dimension(
							aw::Generator::ID_Constant, 3);	
	g2->set_parameter_by_index(0, 3); 							
							
	// make a 1 dimensional adder
	aw::GeneratorShared g3 = aw::Generator::make_with_dimension(
							aw::Generator::ID_Add, 1);
	// add opperands
	g3->set_parameter_by_index(0, g1); 							
	g3->add_parameter_by_index(0, g2); 							
		
	// must expand to 3d
    BOOST_CHECK_EQUAL(g3->get_dimension(), 3);

	g3->render(1);	
	g3->print_inputs();
	g3->print_output();
	
	// this results are based on non-interleaved output presentation 
	// sum in dims 1 and 2 are first two opperands
    BOOST_CHECK_CLOSE(g3->output[0], 5, .0000001);
	// at 2 dim boundary
    BOOST_CHECK_CLOSE(g3->output[0+(g3->get_frame_size())], 5, .0000001);
	// at 3 dim boundary value is different because mixed dim of inputs
    BOOST_CHECK_CLOSE(g3->output[0+(g3->get_frame_size()*2)], 3, .0000001);

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
    BOOST_CHECK_EQUAL(g1->dimension_is_resizable(), true);
    BOOST_CHECK_EQUAL(g1->frame_size_is_resizable(), false);
    BOOST_REQUIRE_THROW(g1->set_frame_size(30), std::domain_error);
    
    
	g1->render(20);
	g1->print_output();
	
    BOOST_CHECK_CLOSE(g1->output[0], 12.7, .0000001);
	// this based on defaults and might change
    BOOST_CHECK_EQUAL(g1->get_output_size(), 64);
	
    g1->set_dimension(4); // calls resize and reset
	g1->render(20);
	g1->print_output();
	g1->print_inputs();
    
    BOOST_CHECK_EQUAL(g1->get_dimension(), 4);
	// this based on defaults and might change
    BOOST_CHECK_EQUAL(g1->get_output_size(), 256);
	
	// change frame size

}



BOOST_AUTO_TEST_CASE(aw_generator_config_1) {
    // a simple way to get and pass defaults s
	aw::GeneratorConfigShared gc1 = aw::GeneratorConfig::make_default();
    
    // 64 is a default for testing
    BOOST_CHECK_EQUAL(gc1->get_init_frame_size(), 64);
    BOOST_CHECK_EQUAL(gc1->get_init_frame_dimension(), 1);


	// see if we can get the default-created Environment
	aw::EnvironmentShared e = gc1->get_environment();
	std::string match("plot.plt");
	std::string fp = e->get_fp_plot();
	std::cout << fp << std::endl;
	BOOST_CHECK_EQUAL(fp.compare(fp.length() - match.length(), 
					match.length(), 
					match), 0);	

	aw::GeneratorConfigShared gc2 = aw::GeneratorConfig::make_with_dimension(3);    
    BOOST_CHECK_EQUAL(gc2->get_init_frame_dimension(), 3);

	aw::GeneratorConfigShared gc3 = aw::GeneratorConfig::make_with_dimension(5);    
    BOOST_CHECK_EQUAL(gc3->get_init_frame_dimension(), 5);

}

BOOST_AUTO_TEST_CASE(aw_generator_config_2) {

	aw::GeneratorConfigShared gc1 = aw::GeneratorConfig::make_with_dimension(3);    
    BOOST_CHECK_EQUAL(gc1->get_init_frame_dimension(), 3);
	
	aw::GeneratorShared g1 = aw::GeneratorShared(new aw::Generator(gc1));
    g1->init();
    BOOST_CHECK_CLOSE(g1->output[0], 0, .0000001);
	BOOST_CHECK_EQUAL(g1->get_dimension(), 3);	
	BOOST_CHECK_EQUAL(g1->get_dimension(), 3);
	BOOST_CHECK_EQUAL(g1->get_output_size(), 192); // 64 * 3
	

	aw::GeneratorConfigShared gc2 = aw::GeneratorConfig::make_with_dimension(5);    
    BOOST_CHECK_EQUAL(gc2->get_init_frame_dimension(), 5);
	
	aw::GeneratorShared g2 = aw::GeneratorShared(new aw::Generator(gc2));
	g2->init(); // must call!
	BOOST_CHECK_EQUAL(g2->get_dimension(), 5);
	BOOST_CHECK_EQUAL(g2->get_output_size(), 320); // 64 * 5
    BOOST_CHECK_CLOSE(g2->output[0], 0, .0000001);

}



BOOST_AUTO_TEST_CASE(aw_generator_resize_2) {
	// test auto constant creation when adding a sample type

	aw::GeneratorShared g2 = aw::Generator::make_with_dimension(
                            aw::Generator::ID_Constant, 1);
	g2->add_parameter_by_index(0, 3);
    g2->set_dimension(2); // set to a higher dimension
    
	aw::GeneratorShared g1 = aw::Generator::make_with_dimension(
                            aw::Generator::ID_Add, 1);
	g1->add_parameter_by_index(0, 1);
    BOOST_CHECK_EQUAL(g1->get_dimension(), 1);    
    // adding a 2d gen should cause this to grow
	g1->add_parameter_by_index(0, g2); 
    // now this is 2d	
    BOOST_CHECK_EQUAL(g1->get_dimension(), 2);
	

    g1->render(1);
    BOOST_CHECK_CLOSE(g1->output[0], 4, .0000001);
    
    // test loading output to passed in vector 
    aw::Generator::VSampleType v1; 
    g1->write_output_to_vector(v1);
    BOOST_CHECK_EQUAL(v1.size(), 128);
    BOOST_CHECK_CLOSE(v1[0], 4, .0000001);
        

}






BOOST_AUTO_TEST_CASE(aw_generator_buffer_1) {
	// test auto constant creation when adding a sample type
    
	aw::GeneratorShared g1 = aw::Generator::make_with_dimension(
							aw::Generator::ID_BufferFile, 1);
    BOOST_CHECK_EQUAL(g1->get_output_size(), 64);
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 64);
    BOOST_CHECK_EQUAL(g1->frame_size_is_resizable(), true);
    BOOST_CHECK_EQUAL(g1->dimension_is_resizable(), true);

	g1->set_frame_size(743);
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 743);
    BOOST_CHECK_EQUAL(g1->get_output_size(), 743);
	
	g1->set_frame_size(34);
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 34);	
    BOOST_CHECK_EQUAL(g1->get_output_size(), 34);


	// 2d version
	aw::GeneratorShared g2 = aw::Generator::make_with_dimension(
							aw::Generator::ID_BufferFile, 2);
	g2->set_frame_size(200);
    BOOST_CHECK_EQUAL(g2->get_frame_size(), 200);
    BOOST_CHECK_EQUAL(g2->get_output_size(), 400);

	g2->set_frame_size(40);
    BOOST_CHECK_EQUAL(g2->get_frame_size(), 40);
    BOOST_CHECK_EQUAL(g2->get_output_size(), 80);
	// cannot do this
	
    
}



BOOST_AUTO_TEST_CASE(aw_generator_buffer_2) {    
	aw::GeneratorShared g1 = aw::Generator::make_with_dimension(
							aw::Generator::ID_BufferFile, 1);
	// testing setting the output from a file path
	
    std::string s("12518-sk1Kick.aif");
    g1->set_output_from_fp(s);
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 2641);
    BOOST_CHECK_EQUAL(g1->get_output_size(), 2641);
    
    //g1->plot_output_to_temp_fp();
//    
//    // just checking that averae values are approximate
//    //std::cout << g1->get_output_abs_average() << std::endl;
    BOOST_CHECK_CLOSE(g1->get_output_abs_average(), 0.16651, .001);
//    //g1->print_output();
    
    std::string s2("testStereo1.aif");
    g1->set_output_from_fp(s2);
    
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 888);
    BOOST_CHECK_EQUAL(g1->get_output_size(), 1776);
    // check one dim at a time
    // left is all pos, right is all neg
    BOOST_CHECK_CLOSE(g1->get_output_average(1),  0.460886, .0001);
    BOOST_CHECK_CLOSE(g1->get_output_average(2), -0.484650, .0001);
    
    BOOST_CHECK_CLOSE(g1->get_output_average(0), -0.011881922816371,  .001);
    //g1->plot_output_to_temp_fp();
    
}



BOOST_AUTO_TEST_CASE(aw_generator_buffer_3) {    
	aw::GeneratorShared g1 = aw::Generator::make_with_dimension(
							aw::Generator::ID_BufferFile, 1);
	// test round trip file reading and writing; this is good for valgrind testing as we have to create dyanmic vectors for temporary storage
	
    std::string s("12518-sk1Kick.aif");
    g1->set_output_from_fp(s);	
    g1->write_output_to_fp("testOutput.aif");	
}






BOOST_AUTO_TEST_CASE(aw_generator_phasor_1) {
	// test auto constant creation when adding a sample type
    
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Phasor);
    
    // this will automatically create constant Generators
	//g3->add_parameter_by_index(0, 11);
	//g3->add_parameter_by_index(0, 20);
	
	//g3->render(200);
	//g3->print_output();
	//g3->print_inputs(true);
	
    //BOOST_CHECK_CLOSE(g3->output[0], 31, .0000001);

}






























