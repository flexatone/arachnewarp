/*
 *  Sieve_unitttest.cpp
 *  arachnewarpTest
 *
 *  Created by Christopher Ariza on 7/1/10.
 *  Copyright 2010 Flexatone HFP. All rights reserved.
 *
 */

#include "aw_Sieve.h"
#include <gtest/gtest.h>


// provide testCase, name
TEST(BasicTests, SieveBasic) {

    Sieve a("5@3|6@3"); 

    //EXPECT_TRUE(a.getValue(0.0) > 0);

    EXPECT_EQ(a.represent(), "5@3|6@3");


    Sieve b("3@1|8@2|13@4|6@5"); 

    EXPECT_EQ(b.represent(), "3@1|8@2|13@4|6@5");


}



