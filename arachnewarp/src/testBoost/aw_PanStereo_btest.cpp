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


#include "aw_PanStereo.h"
#include "aw_GeneratorFactory.h"



using namespace aw;



// provide testCase, name
BOOST_AUTO_TEST_CASE(PanStereoBasic) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System(44100, 8));

    PanStereo gen1(sys); 

}




// provide testCase, name
BOOST_AUTO_TEST_CASE(PanStereoGetValue) {

    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance

    GeneratorPtr gen0 = gf.create("panstereo");
    GeneratorPtr gen1 = gf.create("constant{1}");
    GeneratorPtr gen2 = gf.create("constant{.5}");

    gen0->setParameter(aw::pNameOperand1, gen1);
    gen0->setParameter("operand1", gen1);
    gen0->setParameter("panleftright", gen2);

    // poly size is always 2
    BOOST_CHECK_EQUAL(gen0->getPolyDepth(), 2);

    // get pointer to output array
    aw::WorkingArrayPtr out = gen0->getPolyAtSample(0);

    // at pan .5, we get this (not .5 b/c using sqrt panning)
    BOOST_CHECK_EQUAL(out[0], 0.70710678118654757);
    BOOST_CHECK_EQUAL(out[1], 0.70710678118654757);

    BOOST_CHECK_EQUAL(gen0->getParameterString(), "PanStereo{operand1{Constant{1}}panLeftRight{Constant{0.5}}}");

    gen2->setParameter("value", 0.0);
    out = gen0->getPolyAtSample(0);
    // at pan 0, we get all signal in left
    BOOST_CHECK_EQUAL(out[0], 1);
    BOOST_CHECK_EQUAL(out[1], 0);

    BOOST_CHECK_EQUAL(gen0->getParameterString(), "PanStereo{operand1{Constant{1}}panLeftRight{Constant{0}}}");

    gen2->setParameter("value", 1.0);
    out = gen0->getPolyAtSample(0);
    // at pan 1, we get all signal in right
    BOOST_CHECK_EQUAL(out[0], 0);
    BOOST_CHECK_EQUAL(out[1], 1);


    BOOST_CHECK_EQUAL(gen0->getParameterString(), "PanStereo{operand1{Constant{1}}panLeftRight{Constant{1}}}");

}


BOOST_AUTO_TEST_CASE(PanStereoGetParameterString) {

    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance

//     GeneratorPtr gen1 = gf.create(aw::gNamePanStereo);
//     GeneratorPtr gen2 = gf.create("polyconstant");
// 
//     gen1->setParameter("value", "8,4,2,12");
//     gen2->setParameter("value", "30, 15, 7.5");
// 
//     BOOST_CHECK_EQUAL(gen1->getParameterString(), "PanStereo{8,4,2,12}");
//     BOOST_CHECK_EQUAL(gen1->getParameterString(false), "PanStereo{8,4,2,12}");
//     // if compact is true, name is masked
//     BOOST_CHECK_EQUAL(gen2->getParameterString(true), "30,15,7.5");

}
