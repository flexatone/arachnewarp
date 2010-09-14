/*! \file aw_Output_unittest.cpp
    \brief Object model of output stream
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/



#include "aw_System.h"
#include "aw_Common.h"

#include <boost/shared_ptr.hpp>

#include <gtest/gtest.h>



// provide testCase, name
TEST(BasicTests, SystemBasic) {

    System a(44100, 8); 

    EXPECT_EQ(a.getSamplingRate(), 44100.0);

    // test defaults
    System b; // no parens!
    EXPECT_EQ(a.getSamplingRate(), 44100.0);
    EXPECT_EQ(a.getBlockSize(), 8);


}



