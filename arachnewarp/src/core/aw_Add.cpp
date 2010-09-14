/*! \file aw_WaveSquare.cpp
    \brief Wave Square Generator
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <math.h>

#include <stdexcept>
#include <string>

#include "aw_Add.h"
#include "aw_Common.h"



// =============================================================================
// constructor
Add :: Add(SystemPtr o)
    : Generator :: Generator(o) // call base class constructor
{
    init();
}

// =============================================================================
//! Destroyer, virtual
Add :: ~Add()
{
}

// =============================================================================
//! Perform basic setup
void Add :: init()
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
    Generator::setParameter(aw::pNameOperand1, 0,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameOperand2, 0,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameOperand3, 0,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameOperand4, 0,
                    aw::pContextNameNone);

    reset();
}

// =============================================================================
//! Reset all values used in value generation
void Add :: reset()
{
}

// =============================================================================
//! Return a string name
std::string Add :: getName()
{
    static const std::string name("Add");
    return name;
}


// =============================================================================
double Add :: getValueAtSample(aw::SampleTimeType st)
{
    sr_ = sys_->getSamplingRate();

    value1_ = pMap_[aw::pNameOperand1]->getValueAtSample(st);
    value2_ = pMap_[aw::pNameOperand2]->getValueAtSample(st);
    value3_ = pMap_[aw::pNameOperand3]->getValueAtSample(st);
    value4_ = pMap_[aw::pNameOperand4]->getValueAtSample(st);

    return value1_ + value2_ + value3_ + value4_;

};    


