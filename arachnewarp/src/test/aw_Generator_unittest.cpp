
/*! \file aw_Generator_unittest.cpp
    \brief Tests for aw_Generator.cpp
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/

#include "aw_Common.h"
#include "aw_Generator.h"
#include "aw_GeneratorFactory.h"
#include "aw_System.h"

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>



// provide testCase, name
TEST(BasicTests, GeneratorBasic) {

    //System sys;
    //System *sys = new System(44100, 8); 
    SystemPtr sys(new System(44100, 8));

    Generator gen1(sys); 

    EXPECT_EQ(gen1.getName(), "Generator");

    EXPECT_EQ(gen1.getGeneratorType(), aw::gTypeMono);

    EXPECT_EQ(gen1.getSystem()->getSamplingRate(), 44100);

    EXPECT_EQ(sys.use_count(), 2);


    // try to pass the same object
    Generator gen2(sys); 
    EXPECT_EQ(gen2.getSystem()->getSamplingRate(), 44100);


    // we can find the use count b/c this is a smart pointer!
    EXPECT_EQ(sys.use_count(), 3);


//     EXPECT_EQ(a.getValue(0.0), 0);
//     EXPECT_EQ(a.getValue(0.5), 1);
//     EXPECT_EQ(a.getValue(0.55), 1);
// 

}




TEST(BasicTests, GeneratorUtilityMethods) {

    SystemPtr sys(new System);
    GeneratorFactory gf(sys); // one instance

    GeneratorPtr gen1 = gf.create(aw::gNameWaveSine);
    gen1->setParameter(aw::pNameRate, 69, aw::pContextNamePitch);

    // testing an internal method
    EXPECT_EQ(gen1->getValueWithContextToFrequency(69, aw::pNameRate, 
        gen1->getSystem()->getSamplingRate()), 440.0);

    // set rate by bpm
    gen1->setParameter(aw::pNameRate, 120, aw::pContextNameBeatsPerMinute);
    EXPECT_EQ(gen1->getValueWithContextToPeriodSamples(120, aw::pNameRate, 
        gen1->getSystem()->getSamplingRate()), 22050);
    // half the value
    EXPECT_EQ(gen1->getValueWithContextToPeriodSamples(30, aw::pNameRate, 
        gen1->getSystem()->getSamplingRate()), 88200);

    EXPECT_EQ(gen1->getValueWithContextToFrequency(30, aw::pNameRate, 
        gen1->getSystem()->getSamplingRate()), 0.5);

    EXPECT_EQ(gen1->getValueWithContextToFrequency(120, aw::pNameRate, 
        gen1->getSystem()->getSamplingRate()), 2);


}



TEST(BasicTests, GeneratorSetParameterFunctions) {


    SystemPtr sys(new System);
    GeneratorFactory gf(sys); // one instance

    // with defined strings
    std::string pName("rate");
    std::string pcName("pitch");
    GeneratorPtr gen1 = gf.create("wavesine");
    gen1->setParameter(pName, 69, pcName);


    // with character arrays

    GeneratorPtr gen2 = gf.create("randomuniform");
    gen2->setParameter("min", 10, "none");
    gen2->setParameter("max", 10, "none");
    gen2->setParameter("refresh", 60, "bpm");

    EXPECT_EQ(gen2->getValueAtSample(0), 10);
    gen2->setParameter("max", 20, "none");

    // check that values have been set
    EXPECT_EQ(gen2->getParameter("max")->getValueAtSample(0), 20);
    EXPECT_EQ(gen2->getContext("refresh"), aw::pContextNameBeatsPerMinute);


}




TEST(BasicTests, GeneratorReadParameterString) {


    SystemPtr sys(new System);
    GeneratorFactory gf(sys); // one instance

    GeneratorPtr gen1 = gf.create("wavesine");

    // presently need to remove spaces
    std::string args1("min{constant{-10}}max{constant{10}}rate{constant{30}}{frequency}");    
    //std::string args1("min{-10}max{10}rate{30}{frequency}");

    gen1->readParameterString(args1);

    EXPECT_EQ(gen1->getParameterString(), "WaveSine{rate{Constant{30}}{frequency}minimum{Constant{-10}}maximum{Constant{10}}}");


    GeneratorPtr gen2 = gf.create("constant{30}");
    EXPECT_EQ(gen2->getValueAtSample(0), 30);


    GeneratorPtr gen3 = gf.create("WaveSine{rate{30}{frequency}minimum{-20}maximum{20}}");
    EXPECT_EQ(gen3->getParameter("max")->getValueAtSample(0), 20);
    EXPECT_EQ(gen3->getParameter("min")->getValueAtSample(0), -20);
    EXPECT_EQ(gen3->getParameter("rate")->getValueAtSample(0), 30);

    EXPECT_EQ(gen3->getParameterString(), "WaveSine{rate{Constant{30}}{frequency}minimum{Constant{-20}}maximum{Constant{20}}}");

    // set compact argument
    EXPECT_EQ(gen3->getParameterString(true), "WaveSine{rate{30}{frequency}minimum{-20}maximum{20}}");


    GeneratorPtr gen4 = gf.create("WaveSine{rate{2000}{frequency}minimum{-1}maximum{1}}");

    EXPECT_EQ(gen4->getParameter("rate")->getValueAtSample(0), 2000);
    EXPECT_EQ(gen4->getParameter("min")->getValueAtSample(0), -1);
    EXPECT_EQ(gen4->getParameter("max")->getValueAtSample(0), 1);

}





TEST(BasicTests, GeneratorPolyGeneratorInterfaceEquivalance) {


    SystemPtr sys(new System);
    GeneratorFactory gf(sys); // one instance

    GeneratorPtr gen1 = gf.create("constant{3}");
    EXPECT_EQ(gen1->getValueAtSample(0), 3);
    EXPECT_EQ(gen1->getValueAtSecond(0), 3);
    // can get the same value by dereference pointer
    // even though this is a pointer to a single value, can use the 
    // the array interface to simulate an array-like structure
    EXPECT_EQ(gen1->getPolyAtSample(0)[0], 3);
    EXPECT_EQ(gen1->getPolyAtSecond(0)[0], 3);

    GeneratorPtr gen2 = gf.create("polyconstant{30,40}");
    // default fold down is to return the first value
    EXPECT_EQ(gen2->getValueAtSample(0), 30);
    EXPECT_EQ(gen2->getValueAtSecond(0), 30);
    EXPECT_EQ(gen2->getPolyAtSample(0)[0], 30);
    EXPECT_EQ(gen2->getPolyAtSecond(0)[0], 30);

}
