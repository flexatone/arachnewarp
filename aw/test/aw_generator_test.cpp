// g++-4.7 -std=c++11 -I ../src  aw_generator_test.cpp ../src/aw_generator.cpp ../src/aw_common.cpp ../src/aw_illustration.cpp -DSTAND_ALONE -l boost_unit_test_framework -l boost_filesystem -l boost_system -l sndfile -Wall -g -o aw_generator_test


// -std=c++0x

#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE main
#endif
#include <boost/test/unit_test.hpp>

#include <stdexcept>


#include "aw_generator.h"
#include "aw_common.h"

using namespace aw;


BOOST_AUTO_TEST_CASE(aw_generator_test_1) {

    EnvironmentPtr e = Environment::make();
	Gen g1 = Gen(e);
    g1.init();
	//g1.print_outputs();
	g1.render(1); // can call directly on object
	//g1.print_outputs();
	g1.render(3); // will render twice, moving to 3
	//g1.print_outputs();
    
        
    // this has no parameters so should raise exception on trying to set or add
    BOOST_REQUIRE_THROW(g1.set_input_by_index(0, 30), std::invalid_argument);

	GenPtr g2 = GenPtr(new Gen(e));
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

	
    BOOST_CHECK_EQUAL(g2->get_class_name(), "Gen");
}	


BOOST_AUTO_TEST_CASE(aw_parameter_type_test_1) {
	// testing creation
	PTypePtr pt1 = PTypePtr(new 
                                    PType);
	BOOST_CHECK_EQUAL(pt1->get_class_name(), "PType");

	PTypePtr pt2 = PType::make(PTypeID::Phase);
	BOOST_CHECK_EQUAL(pt2->get_class_name(), "PTypePhase");

	pt2->set_instance_name("degree of spin");
	BOOST_CHECK_EQUAL(pt2->get_instance_name(), "degree of spin");

	PTypePtr pt3 = PType::make(PTypeID::Value);
	BOOST_CHECK_EQUAL(pt3->get_class_name(), "PTypeValue");

	PTypePtr pt4 = PType::make(PTypeID::Frequency);
	BOOST_CHECK_EQUAL(pt4->get_class_name(), "PTypeFrequency");


	PTypePtr pt5 = PType::make(PTypeID::Duration);
	BOOST_CHECK_EQUAL(pt5->get_class_name(), "PTypeDuration");

	PTypePtr pt6 = PType::make(PTypeID::Channels);
	BOOST_CHECK_EQUAL(pt6->get_class_name(), "PTypeChannels");

	PTypePtr pt7 = PType::make(PTypeID::Trigger);
	BOOST_CHECK_EQUAL(pt7->get_class_name(), "PTypeTrigger");


}



BOOST_AUTO_TEST_CASE(aw_generator_constant_test_1) {
    // basic test of constant
    EnvironmentPtr e = Environment::make();
	Constant g3(e);
    g3.init();
    //g3.print_inputs();

    BOOST_CHECK_EQUAL(g3.get_input_count(), 1);
    BOOST_CHECK_EQUAL(g3.get_input_index_from_parameter_name("Constant numerical value"), 0);
    // check index out of range
    BOOST_CHECK_EQUAL(g3.get_input_index_from_class_id(PTypeID::Value), 0);
    
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
    
	GenPtr g1 = Gen::make(GenID::Constant);
	GenPtr g2 = Gen::make(GenID::Constant);
	GenPtr g3 = Gen::make(GenID::Add);

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
	GenPtr g1 = Gen::make(GenID::Constant);
	GenPtr g2 = Gen::make(GenID::Constant);
	GenPtr g3 = Gen::make(GenID::Add);

	GenPtr g4 = Gen::make(GenID::Constant);
	GenPtr g5 = Gen::make(GenID::Constant);
	GenPtr g6 = Gen::make(GenID::Add);

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
    
	GenPtr g3 = Gen::make(GenID::Add);
    
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
    
	GenPtr g1 = Gen::make(GenID::Constant);
	g1->set_input_by_index(0, 2); 							
	GenPtr g2 = Gen::make(GenID::Constant);	
	g2->set_input_by_index(0, 3); 							
							
	// make a 1 dimensional adder
	GenPtr g3 = Gen::make(GenID::Add);
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
	GenPtr g1 = Gen::make(GenID::Add);

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
    
	GenPtr g1 = Gen::make(GenID::Add);

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
    
	GenPtr g1 = Gen::make(GenID::Buffer);
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
	GenPtr g1 = Gen::make(GenID::Buffer);
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
	GenPtr g1 = Gen::make(GenID::Buffer);
	// test round trip file reading and writing; this is good for valgrind testing as we have to create dyanmic vectors for temporary storage
    std::string s("../test/12518-sk1Kick.aif");
    g1->set_outputs_from_fp(s);	
    g1->write_output_to_fp("testOutput.aif");	
}



BOOST_AUTO_TEST_CASE(aw_generator_buffer_7) {
	GenPtr g1 = Gen::make(GenID::Buffer);
	// testing setting the outputs from a file path
    VSampleT v {.2, .5, .4, .8};

    g1->set_outputs_from_vector(v, 1);
    BOOST_CHECK_CLOSE(g1->outputs[0][0], .2, .00001);
    BOOST_CHECK_CLOSE(g1->outputs[0][1], .5, .00001);
    BOOST_CHECK_CLOSE(g1->outputs[0][2], .4, .00001);
    BOOST_CHECK_CLOSE(g1->outputs[0][3], .8, .00001);
    BOOST_CHECK_EQUAL(g1->get_output_count(), 1);
    BOOST_CHECK_EQUAL(g1->get_outputs_size(), 4);
    

}

BOOST_AUTO_TEST_CASE(aw_generator_phasor_1) {    
	GenPtr g1 = Gen::make(GenID::Phasor);
    
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
	
	GenPtr g2 = Gen::make(GenID::Phasor);
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
    
	GenPtr g1 = Gen::make(GenID::Phasor);
    
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
	
	GenPtr g1 = Gen::make(GenID::Buffer);

	// changing the slot calls overridden _update_for_new_slot(), resizes frame size
	g1->set_slot_by_index(1, 20.0); // 20 second buffer
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 882000);

	g1->set_slot_by_index(1, 1.0); // 1 second buffer
    BOOST_CHECK_EQUAL(g1->get_frame_size(), 44100);
		
	// create 
	GenPtr g2 = Gen::make(GenID::Phasor);    
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
	
	//GenPtr g1 = Gen::make(Gen::ID_Recorder);

	GenPtr g1 = Gen::make(GenID::Buffer);
	// create two channel buffer
	g1->set_slot_by_index(0, 2);
	// for one second
	g1->set_slot_by_index(1, .01); // 441 samples
		
	// create 
	GenPtr g2 = Gen::make(GenID::Phasor);    
	g2->add_input_by_index(0, 2); // a constant frequency, period of 22050
	
	GenPtr g3 = Gen::make(GenID::Phasor);    
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
    GenPtr g1 = Gen::make(GenID::Multiply);
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

	GenPtr g1 = Gen::make(GenID::Phasor);
    g1->add_input_by_index(0, 4);

	GenPtr g2 = Gen::make(GenID::Phasor);
    g2->add_input_by_index(0, 8);


    GenPtr g3a = g1 * g2;    

	GenPtr gr = Gen::make(GenID::Buffer);
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
    
    GenPtr g1 = Gen::make(GenID::Multiply);
    
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
    GenPtr g2 = Gen::make(GenID::Add);
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

	GenPtr g1 = Gen::make(GenID::Constant);
    g1->set_input_by_index(0, 4);
    GenPtr g2 = g1 + 2;    
    g2->render(1);
	BOOST_CHECK_CLOSE(g2->outputs[0][0], 6, .001);


	GenPtr g3 = Gen::make(GenID::Constant);
    g3->set_input_by_index(0, 100);
    GenPtr g4 = 2 + g3;    
    g4->render(1);
	BOOST_CHECK_CLOSE(g4->outputs[0][0], 102, .001);
        
}


BOOST_AUTO_TEST_CASE(connect_f_1) {

    GenPtr g1 = Gen::make(GenID::Constant);
    g1->set_input_by_index(0, 3.5);
    GenPtr g2 = Gen::make(GenID::Add);
    
    // this is g1 to g2
    g2 = connect_serial_to_inputs(g1, g2);
    g2->render(1);
    
    BOOST_CHECK_CLOSE(g2->outputs[0][0], 3.5, .0001);
    
    GenPtr g4 = Gen::make(GenID::Constant);
    g4->set_input_by_index(0, 6.5);
    
    // g4 to g2
    g2 = connect_serial_to_inputs(g4, g2);    
    g2->render(2);
    BOOST_CHECK_CLOSE(g2->outputs[0][0], 10.0, .0001);
}

BOOST_AUTO_TEST_CASE(connect_f_2) {
    // test multiple connections in parallel
    GenPtr g1 = Gen::make(GenID::Add);
    g1->set_slot_by_index(0, 3); // three channels
    g1->set_input_by_index(0, 10);
    g1->set_input_by_index(1, 200);
    g1->set_input_by_index(2, 4000);
          
    GenPtr g2 = Gen::make(GenID::Add);
    g2->set_slot_by_index(0, 3); // three channels          
    g2->set_input_by_index(0, 34);
    g2->set_input_by_index(1, 105);
    g2->set_input_by_index(2, 2110);
          

    // g1 into g2
    g2 = connect_serial_to_inputs(g1, g2);

    g2->render(1);
    //g2->illustrate_network();
    // we are connect_serial_to_inputs each of three values independently
	BOOST_CHECK_CLOSE(g2->outputs[0][0], 44, .001);
	BOOST_CHECK_CLOSE(g2->outputs[1][0], 305, .001);
	BOOST_CHECK_CLOSE(g2->outputs[2][0], 6110, .001);
}


BOOST_AUTO_TEST_CASE(connect_f_3) {
    // test multiple connections in parallel
    GenPtr g1 = Gen::make(GenID::Add);
    g1->set_slot_by_index(0, 3); // three channels
    g1->set_input_by_index(0, 10);
    g1->set_input_by_index(1, 200);
    g1->set_input_by_index(2, 4000);
          
    GenPtr g2 = Gen::make(GenID::Add);
    g2->set_slot_by_index(0, 3); // three channels          
    g2->set_input_by_index(0, 34);
    g2->set_input_by_index(1, 105);
    g2->set_input_by_index(2, 2110);
          

    // connect from o/i 2, 1 length
    g2 = connect_serial_to_inputs(g1, g2, 2, 1);

    g2->render(1);
    //g2->illustrate_network();
    // only o2 gets the added values
	BOOST_CHECK_CLOSE(g2->outputs[0][0], 34, .001);
	BOOST_CHECK_CLOSE(g2->outputs[1][0], 105, .001);
	BOOST_CHECK_CLOSE(g2->outputs[2][0], 6110, .001);
    
    
}



BOOST_AUTO_TEST_CASE(aw_generator_opperators_3) {
    // test basic multiplication

    GenPtr g1a = Gen::make(GenID::Constant);
    g1a->set_input_by_index(0, 4);

    GenPtr g2 = Gen::make(GenID::Add);
    g2->set_input_by_index(0, 7);

    g1a >> g2;

    g2->render(1);
	BOOST_CHECK_CLOSE(g2->outputs[0][0], 11, .001);
       
    GenPtr g1b = Gen::make(GenID::Constant);
    g1b->set_input_by_index(0, 12.5);

    // can connect another input to g2 
    g1b >> g2;

    g2->render(2);
	BOOST_CHECK_CLOSE(g2->outputs[0][0], 23.5, .001);

}


BOOST_AUTO_TEST_CASE(aw_generator_opperators_4) {
    // test single and double connections

    // test multiple connections in parallel
    GenPtr g1 = Gen::make(GenID::Add);
    g1->set_slot_by_index(0, 3); // three channels
    g1->set_input_by_index(0, 10);
    g1->set_input_by_index(1, 200);
    g1->set_input_by_index(2, 4000);
          
    GenPtr g2 = Gen::make(GenID::Add);
    g2->set_slot_by_index(0, 3); // three channels          
    g2->set_input_by_index(0, 34);
    g2->set_input_by_index(1, 105);
    g2->set_input_by_index(2, 2110);

    // connect all of 1 to all fo 2
    g1 >> g2;

    g2->render(1);
    //g2->illustrate_network();
    // only o2 gets the added values
	BOOST_CHECK_CLOSE(g2->outputs[0][0], 44, .001);
	BOOST_CHECK_CLOSE(g2->outputs[1][0], 305, .001);
	BOOST_CHECK_CLOSE(g2->outputs[2][0], 6110, .001);

}



BOOST_AUTO_TEST_CASE(aw_generator_opperators_5) {

}


BOOST_AUTO_TEST_CASE(aw_generator_opperators_6) {

    GenPtr g1 = Gen::make(GenID::Add);    
    // connect two constants to g1b
    connect_serial_to_inputs(20, g1);
    connect_serial_to_inputs(11, g1);
    g1->render(1);
	BOOST_CHECK_CLOSE(g1->outputs[0][0], 31, .001);

    GenPtr g2 = Gen::make(GenID::Add);    
    23.5 >> g2;
    100.5 >> g2;
    g2->render(1);
	BOOST_CHECK_CLOSE(g2->outputs[0][0], 124.0, .001);
    
}


BOOST_AUTO_TEST_CASE(aw_generator_opperators_7) {
    // test basic multiplication
	GenPtr g1 = Gen::make(GenID::Constant);
    g1->set_input_by_index(0, 4);
	GenPtr g2 = Gen::make(GenID::Constant);
    g2->set_input_by_index(0, 7);

    GenPtr g10 = g1 + g2; 
    //g10->illustrate_network();   
    g10->render(1);
	BOOST_CHECK_CLOSE(g10->outputs[0][0], 11, .00001);

    GenPtr g11 = g1 + g2 * 10; 
    //g10->illustrate_network();   
    g11->render(1);
	BOOST_CHECK_CLOSE(g11->outputs[0][0], 74, .00001);

    GenPtr g12 = 10 * g1 + .5; 
    //g10->illustrate_network();   
    g12->render(1);
	BOOST_CHECK_CLOSE(g12->outputs[0][0], 40.5, .00001);
	        
}


BOOST_AUTO_TEST_CASE(aw_generator_opperators_8) {
    // test basic multiplication

	GenPtr g1 = Gen::make(GenID::Sine);
    // need to remove defaults
    g1->clear_inputs();
    
    Inj<SampleT>({200, .5}) >> g1;
    g1->render(1);
    //g1->print_inputs();
    
	GenPtr g2 = Gen::make(GenID::Sine);
	GenPtr g3 = Gen::make(GenID::Sine);

    Inj<GenPtr>({g2, g3}) >> g1;
    
    //g1->illustrate_network();

    Gen::VGenPtrOutPair ins;
    
    ins = g1->get_input_gens_by_index(0);
    BOOST_CHECK_EQUAL(ins.size(), 2);
    // second pair is g2
    BOOST_CHECK_EQUAL(ins[1].first, g2);
    
    //BOOST_CHECK_EQUAL(ins[1].size(), 2);

    ins = g1->get_input_gens_by_index(1);
    BOOST_CHECK_EQUAL(ins.size(), 2);
    BOOST_CHECK_EQUAL(ins[1].first, g3);

    
}


BOOST_AUTO_TEST_CASE(aw_generator_opperators_9) {
    // test basic multiplication

	GenPtr g1 = Gen::make(GenID::Buffer);
    // setting slots: channels and duratin in sec
    Inj<SampleT>({3, 1.0}) || g1;

    BOOST_CHECK_EQUAL(g1->get_output_count(), 3);
    BOOST_CHECK_EQUAL(g1->get_outputs_size(),
            3*g1->get_environment()->get_sampling_rate());
    
}




BOOST_AUTO_TEST_CASE(aw_generator_sine_1) {
    // test basic multiplication
    GenPtr g1 = Gen::make(GenID::Sine);
    g1->set_input_by_index(0, 11025);
    g1->render(1);
    // must use check_small here or otherwise get SIGABR
        
	BOOST_CHECK_SMALL(g1->outputs[0][0], .001);
	BOOST_CHECK_CLOSE(g1->outputs[0][1], 1.0, .00001);
	BOOST_CHECK_SMALL(g1->outputs[0][2], .001);
	BOOST_CHECK_CLOSE(g1->outputs[0][3], -1.0, .00001);
	BOOST_CHECK_SMALL(g1->outputs[0][4], .001);
}


BOOST_AUTO_TEST_CASE(aw_generator_sine_2) {
    // test basic multiplication
    GenPtr g1 = Gen::make(GenID::Sine);
    // one cycle for 4 samples
    //882. >> g1; // gets 25 points per half cycle
    g1->set_input_by_index(0, 882);
    g1->set_input_by_index(1, PI * .5);
    
    g1->render(1);
    // must use check_small here or otherwise get SIGABR
    
    //g1->illustrate_outputs();
    
	BOOST_CHECK_CLOSE(g1->outputs[0][0], 1.0, .00001);
	BOOST_CHECK_CLOSE(g1->outputs[0][25], -1.0, .00001);
	BOOST_CHECK_CLOSE(g1->outputs[0][50], 1.0, .00001);
}



BOOST_AUTO_TEST_CASE(aw_generator_map_1) {

    GenPtr g1 = Gen::make(GenID::Map);
    
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

    GenPtr g1 = Gen::make(GenID::Map);
    
    g1->set_input_by_index(0, .5);
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
}


BOOST_AUTO_TEST_CASE(aw_generator_map_3) {
    GenPtr g1 = Gen::make(GenID::Map);
    
    g1->set_input_by_index(0, 0);
    g1->set_input_by_index(1, -1);
    g1->set_input_by_index(2, 1);
    g1->set_input_by_index(3, 10); // dst
    g1->set_input_by_index(4, 20);

    g1->render(1);
    BOOST_CHECK_CLOSE(g1->outputs[0][0], 15.0, .0001);
    
    g1->set_input_by_index(0, -.5);
    g1->render(2);
    BOOST_CHECK_CLOSE(g1->outputs[0][0], 12.5, .0001);

    g1->set_input_by_index(0, .5);
    g1->render(3);
    BOOST_CHECK_CLOSE(g1->outputs[0][0], 17.5, .0001);
}


BOOST_AUTO_TEST_CASE(aw_generator_attack_decay_1) {
    // tesing normal triggering

    GenPtr g1 = Gen::make(GenID::AttackDecay);

    g1->set_input_by_index(1, .125); // atack/decay in sec
    g1->set_input_by_index(2, .125);

    GenPtr g2 = Gen::make(GenID::Phasor);
    2 >> g2; // 4 per second
    
    g1->set_input_by_index(0, g2, 1); // need second output, the phasor trigger
    
	GenPtr gbuf = Gen::make(GenID::Buffer);
	gbuf->set_slot_by_index(0, 4); // want to set this with: {2, 1} | gbuf;
	gbuf->set_slot_by_index(1, 1);
    
    gbuf->set_input_by_index(0, g2, 1); // write trigger, out 1
    gbuf->set_input_by_index(1, g1, 0); // envelope
    gbuf->set_input_by_index(2, g1, 1); // envelope eoa
    gbuf->set_input_by_index(3, g1, 2); // envelope eod
    
    gbuf->render(1);
    //gbuf->illustrate_outputs();
    unsigned int offset = 44100 / 8;
    BOOST_CHECK(gbuf->outputs[1][0] < .1);
    BOOST_CHECK(gbuf->outputs[1][offset * 1] > .9);
    BOOST_CHECK(gbuf->outputs[1][offset * 2] < .1);
    BOOST_CHECK(gbuf->outputs[1][offset * 3] < .1);
    BOOST_CHECK(gbuf->outputs[1][offset * 4] < .1);
    BOOST_CHECK(gbuf->outputs[1][offset * 5] > .9);
    BOOST_CHECK(gbuf->outputs[1][offset * 6] < .1);
    BOOST_CHECK(gbuf->outputs[1][offset * 7] < .1);

    // end of attack; rounding cause off by one
    BOOST_CHECK(gbuf->outputs[2][offset * 1 + 1] > .9);
    // end of decay; again, rounding error to sample results in shift
    BOOST_CHECK(gbuf->outputs[3][offset * 2 + 2] > .9);
}

BOOST_AUTO_TEST_CASE(aw_generator_attack_decay_2) {
    // tesing retriggering during decay

    GenPtr g1 = Gen::make(GenID::AttackDecay);

    g1->set_input_by_index(1, .125); // atack/decay in sec
    g1->set_input_by_index(2, 1);

    GenPtr g2 = Gen::make(GenID::Phasor);
    2 >> g2; // 4 per second
    
    g1->set_input_by_index(0, g2, 1); // need second output, the phasor trigger
    
	GenPtr gbuf = Gen::make(GenID::Buffer);
	gbuf->set_slot_by_index(0, 4); // want to set this with: {2, 1} | gbuf;
	gbuf->set_slot_by_index(1, 1);
    
    gbuf->set_input_by_index(0, g2, 1); // write trigger, out 1
    gbuf->set_input_by_index(1, g1, 0); // envelope
    gbuf->set_input_by_index(2, g1, 1); // envelope eoa
    gbuf->set_input_by_index(3, g1, 2); // envelope eod
    
    gbuf->render(1);
    unsigned int offset = 44100 / 8;
    BOOST_CHECK(gbuf->outputs[1][0] < .1);
    BOOST_CHECK(gbuf->outputs[1][offset * 1] > .9);
    BOOST_CHECK(gbuf->outputs[1][offset * 2] > .1);
    BOOST_CHECK(gbuf->outputs[1][offset * 3] > .1);
    // not at zero at start of next
    BOOST_CHECK(gbuf->outputs[1][offset * 4] > .1);
    BOOST_CHECK(gbuf->outputs[1][offset * 5] > .9);
//    BOOST_CHECK(gbuf->outputs[1][offset * 6] < .1);
//    BOOST_CHECK(gbuf->outputs[1][offset * 7] < .1);
}

BOOST_AUTO_TEST_CASE(aw_generator_attack_decay_3) {
    // tesing auto trigger
    
    GenPtr g1 = Gen::make(GenID::AttackDecay);

    g1->set_input_by_index(1, .125); // atack/decay in sec
    g1->set_input_by_index(2, .125);
    g1->set_input_by_index(4, 1); // self cycle mode
        
	GenPtr gbuf = Gen::make(GenID::Buffer);
	gbuf->set_slot_by_index(0, 3);
	gbuf->set_slot_by_index(1, 1);
    
    gbuf->set_input_by_index(0, g1, 0); // envelope
    gbuf->set_input_by_index(1, g1, 1); // envelope eoa
    gbuf->set_input_by_index(2, g1, 2); // envelope eod
    
    gbuf->render(1);
    //gbuf->illustrate_outputs();
    // 44100 samples, 4 envelopes peaking at .125
    unsigned int offset = 44100 / 8;
    BOOST_CHECK(gbuf->outputs[0][0] < .1);
    BOOST_CHECK(gbuf->outputs[0][offset * 1] > .9);
    BOOST_CHECK(gbuf->outputs[0][offset * 2] < .1);
    BOOST_CHECK(gbuf->outputs[0][offset * 3] > .9);
    BOOST_CHECK(gbuf->outputs[0][offset * 4] < .1);
    BOOST_CHECK(gbuf->outputs[0][offset * 5] > .9);
    BOOST_CHECK(gbuf->outputs[0][offset * 6] < .1);
    BOOST_CHECK(gbuf->outputs[0][offset * 7] > .9);
    BOOST_CHECK(gbuf->outputs[0][offset * 8 - 1] < .1);
    
    // end of attack; rounding cause off by one
    BOOST_CHECK(gbuf->outputs[1][offset * 1 + 1] > .9);
    // end of decay; again, rounding error to sample results in shift
    BOOST_CHECK(gbuf->outputs[2][offset * 2 + 2] > .9);
    
}





BOOST_AUTO_TEST_CASE(aw_generator_buffer_operators_1) {
	GenPtr g1 = Gen::make(GenID::Buffer);

    g1->set_slot_by_index(0, 1); // must set one channel

    // just create a regular instance
    Inj<SampleT> bi(
            {0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 0});
    
    g1->set_outputs(bi);
    BOOST_CHECK(g1->get_frame_size() == 13);
    BOOST_CHECK(g1->get_output_count() == 1);

    BOOST_CHECK(g1->outputs[0][6] == 6);
    
    //g1->illustrate_outputs();
    
	GenPtr g2 = Gen::make(GenID::Buffer);
    //{3, 1, 3, .5, 0} && g1;
    //g2->set_slot_by_index(0, 1); // must set one channel
    bi = Inj<SampleT>({
            {10, 1, 2},
            {5, 4, 5},
            {4, 5, 4},
            {1, 2, 20},
            {4, 5, 4},
            {1, 2, 20}});
    g2->set_outputs(bi);
    BOOST_CHECK(g2->get_frame_size() == 6);
    BOOST_CHECK(g2->get_output_count() == 3);

    BOOST_CHECK(g2->outputs[0][0] == 10);
    BOOST_CHECK(g2->outputs[1][0] == 1);
    BOOST_CHECK(g2->outputs[2][0] == 2);

    BOOST_CHECK(g2->outputs[0][1] == 5);
    BOOST_CHECK(g2->outputs[1][1] == 4);
    BOOST_CHECK(g2->outputs[2][1] == 5);
    
    //g2->illustrate_outputs();
    
}




BOOST_AUTO_TEST_CASE(aw_generator_buffer_operators_2) {
	GenPtr g1 = Gen::make(GenID::Buffer);
    
    Inj<SampleT>({3,4,5,200,2000}) && g1;
    //g1->illustrate_outputs();
    BOOST_CHECK(g1->get_frame_size() == 5);
    BOOST_CHECK(g1->get_output_count() == 1);
    BOOST_CHECK(g1->outputs[0][0] == 3);
    BOOST_CHECK(g1->outputs[0][1] == 4);
    BOOST_CHECK(g1->outputs[0][2] == 5);

    // will reset the existing buffer
    Inj<SampleT>({{0, .5}, {20, .1}, {40, .5},
            {60, .9}, {110, .3}}) && g1;
    //g1->illustrate_outputs();

    BOOST_CHECK(g1->get_frame_size() == 5);
    BOOST_CHECK(g1->get_output_count() == 2);
    BOOST_CHECK(g1->outputs[0][0] == 0);
    BOOST_CHECK(g1->outputs[1][0] == .5);
    BOOST_CHECK(g1->outputs[0][1] == 20);
    BOOST_CHECK(g1->outputs[1][1] == .1);

    
    // can do this:
    // ILSampleT({3,4,5}) && g1;
    // but will have to know type 
    
    // but thisdoes not: TODO: have make method on BuffInjector and have op overload work on that; or find out how to fix
    //{3, 4, 5} && g1;

}


BOOST_AUTO_TEST_CASE(aw_generator_buffer_operators_3) {
	GenPtr g1 = Gen::make(GenID::Buffer);
    
    // using templated version
    Inj<SampleT>({3,4,5,200,2000}) && g1;
    BOOST_CHECK(g1->get_frame_size() == 5);
    BOOST_CHECK(g1->get_output_count() == 1);
    BOOST_CHECK(g1->outputs[0][0] == 3);
    BOOST_CHECK(g1->outputs[0][1] == 4);
    BOOST_CHECK(g1->outputs[0][2] == 5);

}



BOOST_AUTO_TEST_CASE(aw_breakpoints_a) {

    // break points will evaluate coherancy when setting outputs
	GenPtr g1 = Gen::make(GenID::BreakPoints);
    Inj<SampleT>(
            {
                {0, 1},
                {2, .4},
                {20, .1},
                {45, 0},
            }
        ) && g1;
    
    BOOST_CHECK(g1->get_frame_size() == 4);
    BOOST_CHECK(g1->get_output_count() == 2);


	GenPtr g2 = Gen::make(GenID::BreakPoints);
    Inj<SampleT> inj1(
            {
                {0, 1},
                {20, .4},
                {2, .1},
            }
        ) ;
    // fails due to out of order x
    BOOST_REQUIRE_THROW(inj1 && g2, std::invalid_argument);
    // still set, however
    BOOST_CHECK(g2->get_frame_size() == 3);


	GenPtr g3 = Gen::make(GenID::BreakPoints);
    Inj<SampleT> inj2(
            {3, 4, 5}
        ) ;
    // fails due to out of order x
    BOOST_REQUIRE_THROW(inj2 && g3, std::invalid_argument);
    // still set, however
    BOOST_CHECK(g2->get_frame_size() == 3);


}



BOOST_AUTO_TEST_CASE(aw_gen_id_enum_class_a) {

    BOOST_CHECK(GenID::Sine == GenID::Sine);
    

}





















