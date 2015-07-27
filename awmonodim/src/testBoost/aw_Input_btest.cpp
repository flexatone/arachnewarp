/*! \file aw_Output_unittest.cpp
    \brief Object model of output stream
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/

#define BOOST_TEST_MODULE

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/shared_ptr.hpp>


#include <vector>
#include <cmath>

#include "aw_System.h"
#include "aw_Common.h"
#include "aw_Output.h"
#include "aw_Input.h"

#include "aw_WaveSquare.h"
#include "aw_WaveSine.h"
#include "aw_RandomUniform.h"
#include "aw_Multiply.h"
#include "aw_GeneratorFactory.h"


using namespace aw;


// provide testCase, name
BOOST_AUTO_TEST_CASE(InputBasic) {


    SystemPtr sys(new System);
    InputPtr input(new Input(sys));

    int ch = 0; // channels

    aw::DynamicTable table;

    ch = input->read("/Volumes/xdisc/_sync/_x/src/arachnewarp/"
        "arachnewarp/audio/steppedMono.wav", 
        table);

    BOOST_CHECK_EQUAL(ch, 1);    

    // source values written into files for testing
    double match1[] = {-1, -.75, -.5, -.25, 0, .25, .5, .75, 1};
    for (int i=0; i < table.size(); i++) {
        BOOST_CHECK_EQUAL(abs(table[i]-match1[i%9]) < .001, true);
    }
    
    ch = input->read("/Volumes/xdisc/_sync/_x/src/arachnewarp/"
        "arachnewarp/audio/steppedStereo.wav", 
        table);
    
    BOOST_CHECK_EQUAL(ch, 2);    

    // left channel values
    double match2a[] = {-1, -.75, -.5, -.25};
    // right channel values
    double match2b[] = {.25, .5, .75, 1};

    for (int i=0; i < table.size(); i++) {
        if (i % 2 == 0) {
            BOOST_CHECK_EQUAL(abs(table[i]-match2a[i%4]) < .001, true);
        } else {
            BOOST_CHECK_EQUAL(abs(table[i]-match2b[i%4]) < .001, true);
        }        
    }

    // try a string version


    std::string str1 = "/Volumes/xdisc/_sync/_x/src/arachnewarp/"
        "arachnewarp/audio/steppedMono.wav";

    std::string str2 = "/Volumes/xdisc/_sync/_x/src/arachnewarp/"
        "arachnewarp/audio/steppedStereo.wav";


    ch = input->read(str1, table);
    BOOST_CHECK_EQUAL(ch, 1);    

    ch = input->read(str2, table);
    BOOST_CHECK_EQUAL(ch, 2);    

    // out.write(gen1, 3);
    //gen1->printSample(6, 0, 4410);


    // try declaring, then loading
//     InputPtr input2;
// 
//     InputPtr input2(new Input(sys));
// 
//     ch = input2->read(str2, table);
//     BOOST_CHECK_EQUAL(ch, 2);    


}


