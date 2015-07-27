/*! \file aw_RandomUniform_unittest.cpp
    \brief Tests for aw_RandomUniform.cpp
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/

#define BOOST_TEST_MODULE

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/shared_ptr.hpp>


#include "aw_RandomUniform.h"
#include "aw_Common.h"
#include "aw_Constant.h"
#include "aw_System.h"
#include "aw_GeneratorFactory.h"


using namespace aw;



// provide testCase, name
BOOST_AUTO_TEST_CASE(RandomUniformBasic) {

    // System sys;

    SystemPtr sys(new System);

    RandomUniform a(sys); 

    // BOOST_CHECK_EQUAL(a.getName(), "RandomUniform");


    // these names are simply numerical codes for reference
    BOOST_CHECK_EQUAL(aw::pNameMinimum,  0);
    BOOST_CHECK_EQUAL(aw::pNameMaximum,  1);


    //Constant *gen1 = new Constant(sys); // create a constant generator
    ConstantPtr gen1(new Constant(sys));
    gen1->setParameter(aw::pNameValue, 0.0);

    //Constant *gen2 = new Constant(sys); // create a constant generator
    ConstantPtr gen2(new Constant(sys));
    gen2->setParameter(aw::pNameValue, 2.0);
    BOOST_CHECK_EQUAL(gen2->getValueAtSecond(0.0), 2);

    // this will be a trigger, set to constant 1, which means
    // new values are always made
    ConstantPtr gen3(new Constant(sys));
    gen3->setParameter(aw::pNameValue, 1);


    // set constants to min and max
    a.setParameter(aw::pNameMinimum, gen1);
    a.setParameter(aw::pNameMaximum, gen2);
    a.setParameter(aw::pNameRefresh, gen3, aw::pContextNameTrigger);

    BOOST_CHECK_EQUAL(a.getParameter(aw::pNameMinimum)->getName(), "Constant");
    BOOST_CHECK_EQUAL(a.getParameter(aw::pNameMaximum)->getName(), "Constant");


    // test getting the value of an embedded generator
    BOOST_CHECK_EQUAL(a.getParameter(aw::pNameMaximum)->getValueAtSecond(0), 2);
    BOOST_CHECK_EQUAL(a.getParameter(aw::pNameRefresh)->getValueAtSecond(0), 1);

    // values are between 0 and 2
    BOOST_CHECK_EQUAL(a.getValueAtSample(0) >= 0.0, true);
    BOOST_CHECK_EQUAL(a.getValueAtSample(0) <= 2.0, true);

    a.reset();
    a.printSample(10, 0); // display 10 sample outputs, b/n 0 and 1
    a.reset();
    a.printSample(10, 0); // display 10 sample outputs, b/n 0 and 1

    // try to set new values for embedded parameters
    gen1->setParameter(aw::pNameValue, 100.0);
    gen2->setParameter(aw::pNameValue, 200.0);

    // changes are reflected in output
    a.reset();
    BOOST_CHECK_EQUAL(a.getValueAtSecond(0.0) >= 100.0, true);
    BOOST_CHECK_EQUAL(a.getValueAtSecond(0.0) <= 200.0, true);

    // values should not repeat!
    float b = a.getValueAtSample(0);
    float c = a.getValueAtSample(1);
    float d = a.getValueAtSample(2);

    BOOST_CHECK_EQUAL(b != c, true);
    BOOST_CHECK_EQUAL(c != d, true);


    // display 10 sample outputs, b/n 100 and 200, at a refresh rate of 2s
    // gen3 is already controlly parameterReferesh
    gen3->setParameter(aw::pNameValue, 4);
    // need reassign to change context
    a.setParameter(aw::pNameRefresh, gen3, aw::pContextNamePeriodSeconds);
    BOOST_CHECK_EQUAL(a.getParameter(aw::pNameRefresh)->getValueAtSecond(0), 4);
    BOOST_CHECK_EQUAL(a.getContext(aw::pNameRefresh), 
        aw::pContextNamePeriodSeconds);

    // display output incremented in seconds
    a.reset();
    a.printSample(10, 0, 1); 

    // try in increments of 1 second
    gen3->setParameter(aw::pNameValue, 2);
    a.reset();
    a.printSample(10, 0, 1); 


}



BOOST_AUTO_TEST_CASE(RandomUniformBlock) {

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



BOOST_AUTO_TEST_CASE(RandomUniformValidParameterContext) {
    // test getting the valid parameter context

    
    SystemPtr sys(new System(44100, 8));
    RandomUniform a(sys); 

    BOOST_CHECK(a.isValidContext(aw::pNameMinimum,
        aw::pContextNameNone) == true);
    BOOST_CHECK(a.isValidContext(aw::pNameMinimum,
        aw::pContextNameFrequency) == false);
    BOOST_CHECK(a.isValidContext(aw::pNameRefresh,
        aw::pContextNameFrequency) == true);
    BOOST_CHECK(a.isValidContext(aw::pNameRefresh,
        aw::pContextNamePeriodSeconds) == true);
    BOOST_CHECK(a.isValidContext(aw::pNameRefresh,
        aw::pContextNameTrigger) == true);
    BOOST_CHECK(a.isValidContext(aw::pNameRefresh,
        aw::pContextNameNone) == false);

    // if given none, an exception will be thrown
    ConstantPtr gen1(new Constant(sys));
    gen1->setParameter(aw::pNameValue, 5.0);

    BOOST_CHECK_THROW(a.setParameter(aw::pNameRefresh, gen1,
        aw::pContextNameNone), std::invalid_argument);

    // can assign if the proper context
    a.setParameter(aw::pNameRefresh, gen1, aw::pContextNamePeriodSeconds);
}





BOOST_AUTO_TEST_CASE(RandomUniformDefaults) {

    SystemPtr sys(new System(44100, 8));

    // defaults already configured
    RandomUniform gen1(sys); 

    BOOST_CHECK_EQUAL(gen1.getParameter(aw::pNameMinimum)->getValueAtSecond(0),
        0);
    BOOST_CHECK_EQUAL(gen1.getParameter(aw::pNameMaximum)->getValueAtSecond(0),
        1);

    BOOST_CHECK_EQUAL(gen1.getParameter(aw::pNameRefresh)->getValueAtSecond(0),
        1);

    BOOST_CHECK(gen1.getValueAtSample(0) >= 0.0);
    BOOST_CHECK(gen1.getValueAtSample(0) <= 1.0);

    gen1.setParameter(aw::pNameMaximum, 220.0);
    BOOST_CHECK_EQUAL(gen1.getParameter(aw::pNameMaximum)->getValueAtSecond(0),
        220);


    gen1.setParameter(aw::pNameMaximum, 5);
    BOOST_CHECK_EQUAL(gen1.getParameter(aw::pNameMaximum)->getValueAtSecond(0),
        5);



}



BOOST_AUTO_TEST_CASE(RandomUniformValidParameters) {

    SystemPtr sys(new System(44100, 8));
    RandomUniform gen1(sys); 

    BOOST_CHECK_EQUAL(gen1.isValidParameter(aw::pNameMaximum), true);
    BOOST_CHECK_EQUAL(gen1.isValidParameter(aw::pNameMinimum), true);
    BOOST_CHECK_EQUAL(gen1.isValidParameter(aw::pNameRefresh), true);
    BOOST_CHECK_EQUAL(gen1.isValidParameter(aw::pNameRate), false);

    // a bad argument for this generator will throw an exception
    BOOST_CHECK_THROW(gen1.setParameter(aw::pNameRate, .4,
        aw::pContextNamePeriodSamples), std::invalid_argument);
    
}



BOOST_AUTO_TEST_CASE(RandomUniformGeneratorFactory) {

    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance

    // 

    // with all default args realized
    std::string str1 = "randomuniform";
    GeneratorPtr gen1 = gf.create(str1);
    BOOST_CHECK_EQUAL(gen1->getParameterString(), "RandomUniform{refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}}");

    std::string str2("randomUniform{refresh{1}{samples}min{-.8}max{.8}}");
    GeneratorPtr gen2 = gf.create(str2);
    BOOST_CHECK_EQUAL(gen2->getParameterString(), "RandomUniform{refresh{Constant{1}}{samples}minimum{Constant{-0.8}}maximum{Constant{0.8}}}");

    //BOOST_CHECK_EQUAL(gen1.isValidParameter(aw::pNameMaximum), true);
    
}


