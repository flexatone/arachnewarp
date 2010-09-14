/*
 *  ResidualClass_unittest.cpp
 *  arachnewarpTest
 *
 *  Created by Christopher Ariza on 6/30/10.
 *  Copyright 2010 Flexatone HFP. All rights reserved.
 *
 */



#include <gtest/gtest.h>
#include <vector>
#include <set>
#include <iostream>


#include "aw_ResidualClass.h"


// provide testCase, name
TEST(BasicTests, ResidualClassBasic) {

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
    EXPECT_EQ(a.getModulus(), 4);
    EXPECT_EQ(a.getShift(), 1);

    // pass z1 by reference; this should process z1
    // returns the same vector
    std::vector<int> post = a.segment(0, z1);

    // display output
    // std::cout << "post vector: ";
    // std::copy(post.begin(), post.end(), output);
    // std::cout << std::endl;

    // check values
    EXPECT_TRUE(post.at(0) == 1);
    EXPECT_TRUE(post.at(1) == 5);
    EXPECT_TRUE(post.at(2) == 9);

}

TEST(BasicTests, ResidualClassOperators) {

    ResidualClass a(4, 1); 
    ResidualClass b(5, 2); 
    ResidualClass c(5, 2); 

    EXPECT_EQ(a == b, false);
    EXPECT_EQ(a == c, false);
    EXPECT_EQ(b == c, true);


}


// provide testCase, name
TEST(BasicTests, ResidualClassString) {

    // as a1Size is array size, must be a const
    const int a1Size = 11;
    int a1[a1Size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // initialize vector with an array and pointer to end size
    // this copies
    std::vector<int> z1(a1, a1+a1Size);

    // create a residual class
    ResidualClass a; 
    // default constructor values
    EXPECT_EQ(a.represent(), "1@1");

    std::string arg1("4@1"); // create the argument
    a.setModulusAndShift(arg1);
    EXPECT_EQ(a.represent(), "4@1");


    std::string arg2("12@3"); // create the argument
    a.setModulusAndShift(arg2);
    EXPECT_EQ(a.represent(), "12@3");


}


