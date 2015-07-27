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

#include "aw_PolyAdd.h"
#include "aw_GeneratorFactory.h"


using namespace aw;



// provide testCase, name
BOOST_AUTO_TEST_CASE(PolyAddBasic) {

    // set sampling rate to 1 for testing
    SystemPtr sys(new System(44100, 8));

    // create a direct instance
    PolyAdd gen1(sys); 
    //delete gen1;
}




// provide testCase, name
BOOST_AUTO_TEST_CASE(PolyAddBasicA) {

    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance

    GeneratorPtr gen0 = gf.create("polyadd");

    // get pointer to output array
    aw::WorkingArrayPtr out = gen0->getPolyAtSample(0);
    BOOST_CHECK_EQUAL(out[0], 0);
    BOOST_CHECK_EQUAL(out[1], 0);
    BOOST_CHECK_EQUAL(out[2], 0);
    BOOST_CHECK_EQUAL(out[3], 0);


    GeneratorPtr gen1 = gf.create("polyconstant{1,0,11}");
    BOOST_CHECK_EQUAL(gen1->getParameterString(), "PolyConstant{1,0,11}");

    GeneratorPtr gen2 = gf.create("polyconstant{2,0,20}");
    BOOST_CHECK_EQUAL(gen2->getParameterString(), "PolyConstant{2,0,20}");

    gen0->setParameter("op1", gen1);
    gen0->setParameter("op2", gen2);
    BOOST_CHECK_EQUAL(gen0->getParameterString(), "PolyAdd{operand1{PolyConstant{1,0,11}}operand2{PolyConstant{2,0,20}}operand3{PolyConstant{0}}operand4{PolyConstant{0}}}");

    out = gen0->getPolyAtSample(1);

    BOOST_CHECK_EQUAL(out[0], 3);
    BOOST_CHECK_EQUAL(out[1], 0);
    BOOST_CHECK_EQUAL(out[2], 31);
    BOOST_CHECK_EQUAL(out[3], 0); 

    out = gen0->getPolyAtSample(2);

    BOOST_CHECK_EQUAL(out[0], 3);
    BOOST_CHECK_EQUAL(out[1], 0);
    BOOST_CHECK_EQUAL(out[2], 31);
    BOOST_CHECK_EQUAL(out[3], 0); 


}



// provide testCase, name
BOOST_AUTO_TEST_CASE(PolyAddBasicB) {

    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance

    GeneratorPtr gen0 = gf.create("polyadd");

    // get pointer to output array
    aw::WorkingArrayPtr out = gen0->getPolyAtSample(0);
    // here, testing provided non-array values
    GeneratorPtr gen1 = gf.create("constant{3}");
    GeneratorPtr gen2 = gf.create("constant{5}");

    gen0->setParameter("op1", gen1);
    gen0->setParameter("op2", gen2);


    out = gen0->getPolyAtSample(1);

    BOOST_CHECK_EQUAL(out[0], 8);
    BOOST_CHECK_EQUAL(out[1], 0);
    BOOST_CHECK_EQUAL(out[2], 0);
    BOOST_CHECK_EQUAL(out[3], 0); 

//     out = gen0->getPolyAtSample(2);
// 
//     BOOST_CHECK_EQUAL(out[0], 3);
//     BOOST_CHECK_EQUAL(out[1], 0);
//     BOOST_CHECK_EQUAL(out[2], 31);
//     BOOST_CHECK_EQUAL(out[3], 0); 
// 

}