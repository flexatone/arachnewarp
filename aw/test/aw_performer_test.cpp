// g++-4.7 -std=c++11 -I ../src  aw_performer_test.cpp ../src/aw_performer.cpp ../src/aw_illustration.cpp ../src/aw_generator.cpp ../src/aw_common.cpp -DSTAND_ALONE -l boost_filesystem -l boost_system -l boost_unit_test_framework -l boost_unit_test_framework -l sndfile  -l portaudio -l portaudiocpp -Wall -o aw_performer_test


// g++-4.7 -std=c++11 -I ../src aw_common_test.cpp ../src/aw_common.cpp -DSTAND_ALONE -l boost_filesystem -l boost_system -l boost_unit_test_framework -Wall -o aw_common_test

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
#include "aw_performer.h"

using namespace aw;

// provide testCase, name
BOOST_AUTO_TEST_CASE(aw_performer_test_1) {

	GenPtr g2 = Gen::make(GenID::Phasor);
    120 >> g2;
    
    
    PAPerformer pap(g2);
    pap(4);


}













