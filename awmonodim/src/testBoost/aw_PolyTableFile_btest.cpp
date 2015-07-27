/*! \file aw_WaveSquare_unittest.cpp
    \brief Tests for aw_WaveSquare.cpp
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/

#define BOOST_TEST_MODULE

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/shared_ptr.hpp>


#include <stdexcept>
#include <cmath>

#include "aw_PolyTableFile.h"
#include "aw_GeneratorFactory.h"



using namespace aw;



// provide testCase, name
BOOST_AUTO_TEST_CASE(PolyTableFile) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System(44100, 8));
    GeneratorFactory gf(sys); // one instance



    std::string str1 = "/Volumes/xdisc/_sync/_x/src/arachnewarp/"
        "arachnewarp/audio/steppedMono.wav";
    std::string str2 = "/Volumes/xdisc/_sync/_x/src/arachnewarp/"
        "arachnewarp/audio/steppedStereo.wav";


    PolyTableFile gen1a(sys); // a direct instance
    gen1a.setParameter(aw::pNameFilePath, str1);

    BOOST_CHECK_EQUAL(gen1a.getParameterString(), "PolyTableFile{/Volumes/xdisc/_sync/_x/src/arachnewarp/arachnewarp/audio/steppedMono.wav}");

    BOOST_CHECK_EQUAL(gen1a.getPolyDepth(), 1);
    BOOST_CHECK_EQUAL(gen1a.getFramesDefined(), 44);


    double match1[] = {-1, -.75, -.5, -.25, 0, .25, .5, .75, 1};
    WorkingArrayPtr workingArray1;
    int pos(0);
    for (int i=0; i < gen1a.getFramesDefined(); i++) {
        workingArray1 = gen1a.getPolyAtSample(i);
        for (int j=0; j < gen1a.getPolyDepth(); j++) {
            BOOST_CHECK_EQUAL(abs(workingArray1[j]-match1[pos % 9]) < .001, true);
            pos += 1;
        }
    }




    // use generator factor to get a smart pointer
    GeneratorPtr gen1b = gf.create(aw::gNamePolyTableFile);

    BOOST_CHECK_EQUAL(gen1b->getParameterString(), "PolyTableFile{none}");

    gen1b->setParameter(aw::pNameFilePath, str2);
    BOOST_CHECK_EQUAL(gen1b->getParameterString(), "PolyTableFile{/Volumes/xdisc/_sync/_x/src/arachnewarp/arachnewarp/audio/steppedStereo.wav}");


    gen1b->setParameter(aw::pNameFilePath, "/Volumes/xdisc/_sync/_x/src/arachnewarp/arachnewarp/audio/steppedStereo.wav");

    BOOST_CHECK_EQUAL(gen1b->getParameterString(), "PolyTableFile{/Volumes/xdisc/_sync/_x/src/arachnewarp/arachnewarp/audio/steppedStereo.wav}");


    // left channel values
    double match2a[] = {-1, -.75, -.5, -.25};
    double match2b[] = {.25, .5, .75, 1};

    pos = 0;
    for (int i=0; i < gen1b->getFramesDefined(); i++) {
        workingArray1 = gen1b->getPolyAtSample(i);
        // 0 is left
        BOOST_CHECK_EQUAL(abs(workingArray1[0] - match2a[pos % 4]) < .001, true);
        // 1 is right
        BOOST_CHECK_EQUAL(abs(workingArray1[1] - match2b[pos % 4]) < .001, true);
        pos += 1;
    }

    GeneratorPtr gen1c = gf.create("polytablefile{/Volumes/xdisc/_sync/_x/src/arachnewarp/arachnewarp/audio/steppedStereo.wav}");

    BOOST_CHECK_EQUAL(gen1c->getParameterString(), "PolyTableFile{/Volumes/xdisc/_sync/_x/src/arachnewarp/arachnewarp/audio/steppedStereo.wav}");

    pos = 0;
    for (int i=0; i < gen1c->getFramesDefined(); i++) {
        workingArray1 = gen1c->getPolyAtSample(i);
        // 0 is left
        BOOST_CHECK_EQUAL(abs(workingArray1[0] - match2a[pos % 4]) < .001, true);
        // 1 is right
        BOOST_CHECK_EQUAL(abs(workingArray1[1] - match2b[pos % 4]) < .001, true);
        pos += 1;
    }



}


