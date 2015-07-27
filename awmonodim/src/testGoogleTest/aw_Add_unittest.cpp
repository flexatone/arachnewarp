/*! \file aw_WaveSquare_unittest.cpp
    \brief Tests for aw_WaveSquare.cpp
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <stdexcept>

#include <boost/shared_ptr.hpp>


#include "aw_Multiply.h"
#include "aw_Add.h"
#include "aw_RandomUniform.h"
#include "aw_Constant.h"
#include <gtest/gtest.h>


using namespace aw;



// provide testCase, name
TEST(BasicTests, AddBasic) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System(44100, 8));

    Add gen1(sys); 

    gen1.setParameter(aw::pNameOperand1, 1.);
    gen1.setParameter(aw::pNameOperand2, 1.);
    gen1.setParameter(aw::pNameOperand3, 1.);
    gen1.setParameter(aw::pNameOperand4, 1.);
    EXPECT_EQ(gen1.getValueAtSecond(1), 4);

    gen1.setParameter(aw::pNameOperand1, 0.);
    gen1.setParameter(aw::pNameOperand2, 0.);
    gen1.setParameter(aw::pNameOperand3, 0.5);
    gen1.setParameter(aw::pNameOperand4, 0);
    EXPECT_EQ(gen1.getValueAtSecond(1), .5);


    gen1.setParameter(aw::pNameOperand1, 1.);
    gen1.setParameter(aw::pNameOperand2, 1.);
    gen1.setParameter(aw::pNameOperand3, 1.);
    gen1.setParameter(aw::pNameOperand4, .5);
    EXPECT_EQ(gen1.getValueAtSecond(1), 3.5);


    gen1.setParameter(aw::pNameOperand1, .25);
    gen1.setParameter(aw::pNameOperand2, 0);
    gen1.setParameter(aw::pNameOperand3, 0);
    gen1.setParameter(aw::pNameOperand4, 0);
    EXPECT_EQ(gen1.getValueAtSecond(1), .25);


    gen1.setParameter(aw::pNameOperand1, 0);
    gen1.setParameter(aw::pNameOperand2, 0);
    gen1.setParameter(aw::pNameOperand3, 0);
    gen1.setParameter(aw::pNameOperand4, 0);
    EXPECT_EQ(gen1.getValueAtSecond(3),  0);



//     ConstantPtr gen3(new Constant(sys));


//     RandomUniform gen2(sys); 

//     EXPECT_EQ(a.getValueAtSecond(5.00), 0);
// 
//     a.printSample(20, 0);

}


