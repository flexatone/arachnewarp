// g++ -I ../src aw_generator_test.cpp ../src/aw_generator.cpp ../src/aw_common.cpp ../src/aw_plotter.cpp -DSTAND_ALONE -lboost_unit_test_framework -l boost_filesystem -l boost_system -l sndfile -Wall -g -o aw_generator_test

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

    aw::EnvironmentShared e = aw::Environment::make();
	aw::Generator g1 = aw::Generator(e);
    g1.init();
	g1.print_output();
	g1.render(1); // can call directly on object
	g1.print_output();
	g1.render(3); // will render twice, moving to 3
	g1.print_output();
    
        
    // this has no parameters so should raise exception on trying to set or add
    BOOST_REQUIRE_THROW(g1.set_input_by_index(0, 30), std::invalid_argument);

	aw::GeneratorShared g2 = aw::GeneratorShared(new aw::Generator(e));
    g2->init();
    BOOST_CHECK_CLOSE(g2->matrix[0], 0, .0000001);
	
	g2->render(4);
	g2->print_output();
	// check default

    BOOST_CHECK_EQUAL(g2->get_output_count(), 1);

    BOOST_CHECK_EQUAL(g2->get_matrix_size(), e->get_common_frame_size());
    BOOST_CHECK_EQUAL(g2->get_frame_size(), e->get_common_frame_size());

    BOOST_CHECK_EQUAL(g2->get_sampling_rate(), e->get_sampling_rate());
    BOOST_CHECK_EQUAL(g2->get_nyquist(), 22050);

	
    BOOST_CHECK_EQUAL(g2->get_class_name(), "Generator");
}	


BOOST_AUTO_TEST_CASE(aw_parameter_type_test_2) {
	// testing creation
	aw::ParameterTypeShared pt1 = aw::ParameterTypeShared(new 
                                    aw::ParameterType);
}



BOOST_AUTO_TEST_CASE(aw_generator_constant_test_1) {
    // basic test of constant
    aw::EnvironmentShared e = aw::Environment::make();
	aw::Constant g3(e);
    g3.init();
    g3.print_inputs();

    BOOST_CHECK_EQUAL(g3.get_input_count(), 1);
    BOOST_CHECK_EQUAL(g3.get_input_index_from_parameter_name("Constant numerical value"), 0);
    // check index out of range
    BOOST_REQUIRE_THROW(g3.set_input_by_index(2, 30), std::invalid_argument);
    BOOST_REQUIRE_THROW(g3.set_input_by_index(1, 23), std::invalid_argument);
	
    
	// set a constant value
	g3.set_input_by_index(0, 29);
	//g3.print_output();
	g3.render(4);
	//g3.print_output();
    BOOST_CHECK_CLOSE(g3.matrix[0], 29, .0000001);
    // check that we addition occurs with two constant inputs
	g3.add_input_by_index(0, 1);
	g3.render(4);
    BOOST_CHECK_CLOSE(g3.matrix[0], 30, .0000001);
}



BOOST_AUTO_TEST_CASE(aw_generator_add_1) {
    
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Constant);
	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Constant);
	aw::GeneratorShared g3 = aw::Generator::make(aw::Generator::ID_Add);

	g1->set_input_by_index(0, 2);
	//g1->print_output();
	g2->set_input_by_index(0, 3);
	
	// need to set first, so as to clear out the old one
	g3->add_input_by_index(0, g1);
	g3->add_input_by_index(0, g2);
    BOOST_REQUIRE_THROW(g3->set_input_by_index(1, 23), std::invalid_argument);
    BOOST_REQUIRE_THROW(g3->set_input_by_index(-1, 23), std::invalid_argument);
    
    g3->print_inputs(true);
	
	g3->render(1);
//	//g3->print_output();
	g3->render(8);
//	
    BOOST_CHECK_CLOSE(g1->matrix[0], 2, .0000001);	
    BOOST_CHECK_CLOSE(g2->matrix[0], 3, .0000001);	
    BOOST_CHECK_CLOSE(g3->matrix[0], 5, .0000001);

}


BOOST_AUTO_TEST_CASE(aw_generator_add_2) {
    // do not need to init() when called with make()
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Constant);
	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Constant);
	aw::GeneratorShared g3 = aw::Generator::make(aw::Generator::ID_Add);

	aw::GeneratorShared g4 = aw::Generator::make(aw::Generator::ID_Constant);
	aw::GeneratorShared g5 = aw::Generator::make(aw::Generator::ID_Constant);
	aw::GeneratorShared g6 = aw::Generator::make(aw::Generator::ID_Add);

	g4->set_input_by_index(0, 11);
	g5->set_input_by_index(0, 20);
	g6->add_input_by_index(0, g4);
	g6->add_input_by_index(0, g5);

	g1->set_input_by_index(0, 2);
    g3->add_input_by_index(0, g1);
	g3->add_input_by_index(0, g6);
    BOOST_CHECK_EQUAL(g3->get_input_count(), 1);
    BOOST_CHECK_EQUAL(g3->get_slot_count(), 1);
    
	//g3->print_inputs();
	
	g3->render(1);
	//g3->print_output();
	g3->render(8);
	//g3->print_output();

	//g3->print_inputs(true);
    BOOST_CHECK_CLOSE(g3->matrix[0], 33, .0000001);

    // will create three channels, clear all existing input
    g3->set_slot_by_index(0, 3);
    BOOST_CHECK_EQUAL(g3->get_input_count(), 3);
    BOOST_CHECK_EQUAL(g3->get_slot_count(), 1);
    
	g3->print_inputs();
    
    // TODO: this is  good candidate for graphoz .dot writing

}
//
//
//
//
//BOOST_AUTO_TEST_CASE(aw_generator_add_3) {
//	// test auto constant creation when adding a sample type
//    
//	aw::GeneratorShared g3 = aw::Generator::make(aw::Generator::ID_Add);
//    
//    // this will automatically create constant Generators
//	g3->add_input_by_index(0, 11);
//	g3->add_input_by_index(0, 20);
//	
//	g3->render(200);
//	//g3->print_output();
//	//g3->print_inputs(true);
//	
//    BOOST_CHECK_CLOSE(g3->matrix[0], 31, .0000001);
//
//}
//
//
//BOOST_AUTO_TEST_CASE(aw_generator_add_4) {
//	// test auto constant creation when adding a sample type
//    
//	aw::GeneratorShared g1 = aw::Generator::make_with_dimension(
//							aw::Generator::ID_Constant, 2);
//	g1->set_input_by_index(0, 2); 							
//	aw::GeneratorShared g2 = aw::Generator::make_with_dimension(
//							aw::Generator::ID_Constant, 3);	
//	g2->set_input_by_index(0, 3); 							
//							
//	// make a 1 dimensional adder
//	aw::GeneratorShared g3 = aw::Generator::make_with_dimension(
//							aw::Generator::ID_Add, 1);
//	// add opperands
//	g3->set_input_by_index(0, g1); 							
//	g3->add_input_by_index(0, g2); 							
//		
//	// must expand to 3d
//    BOOST_CHECK_EQUAL(g3->get_output_count(), 3);
//
//	g3->render(1);	
//	//g3->print_inputs();
//	//g3->print_output();
//	
//	// this results are based on non-interleaved matrix presentation 
//	// sum in dims 1 and 2 are first two opperands
//    BOOST_CHECK_CLOSE(g3->matrix[0], 5, .0000001);
//	// at 2 dim boundary
//    BOOST_CHECK_CLOSE(g3->matrix[0+(g3->get_frame_size())], 5, .0000001);
//	// at 3 dim boundary value is different because mixed dim of inputs
//    BOOST_CHECK_CLOSE(g3->matrix[0+(g3->get_frame_size()*2)], 3, .0000001);
//
//}
//
//
//
//
//BOOST_AUTO_TEST_CASE(aw_generator_make_1) {
//	// test auto constant creation when adding a sample type
//    
//	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Add);
//
//    // this will automatically create constant Generators
//	g1->add_input_by_index(0, 1.5);
//	g1->add_input_by_index(0, 1.2);
//	
//    BOOST_CHECK_EQUAL(g1->get_class_name(), "Add");
//    
//	g1->render(50);
//	//g1->print_output();
//	//g1->print_inputs(true);
//	
//    BOOST_CHECK_CLOSE(g1->matrix[0], 2.7, .0000001);
//
//}
//
//
//
//
//BOOST_AUTO_TEST_CASE(aw_generator_resize_1) {
//	// test auto constant creation when adding a sample type
//    
//	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Add);
//
//    // this will automatically create constant Generators
//	g1->add_input_by_index(0, 3.5);
//	g1->add_input_by_index(0, 9.2);
//	
//    BOOST_CHECK_EQUAL(g1->get_class_name(), "Add");
//    BOOST_CHECK_EQUAL(g1->get_dimension_dyanmics(), aw::Generator::DD_ResizableFreely);
//    BOOST_CHECK_EQUAL(g1->frame_size_is_resizable(), false);
//    BOOST_REQUIRE_THROW(g1->_set_frame_size(30), std::domain_error);
//    
//    
//	g1->render(20);
//	//g1->print_output();
//	
//    BOOST_CHECK_CLOSE(g1->matrix[0], 12.7, .0000001);
//	// this based on defaults and might change
//    BOOST_CHECK_EQUAL(g1->get_matrix_size(), 64);
//	
//    g1->_set_output_count(4); // calls resize and reset
//	g1->render(20);
//	//g1->print_output();
//	//g1->print_inputs();
//    
//    BOOST_CHECK_EQUAL(g1->get_output_count(), 4);
//	// this based on defaults and might change
//    BOOST_CHECK_EQUAL(g1->get_matrix_size(), 256);
//	
//	// change frame size
//
//}
//
//
//
//BOOST_AUTO_TEST_CASE(aw_generator_config_1) {
//    // a simple way to get and pass defaults s
//	aw::GeneratorConfigShared gc1 = aw::GeneratorConfig::make_default();
//    
//    // 64 is a default for testing
//    BOOST_CHECK_EQUAL(gc1->get_init_frame_size(), 64);
//
//
//	// see if we can get the default-created Environment
//	aw::EnvironmentShared e = gc1->get_environment();
//
//	std::string match("test.aif");
//	std::string fp = e->get_fp_temp("test.aif");
//	std::cout << fp << std::endl;
//	BOOST_CHECK_EQUAL(fp.compare(fp.length() - match.length(), 
//					match.length(), 
//					match), 0);	
//
//
//}
//
//BOOST_AUTO_TEST_CASE(aw_generator_config_2) {
//
//
//}
//
//
//
//BOOST_AUTO_TEST_CASE(aw_generator_resize_2) {
//	// test auto constant creation when adding a sample type
//
//	aw::GeneratorShared g2 = aw::Generator::make_with_dimension(
//                            aw::Generator::ID_Constant, 1);
//	g2->add_input_by_index(0, 3);
//    g2->_set_output_count(2); // set to a higher _output_count
//    
//	aw::GeneratorShared g1 = aw::Generator::make_with_dimension(
//                            aw::Generator::ID_Add, 1);
//	g1->add_input_by_index(0, 1);
//    BOOST_CHECK_EQUAL(g1->get_output_count(), 1);    
//    // adding a 2d gen should cause this to grow
//	g1->add_input_by_index(0, g2); 
//    // now this is 2d	
//    BOOST_CHECK_EQUAL(g1->get_output_count(), 2);
//	
//
//    g1->render(1);
//    BOOST_CHECK_CLOSE(g1->matrix[0], 4, .0000001);
//    
//    // test loading matrix to passed in vector 
//    aw::Generator::VSampleType v1; 
//    g1->write_output_to_vector(v1);
//    BOOST_CHECK_EQUAL(v1.size(), 128);
//    BOOST_CHECK_CLOSE(v1[0], 4, .0000001);
//        
//
//}
//
//
//
//
//
//
//BOOST_AUTO_TEST_CASE(aw_generator_buffer_1) {
//	// test auto constant creation when adding a sample type
//    
//	aw::GeneratorShared g1 = aw::Generator::make_with_dimension(
//							aw::Generator::ID_BufferFile, 1);
//    BOOST_CHECK_EQUAL(g1->get_matrix_size(), 64);
//    BOOST_CHECK_EQUAL(g1->get_frame_size(), 64);
//    BOOST_CHECK_EQUAL(g1->frame_size_is_resizable(), true);
//    BOOST_CHECK_EQUAL(g1->get_dimension_dyanmics(), aw::Generator::DD_ResizableFreely);
//
//	g1->_set_frame_size(743);
//    BOOST_CHECK_EQUAL(g1->get_frame_size(), 743);
//    BOOST_CHECK_EQUAL(g1->get_matrix_size(), 743);
//	
//	g1->_set_frame_size(34);
//    BOOST_CHECK_EQUAL(g1->get_frame_size(), 34);	
//    BOOST_CHECK_EQUAL(g1->get_matrix_size(), 34);
//
//
//	// 2d version
//	aw::GeneratorShared g2 = aw::Generator::make_with_dimension(
//							aw::Generator::ID_BufferFile, 2);
//	g2->_set_frame_size(200);
//    BOOST_CHECK_EQUAL(g2->get_frame_size(), 200);
//    BOOST_CHECK_EQUAL(g2->get_matrix_size(), 400);
//
//	g2->_set_frame_size(40);
//    BOOST_CHECK_EQUAL(g2->get_frame_size(), 40);
//    BOOST_CHECK_EQUAL(g2->get_matrix_size(), 80);
//	// cannot do this
//	
//    
//}
//
//
//
//BOOST_AUTO_TEST_CASE(aw_generator_buffer_2) {    
//	aw::GeneratorShared g1 = aw::Generator::make_with_dimension(
//							aw::Generator::ID_BufferFile, 1);
//	// testing setting the matrix from a file path
//	
//    std::string s("../test/12518-sk1Kick.aif");
//    g1->set_output_from_fp(s);
//    BOOST_CHECK_EQUAL(g1->get_frame_size(), 2641);
//    BOOST_CHECK_EQUAL(g1->get_matrix_size(), 2641);
//    
//    //g1->plot_output_to_temp_fp();
////    
////    // just checking that averae values are approximate
////    //std::cout << g1->get_matrix_average() << std::endl;
//    BOOST_CHECK_CLOSE(g1->get_matrix_average(), 0.16651, .001);
////    //g1->print_output();
//    
//    std::string s2("../test/testStereo1.aif");
//    g1->set_output_from_fp(s2);
//    
//    BOOST_CHECK_EQUAL(g1->get_frame_size(), 888);
//    BOOST_CHECK_EQUAL(g1->get_matrix_size(), 1776);
//    // check one dim at a time
//    // left is all pos, right is all neg
//    BOOST_CHECK_CLOSE(g1->get_output_average(1),  0.460886, .0001);
//    BOOST_CHECK_CLOSE(g1->get_output_average(2), -0.484650, .0001);
//    
//    BOOST_CHECK_CLOSE(g1->get_output_average(0), -0.011881922816371,  .001);
//    //g1->plot_output_to_temp_fp();
//    
//}
//
//
//
//BOOST_AUTO_TEST_CASE(aw_generator_buffer_3) {    
//	aw::GeneratorShared g1 = aw::Generator::make_with_dimension(
//							aw::Generator::ID_BufferFile, 1);
//	// test round trip file reading and writing; this is good for valgrind testing as we have to create dyanmic vectors for temporary storage
//	
//    std::string s("../test/12518-sk1Kick.aif");
//    g1->set_output_from_fp(s);	
//    g1->write_output_to_fp("testOutput.aif");	
//}
//
//
//
//
//
//
//BOOST_AUTO_TEST_CASE(aw_generator_phasor_1) {    
//	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Phasor);
//    
//    // this will automatically create constant Generators
//	//g1->add_input_by_index(0, 5512.5);
//
//	// this is an eq of 4 samples
//	//g1->add_input_by_index(0, 11025.0); 
//	g1->add_input_by_index(0, 5512.5); // 8 samples
//	g1->render(1);
//	// cycle is 8 samples long
//    BOOST_CHECK_CLOSE(g1->matrix[0], 0, .00001);
//    BOOST_CHECK_CLOSE(g1->matrix[4], .57142857, .00001);	
//    BOOST_CHECK_CLOSE(g1->matrix[7], 1, .00001);
//    BOOST_CHECK_CLOSE(g1->matrix[8], 0, .00001);
//	
//	g1->print_output();
//	
//	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Phasor);
//	g2->add_input_by_index(0, 11025.5); // 8 samples
//	g2->render(1);	
//    BOOST_CHECK_CLOSE(g2->matrix[0], 0, .00001);
//    BOOST_CHECK_CLOSE(g2->matrix[1], 0.333333333, .00001);
//    BOOST_CHECK_CLOSE(g2->matrix[2], 0.666666666, .00001);
//    BOOST_CHECK_CLOSE(g2->matrix[3], 1, .00001);
//    BOOST_CHECK_CLOSE(g2->matrix[4], 0, .00001);
//	
//
//	// this will raise an exception because Phasor is defined as DD_FixedMono, and can only be created with an matrix dimensionality of 1	
//    BOOST_REQUIRE_THROW(aw::Generator::make_with_dimension(aw::Generator::ID_Phasor, 2), 
//						std::invalid_argument);
//
//
//}
//
//
//BOOST_AUTO_TEST_CASE(aw_generator_phasor_2) {
//	// test summing if inputs for frequency
//    
//	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Phasor);
//    
//    // 5512.5 / 2 == 2206.25
//	g1->add_input_by_index(0, 2756.25); //
//	
//	// TODO: add a multidimensional Constant here to make sure that it does not break anything. 
//	g1->add_input_by_index(0, 2756.25); //	
//	g1->render(1);
//	// cycle is 8 samples long
//    BOOST_CHECK_CLOSE(g1->matrix[0], 0, .00001);
//    BOOST_CHECK_CLOSE(g1->matrix[4], .57142857, .00001);	
//    BOOST_CHECK_CLOSE(g1->matrix[7], 1, .00001);
//    BOOST_CHECK_CLOSE(g1->matrix[8], 0, .00001);
//	
//	g1->print_output();
//
//
//}
//
//
//
//
//
//BOOST_AUTO_TEST_CASE(aw_generator_buffer_4) {
//	
//	std::cerr << std::string(80, '-') << std::endl;
//	//aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Recorder);
//
//	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_BufferFile);
//
//	// changing the slot calls overridden _update_for_new_slot(), resizes frame size
//	g1->set_slot_by_index(0, 20.0); // 20 second buffer
//    BOOST_CHECK_EQUAL(g1->get_frame_size(), 882000);
//
//	g1->set_slot_by_index(0, 1.0); // 1 second buffer
//    BOOST_CHECK_EQUAL(g1->get_frame_size(), 44100);
//	
//	g1->set_slot_by_index(0, 5.0); // 5 second buffer
//    BOOST_CHECK_EQUAL(g1->get_frame_size(), 220500);
//	
//	// create 
//	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Phasor);    
//    // 5512.5 / 2 == 2206.25
//	g2->add_input_by_index(0, 2756.25); // a constant frequency
//	
//	// add phasor to buffer input; might scale buffer if necessary; could mix multiple too
//	g1->add_input_by_index(0, g2);
//    BOOST_CHECK_EQUAL(g1->get_frame_size(), 220500);
//	
//	g1->render(1); // count deso not amtter here, as will render to fill frame size
//	
//	
//}
//
//
//
//























