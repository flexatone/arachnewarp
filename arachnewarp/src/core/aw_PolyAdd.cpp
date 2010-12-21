/*! \file aw_WaveSquare.cpp
    \brief Wave Square Generator
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <math.h>


#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <math.h>

#include "aw_PolyAdd.h"
#include "aw_Common.h"



namespace aw {


// =============================================================================
// constructor
PolyAdd :: PolyAdd(aw::SystemPtr o)
    : aw::PolyGenerator::PolyGenerator(o) // call base class constructor
{
    init();
}

// =============================================================================
//! Destroyer, virtual
PolyAdd :: ~PolyAdd()
{
}

// =============================================================================
//! Perform basic setup
void PolyAdd :: init()
{
    // sources signal; using operand names 
    pValid_.push_back(aw::pNameOperand1);
    pTypeMap_[aw::pNameOperand1] = aw::gTypeAny; // either accepted

    pValid_.push_back(aw::pNameOperand2);
    pTypeMap_[aw::pNameOperand2] = aw::gTypeAny;

    pValid_.push_back(aw::pNameOperand3);
    pTypeMap_[aw::pNameOperand3] = aw::gTypeAny;

    pValid_.push_back(aw::pNameOperand4);
    pTypeMap_[aw::pNameOperand4] = aw::gTypeAny;


    std::vector<double> defaultValues; // for storage of string conversion
    defaultValues.push_back(0.0); 

    // setup some default generators 
    Generator::setParameter(aw::pNameOperand1, defaultValues,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameOperand2, defaultValues,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameOperand3, defaultValues,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameOperand4, defaultValues,
                    aw::pContextNameNone);

    defaultValues.clear();


}

// =============================================================================
//! Reset all values used in value generation
void PolyAdd :: reset()
{
}

// =============================================================================
//! Return a string name
std::string PolyAdd :: getName()
{
    static const std::string name("PolyAdd");
    return name;
}


// =============================================================================
// return a pointer to a working array
aw::WorkingArrayPtr PolyAdd :: getPolyAtSample(aw::SampleTimeType st)
{
    sampleTime_ = st;
    sr_ = sys_->getSamplingRate();

    srcSize1_ = pMap_[aw::pNameOperand1]->getPolyDepth();
    srcSize2_ = pMap_[aw::pNameOperand2]->getPolyDepth();
    srcSize3_ = pMap_[aw::pNameOperand3]->getPolyDepth();
    srcSize4_ = pMap_[aw::pNameOperand4]->getPolyDepth();

    // poly size is dynamic, based on largest operand?
    polyDepth_ = 0; // init
    if (srcSize1_ >= polyDepth_)
        polyDepth_ = srcSize1_;
    if (srcSize2_ >= polyDepth_)
        polyDepth_ = srcSize2_;
    if (srcSize3_ >= polyDepth_)
        polyDepth_ = srcSize3_;
    if (srcSize4_ >= polyDepth_)
        polyDepth_ = srcSize4_;

    // once poly depth is obtained, make sure we have enough size
    // this will only resize if necessary
    resizeWorkingArray(polyDepth_);


    // get a pointer to working array from each component
    workingArrayOperand1_ = pMap_[aw::pNameOperand1]->getPolyAtSample(st);
    workingArrayOperand2_ = pMap_[aw::pNameOperand2]->getPolyAtSample(st);
    workingArrayOperand3_ = pMap_[aw::pNameOperand3]->getPolyAtSample(st);
    workingArrayOperand4_ = pMap_[aw::pNameOperand4]->getPolyAtSample(st);


//     std::cout << "PolyAdd getPolyAtSample" << ": " << std::endl;
//     aw::printArray(workingArrayOperand1_, srcSize1_);
//     aw::printArray(workingArrayOperand2_, srcSize2_);
//     aw::printArray(workingArrayOperand3_, srcSize3_);
//     aw::printArray(workingArrayOperand4_, srcSize4_);

    for (int i=0; i<polyDepth_; i++) {
        sum_ = 0;
        if (i < srcSize1_) {
            sum_ += workingArrayOperand1_[i];
        }
        if (i < srcSize2_) {
            sum_ += workingArrayOperand2_[i];
        }
        if (i < srcSize3_) {
            sum_ += workingArrayOperand3_[i];
        }
        if (i < srcSize4_) {
            sum_ += workingArrayOperand4_[i];
        }
        // add up values in each; taking modulus of indices
        workingArray_[i] = sum_; 
    };

    return workingArray_;
}



} // end namespace aw
