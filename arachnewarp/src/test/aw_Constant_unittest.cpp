/*! \file aw_Constant_unittest.cpp
    \brief Tests for Constant.
    
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include "aw_Constant.h"
#include "aw_System.h"
#include "aw_GeneratorFactory.h"

#include <boost/shared_ptr.hpp>

#include <gtest/gtest.h>



// provide testCase, name
TEST(BasicTests, ConstantBasic) {


    SystemPtr sys(new System(44100, 8));

    Constant a(sys); 

    EXPECT_EQ(a.getName(), "Constant");


    // set parameter by parameter name value
    a.setParameter(aw::pNameValue, 2.5);

    EXPECT_EQ(a.getValueAtSecond(0.0), 2.5);
    EXPECT_EQ(a.getValueAtSecond(0.5), 2.5);
    EXPECT_EQ(a.getValueAtSample(0.55), 2.5);
 

    // set a different value
    a.setParameter(aw::pNameValue, 300);
    EXPECT_EQ(a.getValueAtSecond(0.0), 300);


}


// provide testCase, name
TEST(BasicTests, ConstantViaReference) {

    SystemPtr sys(new System(44100, 8));

    Constant *gen1 = new Constant(sys); // create a constant generator
    gen1->setParameter(aw::pNameValue, 300);


    EXPECT_EQ(gen1->getValueAtSecond(0.0), 300);


}



TEST(BasicTests, ConstantValidParameters) {

    SystemPtr sys(new System(44100, 8));
    Constant gen1(sys); 

    EXPECT_EQ(gen1.isValidParameter(aw::pNameMaximum), false);
    EXPECT_EQ(gen1.isValidParameter(aw::pNameMinimum), false);
    EXPECT_EQ(gen1.isValidParameter(aw::pNameRefresh), false);
    EXPECT_EQ(gen1.isValidParameter(aw::pNameRate), false);

    EXPECT_EQ(gen1.isValidParameter(aw::pNameValue), true);

    // a bad argument for this generator will throw an exception
    ASSERT_THROW(gen1.setParameter(aw::pNameRate, .4,
        aw::pContextNamePeriodSamples), std::invalid_argument);
    
}



TEST(BasicTests, ConstantParameterString) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System());
    GeneratorFactory gf(sys); // one instance


    //GeneratorPtr gen1 = gf.create(aw::gNameConstant);
    Constant *gen1 = new Constant(sys); // create a constant generator

    // this depends on defaults
    EXPECT_EQ(gen1->getParameterString(), "Constant{0}");

    gen1->setParameter(aw::pNameValue, 220);
    EXPECT_EQ(gen1->getValueAtSecond(0.0), 220);

    EXPECT_EQ(gen1->getParameterString(), "Constant{220}");




    // try doing the same with GeneratorFactory
    GeneratorPtr gen2 = gf.create(aw::gNameConstant);

    // this depends on defaults
    EXPECT_EQ(gen2->getParameterString(), "Constant{0}");
    gen2->setParameter(aw::pNameValue, 220);
    EXPECT_EQ(gen2->getValueAtSecond(0.0), 220);
    EXPECT_EQ(gen2->getParameterString(), "Constant{220}");

}

