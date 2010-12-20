/*! \file aw_WaveSquare.cpp
    \brief Wave Square Generator
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <math.h>


#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "aw_Multiply.h"
#include "aw_Common.h"




namespace aw {


// =============================================================================
// constructor
// :: defines a member of a class from outside the class definition itself
Multiply :: Multiply(aw::SystemPtr o)
    : aw::Generator::Generator(o) // call base class constructor
{
    init();
}

// =============================================================================
//! Destroyer, virtual
Multiply :: ~Multiply()
{
}

// =============================================================================
//! Perform basic setup
void Multiply :: init()
{   

    pValid_.push_back(aw::pNameOperand1);
    pTypeMap_[aw::pNameOperand1] = aw::gTypeMono;

    pValid_.push_back(aw::pNameOperand2);
    pTypeMap_[aw::pNameOperand2] = aw::gTypeMono;

    pValid_.push_back(aw::pNameOperand3);
    pTypeMap_[aw::pNameOperand3] = aw::gTypeMono;

    pValid_.push_back(aw::pNameOperand4);
    pTypeMap_[aw::pNameOperand4] = aw::gTypeMono;

    // setup some default generators 
    Generator::setParameter(aw::pNameOperand1, 1,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameOperand2, 1,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameOperand3, 1,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameOperand4, 1,
                    aw::pContextNameNone);

    reset();
}

// =============================================================================
//! Reset all values used in value generation
void Multiply :: reset()
{
}

// =============================================================================
//! Return a string name
std::string Multiply :: getName()
{
    static const std::string name("Multiply");
    return name;
}


// =============================================================================
double Multiply :: getValueAtSample(aw::SampleTimeType st)
{
    sr_ = sys_->getSamplingRate();

    value1_ = pMap_[aw::pNameOperand1]->getValueAtSample(st);
    value2_ = pMap_[aw::pNameOperand2]->getValueAtSample(st);
    value3_ = pMap_[aw::pNameOperand3]->getValueAtSample(st);
    value4_ = pMap_[aw::pNameOperand4]->getValueAtSample(st);

    return value1_ * value2_ * value3_ * value4_;
};    


} // end namespace aw
