// g++ aw_generator_assert.cpp aw_generator.cpp aw_common.cpp -Wall -o aw_generator_assert

#include <cassert>


#include "aw_generator.h"
#include "aw_common.h"


int main() {

	// can creat the table subclass
//	aw::Constant g3;
//    assert(g3.get_parameter_count() == 1);
//
//    aw::ConstantShared g4 = aw::ConstantShared(new aw::Constant);
//    assert(g4->get_parameter_count() == 1);
//    
//    // check that dynamically created constants are managed properly
//    
//    aw::AddShared g1 = aw::AddShared(new aw::Add);
//    
//    g1->add_parameter_by_index(0, 1);
//    g1->add_parameter_by_index(0, 20.5);
//    g1->render(100);
//    g1->print_inputs(true);
//    g1->print_output();
//	
    
    
    // test resizing
    
    // will be initialized();
	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Add);

    // this will automatically create constant Generators
	g2->add_parameter_by_index(0, 6);
	g2->add_parameter_by_index(0, -5);
	    
	g2->render(50);
	g2->print_output();
	g2->print_inputs(true);
	
    g2->set_dimension(3);
	g2->print_output(); // output has been reset
    
    // we need to re-renter to get output
    // problem, however, is lower-level gens only have default dimensionality
    g2->render(10);
	g2->print_output();

    
}















