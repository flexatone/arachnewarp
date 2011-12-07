/*
 *  ResidualClass_unittest.cpp
 *  arachnewarpTest
 *
 *  Created by Christopher Ariza on 6/30/10.
 *  Copyright 2010 Flexatone HFP. All rights reserved.
 *
 */

#define BOOST_TEST_MODULE

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>
#include <set>
#include <iostream>


#include "aw_ResidualClass.h"

using namespace aw;


// provide testCase, name
BOOST_AUTO_TEST_CASE(ResidualClassBasic) {

    // utility output iterator for vectors
    std::ostream_iterator<int>  output(std::cout, " ");
    
    // as a1Size is array size, must be a const
    const int a1Size = 11;
    int a1[a1Size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // initialize vector with an array and pointer to end size
    // this copies
    std::vector<int> z1(a1, a1+a1Size);

    // display output
    // std::cout << "source vector: ";
    // std::copy(z1.begin(), z1.end(), output);
    // std::cout << std::endl;

    // create a residual class
    ResidualClass a(4, 1); 

    // direct access to modulus and shift
    BOOST_CHECK_EQUAL(a.getModulus(), 4);
    BOOST_CHECK_EQUAL(a.getShift(), 1);

    // pass z1 by reference; this should process z1
    // returns the same vector
    std::vector<int> post = a.getSegment(0, z1);

    // display output
    // std::cout << "post vector: ";
    // std::copy(post.begin(), post.end(), output);
    // std::cout << std::endl;

    // check values
    BOOST_CHECK_EQUAL(post.at(0) == 1, true);
    BOOST_CHECK_EQUAL(post.at(1) == 5, true);
    BOOST_CHECK_EQUAL(post.at(2) == 9, true);

}

BOOST_AUTO_TEST_CASE(ResidualClassOperators) {

    ResidualClass a(4, 1); 
    ResidualClass b(5, 2); 
    ResidualClass c(5, 2); 

    BOOST_CHECK_EQUAL(a == b, false);
    BOOST_CHECK_EQUAL(a == c, false);
    BOOST_CHECK_EQUAL(b == c, true);


}


// provide testCase, name
BOOST_AUTO_TEST_CASE(ResidualClassString) {

    // as a1Size is array size, must be a const
    const int a1Size = 11;
    int a1[a1Size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // initialize vector with an array and pointer to end size
    // this copies
    std::vector<int> z1(a1, a1+a1Size);

    // create a residual class
    ResidualClass a; 
    // default constructor values
    BOOST_CHECK_EQUAL(a.getString(), "1@1");

    std::string arg1("4@1"); // create the argument
    a.setModulusAndShift(arg1);
    BOOST_CHECK_EQUAL(a.getString(), "4@1");


    std::string arg2("12@3"); // create the argument
    a.setModulusAndShift(arg2);
    BOOST_CHECK_EQUAL(a.getString(), "12@3");


}


