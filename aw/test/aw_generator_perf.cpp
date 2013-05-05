// g++ aw_generator_perf.cpp -I ../src ../src/aw_generator.cpp ../src/aw_common.cpp ../src/aw_timer.cpp ../src/aw_illustration.cpp -l boost_filesystem -l boost_system -l sndfile -Wall -O3 -o aw_generator_perf



#include <cassert>
#include "aw_generator.h"
#include "aw_common.h"
#include "aw_timer.h"



bool a() {
    // this will automatically create constant Generators
    
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

    aw::Timer t1("fm mod");
    t1.start();
	gbuf->render(1); // render count here meaningless
    std::cout << "total time for 60 second of audio: " << t1 << std::endl;
    return true;
}


bool b() {


    aw::GeneratorShared gsine2 = aw::Generator::make(aw::Generator::ID_Sine);
    4 >> gsine2;
    aw::GeneratorShared gsine3 = aw::Generator::make(aw::Generator::ID_Sine);
    40 >> gsine3;
    
	aw::GeneratorShared gbuf = aw::Generator::make(aw::Generator::ID_Buffer);
	gbuf->set_slot_by_index(0, 1);
	gbuf->set_slot_by_index(1, 60);
        
    aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Map);
    g1->set_input_by_index(0, gsine3);
    g1->set_input_by_index(1, 0); // source
    g1->set_input_by_index(2, 1);
    g1->set_input_by_index(3, -2); // dst
    g1->set_input_by_index(4, gsine2); // -1 to 1 naturally

    gbuf->set_input_by_index(0, g1);

    aw::Timer t1("mapping two sinewaves");
    t1.start();
    gbuf->render(1);    
    std::cout << "total time for 60 second of audio: " << t1 << std::endl;
    return true;
}



int main() {

    assert(
        a() &&
        b()
        );
    
}




// 20130505
//pythagoras-ix % ./aw_generator_perf 
//total time for 60 second of audio: <Timer: fm mod: 294.129 msec>
//total time for 60 second of audio: <Timer: mapping two sinewaves: 308.395 msec>
//












