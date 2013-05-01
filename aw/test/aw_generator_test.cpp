// g++ -I ../src  aw_generator_test.cpp ../src/aw_generator.cpp ../src/aw_common.cpp ../src/aw_illustration.cpp -DSTAND_ALONE -l boost_unit_test_framework -l boost_filesystem -l boost_system -l sndfile -Wall -g -o aw_generator_test

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
	//g1.print_outputs();
	g1.render(1); // can call directly on object
	//g1.print_outputs();
	g1.render(3); // will render twice, moving to 3
	//g1.print_outputs();
    
        
    // this has no parameters so should raise exception on trying to set or add
    BOOST_REQUIRE_THROW(g1.set_input_by_index(0, 30), std::invalid_argument);

	aw::GeneratorShared g2 = aw::GeneratorShared(new aw::Generator(e));
    g2->init();
    BOOST_CHECK_CLOSE(g2->outputs[0][0], 0, .0000001);
	
	g2->render(4);
	//g2->print_outputs();
	// check default

    BOOST_CHECK_EQUAL(g2->get_output_count(), 1);

    BOOST_CHECK_EQUAL(g2->get_outputs_size(), e->get_common_frame_size());
    BOOST_CHECK_EQUAL(g2->get_frame_size(), e->get_common_frame_size());

    BOOST_CHECK_EQUAL(g2->get_sampling_rate(), e->get_sampling_rate());
    BOOST_CHECK_EQUAL(g2->get_nyquist(), 22050);

	
    BOOST_CHECK_EQUAL(g2->get_class_name(), "Generator");
}	


BOOST_AUTO_TEST_CASE(aw_parameter_type_test_1) {
	// testing creation
	aw::ParameterTypeShared pt1 = aw::ParameterTypeShared(new 
                                    aw::ParameterType);
	BOOST_CHECK_EQUAL(pt1->get_class_name(), "ParameterType");

	aw::ParameterTypeShared pt2 = aw::ParameterType::make(aw::ParameterType::ID_Phase);
	BOOST_CHECK_EQUAL(pt2->get_class_name(), "ParameterTypePhase");

	pt2->set_instance_name("degree of spin");
	BOOST_CHECK_EQUAL(pt2->get_instance_name(), "degree of spin");

	aw::ParameterTypeShared pt3 = aw::ParameterType::make(aw::ParameterType::ID_Value);
	BOOST_CHECK_EQUAL(pt3->get_class_name(), "ParameterTypeValue");

	aw::ParameterTypeShared pt4 = aw::ParameterType::make(aw::ParameterType::ID_Frequency);
	BOOST_CHECK_EQUAL(pt4->get_class_name(), "ParameterTypeFrequency");


	aw::ParameterTypeShared pt5 = aw::ParameterType::make(aw::ParameterType::ID_Duration);
	BOOST_CHECK_EQUAL(pt5->get_class_name(), "ParameterTypeDuration");

	aw::ParameterTypeShared pt6 = aw::ParameterType::make(aw::ParameterType::ID_Channels);
	BOOST_CHECK_EQUAL(pt6->get_class_name(), "ParameterTypeChannels");

	aw::ParameterTypeShared pt7 = aw::ParameterType::make(aw::ParameterType::ID_Trigger);
	BOOST_CHECK_EQUAL(pt7->get_class_name(), "ParameterTypeTrigger");


}



BOOST_AUTO_TEST_CASE(aw_generator_constant_test_1) {
    // basic test of constant
    aw::EnvironmentShared e = aw::Environment::make();
	aw::Constant g3(e);
    g3.init();
    //g3.print_inputs();

    BOOST_CHECK_EQUAL(g3.get_input_count(), 1);
    BOOST_CHECK_EQUAL(g3.get_input_index_from_parameter_name("Constant numerical value"), 0);
    // check index out of range
    BOOST_REQUIRE_THROW(g3.set_input_by_index(2, 30), std::invalid_argument);
    BOOST_REQUIRE_THROW(g3.set_input_by_index(1, 23), std::invalid_argument);
	
    
	// set a constant value
	g3.set_input_by_index(0, 29);
	//g3.print_outputs();
	g3.render(4);
	//g3.print_outputs();
    BOOST_CHECK_CLOSE(g3.outputs[0][0], 29, .0000001);
    // check that we addition occurs with two constant inputs
	g3.add_input_by_index(0, 1);
	g3.render(4);
    BOOST_CHECK_CLOSE(g3.outputs[0][0], 30, .0000001);
}



BOOST_AUTO_TEST_CASE(aw_generator_add_1) {
    
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Constant);
	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Constant);
	aw::GeneratorShared g3 = aw::Generator::make(aw::Generator::ID_Add);

	g1->set_input_by_index(0, 2);
	//g1->print_outputs();
	g2->set_input_by_index(0, 3);
	
	// need to set first, so as to clear out the old one
	g3->add_input_by_index(0, g1);
	g3->add_input_by_index(0, g2);
    BOOST_REQUIRE_THROW(g3->set_input_by_index(1, 23), std::invalid_argument);
    BOOST_REQUIRE_THROW(g3->set_input_by_index(-1, 23), std::invalid_argument);
    
    //g3->print_inputs(true);
	
	g3->render(1);
//	//g3->print_outputs();
	g3->render(8);
//	
    BOOST_CHECK_CLOSE(g1->outputs[0][0], 2, .0000001);	
    BOOST_CHECK_CLOSE(g2->outputs[0][0], 3, .0000001);	
    BOOST_CHECK_CLOSE(g3->outputs[0][0], 5, .0000001);

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
	//g3->print_outputs();
	g3->render(8);
	//g3->print_outputs();

	//g3->print_inputs(true);
    BOOST_CHECK_CLOSE(g3->outputs[0][0], 33, .0000001);

    // will create three channels, clear all existing input
    g3->set_slot_by_index(0, 3);
    BOOST_CHECK_EQUAL(g3->get_input_count(), 3);
    BOOST_CHECK_EQUAL(g3->get_slot_count(), 1);
    
	//g3->print_inputs();
    
    // TODO: this is  good candidate for graphoz .dot writing

}

BOOST_AUTO_TEST_CASE(aw_generator_add_3) {
	// test auto constant creation when adding a sample type
    
	aw::GeneratorShared g3 = aw::Generator::make(aw::Generator::ID_Add);
    
    // this will automatically create constant Generators
	g3->add_input_by_index(0, 11);
	g3->add_input_by_index(0, 20);
	
	g3->render(200);
	//g3->print_outputs();
	//g3->print_inputs(true);
	
    BOOST_CHECK_CLOSE(g3->outputs[0][0], 31, .0000001);

}


BOOST_AUTO_TEST_CASE(aw_generator_add_4) {
	// test auto constant creation when adding a sample type
    
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Constant);
	g1->set_input_by_index(0, 2); 							
	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Constant);	
	g2->set_input_by_index(0, 3); 							
							
	// make a 1 dimensional adder
	aw::GeneratorShared g3 = aw::Generator::make(aw::Generator::ID_Add);
	// add opperands
	g3->set_input_by_index(0, g1); 							
	g3->add_input_by_index(0, g2); 							
		
	// stays the same
    BOOST_CHECK_EQUAL(g3->get_output_count(), 1);

	g3->render(1);	
	//g3->print_inputs();
	//g3->print_outputs();
	
	// this results are based on non-interleaved outputs presentation 
	// sum in dims 1 and 2 are first two opperands
    BOOST_CHECK_CLOSE(g3->outputs[0][0], 5, .0000001);

}


BOOST_AUTO_TEST_CASE(aw_generator_make_1) {
    // floating point addition
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Add);

    // this will automatically create constant Generators
	g1->add_input_by_index(0, 1.5);
	g1->add_input_by_index(0, 1.2);
	
    BOOST_CHECK_EQUAL(g1->get_class_name(), "Add");
    
	g1->render(50);
	//g1->print_outputs();
	//g1->print_inputs(true);
	
    BOOST_CHECK_CLOSE(g1->outputs[0][0], 2.7, .0000001);

}

BOOST_AUTO_TEST_CASE(aw_generator_resize_1) {
	// test auto constant creation when adding a sample type
    
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Add);

    // this will automatically create constant Generators
	g1->add_input_by_index(0, 3.5);
	g1->add_input_by_index(0, 9.2);
	
    BOOST_CHECK_EQUAL(g1->get_class_name(), "Add");
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 64);
    BOOST_CHECK_EQUAL(g1->frame_size_is_resizable(), false);
    BOOST_CHECK_EQUAL(g1->get_outputs_size(), 64);    
    
    // BOOST_REQUIRE_THROW(g1->_set_frame_size(30), std::domain_error);

    // slots will chane inputs and output
    g1->set_slot_by_index(0, 3);
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 64);
    BOOST_CHECK_EQUAL(g1->get_output_count(), 3);
    BOOST_CHECK_EQUAL(g1->get_outputs_size(), 192);

	g1->add_input_by_index(0, 1);
	g1->add_input_by_index(0, 2);
	g1->add_input_by_index(1, 10);
	g1->add_input_by_index(1, 11);
	g1->add_input_by_index(2, 101);
	g1->add_input_by_index(2, 102);

	g1->render(20);
    
    BOOST_CHECK_CLOSE(g1->outputs[0][0], 3, .0000001);
    BOOST_CHECK_CLOSE(g1->outputs[1][0], 21, .0000001);
    BOOST_CHECK_CLOSE(g1->outputs[2][0], 203, .0000001);


    g1->set_slot_by_index(0, 5); // this also resets render count
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 64);
    BOOST_CHECK_EQUAL(g1->get_output_count(), 5);
    BOOST_CHECK_EQUAL(g1->get_outputs_size(), 320);
    BOOST_CHECK_EQUAL(g1->get_input_count(), 5);

    // inputs are cleared when resizing happens, we can add new ones
	g1->add_input_by_index(0, 3);
	g1->add_input_by_index(0, 4);
	g1->add_input_by_index(1, 30);
	g1->add_input_by_index(1, 31);
    
	g1->render(2);    
	
    BOOST_CHECK_CLOSE(g1->outputs[0][0], 7, .0000001);
    BOOST_CHECK_CLOSE(g1->outputs[1][0], 61, .0000001);

    g1->set_slot_by_index(0, 1);
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 64);
    BOOST_CHECK_EQUAL(g1->get_output_count(), 1);
    BOOST_CHECK_EQUAL(g1->get_outputs_size(), 64);

    // input at index 1 is not available now
    BOOST_REQUIRE_THROW(g1->add_input_by_index(1, 4), std::invalid_argument);

    
	g1->add_input_by_index(0, 3.5);
	g1->add_input_by_index(0, 9.2);
    
	g1->render(20);
	//g1->print_outputs();
	
    BOOST_CHECK_CLOSE(g1->outputs[0][0], 12.7, .0000001);
	// this based on defaults and might change
    BOOST_CHECK_EQUAL(g1->get_outputs_size(), 64);
	
    g1->set_slot_by_index(0, 4);
	g1->render(20);
	//g1->print_outputs();
	//g1->print_inputs();
    
    BOOST_CHECK_EQUAL(g1->get_output_count(), 4);
	// this based on defaults and might change
    BOOST_CHECK_EQUAL(g1->get_outputs_size(), 256);
	
	// change frame size

}



BOOST_AUTO_TEST_CASE(aw_generator_buffer_1) {
	// test auto constant creation when adding a sample type
    
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Buffer);
    BOOST_CHECK_EQUAL(g1->get_outputs_size(), 44100);
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 44100);
    BOOST_CHECK_EQUAL(g1->frame_size_is_resizable(), true);
    
    // changing duration to two seconds
    g1->set_slot_by_index(1, 2); 
    
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 88200);
    BOOST_CHECK_EQUAL(g1->get_outputs_size(), 88200);

	// set number of channels; duration is still 2
    g1->set_slot_by_index(0, 3); 
	BOOST_CHECK_EQUAL(g1->get_output_count(), 3);
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 88200);	
    BOOST_CHECK_EQUAL(g1->get_outputs_size(), 264600);
	
	// duration of .5 sec, one ouput
    g1->set_slot_by_index(1, .5); 
    g1->set_slot_by_index(0, 1); 
	BOOST_CHECK_EQUAL(g1->get_output_count(), 1);
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 22050);	
    BOOST_CHECK_EQUAL(g1->get_outputs_size(), 22050);	

}



BOOST_AUTO_TEST_CASE(aw_generator_buffer_2) {    
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Buffer);
	// testing setting the outputs from a file path
	
    std::string s("../test/12518-sk1Kick.aif");
    g1->set_outputs_from_fp(s);
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 2641);
    BOOST_CHECK_EQUAL(g1->get_outputs_size(), 2641);
    
    //g1->illustrate_outputs();
	
    BOOST_CHECK_CLOSE(g1->get_outputs_average(), 0.16651, .001);
    
    std::string s2("../test/testStereo1.aif");
    g1->set_outputs_from_fp(s2);
    
    BOOST_CHECK_EQUAL(g1->get_output_count(), 1);
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 888);
    BOOST_CHECK_EQUAL(g1->get_outputs_size(), 888); // we only read in one channel
    // g1->illustrate_outputs();
    BOOST_CHECK_CLOSE(g1->get_output_average(1),  0.460886, .0001);

	// set to two channels
    g1->set_slot_by_index(0, 2); 
    g1->set_outputs_from_fp(s2);
    BOOST_CHECK_EQUAL(g1->get_output_count(), 2);
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 888);	
    BOOST_CHECK_EQUAL(g1->get_outputs_size(), 1776);
    // g1->illustrate_outputs();
	
    ////// check one dim at a time
    ////// left is all pos, right is all neg
    BOOST_CHECK_CLOSE(g1->get_output_average(1),  0.460886, .0001);
    BOOST_CHECK_CLOSE(g1->get_output_average(2), -0.484650, .0001);
    
    BOOST_CHECK_CLOSE(g1->get_output_average(0), -0.011881922816371,  .001);
    
}


BOOST_AUTO_TEST_CASE(aw_generator_buffer_3) {    
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Buffer);
	// test round trip file reading and writing; this is good for valgrind testing as we have to create dyanmic vectors for temporary storage
    std::string s("../test/12518-sk1Kick.aif");
    g1->set_outputs_from_fp(s);	
    g1->write_output_to_fp("testOutput.aif");	
}



BOOST_AUTO_TEST_CASE(aw_generator_phasor_1) {    
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Phasor);
    
    // this will automatically create constant Generators
	//g1->add_input_by_index(0, 5512.5);

	// this is an eq of 4 samples
	//g1->add_input_by_index(0, 11025.0); 
	g1->add_input_by_index(0, 5512.5); // 8 samples
	g1->render(1);
	// cycle is 8 samples long
    BOOST_CHECK_CLOSE(g1->outputs[0][0], 0, .00001);
    BOOST_CHECK_CLOSE(g1->outputs[1][0], 1, .00001); // trigger 
    BOOST_CHECK_CLOSE(g1->outputs[0][4], .57142857, .00001);	
    BOOST_CHECK_CLOSE(g1->outputs[0][7], 1, .00001);
    BOOST_CHECK_CLOSE(g1->outputs[0][8], 0, .00001);
	
	//g1->print_outputs();
    //g1->illustrate_outputs();
	
	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Phasor);
	g2->add_input_by_index(0, 11025.5); // 8 samples
	g2->render(1);	
    BOOST_CHECK_CLOSE(g2->outputs[0][0], 0, .00001);
    BOOST_CHECK_CLOSE(g2->outputs[0][1], 0.333333333, .00001);
    BOOST_CHECK_CLOSE(g2->outputs[0][2], 0.666666666, .00001);
    BOOST_CHECK_CLOSE(g2->outputs[0][3], 1, .00001);
    BOOST_CHECK_CLOSE(g2->outputs[0][4], 0, .00001);
    BOOST_CHECK_CLOSE(g2->outputs[1][4], 1, .00001); // trigger 	


}



BOOST_AUTO_TEST_CASE(aw_generator_phasor_2) {
	// test summing if inputs for frequency
    
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Phasor);
    
    // 5512.5 / 2 == 2206.25
	g1->add_input_by_index(0, 2756.25); //	
	g1->add_input_by_index(0, 2756.25); //	
	g1->render(1);
	// cycle is 8 samples long
    BOOST_CHECK_CLOSE(g1->outputs[0][0], 0, .00001);
    BOOST_CHECK_CLOSE(g1->outputs[0][4], .57142857, .00001);	
    BOOST_CHECK_CLOSE(g1->outputs[0][7], 1, .00001);
    BOOST_CHECK_CLOSE(g1->outputs[0][8], 0, .00001);
    BOOST_CHECK_CLOSE(g1->outputs[1][8], 1, .00001); // trigger
	
	// g1->print_outputs();

}




BOOST_AUTO_TEST_CASE(aw_generator_buffer_4) {
	
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Buffer);

	// changing the slot calls overridden _update_for_new_slot(), resizes frame size
	g1->set_slot_by_index(1, 20.0); // 20 second buffer
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 882000);

	g1->set_slot_by_index(1, 1.0); // 1 second buffer
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 44100);
		
	// create 
	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Phasor);    
	g2->add_input_by_index(0, 4); // a constant frequency
	
	// add phasor to buffer input; might scale buffer if necessary; could mix multiple too
	g1->add_input_by_index(0, g2);
	
	// for one render cycle of the buffer, we render inputs until we fill the frame
	// need to time this generation 
	//g1->render(1); 
	//g1->illustrate_outputs();
	BOOST_CHECK_EQUAL(g1->outputs[0][0], 0);
	// TODO: why are these values not found
	//BOOST_CHECK_CLOSE(g1->outputs[11025], 1, .001);
	//BOOST_CHECK_CLOSE(g1->outputs[11026], 0, .001);
	
}


BOOST_AUTO_TEST_CASE(aw_generator_buffer_5) {
	
	//aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Recorder);

	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Buffer);
	// create two channel buffer
	g1->set_slot_by_index(0, 2);
	// for one second
	g1->set_slot_by_index(1, .01); // 441 samples
		
	// create 
	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Phasor);    
	g2->add_input_by_index(0, 2); // a constant frequency, period of 22050
	
	aw::GeneratorShared g3 = aw::Generator::make(aw::Generator::ID_Phasor);    
	g3->add_input_by_index(0, 4410); // a constant frequency, period of 10 

	// add phasor to buffer input; might scale buffer if necessary; could mix multiple too
	g1->add_input_by_index(0, g2);
	g1->add_input_by_index(1, g3);
	
        
	// for one render cycle of the buffer, we render inputs until we fill the frame
	// need to time this generation 
	g1->render(1); // render count here meaningless
	//g1->illustrate_outputs();
	
    BOOST_CHECK_EQUAL(g1->outputs[0].size(), 441);
    

	//g1->print_outputs(0, 100);      
	// last sample should be at 1 for both
	//g1->print_outputs(g1->outputs[0].size()-100, 0);  
    
	//BOOST_CHECK_CLOSE(g1->outputs[0][4409], 1, .001);
	//BOOST_CHECK_CLOSE(g1->outputs[0][11025], 0, .001);
	
}



BOOST_AUTO_TEST_CASE(aw_generator_multiply_1) {
    // test basic multiplication
    aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Multiply);
    // sets channels
    g1->set_slot_by_index(0, 1);
    
    g1->add_input_by_index(0, 3);
    g1->add_input_by_index(0, 2);
    
    //g1->print_inputs();
    g1->render(2);
    
	BOOST_CHECK_CLOSE(g1->outputs[0][0], 6, .001);
    
    g1->add_input_by_index(0, 20);
    g1->render(3);

	BOOST_CHECK_CLOSE(g1->outputs[0][0], 120, .001);
    
}


BOOST_AUTO_TEST_CASE(aw_generator_opperators_1) {
    // test basic multiplication

	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Phasor);
    g1->add_input_by_index(0, 4);

	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Phasor);
    g2->add_input_by_index(0, 8);


    aw::GeneratorShared g3a = g1 * g2;    

	aw::GeneratorShared gr = aw::Generator::make(aw::Generator::ID_Buffer);
	// create two channel buffer
	gr->set_slot_by_index(0, 1);
	// for one second
	gr->set_slot_by_index(1, 1);
        
    gr->set_input_by_index(0, g3a);
    gr->render(1);

    // the increment here 5512.5, so there is some error
	BOOST_CHECK_CLOSE(gr->outputs[0][5512], .5, .001);
	BOOST_CHECK_CLOSE(gr->outputs[0][11025], 0, .001);
	BOOST_CHECK_CLOSE(gr->outputs[0][16537+1], .5, .1);
    
	BOOST_CHECK_CLOSE(gr->outputs[0][22050], 0, .001);
	BOOST_CHECK_CLOSE(gr->outputs[0][27562+2], .5, .1);
    
	BOOST_CHECK_CLOSE(gr->outputs[0][33075], 0, .001);
	BOOST_CHECK_CLOSE(gr->outputs[0][38587+3], .5, .1);
        
}


BOOST_AUTO_TEST_CASE(aw_generator_multiply_2) {
    // multy channel multiplication
    
    aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Multiply);
    
    // three channel
    g1->set_slot_by_index(0, 3);
    
    g1->add_input_by_index(0, 4, 0);
    g1->add_input_by_index(0, 2, 0); // 8

    g1->add_input_by_index(1, -3, 0);
    g1->add_input_by_index(1, 4, 0); // -12

    g1->add_input_by_index(2, 10, 0);
    g1->add_input_by_index(2, 11, 0); // 110
    
    
    g1->render(1);
	BOOST_CHECK_CLOSE(g1->outputs[0][0], 8, .001);
    // note that outpout number start at 1
	BOOST_CHECK_CLOSE(g1->get_output_average(1), 8, .0001);
      
	BOOST_CHECK_CLOSE(g1->outputs[1][0], -12, .001);    
	BOOST_CHECK_CLOSE(g1->get_output_average(2), -12, .0001);
    
	BOOST_CHECK_CLOSE(g1->outputs[2][0], 110, .001);
	BOOST_CHECK_CLOSE(g1->get_output_average(3), 110, .0001);
        
    
    // sum each of three outputs of g1
    aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Add);
    // set g2 to receive g1 from each of its outputs
    g2->add_input_by_index(0, g1, 0);
    g2->add_input_by_index(0, g1, 1);
    g2->add_input_by_index(0, g1, 2);
       
    g2->render(2);

	BOOST_CHECK_CLOSE(g2->outputs[0][0], 106, .001);
	BOOST_CHECK_CLOSE(g2->get_output_average(0), 106, .0001);
      
    //g2->illustrate_network();
	//g1->illustrate_outputs();

}




BOOST_AUTO_TEST_CASE(aw_generator_opperators_2) {
    // test basic multiplication

	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Constant);
    g1->set_input_by_index(0, 4);
    aw::GeneratorShared g2 = g1 + 2;    
    g2->render(1);
	BOOST_CHECK_CLOSE(g2->outputs[0][0], 6, .001);


	aw::GeneratorShared g3 = aw::Generator::make(aw::Generator::ID_Constant);
    g3->set_input_by_index(0, 100);
    aw::GeneratorShared g4 = 2 + g3;    
    g4->render(1);
	BOOST_CHECK_CLOSE(g4->outputs[0][0], 102, .001);
        
}


BOOST_AUTO_TEST_CASE(connect_f_1) {

    aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Constant);
    g1->set_input_by_index(0, 3.5);
    aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Add);
    
    // this is g1 to g2
    g2 = aw::connect_serial(g1, g2);
    g2->render(1);
    
    BOOST_CHECK_CLOSE(g2->outputs[0][0], 3.5, .0001);
    
    aw::GeneratorShared g4 = aw::Generator::make(aw::Generator::ID_Constant);
    g4->set_input_by_index(0, 6.5);
    
    // g4 to g2
    g2 = aw::connect_serial(g4, g2);    
    g2->render(2);
    BOOST_CHECK_CLOSE(g2->outputs[0][0], 10.0, .0001);
}

BOOST_AUTO_TEST_CASE(connect_f_2) {
    // test multiple connections in parallel
    aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Add);
    g1->set_slot_by_index(0, 3); // three channels
    g1->set_input_by_index(0, 10);
    g1->set_input_by_index(1, 200);
    g1->set_input_by_index(2, 4000);
          
    aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Add);
    g2->set_slot_by_index(0, 3); // three channels          
    g2->set_input_by_index(0, 34);
    g2->set_input_by_index(1, 105);
    g2->set_input_by_index(2, 2110);
          

    // g1 into g2
    g2 = aw::connect_serial(g1, g2);

    g2->render(1);
    //g2->illustrate_network();
    // we are connect_serial each of three values independently
	BOOST_CHECK_CLOSE(g2->outputs[0][0], 44, .001);
	BOOST_CHECK_CLOSE(g2->outputs[1][0], 305, .001);
	BOOST_CHECK_CLOSE(g2->outputs[2][0], 6110, .001);
}


BOOST_AUTO_TEST_CASE(connect_f_3) {
    // test multiple connections in parallel
    aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Add);
    g1->set_slot_by_index(0, 3); // three channels
    g1->set_input_by_index(0, 10);
    g1->set_input_by_index(1, 200);
    g1->set_input_by_index(2, 4000);
          
    aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Add);
    g2->set_slot_by_index(0, 3); // three channels          
    g2->set_input_by_index(0, 34);
    g2->set_input_by_index(1, 105);
    g2->set_input_by_index(2, 2110);
          

    // connect from o/i 2, 1 length
    g2 = aw::connect_serial(g1, g2, 2, 1);

    g2->render(1);
    //g2->illustrate_network();
    // only o2 gets the added values
	BOOST_CHECK_CLOSE(g2->outputs[0][0], 34, .001);
	BOOST_CHECK_CLOSE(g2->outputs[1][0], 105, .001);
	BOOST_CHECK_CLOSE(g2->outputs[2][0], 6110, .001);
    
    
}



BOOST_AUTO_TEST_CASE(aw_generator_opperators_3) {
    // test basic multiplication

    aw::GeneratorShared g1a = aw::Generator::make(aw::Generator::ID_Constant);
    g1a->set_input_by_index(0, 4);

    aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Add);
    g2->set_input_by_index(0, 7);

    g1a >> g2;

    g2->render(1);
	BOOST_CHECK_CLOSE(g2->outputs[0][0], 11, .001);
       
    aw::GeneratorShared g1b = aw::Generator::make(aw::Generator::ID_Constant);
    g1b->set_input_by_index(0, 12.5);

    // can connect another input to g2 
    g1b >> g2;

    g2->render(2);
	BOOST_CHECK_CLOSE(g2->outputs[0][0], 23.5, .001);

}


BOOST_AUTO_TEST_CASE(aw_generator_opperators_4) {
    // test single and double connections

    // test multiple connections in parallel
    aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Add);
    g1->set_slot_by_index(0, 3); // three channels
    g1->set_input_by_index(0, 10);
    g1->set_input_by_index(1, 200);
    g1->set_input_by_index(2, 4000);
          
    aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Add);
    g2->set_slot_by_index(0, 3); // three channels          
    g2->set_input_by_index(0, 34);
    g2->set_input_by_index(1, 105);
    g2->set_input_by_index(2, 2110);
          

    // connect all of 1 to all fo 2
    g1 >> g2;

    g2->render(1);
    g2->illustrate_network();
    // only o2 gets the added values
	BOOST_CHECK_CLOSE(g2->outputs[0][0], 44, .001);
	BOOST_CHECK_CLOSE(g2->outputs[1][0], 305, .001);
	BOOST_CHECK_CLOSE(g2->outputs[2][0], 6110, .001);

}



BOOST_AUTO_TEST_CASE(aw_generator_opperators_5) {
    // test single and double connections

    // test multiple connections in parallel
    aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Add);
    g1->set_slot_by_index(0, 3); // three channels
    g1->set_input_by_index(0, 10);
    g1->set_input_by_index(1, 200);
    g1->set_input_by_index(2, 4000);
          
    aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Add);
    g2->set_slot_by_index(0, 3); // three channels          
    g2->set_input_by_index(0, 34);
    g2->set_input_by_index(1, 105);
    g2->set_input_by_index(2, 2110);
          

    // connect first o/i of each
    g1 > g2;

    g2->render(1);
    g2->illustrate_network();
    // only o2 gets the added values
	BOOST_CHECK_CLOSE(g2->outputs[0][0], 44, .001);
	BOOST_CHECK_CLOSE(g2->outputs[1][0], 105, .001);
	BOOST_CHECK_CLOSE(g2->outputs[2][0], 2110, .001);

}


BOOST_AUTO_TEST_CASE(aw_generator_opperators_6) {

    aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Add);    
    // connect two constants to g1b
    aw::connect_serial(20, g1);
    aw::connect_serial(11, g1);
    g1->render(1);
	BOOST_CHECK_CLOSE(g1->outputs[0][0], 31, .001);

    aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Add);    
    23.5 >> g2;
    100.5 >> g2;
    g2->render(1);
	BOOST_CHECK_CLOSE(g2->outputs[0][0], 124.0, .001);
    
}


BOOST_AUTO_TEST_CASE(aw_generator_opperators_7) {
    // test basic multiplication
	std::cerr << std::string(80, '-') << std::endl;

	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Constant);
    g1->set_input_by_index(0, 4);
	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Constant);
    g2->set_input_by_index(0, 7);

    aw::GeneratorShared g10 = g1 + g2; 
    //g10->illustrate_network();   
    g10->render(1);
	BOOST_CHECK_CLOSE(g10->outputs[0][0], 11, .00001);

    aw::GeneratorShared g11 = g1 + g2 * 10; 
    //g10->illustrate_network();   
    g11->render(1);
	BOOST_CHECK_CLOSE(g11->outputs[0][0], 74, .00001);

    aw::GeneratorShared g12 = 10 * g1 + .5; 
    //g10->illustrate_network();   
    g12->render(1);
	BOOST_CHECK_CLOSE(g12->outputs[0][0], 40.5, .00001);
	        
}



BOOST_AUTO_TEST_CASE(aw_generator_sine_1) {
    // test basic multiplication
    aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Sine);

    // one cycle for 4 samples
    11025 >> g1;
    //g1->set_input_by_index(0, 22050);        
    g1->render(1);
    //g1->illustrate_outputs();
	BOOST_CHECK_CLOSE(g1->outputs[0][0], 0, .00001);
	BOOST_CHECK_CLOSE(g1->outputs[0][1], 1, .00001);
	BOOST_CHECK_SMALL(g1->outputs[0][2], .001);
	BOOST_CHECK_CLOSE(g1->outputs[0][3], -1, .00001);
	BOOST_CHECK_SMALL(g1->outputs[0][4], .001);
    
    
	//BOOST_CHECK_CLOSE(g1->outputs[0][0], 120, .001);
}


BOOST_AUTO_TEST_CASE(aw_generator_map_1) {

    aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Map);
    
    g1->set_input_by_index(0, .5);
    g1->set_input_by_index(1, 0); // source
    g1->set_input_by_index(2, 1);
    g1->set_input_by_index(3, 10); // dst
    g1->set_input_by_index(4, 20);

    g1->render(1);
    BOOST_CHECK_CLOSE(g1->outputs[0][0], 15.0, .0001);
    
    g1->set_input_by_index(0, -1);
    g1->render(2);
    BOOST_CHECK_CLOSE(g1->outputs[0][0], 10.0, .0001);

    g1->set_input_by_index(0, 2.5);
    g1->render(3);
    BOOST_CHECK_CLOSE(g1->outputs[0][0], 20.0, .0001);

    g1->set_input_by_index(0, .5);
    g1->set_input_by_index(3, -100);
    g1->set_input_by_index(4, 100);
    g1->render(4);
    BOOST_CHECK_CLOSE(g1->outputs[0][0], 0.0, .0001);

    g1->set_input_by_index(0, 2);
    g1->set_input_by_index(1, 0);
    g1->set_input_by_index(2, 100);
    g1->set_input_by_index(3, 0);
    g1->set_input_by_index(4, 1000);
    g1->render(5);
    BOOST_CHECK_CLOSE(g1->outputs[0][0], 20.0, .0001);


}

BOOST_AUTO_TEST_CASE(aw_generator_map_2) {

    aw::GeneratorShared gsine1 = aw::Generator::make(aw::Generator::ID_Sine);
    6 >> gsine1;
    aw::GeneratorShared gsine2 = aw::Generator::make(aw::Generator::ID_Sine);
    4 >> gsine2;
    aw::GeneratorShared gsine3 = aw::Generator::make(aw::Generator::ID_Sine);
    20 >> gsine3;
    
	aw::GeneratorShared gbuf = aw::Generator::make(aw::Generator::ID_Buffer);
	gbuf->set_slot_by_index(0, 1);
	gbuf->set_slot_by_index(1, 60); // 60 sec
    
    aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Map);
    g1->set_input_by_index(0, gsine3);
    g1->set_input_by_index(1, -1); // source
    g1->set_input_by_index(2, 1);
    g1->set_input_by_index(3, gsine1); // dst
    g1->set_input_by_index(4, gsine2);

    gbuf->set_input_by_index(0, g1);
//    gbuf->render(1);
//    gbuf->illustrate_outputs();
//    gbuf->illustrate_network();
    
}



























