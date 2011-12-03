
/*! \file aw_Generator_unittest.cpp
    \brief Tests for aw_Generator.cpp
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/

#define BOOST_TEST_MODULE

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/shared_ptr.hpp>


#include "aw_Common.h"
#include "aw_Generator.h"
#include "aw_GeneratorFactory.h"

// #include "aw_GeneratorFactory.h"
#include "aw_System.h"



using namespace aw;



// provide testCase, name
BOOST_AUTO_TEST_CASE(GeneratorBasic) {

    //System sys;
    //System *sys = new System(44100, 8); 
    SystemPtr sys(new System(44100, 8));

    Generator gen1(sys); 

    BOOST_CHECK_EQUAL(gen1.getName(), "Generator");

    BOOST_CHECK_EQUAL(gen1.getGeneratorType(), aw::gTypeMono);

    BOOST_CHECK_EQUAL(gen1.getSystem()->getSamplingRate(), 44100);

    BOOST_CHECK_EQUAL(sys.use_count(), 2);


    // try to pass the same object
    Generator gen2(sys); 
    BOOST_CHECK_EQUAL(gen2.getSystem()->getSamplingRate(), 44100);


    // we can find the use count b/c this is a smart pointer!
    BOOST_CHECK_EQUAL(sys.use_count(), 3);


    BOOST_CHECK_EQUAL(gen1.getFramesDefined(), 1);


}



BOOST_AUTO_TEST_CASE(GeneratorUtilityMethods) {

    SystemPtr sys(new System);
    GeneratorFactory gf(sys); // one instance

    GeneratorPtr gen1 = gf.create(aw::gNameWaveSine);
    gen1->setParameter(aw::pNameRate, 69, aw::pContextNamePitch);

    // testing an internal method
    BOOST_CHECK_EQUAL(gen1->getValueWithContextToFrequency(69, aw::pNameRate, 
        gen1->getSystem()->getSamplingRate()), 440.0);

    // set rate by bpm
    gen1->setParameter(aw::pNameRate, 120, aw::pContextNameBeatsPerMinute);
    BOOST_CHECK_EQUAL(gen1->getValueWithContextToPeriodSamples(120, aw::pNameRate, 
        gen1->getSystem()->getSamplingRate()), 22050);
    // half the value
    BOOST_CHECK_EQUAL(gen1->getValueWithContextToPeriodSamples(30, aw::pNameRate, 
        gen1->getSystem()->getSamplingRate()), 88200);

    BOOST_CHECK_EQUAL(gen1->getValueWithContextToFrequency(30, aw::pNameRate, 
        gen1->getSystem()->getSamplingRate()), 0.5);

    BOOST_CHECK_EQUAL(gen1->getValueWithContextToFrequency(120, aw::pNameRate, 
        gen1->getSystem()->getSamplingRate()), 2);


}



BOOST_AUTO_TEST_CASE(GeneratorSetParameterFunctions) {


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

    BOOST_CHECK_EQUAL(gen2->getValueAtSample(0), 10);
    gen2->setParameter("max", 20, "none");

    // check that values have been set
    BOOST_CHECK_EQUAL(gen2->getParameter("max")->getValueAtSample(0), 20);
    BOOST_CHECK_EQUAL(gen2->getContext("refresh"), aw::pContextNameBeatsPerMinute);


}




BOOST_AUTO_TEST_CASE(GeneratorReadParameterString) {


    SystemPtr sys(new System);
    GeneratorFactory gf(sys); // one instance

    GeneratorPtr gen1 = gf.create("wavesine");

    // presently need to remove spaces
    std::string args1(
        "min{Constant{-10}}max{constant{10}}rate{constant{30}}{frequency}");    
    //std::string args1("min{-10}max{10}rate{30}{frequency}");

    gen1->readParameterString(args1);

    BOOST_CHECK_EQUAL(gen1->getParameterString(), "WaveSine{rate{Constant{30}}{frequency}minimum{Constant{-10}}maximum{Constant{10}}}");


    GeneratorPtr gen2 = gf.create("constant{30}");
    BOOST_CHECK_EQUAL(gen2->getValueAtSample(0), 30);


    GeneratorPtr gen3 = gf.create("WaveSine{rate{30}{frequency}minimum{-20}maximum{20}}");
    BOOST_CHECK_EQUAL(gen3->getParameter("max")->getValueAtSample(0), 20);
    BOOST_CHECK_EQUAL(gen3->getParameter("min")->getValueAtSample(0), -20);
    BOOST_CHECK_EQUAL(gen3->getParameter("rate")->getValueAtSample(0), 30);

    BOOST_CHECK_EQUAL(gen3->getParameterString(), "WaveSine{rate{Constant{30}}{frequency}minimum{Constant{-20}}maximum{Constant{20}}}");

    // set compact argument
    BOOST_CHECK_EQUAL(gen3->getParameterString(true), "WaveSine{rate{30}{frequency}minimum{-20}maximum{20}}");


    GeneratorPtr gen4 = gf.create("WaveSine{rate{2000}{frequency}minimum{-1}maximum{1}}");

    BOOST_CHECK_EQUAL(gen4->getParameter("rate")->getValueAtSample(0), 2000);
    BOOST_CHECK_EQUAL(gen4->getParameter("min")->getValueAtSample(0), -1);
    BOOST_CHECK_EQUAL(gen4->getParameter("max")->getValueAtSample(0), 1);

}





BOOST_AUTO_TEST_CASE(GeneratorPolyGeneratorInterfaceEquivalance) {


    SystemPtr sys(new System);
    GeneratorFactory gf(sys); // one instance

    GeneratorPtr gen1 = gf.create("constant{3}");
    BOOST_CHECK_EQUAL(gen1->getValueAtSample(0), 3);
    BOOST_CHECK_EQUAL(gen1->getValueAtSecond(0), 3);
    // can get the same value by dereference pointer
    // even though this is a pointer to a single value, can use the 
    // the array interface to simulate an array-like structure
    BOOST_CHECK_EQUAL(gen1->getPolyAtSample(0)[0], 3);
    BOOST_CHECK_EQUAL(gen1->getPolyAtSecond(0)[0], 3);

    GeneratorPtr gen2 = gf.create("polyconstant{30,40}");
    // default fold down is to return the first value
    BOOST_CHECK_EQUAL(gen2->getValueAtSample(0), 30);
    BOOST_CHECK_EQUAL(gen2->getValueAtSecond(0), 30);
    BOOST_CHECK_EQUAL(gen2->getPolyAtSample(0)[0], 30);
    BOOST_CHECK_EQUAL(gen2->getPolyAtSecond(0)[0], 30);

}
