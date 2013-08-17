// g++-4.7 -std=c++11 aw_generator_perf.cpp -I ../src ../src/aw_generator.cpp ../src/aw_common.cpp ../src/aw_timer.cpp ../src/aw_illustration.cpp -l boost_filesystem -l boost_system -l sndfile -L /usr/local/lib -Wall -O3 -o aw_generator_perf

// g++-4.7 -std=c++11 -I ../src  aw_generator_test.cpp ../src/aw_generator.cpp ../src/aw_common.cpp ../src/aw_illustration.cpp -DSTAND_ALONE -l boost_unit_test_framework -l boost_filesystem -l boost_system -l sndfile -Wall -g -o aw_generator_test

#include <cassert>
#include <cmath>


#include "aw_generator.h"
#include "aw_common.h"
#include "aw_timer.h"


using namespace aw;

bool a() {
    // this will automatically create constant Generators
    
    //g2->print_inputs(true);
	aw::GenPtr gbuf = aw::Gen::make(aw::GenID::Buffer);
	gbuf->set_slot_by_index(0, 2);
	gbuf->set_slot_by_index(1, 60); // 60 sec

    aw::GenPtr g1 = aw::Gen::make(aw::GenID::Sine);
    aw::GenPtr g2 = aw::Gen::make(aw::GenID::Sine);
    //aw::GenPtr g3 = aw::Gen::make(aw::GenID::Sine);

    // one cycle for 4 samples
    2 >> g1;
    aw::GenPtr g3 = g1 * .5 + .5;
    
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


    aw::GenPtr gsine2 = aw::Gen::make(aw::GenID::Sine);
    4 >> gsine2;
    aw::GenPtr gsine3 = aw::Gen::make(aw::GenID::Sine);
    40 >> gsine3;
    
	aw::GenPtr gbuf = aw::Gen::make(aw::GenID::Buffer);
	gbuf->set_slot_by_index(0, 1);
	gbuf->set_slot_by_index(1, 60);
        
    aw::GenPtr g1 = aw::Gen::make(aw::GenID::Map);
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


bool c() {

	aw::GenPtr g1 = aw::Gen::make(aw::GenID::Buffer);
	// create two channel buffer
	g1->set_slot_by_index(0, 2);
	g1->set_slot_by_index(1, 60.0);
		
	// create 
	aw::GenPtr g2 = aw::Gen::make(aw::GenID::Phasor);    
	g2->add_input_by_index(0, 4); // a constant frequency
	
	aw::GenPtr g3 = aw::Gen::make(aw::GenID::Phasor);    
	g3->add_input_by_index(0, 12); // a constant frequency

	aw::GenPtr g4 = aw::Gen::make(aw::GenID::Phasor);    
	g3->add_input_by_index(0, -2); // a constant frequency

	// add phasor to buffer input; might scale buffer if necessary; could mix multiple too
	g1->add_input_by_index(0, g2);
	g1->add_input_by_index(1, g3);
	g1->add_input_by_index(1, g4);
	
    
    aw::Timer t1("summing three phasors in two channels");
    t1.start();
	g1->render(1); // render count here meaningless
	//g1->illustrate_outputs();
    std::cout << "total time for 60 second of audio: " << t1 << std::endl;
	
    return true;
}

bool d() {
    // test some math opperations for comparison
    aw::OutputsSizeT x;
    aw::OutputsSizeT y;

    aw::Timer t1("calling sine through iterated numbers");
    t1.start();
    for (x=0; x<(44100*60); ++x) {
        y += sin(x); // just doing an assignment
    }
	//g1->illustrate_outputs();
    std::cout << "total time for 60 seconds (" << x << " samples) of audio: " << t1 << std::endl;
    return true;
}


bool e() {
    unsigned int i;
    unsigned int count {44100*60};

    DirectedIndex d1(100);    
    d1.set_direction(PTypeDirection::Opt::RandomPermutate);
    aw::Timer t1("random permutation");    
    for (i=0; i<count; ++i) {
        d1.next();
    }
    std::cout << "iterations " << i << ": " << t1 << std::endl;


    DirectedIndex d2(100);    
    d2.set_direction(PTypeDirection::Opt::RandomWalk);
    aw::Timer t2("random walk");    
    for (i=0; i<count; ++i) {
        d2.next();
    }
    std::cout << "iterations " << i << ": " << t2 << std::endl;


    DirectedIndex d3(100);    
    d3.set_direction(PTypeDirection::Opt::RandomSelect);
    aw::Timer t3("random select");    
    for (i=0; i<count; ++i) {
        d3.next();
    }
    std::cout << "iterations " << i << ": " << t3 << std::endl;


    DirectedIndex d4(100);    
    d4.set_direction(PTypeDirection::Opt::Cycle);
    aw::Timer t4("cycle");    
    for (i=0; i<count; ++i) {
        d4.next();
    }
    std::cout << "iterations " << i << ": " << t4 << std::endl;

    DirectedIndex d5(100);    
    d5.set_direction(PTypeDirection::Opt::Forward);
    aw::Timer t5("forward");    
    for (i=0; i<count; ++i) {
        d5.next();
    }
    std::cout << "iterations " << i << ": " << t5 << std::endl;

    DirectedIndex d6(100);    
    d6.set_direction(PTypeDirection::Opt::Reverse);
    aw::Timer t6("reverse");
    for (i=0; i<count; ++i) {
        d6.next();
    }
    std::cout << "iterations " << i << ": " << t6 << std::endl;


    return true;
}


int main() {

    assert(
        a() &&
        b() &&
        c() &&
        d() &&
        e() 
        );
    
}




// 20130505
//pythagoras-ix % ./aw_generator_perf 
//total time for 60 second of audio: <Timer: fm mod: 294.129 msec>
//total time for 60 second of audio: <Timer: mapping two sinewaves: 308.395 msec>
//


// after adding min/max
//total time for 60 second of audio: <Timer: fm mod: 404.061 msec>
//total time for 60 second of audio: <Timer: mapping two sinewaves: 417.88 msec>










