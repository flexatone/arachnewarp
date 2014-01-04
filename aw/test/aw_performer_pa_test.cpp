// g++-4.7 -std=c++11 -I ../src  aw_performer_pa_test.cpp ../src/aw_performer_pa.cpp ../src/aw_illustration.cpp ../src/aw_generator.cpp ../src/aw_common.cpp -DSTAND_ALONE -l boost_filesystem -l boost_system -l boost_unit_test_framework -l boost_unit_test_framework -l sndfile -l portaudio  -L /usr/local/lib -Wall -o aw_performer_pa_test





#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE main
#endif  
#include <boost/test/unit_test.hpp>


#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>


#include "aw_common.h"
#include "aw_illustration.h"
#include "aw_generator.h"
#include "aw_performer_pa.h"

using namespace aw;

// provide testCase, name
BOOST_AUTO_TEST_CASE(aw_performer_test_1) {

	GenPtr g2 = Gen::make(GenID::Phasor);
    120 >> g2;
    
    
    PAPerformer pap(g2);
    pap(4);


}


// BOOST_AUTO_TEST_CASE(aw_performer_test_2) {

// 	EnvironmentPtr e = Environment::make_with_frame_size(1024);
// 	Environment::set_default_env(e);

// 	GenPtr s1 = Gen::make(GenID::Sine);
// 	PTypeRateContext::Opt::Pitch || s1;
//     60 >> s1;    

// 	GenPtr s2 = Gen::make(GenID::Sine);
// 	PTypeRateContext::Opt::Pitch || s2;
//     63 >> s2;    

// 	GenPtr s3 = Gen::make(GenID::Sine);
// 	PTypeRateContext::Opt::Pitch || s3;
//     67 >> s3;


//     GenPtr c1 = (s1 * .5) + (s2 * .3) + (s3 * .3);

// 	GenPtr p1 = Gen::make(GenID::Panner);
//    	Inj<GenPtr>({c1, Gen::make(.5)}) >> p1;

//     PAPerformer pap(p1);
//     pap(4);


// }











