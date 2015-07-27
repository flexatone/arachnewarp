/*
 *  Sieve_unitttest.cpp
 *  arachnewarpTest
 *
 *  Created by Christopher Ariza on 7/1/10.
 *  Copyright 2010 Flexatone HFP. All rights reserved.
 *
 */

#define BOOST_TEST_MODULE

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/shared_ptr.hpp>

#include "aw_Sieve.h"


using namespace aw;


// provide testCase, name
BOOST_AUTO_TEST_CASE(SieveBasic) {

    Sieve a("5@3|6@3"); 

    //BOOST_CHECK(a.getValue(0.0) > 0);

    BOOST_CHECK_EQUAL(a.getString(), "5@3|6@3");


    Sieve b("3@1|8@2|13@4|6@5"); 

    BOOST_CHECK_EQUAL(b.getString(), "3@1|8@2|13@4|6@5");


}



