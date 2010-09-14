/*! \file aw_WaveSquare_unittest.cpp
    \brief Tests for aw_WaveSquare.cpp
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <stdexcept>

#include <boost/shared_ptr.hpp>


#include "aw_WaveSine.h"
#include "aw_Constant.h"
#include <gtest/gtest.h>



// provide testCase, name
TEST(BasicTests, WaveSineBasic) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System(44100, 8));


    WaveSine gen1(sys); 

    gen1.setParameter(aw::pNameMinimum, -5);
    gen1.setParameter(aw::pNameMaximum, 5);
    // period of 5 seconds
    gen1.setParameter(aw::pNameRate, .2, aw::pContextNameFrequency);

    gen1.printSample(20, 0);

    ASSERT_NEAR(gen1.getValueAtSecond(0), 0,.000001);
    ASSERT_NEAR(gen1.getValueAtSecond(5), 0, .000001);
    ASSERT_NEAR(gen1.getValueAtSecond(10), 0, .000001);


//     EXPECT_EQ(gen1.getValueAtSample(0), 0);
//     EXPECT_EQ(gen1.getValueAtSample(1), 0);
//     EXPECT_EQ(gen1.getValueAtSample(2), 1);
//     EXPECT_EQ(gen1.getValueAtSample(3), 1);

}


