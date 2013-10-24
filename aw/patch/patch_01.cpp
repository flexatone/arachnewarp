// g++-4.7 -std=c++11 patch_01.cpp -I ../src ../src/aw_generator.cpp ../src/aw_common.cpp ../src/aw_timer.cpp ../src/aw_illustration.cpp ../src/aw_performer.cpp -l boost_filesystem -l boost_system -l sndfile -l portaudio -l portaudiocpp -L /usr/local/lib -Wall -O3 -o patch_01


#include <cassert>
#include <cmath>


#include "aw_generator.h"
#include "aw_common.h"
#include "aw_timer.h"
#include "aw_performer.h"



using namespace aw;



int main() {

    EnvironmentPtr e = Environment::make_with_frame_size(128);
    Environment::set_default_env(e);

    GenPtr s1 = Gen::make(GenID::Sine);
    PTypeRateContext::Opt::Pitch || s1;
    60 >> s1;    

    GenPtr s2 = Gen::make(GenID::Sine);
    PTypeRateContext::Opt::Pitch || s2;
    63 >> s2;    

    GenPtr s3 = Gen::make(GenID::Sine);
    PTypeRateContext::Opt::Pitch || s3;
    67 >> s3;

    GenPtr am1 = Gen::make(GenID::Sine);
    PTypeRateContext::Opt::Seconds || am1;
    1 >> am1;

    GenPtr map1 = Gen::make(GenID::Map);
    Inj<GenPtr>({
            am1, 
            Gen::make(0), 
            Gen::make(1), 
            Gen::make(-1), 
            Gen::make(1)}) >> map1;   


    GenPtr c1 = ((s1 * .2) + (s2 * .3) + (s3 * .5)) * map1;

    GenPtr p1 = Gen::make(GenID::Panner);
    Inj<GenPtr>({c1, Gen::make(.5)}) >> p1;

    PAPerformer pap(p1);
    pap(4);


    
}




// 20130505
//pythagoras-ix % ./aw_generator_perf 
//total time for 60 second of audio: <Timer: fm mod: 294.129 msec>
//total time for 60 second of audio: <Timer: mapping two sinewaves: 308.395 msec>
//


// after adding min/max
//total time for 60 second of audio: <Timer: fm mod: 404.061 msec>
//total time for 60 second of audio: <Timer: mapping two sinewaves: 417.88 msec>










