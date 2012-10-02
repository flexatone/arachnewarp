// g++ aw_generator_perf.cpp aw_generator.cpp aw_common.cpp aw_timer.cpp -Wall -o aw_generator_perf

#include <cassert>


#include "aw_generator.h"
#include "aw_common.h"
#include "aw_timer.h"


int main() {

    //44100 / 64 == 689 // default
    //44100 / 128 == 345
    //44100 / 256 == 172
    
    // will be initialized();
	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Add);

    // this will automatically create constant Generators
    aw::Timer t1("adding two constant inputs");    
	g2->add_parameter_by_index(0, 6);
	g2->add_parameter_by_index(0, -5);
    std::cout << t1 << std::endl;
    
    g2->print_inputs(true);
    
    aw::Timer t2("44100 additions");
	g2->render(689);
    std::cout << t2 << std::endl;
    
	g2->print_output();

    
}















