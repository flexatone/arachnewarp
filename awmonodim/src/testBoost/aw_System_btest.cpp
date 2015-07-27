/*! \file aw_Output_unittest.cpp
    \brief Object model of output stream
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/

#define BOOST_TEST_MODULE

#include <boost/shared_ptr.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "aw_System.h"


using namespace aw;


// provide testCase, name
BOOST_AUTO_TEST_CASE(SystemBasic) {

    System a(44100, 8); 

    BOOST_CHECK_EQUAL(a.getSamplingRate(), 44100.0);

    // test defaults
    System b; // no parens!
    BOOST_CHECK_EQUAL(a.getSamplingRate(), 44100.0);
    BOOST_CHECK_EQUAL(a.getBlockSize(), 8);

}



