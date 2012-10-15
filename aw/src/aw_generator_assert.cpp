// g++ aw_generator_assert.cpp aw_generator.cpp aw_common.cpp -l boost_filesystem -l boost_system -Wall -o aw_generator_assert

#include <cassert>


#include "aw_generator.h"
#include "aw_common.h"



bool test_1() {
//	aw::Constant g3;
//    assert(g3.get_parameter_count() == 1);

	aw::GeneratorShared g4 = aw::Generator::make(aw::Generator::ID_Constant);

    //aw::ConstantShared g4 = aw::ConstantShared(new aw::Constant);
    assert(g4->get_parameter_count() == 1);
    
    // check that dynamically created constants are managed properly
    
    //aw::AddShared g1 = aw::AddShared(new aw::Add);
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Add);
    
    g1->add_parameter_by_index(0, 1);
    g1->add_parameter_by_index(0, 20.5);
    g1->render(100);
    g1->print_inputs(true);
    g1->print_output();

	return true;
}


bool test_2() {
	// can creat the table subclass
    
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
    
    return true;

}


int main() {


    assert(test_1() & test_2());
    
}















