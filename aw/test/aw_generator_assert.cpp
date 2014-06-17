// g++ aw_generator_assert.cpp -I ../src ../src/aw_generator.cpp ../src/aw_common.cpp ../src/aw_illustration.cpp ../src/aw_timer.cpp -l boost_filesystem -l boost_system -l sndfile -Wall -O3 -o aw_generator_assert


// g++-4.7 -std=c++11 -I ../src  aw_generator_assert.cpp ../src/aw_generator.cpp ../src/aw_common.cpp ../src/aw_illustration.cpp -DSTAND_ALONE -l boost_unit_test_framework -l boost_filesystem -l boost_system -l sndfile -Wall -g -o aw_generator_assert


#include <cassert>


#include "aw_generator.h"
#include "aw_common.h"
#include "aw_timer.h"



bool test_1() {
//	aw::Constant g3;
//    assert(g3.get_input_count() == 1);

	aw::GenPtr g4 = aw::Gen::make(aw::GenID::Constant);

    //aw::ConstantPtr g4 = aw::ConstantPtr(new aw::Constant);
    assert(g4->get_input_count() == 1);
    
    // check that dynamically created constants are managed properly
    
    //aw::AddPtr g1 = aw::AddPtr(new aw::Add);
	aw::GenPtr g1 = aw::Gen::make(aw::GenID::Add);
    
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
	aw::GenPtr g2 = aw::Gen::make(aw::GenID::Add);

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
    
   	aw::GenPtr g1 = aw::Gen::make(aw::GenID::SecondsBuffer);
    // set channels to 3
    
	aw::VSampleT v;
	for (std::size_t i=0; i < 120; ++i) {
		v.push_back(i % 5);
	}
	
	g1->set_outputs_from_vector(v, 3); // declare 2d
	g1->print_outputs();
    //g1->illustrate_outputs();
    return true;

}

bool test_4() {
	aw::GenPtr g1 = aw::Gen::make(aw::GenID::SecondsBuffer);
    // set channels to 1
    std::string s("12518-sk1Kick.aif");
    g1->set_outputs_from_fp(s);	
    g1->write_output_to_fp("testOutput.aif");	
    
    //g1->illustrate_outputs();
	return true;
}




bool test_5() {
    // testing reassignemnt through connect for memory management
    
    aw::GenPtr g1 = aw::Gen::make(aw::GenID::Add);
    g1->set_slot_by_index(0, 3); // three channels
    g1->set_input_by_index(0, 10);
    g1->set_input_by_index(1, 200);
    g1->set_input_by_index(2, 4000);
          
    aw::GenPtr g2 = aw::Gen::make(aw::GenID::Add);
    g2->set_slot_by_index(0, 3); // three channels          
    g2->set_input_by_index(0, 34);
    g2->set_input_by_index(1, 105);
    g2->set_input_by_index(2, 2110);
          
    // g1 into g2
    g2 = aw::connect_serial_to_inputs(g1, g2);

    g2->render(1);
    
    return true;
}


bool test_6() {
    
    using namespace aw;
    
    GenPtr sq1 = Gen::make(GenID::Sequencer);
    std::cout << sq1 << std::endl;

    GenPtr b1 = Gen::make(GenID::SamplesBuffer);
    Inj<SampleT>({60, 58, 60, 60, 69, 60, 60}) && b1;
    std::cout << b1 << std::endl;

    assert(b1->get_class_id() == GenID::SamplesBuffer);

    GenPtr b2 = Gen::make(GenID::SamplesBuffer);
    Inj<SampleT>(
            {{.5, 1, 60}, {0, .8, 69}, {-1, .5, 59}, {-.25, .9, 69}}
            ) && b2;
    std::cout << b2 << std::endl;

    // returns a GenID: b1->get_class_id();
    
    b1 || sq1;
    b2 || sq1;
    
    
    
    return true;
}



int main() {
	// TODO: read command line args to support selecting test by name

//    assert(test_1() &&
//            test_2() &&
//            test_3() &&
//            test_4() &&
//            test_5());
	
	test_6();
    
}















