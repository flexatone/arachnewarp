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
TEST(BasicTests, PolyConstantBasic) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System(44100, 8));
    GeneratorFactory gf(sys); // one instance


    GeneratorPtr gen0 = gf.create("polyconstant");
    EXPECT_EQ(gen0->getParameterString(), "PolyConstant{0}");



    PolyConstant gen1(sys); 
    // default is 1
    EXPECT_EQ(gen1.getPolySize(), 1);

    EXPECT_EQ(gen1.getName(), "PolyConstant");

    std::vector<double> inputVector1;
    inputVector1.push_back(4);
    inputVector1.push_back(5);

    gen1.setParameter(aw::pNameValue, inputVector1);

    // poly size is set to size of input
    EXPECT_EQ(gen1.getPolySize(), 2);


    std::vector<double> inputVector2;
    inputVector2.push_back(23);
    inputVector2.push_back(55);
    inputVector2.push_back(98);

    gen1.setParameter(aw::pNameValue, inputVector2);

    // poly size is set to size of input
    EXPECT_EQ(gen1.getPolySize(), 3);


    // can set with a string
    std::string inputString1("3, 4, 5, 20");
    gen1.setParameter(aw::pNameValue, inputString1);

    // poly size is set to size of input
    EXPECT_EQ(gen1.getPolySize(), 4);

    std::string inputString2("53,24,15,20,1230,60234");
    gen1.setParameter(aw::pNameValue, inputString2);
    // poly size is set to size of input
    EXPECT_EQ(gen1.getPolySize(), 6);


    // setting with a character array
    gen1.setParameter(aw::pNameValue, "6,5");
    // poly size is set to size of input
    EXPECT_EQ(gen1.getPolySize(), 2);


    gen1.setParameter(aw::pNameValue, "1, 2, 3,   5,   7, 8");
    // poly size is set to size of input
    EXPECT_EQ(gen1.getPolySize(), 6);


    // get the pointer to the working array
    // double value_[aw::maximumPolySize]
    aw::WorkingArrayPtr workingArray1 = gen1.getPolyAtSample(0);
    EXPECT_EQ(workingArray1[0], 1);
    EXPECT_EQ(workingArray1[1], 2);
    EXPECT_EQ(workingArray1[3], 5);

    // write to the working array to make sure that we get a new one
    workingArray1[3] = 1.25;    
    EXPECT_EQ(workingArray1[3], 1.25);

    // calling get poly at sample re-fills all values
    aw::WorkingArrayPtr workingArray2 = gen1.getPolyAtSample(0);
    EXPECT_EQ(workingArray2[3], 5);

    gen1.setParameter(aw::pNameValue, "6,5");
    // after setting new values, working array has not been updated    
    EXPECT_EQ(workingArray2[0], 1);
    aw::WorkingArrayPtr workingArray3 = gen1.getPolyAtSample(0);
    EXPECT_EQ(workingArray2[0], 6);
    EXPECT_EQ(workingArray3[0], 6);
    EXPECT_EQ(workingArray2[1], 5);
    EXPECT_EQ(workingArray3[1], 5);


}





// provide testCase, name
TEST(BasicTests, PolyConstantGetValue) {

    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance

    GeneratorPtr gen1 = gf.create(aw::gNamePolyConstant);
    GeneratorPtr gen2 = gf.create("polyconstant");

    gen1->setParameter("value", "8,4,2,12");
    gen2->setParameter("value", "30, 15, 7.5");

    int listSize1 = gen1->getPolySize();
    EXPECT_EQ(listSize1, 4);
    int listSize2 = gen2->getPolySize();
    EXPECT_EQ(listSize2, 3);

    aw::WorkingArrayPtr workingArray1 = gen1->getPolyAtSample(0);
    aw::WorkingArrayPtr workingArray2 = gen2->getPolyAtSample(0);

    EXPECT_EQ(workingArray1[0], 8);
    EXPECT_EQ(workingArray2[0], 30);



}


TEST(BasicTests, PolyConstantGetParameterString) {

    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance

    GeneratorPtr gen1 = gf.create(aw::gNamePolyConstant);
    GeneratorPtr gen2 = gf.create("polyconstant");

    gen1->setParameter("value", "8,4,2,12");
    gen2->setParameter("value", "30, 15, 7.5");

    EXPECT_EQ(gen1->getParameterString(), "PolyConstant{8,4,2,12}");
    EXPECT_EQ(gen1->getParameterString(false), "PolyConstant{8,4,2,12}");
    // if compact is true, name is masked
    EXPECT_EQ(gen2->getParameterString(true), "30,15,7.5");

}
