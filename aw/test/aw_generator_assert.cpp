// g++ aw_generator_assert.cpp -I ../src ../src/aw_generator.cpp ../src/aw_common.cpp ../src/aw_illustration.cpp ../src/aw_timer.cpp -l boost_filesystem -l boost_system -l sndfile -Wall -O3 -o aw_generator_assert


#include <cassert>


#include "aw_generator.h"
#include "aw_common.h"
#include "aw_timer.h"



bool test_1() {
//	aw::Constant g3;
//    assert(g3.get_input_count() == 1);

	aw::GeneratorShared g4 = aw::Generator::make(aw::Generator::ID_Constant);

    //aw::ConstantShared g4 = aw::ConstantShared(new aw::Constant);
    assert(g4->get_input_count() == 1);
    
    // check that dynamically created constants are managed properly
    
    //aw::AddShared g1 = aw::AddShared(new aw::Add);
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Add);
    
    g1->add_input_by_index(0, 1);
    g1->add_input_by_index(0, 20.5);
    g1->render(100);
    g1->print_inputs(true);
    g1->print_outputs();

	return true;
}


bool test_2() {
	// can create the table subclass
    
    // test resizing
    
    // will be initialized();
	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Add);

    // this will automatically create constant Generators
	g2->add_input_by_index(0, 6);
	g2->add_input_by_index(0, -5);
	    
	g2->render(50);
	g2->print_outputs();
	g2->print_inputs(true);
	
    return true;

}

bool test_3() {
	// can create the table subclass
    
   	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Buffer);
    // set channels to 3
    
	aw::Generator::VSampleType v;
	for (std::size_t i=0; i < 120; ++i) {
		v.push_back(i % 5);
	}
	
	g1->set_outputs_from_vector(v, 3); // declare 2d
	g1->print_outputs();
    //g1->illustrate_outputs();
    return true;

}

bool test_4() {
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Buffer);
    // set channels to 1
    std::string s("12518-sk1Kick.aif");
    g1->set_outputs_from_fp(s);	
    g1->write_output_to_fp("testOutput.aif");	
    
    //g1->illustrate_outputs();
	return true;
}


bool test_5() {
	std::cerr << std::string(80, '-') << std::endl;
	//aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Recorder);

	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Buffer);
	// create two channel buffer
	g1->set_slot_by_index(0, 2);
	// for five second
	g1->set_slot_by_index(1, 5.0);
		
	// create 
	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Phasor);    
	g2->add_input_by_index(0, 4); // a constant frequency
	
	aw::GeneratorShared g3 = aw::Generator::make(aw::Generator::ID_Phasor);    
	g3->add_input_by_index(0, 12); // a constant frequency

	aw::GeneratorShared g4 = aw::Generator::make(aw::Generator::ID_Phasor);    
	g3->add_input_by_index(0, -2); // a constant frequency

	// add phasor to buffer input; might scale buffer if necessary; could mix multiple too
	g1->add_input_by_index(0, g2);
	g1->add_input_by_index(1, g3);
	g1->add_input_by_index(1, g4);
	
    
    aw::Timer t("rendering buffer");
    t.start();
    
	// for one render cycle of the buffer, we render inputs until we fill the frame
	// need to time this generation 
	g1->render(1); // render count here meaningless
	//g1->illustrate_outputs();
    t.stop();
    std::cout << "total time: " << t << std::endl;
	
	// on ubuntu w/o optimization, array impl: 60-70 msec, 
	// on ubuntu w optimization, array impl: 0-10 msec, 
	// on ubuntu w optimization, vector impl: 0-10 msec, slightly favoring 10, 
	// on ubuntu w/o optimization, vector impl: 60 msec, 
        
    // on macos w optimization, 22.25 msec    
    return true;
}



bool test_6() {
    // testing reassignemnt through connect for memory management
    
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
    g2 = aw::connect(g1, g2);

    g2->render(1);
    
    return true;
}




int main() {
	// TODO: read command line args to support selecting test by name

    assert(test_1() && test_2() && test_3() 
            && test_4() && test_5() && test_6());
	
	// test_6();
    
}















