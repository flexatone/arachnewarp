/*! \file aw_Output_unittest.cpp
    \brief Object model of output stream
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/



#define BOOST_TEST_MODULE

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/shared_ptr.hpp>


#include "aw_System.h"
#include "aw_Common.h"
#include "aw_Output.h"
#include "aw_WaveSquare.h"
#include "aw_WaveSine.h"
#include "aw_RandomUniform.h"
#include "aw_Multiply.h"
#include "aw_Generator.h"
#include "aw_GeneratorFactory.h"
#include "aw_PolyAdd.h"


using namespace aw;


// provide testCase, name
BOOST_AUTO_TEST_CASE(GeneratorFactoryBasic) {


    SystemPtr sys(new System);
    GeneratorFactory gf(sys); 

//     WaveSquarePtr gen1(new WaveSquare(sys));
//     RandomUniformPtr gen2(new RandomUniform(sys));

    
    GeneratorPtr gen1 = gf.create(aw::gNameAdd);
    BOOST_CHECK_EQUAL(gen1->getName(), "Add");
    gen1->printSample(6, 0, 1);

    GeneratorPtr gen2 = gf.create(aw::gNameMultiply);
    BOOST_CHECK_EQUAL(gen2->getName(), "Multiply");
    gen2->printSample(6, 0, 1);

    BOOST_CHECK_EQUAL(gen2->getValueAtSecond(0), 1);
    gen2->setParameter(aw::pNameOperand1, 2);
    // this doe snot work
    BOOST_CHECK_EQUAL(gen2->getValueAtSecond(0), 2);


    GeneratorPtr gen3 = gf.create(aw::gNameConstant);
    BOOST_CHECK_EQUAL(gen3->getName(), "Constant");
    gen3->printSample(6, 0, 1);


    GeneratorPtr gen4 = gf.create(aw::gNameRandomUniform);
    BOOST_CHECK_EQUAL(gen4->getName(), "RandomUniform");
    gen4->printSample(6, 0, 1);

    GeneratorPtr gen5 = gf.create(aw::gNameWaveSine);
    BOOST_CHECK_EQUAL(gen5->getName(), "WaveSine");
    gen5->printSample(6, 0, 1);

    GeneratorPtr gen6 = gf.create(aw::gNameWaveSquare);
    BOOST_CHECK_EQUAL(gen6->getName(), "WaveSquare");
    gen6->printSample(6, 0, 1);



    GeneratorPtr gen7 = gf.create(aw::gNameConstant);
    BOOST_CHECK_EQUAL(gen7->getName(), "Constant");
    gen7->setParameter(aw::pNameValue, 33);
    BOOST_CHECK_EQUAL(gen7->getValueAtSecond(0), 33);


    GeneratorPtr gen8 = gf.create(aw::gNamePolyAdd);
    BOOST_CHECK_EQUAL(gen8->getName(), "PolyAdd");


}



// provide testCase, name
BOOST_AUTO_TEST_CASE(ParameterStrings) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System());
    GeneratorFactory gf(sys); // one instance


    GeneratorPtr gen1 = gf.create(aw::gNameMultiply);

    // this depends on defaults
    BOOST_CHECK_EQUAL(gen1->getParameterString(), "Multiply{operand1{Constant{1}}operand2{Constant{1}}operand3{Constant{1}}operand4{Constant{1}}}");


    std::string str1 = "randomuniform";
    GeneratorPtr gen2 = gf.create(str1);
    BOOST_CHECK_EQUAL(gen2->getParameterString(), "RandomUniform{refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}}");

    GeneratorPtr gen3 = gf.create(aw::gNameFilterLowPass);
    // this depends on defaults
    BOOST_CHECK_EQUAL(gen3->getParameterString(), "FilterLowPass{operand1{Constant{0}}rate{Constant{220}}{frequency}}");


    std::string str2 = "filterlowpass";
    GeneratorPtr gen4 = gf.create(str2);
    BOOST_CHECK_EQUAL(gen4->getParameterString(), "FilterLowPass{operand1{Constant{0}}rate{Constant{220}}{frequency}}");



}



// creating a generator given a char array
BOOST_AUTO_TEST_CASE(CreateFromCharArray) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System());
    GeneratorFactory gf(sys); // one instance


    GeneratorPtr gen1 = gf.create("multiply");

    // this depends on defaults
    BOOST_CHECK_EQUAL(gen1->getParameterString(), "Multiply{operand1{Constant{1}}operand2{Constant{1}}operand3{Constant{1}}operand4{Constant{1}}}");

}

