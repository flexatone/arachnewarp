/*! \file aw_WaveSquare_unittest.cpp
    \brief Tests for aw_WaveSquare.cpp
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <stdexcept>

#include <boost/shared_ptr.hpp>

#include "aw_PolyConstant.h"
#include "aw_GeneratorFactory.h"

#include <gtest/gtest.h>


using namespace aw;



// provide testCase, name
TEST(BasicTests, PolyTable) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System(44100, 8));
    GeneratorFactory gf(sys); // one instance


}


