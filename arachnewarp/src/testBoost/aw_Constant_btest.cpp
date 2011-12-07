/*! \file aw_Constant_unittest.cpp
    \brief Tests for Constant.
    
Copyright 2010 Flexatone HFP. All rights reserved.
*/

#define BOOST_TEST_MODULE

#include "aw_Constant.h"
#include "aw_System.h"
#include "aw_GeneratorFactory.h"

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/shared_ptr.hpp>



using namespace aw;


// provide testCase, name
BOOST_AUTO_TEST_CASE(ConstantBasic) {


    SystemPtr sys(new System(44100, 8));

    Constant a(sys); 

    BOOST_CHECK_EQUAL(a.getName(), "Constant");


    // set parameter by parameter name value
    a.setParameter(aw::pNameValue, 2.5);

    BOOST_CHECK_EQUAL(a.getValueAtSecond(0.0), 2.5);
    BOOST_CHECK_EQUAL(a.getValueAtSecond(0.5), 2.5);
    BOOST_CHECK_EQUAL(a.getValueAtSample(0.55), 2.5);
 

    // set a different value
    a.setParameter(aw::pNameValue, 300);
    BOOST_CHECK_EQUAL(a.getValueAtSecond(0.0), 300);


}


// provide testCase, name
BOOST_AUTO_TEST_CASE(ConstantViaReference) {

    SystemPtr sys(new System(44100, 8));

    Constant *gen1 = new Constant(sys); // create a constant generator
    gen1->setParameter(aw::pNameValue, 300);


    BOOST_CHECK_EQUAL(gen1->getValueAtSecond(0.0), 300);


}



BOOST_AUTO_TEST_CASE(ConstantValidParameters) {

    SystemPtr sys(new System(44100, 8));
    Constant gen1(sys); 

    BOOST_CHECK_EQUAL(gen1.isValidParameter(aw::pNameMaximum), false);
    BOOST_CHECK_EQUAL(gen1.isValidParameter(aw::pNameMinimum), false);
    BOOST_CHECK_EQUAL(gen1.isValidParameter(aw::pNameRefresh), false);
    BOOST_CHECK_EQUAL(gen1.isValidParameter(aw::pNameRate), false);

    BOOST_CHECK_EQUAL(gen1.isValidParameter(aw::pNameValue), true);

    // a bad argument for this generator will throw an exception
    BOOST_CHECK_THROW(gen1.setParameter(aw::pNameRate, .4,
        aw::pContextNamePeriodSamples), std::invalid_argument);
    
}



BOOST_AUTO_TEST_CASE(ConstantParameterString) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System());
    GeneratorFactory gf(sys); // one instance


    //GeneratorPtr gen1 = gf.create(aw::gNameConstant);
    Constant *gen1 = new Constant(sys); // create a constant generator

    // this depends on defaults
    BOOST_CHECK_EQUAL(gen1->getParameterString(), "Constant{0}");

    gen1->setParameter(aw::pNameValue, 220);
    BOOST_CHECK_EQUAL(gen1->getValueAtSecond(0.0), 220);

    BOOST_CHECK_EQUAL(gen1->getParameterString(), "Constant{220}");

    // try doing the same with GeneratorFactory
    GeneratorPtr gen2 = gf.create(aw::gNameConstant);

    // this depends on defaults
    BOOST_CHECK_EQUAL(gen2->getParameterString(), "Constant{0}");
    gen2->setParameter(aw::pNameValue, 220);
    BOOST_CHECK_EQUAL(gen2->getValueAtSecond(0.0), 220);
    BOOST_CHECK_EQUAL(gen2->getParameterString(), "Constant{220}");

}

