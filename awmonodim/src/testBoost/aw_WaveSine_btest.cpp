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

#include "aw_WaveSine.h"
#include "aw_Constant.h"


using namespace aw;



// provide testCase, name
BOOST_AUTO_TEST_CASE(WaveSineBasic) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System(44100, 8));

    WaveSine gen1(sys); 

    gen1.setParameter(aw::pNameMinimum, -5);
    gen1.setParameter(aw::pNameMaximum, 5);
    // period of 5 seconds
    gen1.setParameter(aw::pNameRate, .2, aw::pContextNameFrequency);

    gen1.printSample(20, 0);

    BOOST_CHECK_CLOSE(gen1.getValueAtSecond(0), 0.0,.1);

    BOOST_CHECK_EQUAL(gen1.getValueAtSample(5) > -0.1, true);
    BOOST_CHECK_EQUAL(gen1.getValueAtSample(5) < 0.1, true);

    BOOST_CHECK_EQUAL(gen1.getValueAtSample(10) > -0.1, true);
    BOOST_CHECK_EQUAL(gen1.getValueAtSample(10) < 0.1, true);

//     BOOST_CHECK_CLOSE(gen1.getValueAtSecond(5), 0.0, .1);
//     BOOST_CHECK_CLOSE(gen1.getValueAtSecond(10), 0.0, .1);

// BOOST_CHECK_CLOSE( v1, v2, 0.0001 );

//     BOOST_CHECK_EQUAL(gen1.getValueAtSample(0), 0);
//     BOOST_CHECK_EQUAL(gen1.getValueAtSample(1), 0);
//     BOOST_CHECK_EQUAL(gen1.getValueAtSample(2), 1);
//     BOOST_CHECK_EQUAL(gen1.getValueAtSample(3), 1);

}


