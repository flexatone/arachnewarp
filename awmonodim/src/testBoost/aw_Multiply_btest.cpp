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


#include "aw_Multiply.h"
#include "aw_RandomUniform.h"
#include "aw_Constant.h"


using namespace aw;



// provide testCase, name
BOOST_AUTO_TEST_CASE(MultiplyBasic) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System(44100, 8));

    Multiply gen1(sys); 

    gen1.setParameter(aw::pNameOperand1, 2.);
    gen1.setParameter(aw::pNameOperand2, 1.);
    gen1.setParameter(aw::pNameOperand3, 1.);
    gen1.setParameter(aw::pNameOperand4, 1.);
    BOOST_CHECK_EQUAL(gen1.getValueAtSecond(1), 2);

    gen1.setParameter(aw::pNameOperand1, 2.);
    gen1.setParameter(aw::pNameOperand2, 2.);
    gen1.setParameter(aw::pNameOperand3, 1.);
    gen1.setParameter(aw::pNameOperand4, 1.);
    BOOST_CHECK_EQUAL(gen1.getValueAtSecond(1), 4);


    gen1.setParameter(aw::pNameOperand1, 1.);
    gen1.setParameter(aw::pNameOperand2, 1.);
    gen1.setParameter(aw::pNameOperand3, 1.);
    gen1.setParameter(aw::pNameOperand4, .5);
    BOOST_CHECK_EQUAL(gen1.getValueAtSecond(1), .5);


    gen1.setParameter(aw::pNameOperand1, 1.);
    gen1.setParameter(aw::pNameOperand2, .5);
    gen1.setParameter(aw::pNameOperand3, 1.);
    gen1.setParameter(aw::pNameOperand4, .5);
    BOOST_CHECK_EQUAL(gen1.getValueAtSecond(1), .25);


    gen1.setParameter(aw::pNameOperand1, 1.);
    gen1.setParameter(aw::pNameOperand2, 100);
    gen1.setParameter(aw::pNameOperand3, 1.);
    gen1.setParameter(aw::pNameOperand4, 1.);
    BOOST_CHECK_EQUAL(gen1.getValueAtSecond(3), 100);



//     ConstantPtr gen3(new Constant(sys));


//     RandomUniform gen2(sys); 

//     BOOST_CHECK_EQUAL(a.getValueAtSecond(5.00), 0);
// 
//     a.printSample(20, 0);

}


