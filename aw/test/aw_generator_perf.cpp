// g++ aw_generator_perf.cpp -I ../src ../src/aw_generator.cpp ../src/aw_common.cpp ../src/aw_timer.cpp ../src/aw_illustration.cpp -l boost_filesystem -l boost_system -l sndfile -Wall -O3 -o aw_generator_perf



#include <cassert>
#include "aw_generator.h"
#include "aw_common.h"
#include "aw_timer.h"


int main() {

    // this will automatically create constant Generators
    aw::Timer t1("fm mod");    

    std::cout << t1 << std::endl;
    
    //g2->print_inputs(true);
	aw::GeneratorShared gbuf = aw::Generator::make(aw::Generator::ID_Buffer);
	gbuf->set_slot_by_index(0, 2);
	gbuf->set_slot_by_index(1, 60); // 60 sec

    aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Sine);
    aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Sine);
    //aw::GeneratorShared g3 = aw::Generator::make(aw::Generator::ID_Sine);

    // one cycle for 4 samples
    2 >> g1;
    aw::GeneratorShared g3 = g1 * .5 + .5;
    
    g3 >> g2;
    //g1->set_input_by_index(0, 22050);        

	gbuf->add_input_by_index(0, g2);
	gbuf->add_input_by_index(1, g3);

	gbuf->render(1); // render count here meaningless
    
    
    std::cout << "total time for 60 second of audio: " << t1 << std::endl;
    
	//g2->print_outputs();

    
}















