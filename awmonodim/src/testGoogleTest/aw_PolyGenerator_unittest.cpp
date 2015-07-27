/*! \file aw_WaveSquare_unittest.cpp
    \brief Tests for aw_WaveSquare.cpp
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <gtest/gtest.h>


#include "aw_PolyGenerator.h"
#include "aw_PolyConstant.h"
#include "aw_GeneratorFactory.h"



using namespace aw;



// provide testCase, name
TEST(BasicTests, PolyGeneratorBasic) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System(44100, 8));

    PolyGenerator gen1(sys); 

    EXPECT_EQ(gen1.getPolyDepth(), 1);

    // test generator type attribute
    EXPECT_EQ(gen1.getGeneratorType(), aw::gTypePoly);



}



// provide testCase, name
TEST(BasicTests, PolyGeneratorResizeWorkingArray) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System(44100, 8));
    GeneratorFactory gf(sys); // one instance

    //PolyGenerator gen1(sys); 
    GeneratorPtr gen0 = gf.create("polyconstant");

    std::vector<double> inputVector2;
    inputVector2.push_back(23);
    inputVector2.push_back(55);
    inputVector2.push_back(98);

    gen0->setParameter(aw::pNameValue, inputVector2);

    // poly size is set to size of input
    EXPECT_EQ(gen0->getPolyDepth(), 3);




    gen0->setParameter("value", "1, 2, 3,   5,   7, 8");
    // poly size is set to size of input
    EXPECT_EQ(gen0->getPolyDepth(), 6);


    gen0->clearWorkingArray();
    // this will not affect the constant as it stores its values elsewhere
    EXPECT_EQ(gen0->getValueAtSample(0), 1);


    // can directly resize working array
    gen0->resizeWorkingArray(200);
    // poly depth does not change automatically after resizing
    EXPECT_EQ(gen0->getPolyDepth(), 6);

    



}


