



// g++-4.7 -std=c++11 aw_generator_rt.cpp -I ../src ../src/aw_generator.cpp ../src/aw_performer.cpp ../src/aw_common.cpp ../src/aw_illustration.cpp ../src/aw_timer.cpp -l boost_filesystem -l boost_system -l sndfile -l portaudio -l portaudiocpp -L /usr/local/lib -Wall -O3 -o aw_generator_rt




#include <iostream>
#include "portaudiocpp/PortAudioCpp.hxx"

#include "aw_generator.h"
#include "aw_performer.h"


// ----------------------------------------------------------------------------

using namespace aw;


void am_test() {

    EnvironmentPtr e = Environment::make_with_frame_size(64);
    Environment::set_default_env(e);
    
    
    GenPtr g1 = Gen::make(GenID::Sine);
    220 >> g1;

    GenPtr gmap = Gen::make(GenID::Map);
    // source
    Inj<GenPtr>({
        18 >> Gen::make(GenID::Sine), // rate of fm
        Gen::make(-1),
        Gen::make(1),
        Gen::make(880+440),
        Gen::make(880-440),
        }) >> gmap;
    
    GenPtr g2 = Gen::make(GenID::Sine);
    gmap >> g2; // set the frequency with the mapped

    GenPtr genv = Gen::make(GenID::AttackDecay);

    Inj<SampleT>({
        0,
        .75, // attack
        .25, // decay
        4, // expoonent
        1, // cycle on
        }) >> genv;

    GenPtr root_gen = (g1 + g2) * genv;
    root_gen->print_inputs(true);
    
    PAPerformer pap(root_gen);
    pap(6);    

}

void pan_test() {

    GenPtr src_1 = Gen::make(GenID::Sine);
    330 >> src_1;
    GenPtr pan_ctrl_1 = Gen::make(GenID::Sine);
    2 >> pan_ctrl_1;	


    GenPtr src_2 = Gen::make(GenID::Phasor);
    8 >> src_2;
    GenPtr pan_ctrl_2 = Gen::make(GenID::Sine);
    1 >> pan_ctrl_2;


    GenPtr src_3 = Gen::make(GenID::Sine);
    783 >> src_3;
    GenPtr pan_ctrl_3 = Gen::make(GenID::Sine);
    .33333 >> pan_ctrl_3;



    //Inj<SampleT>({2, 0}) >> gctrl;
    //GenPtr gctrl_mapped = Gen::make(GenID::Map);
    //src lower, upper, dst lower uppper

    // pan from left to right over 1/10th a sec
    GenPtr pan_1 = Gen::make(GenID::Panner);
    Inj<GenPtr>({src_1 * .4, pan_ctrl_1}) >> pan_1;

    // multiplication does not take proxy into account
    GenPtr pan_2 = Gen::make(GenID::Panner);
    Inj<GenPtr>({(src_2 % 1) * .5, pan_ctrl_2}) >> pan_2;

    GenPtr pan_3 = Gen::make(GenID::Panner);
    Inj<GenPtr>({src_3 * .2, pan_ctrl_3}) >> pan_3;


    GenPtr root_gen = pan_2; //pan_1 + pan_2 + pan_3;
    std::cout << root_gen->get_output_count() << std::endl;

    root_gen->illustrate_network();

    PAPerformer pap(root_gen);
    pap(6);    

    // to record and write
	// GenPtr buf = Gen::make(GenID::Buffer);
 //    Inj<SampleT>({2, 6}) || buf; // 2 ch, 6 seconds
 //    root_gen >> buf;
 //    buf->render(1);
 //    buf->write_output_to_fp("pan_test.aif");


//    Inj<GenPtr>({
//            gctrl, 
//            Gen::make(0), 
//            Gen::make(1), 
//            Gen::make(-1), 
//            Gen::make(1)}) >> gctrl_mapped;   }
}


int main() {

    pan_test();

}





