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

#include "aw_GeneratorFactory.h"
#include "aw_FilterLowPass.h"
#include "aw_RandomUniform.h"
#include "aw_Constant.h"


using namespace aw;



// provide testCase, name
BOOST_AUTO_TEST_CASE(FilterLowPassBasic) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System());
    GeneratorFactory gf(sys); // one instance

    FilterLowPass gen1(sys); 
    gen1.setParameter(aw::pNameRate, 200, aw::pContextNameFrequency);


    // use generator factor
    GeneratorPtr gen2 = gf.create(aw::gNameFilterLowPass);
    gen2->setParameter(aw::pNameRate, 20, aw::pContextNameFrequency);




}

