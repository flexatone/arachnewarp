// for portaudio with installed aw shared library
// g++-4.7 -std=c++11 patch_01.cpp -I ../src -I /usr/local/include/boost -I /usr/local/include ../src/aw_performer_pa.cpp -l boost_filesystem -l boost_system -l sndfile -l portaudio -l portaudiocpp -l arachnewarp -L /usr/local/lib -L ../bin -Wall -O3 -o patch_01


// for alsa verision
// g++-4.7 -std=c++11 patch_01.cpp -I ../src ../src/aw_generator.cpp ../src/aw_common.cpp ../src/aw_timer.cpp ../src/aw_illustration.cpp ../src/aw_alsa_performer.cpp -l boost_filesystem -l boost_system -l sndfile -l asound -L /usr/local/lib -Wall -O3 -o patch_01

// for portaudio
// g++-4.7 -std=c++11 patch_01.cpp -I ../src ../src/aw_generator.cpp ../src/aw_common.cpp ../src/aw_timer.cpp ../src/aw_illustration.cpp ../src/aw_performer_pa.cpp -l boost_filesystem -l boost_system -l sndfile -l portaudio -l portaudiocpp -L /usr/local/lib -Wall -O3 -o patch_01



#include <cassert>
#include <cmath>


#include "aw_generator.h"
#include "aw_common.h"
#include "aw_timer.h"
#include "aw_performer_pa.h"



using namespace aw;



int main() {

    EnvironmentPtr e = Environment::make_with_frame_size(64);
    Environment::set_default_env(e);


    // create sine chord
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
    PTypeRateContext::Opt::BPM || am1;
    30 >> am1;

    GenPtr map1 = Gen::make(GenID::Map);
    Inj<GenPtr>({
            am1, 
            Gen::make(0), 
            Gen::make(1), 
            Gen::make(-1), 
            Gen::make(1)}) >> map1;   


    GenPtr c1 = ((s1 * .2) + (s2 * .3) + (s3 * .5)) * map1;

    GenPtr p1 = Gen::make(GenID::Panner);
    Inj<GenPtr>({c1, Gen::make(.3)}) >> p1;


    // create noise texture
    GenPtr env1 = Gen::make(GenID::AttackDecay);
    GenPtr t1 = Gen::make(GenID::Phasor);
    PTypeRateContext::Opt::BPM || t1;
    120 >> t1;

    Inj<GenPtr>({
            t1 % 1, 
            Gen::make(.025), 
            Gen::make(.125)
            }) >> env1;

    GenPtr w1 = Gen::make(GenID::White);
    GenPtr p2 = Gen::make(GenID::Panner);
    Inj<GenPtr>({
            w1 * env1 * .4, 
            Gen::make(.9),
            }) >> p2;    


    // create noise texture
    GenPtr env2 = Gen::make(GenID::AttackDecay);
    GenPtr t2 = Gen::make(GenID::Phasor);
    PTypeRateContext::Opt::BPM || t2;
    90 >> t2;

    Inj<GenPtr>({
            t2 % 1, 
            Gen::make(.125), 
            Gen::make(.005)
            }) >> env2;

    GenPtr w2 = Gen::make(GenID::White);
    GenPtr p3 = Gen::make(GenID::Panner);
    Inj<GenPtr>({
            w2 * env2 * .2, 
            Gen::make(.1),
            }) >> p3;    



    PAPerformer pap(p1 + p2 + p3);
    pap(10);


    
}




// 20130505
//pythagoras-ix % ./aw_generator_perf 
//total time for 60 second of audio: <Timer: fm mod: 294.129 msec>
//total time for 60 second of audio: <Timer: mapping two sinewaves: 308.395 msec>
//


// after adding min/max
//total time for 60 second of audio: <Timer: fm mod: 404.061 msec>
//total time for 60 second of audio: <Timer: mapping two sinewaves: 417.88 msec>










