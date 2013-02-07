// g++ aw_generator_perf.cpp aw_generator.cpp aw_common.cpp aw_timer.cpp aw_illustration.cpp -lboost_unit_test_framework -l boost_filesystem -l boost_system -l sndfile -Wall -o aw_generator_perf

#include <cassert>


#include "aw_generator.h"
#include "aw_common.h"
#include "aw_timer.h"


int main() {

    //44100 / 64 == 689 // default
    //44100 / 128 == 345
    //44100 / 256 == 172
    
    
    // will be initialized();
	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Phasor);

    // this will automatically create constant Generators
    aw::Timer t1("adding two constant inputs");    
	g2->add_input_by_index(0, 0);
    std::cout << t1 << std::endl;
    
    //g2->print_inputs(true);
    
    aw::Timer t2("44100 phase calculations");
	g2->render(689);
    std::cout << "total time for 1 second of audio: " << t2 << std::endl;
    
	//g2->print_outputs();

    
}















