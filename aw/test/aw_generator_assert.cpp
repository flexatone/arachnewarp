// g++ aw_generator_assert.cpp -I ../src ../src/aw_generator.cpp ../src/aw_common.cpp ../src/aw_plotter.cpp -l boost_filesystem -l boost_system -l sndfile -Wall -o aw_generator_assert


#include <cassert>


#include "aw_generator.h"
#include "aw_common.h"



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
    g1->print_output();

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
	g2->print_output();
	g2->print_inputs(true);
	
//    g2->_set_output_count(3);
//	assert(g2->matrix[0] == 0.0);
//	g2->print_output(); // matrix has been reset to zero
//    
//    // we need to re-renter to get matrix
//    // problem, however, is lower-level gens only have default dimensionality
//    g2->render(10);
//	g2->print_output();
//	
    //g2->plot_output();
    return true;

}

bool test_3() {
	// can create the table subclass
    
   	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_BufferFile);
    // set channels to 3
    
	aw::Generator::VSampleType v;
	for (std::size_t i=0; i < 120; ++i) {
		v.push_back(i % 5);
	}
	
	g1->set_matrx_from_vector(v, 3); // declare 2d
	g1->print_output();
    //g1->plot_output();
    return true;

}

bool test_4() {
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_BufferFile);
    // set channels to 1
    std::string s("12518-sk1Kick.aif");
    g1->set_matrix_from_fp(s);	
    g1->write_output_to_fp("testOutput.aif");	
    
    //g1->plot_output();
	return true;
}


int main() {
	// TODO: read command line args to support selecting test by name

    //assert(test_1() && test_2() && test_3());
	
	test_4();
    
}















