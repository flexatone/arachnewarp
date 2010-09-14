/*! \file aw_RandomUniform_unittest.cpp
    \brief Tests for aw_RandomUniform.cpp
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include "aw_RandomUniform.h"
#include "aw_Common.h"
#include "aw_Constant.h"
#include "aw_System.h"


#include <boost/shared_ptr.hpp>

#include <gtest/gtest.h>



// provide testCase, name
TEST(BasicTests, RandomUniformBasic) {

    // System sys;

    SystemPtr sys(new System);

    RandomUniform a(sys); 

    // EXPECT_EQ(a.getName(), "RandomUniform");


    // these names are simply numerical codes for reference
    EXPECT_EQ(aw::pNameMinimum,  0);
    EXPECT_EQ(aw::pNameMaximum,  1);


    //Constant *gen1 = new Constant(sys); // create a constant generator
    ConstantPtr gen1(new Constant(sys));
    gen1->setParameter(aw::pNameValue, 0.0);

    //Constant *gen2 = new Constant(sys); // create a constant generator
    ConstantPtr gen2(new Constant(sys));
    gen2->setParameter(aw::pNameValue, 2.0);
    EXPECT_EQ(gen2->getValueAtSecond(0.0), 2);

    // this will be a trigger, set to constant 1, which means
    // new values are always made
    ConstantPtr gen3(new Constant(sys));
    gen3->setParameter(aw::pNameValue, 1);


    // set constants to min and max
    a.setParameter(aw::pNameMinimum, gen1);
    a.setParameter(aw::pNameMaximum, gen2);
    a.setParameter(aw::pNameRefresh, gen3, aw::pContextNameTrigger);

    EXPECT_EQ(a.getParameter(aw::pNameMinimum)->getName(), "Constant");
    EXPECT_EQ(a.getParameter(aw::pNameMaximum)->getName(), "Constant");


    // test getting the value of an embedded generator
    EXPECT_EQ(a.getParameter(aw::pNameMaximum)->getValueAtSecond(0), 2);
    EXPECT_EQ(a.getParameter(aw::pNameRefresh)->getValueAtSecond(0), 1);

    // values are between 0 and 2
    EXPECT_TRUE(a.getValueAtSample(0) >= 0.0);
    EXPECT_TRUE(a.getValueAtSample(0) <= 2.0);

    a.reset();
    a.printSample(10, 0); // display 10 sample outputs, b/n 0 and 1
    a.reset();
    a.printSample(10, 0); // display 10 sample outputs, b/n 0 and 1

    // try to set new values for embedded parameters
    gen1->setParameter(aw::pNameValue, 100.0);
    gen2->setParameter(aw::pNameValue, 200.0);

    // changes are reflected in output
    a.reset();
    EXPECT_TRUE(a.getValueAtSecond(0.0) >= 100.0);
    EXPECT_TRUE(a.getValueAtSecond(0.0) <= 200.0);

    // values should not repeat!
    float b = a.getValueAtSample(0);
    float c = a.getValueAtSample(1);
    float d = a.getValueAtSample(2);

    EXPECT_TRUE(b != c);
    EXPECT_TRUE(c != d);


    // display 10 sample outputs, b/n 100 and 200, at a refresh rate of 2s
    // gen3 is already controlly parameterReferesh
    gen3->setParameter(aw::pNameValue, 4);
    // need reassign to change context
    a.setParameter(aw::pNameRefresh, gen3, aw::pContextNamePeriodSeconds);
    EXPECT_EQ(a.getParameter(aw::pNameRefresh)->getValueAtSecond(0), 4);
    EXPECT_EQ(a.getContext(aw::pNameRefresh), 
        aw::pContextNamePeriodSeconds);

    // display output incremented in seconds
    a.reset();
    a.printSample(10, 0, 1, aw::timeContextNameSecond); 

    // try in increments of 1 second
    gen3->setParameter(aw::pNameValue, 2);
    a.reset();
    a.printSample(10, 0, 1, aw::timeContextNameSecond); 


}



TEST(BasicTests, RandomUniformBlock) {

    //System syse;
    SystemPtr sys(new System(44100, 8));

    RandomUniform a(sys); 

    ConstantPtr gen1(new Constant(sys));
    gen1->setParameter(aw::pNameValue, 5.0);

    ConstantPtr gen2(new Constant(sys));
    gen2->setParameter(aw::pNameValue, 8.0);

    ConstantPtr gen3(new Constant(sys));
    gen3->setParameter(aw::pNameValue, 1);


    // set constants to min and max
    a.setParameter(aw::pNameMinimum, gen1);
    a.setParameter(aw::pNameMaximum, gen2);
    a.setParameter(aw::pNameRefresh, gen3, aw::pContextNameTrigger);

    // our test size is 8
//     double outputSamples[8]; 
//     a.getBlock(0, outputSamples);
//     aw::printArray(outputSamples, 8);

}



TEST(BasicTests, RandomUniformValidParameterContext) {
    // test getting the valid parameter context

    
    SystemPtr sys(new System(44100, 8));
    RandomUniform a(sys); 

    EXPECT_TRUE(a.isValidContext(aw::pNameMinimum,
        aw::pContextNameNone) == true);
    EXPECT_TRUE(a.isValidContext(aw::pNameMinimum,
        aw::pContextNameFrequency) == false);
    EXPECT_TRUE(a.isValidContext(aw::pNameRefresh,
        aw::pContextNameFrequency) == true);
    EXPECT_TRUE(a.isValidContext(aw::pNameRefresh,
        aw::pContextNamePeriodSeconds) == true);
    EXPECT_TRUE(a.isValidContext(aw::pNameRefresh,
        aw::pContextNameTrigger) == true);
    EXPECT_TRUE(a.isValidContext(aw::pNameRefresh,
        aw::pContextNameNone) == false);

    // if given none, an exception will be thrown
    ConstantPtr gen1(new Constant(sys));
    gen1->setParameter(aw::pNameValue, 5.0);

    ASSERT_THROW(a.setParameter(aw::pNameRefresh, gen1,
        aw::pContextNameNone), std::invalid_argument);

    // can assign if the proper context
    a.setParameter(aw::pNameRefresh, gen1, aw::pContextNamePeriodSeconds);
}





TEST(BasicTests, RandomUniformDefaults) {

    SystemPtr sys(new System(44100, 8));

    // defaults already configured
    RandomUniform gen1(sys); 

    EXPECT_EQ(gen1.getParameter(aw::pNameMinimum)->getValueAtSecond(0),
        0);
    EXPECT_EQ(gen1.getParameter(aw::pNameMaximum)->getValueAtSecond(0),
        1);

    EXPECT_EQ(gen1.getParameter(aw::pNameRefresh)->getValueAtSecond(0),
        1);

    EXPECT_TRUE(gen1.getValueAtSample(0) >= 0.0);
    EXPECT_TRUE(gen1.getValueAtSample(0) <= 1.0);

    gen1.setParameter(aw::pNameMaximum, 220.0);
    EXPECT_EQ(gen1.getParameter(aw::pNameMaximum)->getValueAtSecond(0),
        220);


    gen1.setParameter(aw::pNameMaximum, 5);
    EXPECT_EQ(gen1.getParameter(aw::pNameMaximum)->getValueAtSecond(0),
        5);



}



TEST(BasicTests, RandomUniformValidParameters) {

    SystemPtr sys(new System(44100, 8));
    RandomUniform gen1(sys); 

    EXPECT_EQ(gen1.isValidParameter(aw::pNameMaximum), true);
    EXPECT_EQ(gen1.isValidParameter(aw::pNameMinimum), true);
    EXPECT_EQ(gen1.isValidParameter(aw::pNameRefresh), true);
    EXPECT_EQ(gen1.isValidParameter(aw::pNameRate), false);

    // a bad argument for this generator will throw an exception
    ASSERT_THROW(gen1.setParameter(aw::pNameRate, .4,
        aw::pContextNamePeriodSamples), std::invalid_argument);
    
}



