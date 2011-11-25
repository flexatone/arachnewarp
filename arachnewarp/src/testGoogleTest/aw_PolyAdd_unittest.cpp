/*! \file aw_WaveSquare_unittest.cpp
    \brief Tests for aw_WaveSquare.cpp
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <stdexcept>

#include <boost/shared_ptr.hpp>

#include "aw_PolyAdd.h"
#include "aw_GeneratorFactory.h"

#include <gtest/gtest.h>


using namespace aw;



// provide testCase, name
TEST(BasicTests, PolyAddBasic) {

    // set sampling rate to 1 for testing
    SystemPtr sys(new System(44100, 8));

    // create a direct instance
    PolyAdd gen1(sys); 
    //delete gen1;
}




// provide testCase, name
TEST(BasicTests, PolyAddBasicA) {

    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance

    GeneratorPtr gen0 = gf.create("polyadd");

    // get pointer to output array
    aw::WorkingArrayPtr out = gen0->getPolyAtSample(0);
    EXPECT_EQ(out[0], 0);
    EXPECT_EQ(out[1], 0);
    EXPECT_EQ(out[2], 0);
    EXPECT_EQ(out[3], 0);


    GeneratorPtr gen1 = gf.create("polyconstant{1,0,11}");
    EXPECT_EQ(gen1->getParameterString(), "PolyConstant{1,0,11}");

    GeneratorPtr gen2 = gf.create("polyconstant{2,0,20}");
    EXPECT_EQ(gen2->getParameterString(), "PolyConstant{2,0,20}");

    gen0->setParameter("op1", gen1);
    gen0->setParameter("op2", gen2);
    EXPECT_EQ(gen0->getParameterString(), "PolyAdd{operand1{PolyConstant{1,0,11}}operand2{PolyConstant{2,0,20}}operand3{PolyConstant{0}}operand4{PolyConstant{0}}}");

    out = gen0->getPolyAtSample(1);

    EXPECT_EQ(out[0], 3);
    EXPECT_EQ(out[1], 0);
    EXPECT_EQ(out[2], 31);
    EXPECT_EQ(out[3], 0); 

    out = gen0->getPolyAtSample(2);

    EXPECT_EQ(out[0], 3);
    EXPECT_EQ(out[1], 0);
    EXPECT_EQ(out[2], 31);
    EXPECT_EQ(out[3], 0); 


}



// provide testCase, name
TEST(BasicTests, PolyAddBasicB) {

    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance

    GeneratorPtr gen0 = gf.create("polyadd");

    // get pointer to output array
    aw::WorkingArrayPtr out = gen0->getPolyAtSample(0);
    // here, testing provided non-array values
    GeneratorPtr gen1 = gf.create("constant{3}");
    GeneratorPtr gen2 = gf.create("constant{5}");

    gen0->setParameter("op1", gen1);
    gen0->setParameter("op2", gen2);


    out = gen0->getPolyAtSample(1);

    EXPECT_EQ(out[0], 8);
    EXPECT_EQ(out[1], 0);
    EXPECT_EQ(out[2], 0);
    EXPECT_EQ(out[3], 0); 

//     out = gen0->getPolyAtSample(2);
// 
//     EXPECT_EQ(out[0], 3);
//     EXPECT_EQ(out[1], 0);
//     EXPECT_EQ(out[2], 31);
//     EXPECT_EQ(out[3], 0); 
// 

}