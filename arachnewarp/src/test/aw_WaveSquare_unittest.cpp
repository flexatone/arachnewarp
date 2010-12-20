/*! \file aw_WaveSquare_unittest.cpp
    \brief Tests for aw_WaveSquare.cpp
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include "aw_WaveSquare.h"
#include "aw_Constant.h"


#include <boost/shared_ptr.hpp>
#include <gtest/gtest.h>


#include <stdexcept>


using namespace aw;



// provide testCase, name
TEST(BasicTests, WaveSquareBasic) {

    // System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System(44100, 8));


    WaveSquare a(sys);

    ConstantPtr gen1(new Constant(sys));
    gen1->setParameter(aw::pNameValue, 0.0);

    ConstantPtr gen2(new Constant(sys));
    gen2->setParameter(aw::pNameValue, 1.0);

    ConstantPtr gen3(new Constant(sys));
    gen3->setParameter(aw::pNameValue, 1.0);

    a.setParameter(aw::pNameMinimum, gen1);
    a.setParameter(aw::pNameMaximum, gen2);

    a.setParameter(aw::pNameRate, gen3,
                   aw::pContextNameFrequency);

    EXPECT_EQ(a.getValueAtSecond(0.0), 0);
    EXPECT_EQ(a.getValueAtSecond(0.5), 1);
    EXPECT_EQ(a.getValueAtSecond(0.55), 1);


    // gen 4, 5 seconds per cycle
    ConstantPtr gen4(new Constant(sys));
    gen4->setParameter(aw::pNameValue, 5.0);

    a.setParameter(aw::pNameRate, gen4,
                   aw::pContextNamePeriodSeconds);
    // a.setSecondsPerCycle(5);// 5 seconds per cycle

    // this is now zero
    EXPECT_EQ(a.getValueAtSecond(0.55), 0);
    // this is now more than half
    EXPECT_EQ(a.getValueAtSecond(2.5), 1);
    EXPECT_EQ(a.getValueAtSecond(4.99), 1);
    EXPECT_EQ(a.getValueAtSecond(5.00), 0);

    a.printSample(20, 0);

}



// provide testCase, name
TEST(BasicTests, WaveSquareParameterSetting) {


    SystemPtr sys(new System(44100, 8));

    WaveSquare a(sys);

    ConstantPtr gen1(new Constant(sys));
    gen1->setParameter(aw::pNameValue, 0.0);

    ConstantPtr gen2(new Constant(sys));
    gen2->setParameter(aw::pNameValue, 1.0);

    ConstantPtr gen3(new Constant(sys));
    gen3->setParameter(aw::pNameValue, 5.0);


    a.setParameter(aw::pNameMinimum, gen1);
    a.setParameter(aw::pNameMaximum, gen2);

    a.setParameter(aw::pNameRate, gen3, aw::pContextNameFrequency);
    // test that we have set the right thing
    EXPECT_EQ(a.getContext(aw::pNameRate),
             aw::pContextNameFrequency);

    // change to a different context
    a.setParameter(aw::pNameRate, gen3, aw::pContextNamePeriodSeconds);
    // test that we have set the right thing
    EXPECT_EQ(a.getContext(aw::pNameRate),
             aw::pContextNamePeriodSeconds);
}


TEST(BasicTests, WaveSquareValidParameterContext) {
    // test getting the valid parameter context

    
    SystemPtr sys(new System(44100, 8));
    WaveSquare a(sys);

    EXPECT_EQ(a.isValidContext(aw::pNameMinimum,
        aw::pContextNameNone), true);
    EXPECT_EQ(a.isValidContext(aw::pNameMinimum,
        aw::pContextNameFrequency), false);

    // rate can be fq or period, not none or trigger
    EXPECT_EQ(a.isValidContext(aw::pNameRate,
        aw::pContextNameFrequency), true);
    EXPECT_EQ(a.isValidContext(aw::pNameRate,
        aw::pContextNamePeriodSeconds), true);
    EXPECT_EQ(a.isValidContext(aw::pNameRate,
        aw::pContextNameTrigger), false);
    EXPECT_EQ(a.isValidContext(aw::pNameRate,
        aw::pContextNameNone), false);

    // if given none, an exception will be thrown
    ConstantPtr gen1(new Constant(sys));
    gen1->setParameter(aw::pNameValue, 5.0);

    ASSERT_THROW(a.setParameter(aw::pNameRate, gen1,
        aw::pContextNameNone), std::invalid_argument);

    a.setParameter(aw::pNameRate, gen1, aw::pContextNamePeriodSeconds);


}






TEST(BasicTests, WaveSquareDefaults) {

    SystemPtr sys(new System(44100, 8));

    // defaults already configured
    WaveSquare gen1(sys);

    // check the actual parameters

    EXPECT_EQ(gen1.getParameter(aw::pNameMinimum)->getValueAtSample(0),
             0);

    EXPECT_EQ(gen1.getParameter(aw::pNameMaximum)->getValueAtSample(0),
             1);
    EXPECT_EQ(gen1.getParameter(aw::pNameRate)->getValueAtSample(0),
             6);

    // default period is 6 seconds;4 should be in up cycle 
    EXPECT_EQ(gen1.getValueAtSecond(0), 0);
    EXPECT_EQ(gen1.getValueAtSecond(4), 1);


    gen1.setParameter(aw::pNameRate, 220.0,
                    aw::pContextNameFrequency);

    EXPECT_EQ(gen1.getParameter(aw::pNameRate)->getValueAtSample(0),
             220);

    // set to an int
    gen1.setParameter(aw::pNameRate, 5,
                    aw::pContextNameFrequency);
    EXPECT_EQ(gen1.getParameter(aw::pNameRate)->getValueAtSample(0),
             5);
}


TEST(BasicTests, WaveSquareFrequencyParameter) {

    SystemPtr sys(new System(44100, 8));

    WaveSquare gen1(sys);
    gen1.setParameter(aw::pNameMinimum, -2);
    gen1.setParameter(aw::pNameMaximum, 8);
    gen1.setParameter(aw::pNameDutyCycle, .5);

    ConstantPtr gen2(new Constant(sys));
    // oscillates at a rate of 4
    gen2->setParameter(aw::pNameValue, 0.25);
    EXPECT_EQ(gen2->getValueAtSecond(0), .25);

    // the default frequency is 6
    EXPECT_EQ(gen1.getParameter(aw::pNameRate)->getValueAtSample(0),
             6);
    // set a new frequency
    gen1.setParameter(aw::pNameRate, 0.25, aw::pContextNameFrequency);
    EXPECT_EQ(gen1.getParameter(aw::pNameRate)->getValueAtSample(0),
             .25);
    EXPECT_EQ(gen1.getParameter(aw::pNameRate)->getValueAtSample(6),
             .25);


    EXPECT_EQ(gen1.getValueAtSecond(0), -2);
    EXPECT_EQ(gen1.getValueAtSecond(1), -2);
    EXPECT_EQ(gen1.getValueAtSecond(2), 8);
    EXPECT_EQ(gen1.getValueAtSecond(3), 8);
    EXPECT_EQ(gen1.getValueAtSecond(4), -2);
    EXPECT_EQ(gen1.getValueAtSecond(5), -2);

    gen1.printSample(20, 0);

    EXPECT_EQ(gen1.getParameter(aw::pNameRate)->getValueAtSample(0),
             .25);


    WaveSquare c(sys);
    c.setParameter(aw::pNameMinimum, -8);
    c.setParameter(aw::pNameMaximum, 2);

    ConstantPtr gen6(new Constant(sys));
    // oscillates at a rate of 5
    gen6->setParameter(aw::pNameValue, .2);
    EXPECT_EQ(gen6->getValueAtSecond(0), .2);

    c.setParameter(aw::pNameRate, gen6,
                   aw::pContextNameFrequency);

    EXPECT_EQ(c.getParameter(aw::pNameRate)->getValueAtSample(0),
             .2);

    EXPECT_EQ(c.getValueAtSecond(0), -8);
    EXPECT_EQ(c.getValueAtSecond(1), -8);
    EXPECT_EQ(c.getValueAtSecond(2), 2);
    EXPECT_EQ(c.getValueAtSecond(3), 2);
    EXPECT_EQ(c.getValueAtSecond(4), 2);
    EXPECT_EQ(c.getValueAtSecond(5), -8);

    c.printSample(20, 0);

}



TEST(BasicTests, WaveSquarePeriodInSamples) {

    SystemPtr sys(new System(44100, 8));

    WaveSquare gen1(sys);
    gen1.setParameter(aw::pNameRate, 4, aw::pContextNamePeriodSamples);
    EXPECT_EQ(gen1.getValueAtSample(0), 0);
    EXPECT_EQ(gen1.getValueAtSample(1), 0);
    EXPECT_EQ(gen1.getValueAtSample(2), 1);
    EXPECT_EQ(gen1.getValueAtSample(3), 1);
}



TEST(BasicTests, WaveSquareValidParameters) {

    SystemPtr sys(new System(44100, 8));
    WaveSquare gen1(sys);

    EXPECT_EQ(gen1.isValidParameter(aw::pNameMaximum), true);
    EXPECT_EQ(gen1.isValidParameter(aw::pNameMinimum), true);
    EXPECT_EQ(gen1.isValidParameter(aw::pNameRefresh), false);
    EXPECT_EQ(gen1.isValidParameter(aw::pNameRate), true);
    EXPECT_EQ(gen1.isValidParameter(aw::pNameDutyCycle), true);

    // a bad argument for this generator will throw an exception
    ASSERT_THROW(gen1.setParameter(aw::pNameRefresh, .4,
        aw::pContextNamePeriodSamples), std::invalid_argument);
    
}



