/*! \file aw_WaveSquare.cpp
    \brief Wave Square Generator
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <math.h>


#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <math.h>

#include "aw_PanStereo.h"
#include "aw_Common.h"



// =============================================================================
// constructor
PanStereo :: PanStereo(SystemPtr o)
    : PolyGenerator :: PolyGenerator(o) // call base class constructor
{
    init();
}

// =============================================================================
//! Destroyer, virtual
PanStereo :: ~PanStereo()
{
}

// =============================================================================
//! Perform basic setup
void PanStereo :: init()
{
    // sources signal
    pValid_.push_back(aw::pNameOperand1);
    pTypeMap_[aw::pNameOperand1] = aw::gTypeMono;

    pValid_.push_back(aw::pNamePanLeftRight);
    pTypeMap_[aw::pNamePanLeftRight] = aw::gTypeMono;


    // setup some default generators 
    Generator::setParameter(aw::pNameOperand1, 0.0,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNamePanLeftRight, 0.5,
                    aw::pContextNameNone);


    // poly size is always 2 for stereo panner
    polySize_ = 2;
}

// =============================================================================
//! Reset all values used in value generation
void PanStereo :: reset()
{
}

// =============================================================================
//! Return a string name
std::string PanStereo :: getName()
{
    static const std::string name("PanStereo");
    return name;
}


// =============================================================================
// return a pointer to a working array
aw::WorkingArrayPtr PanStereo :: getPolyAtSample(aw::SampleTimeType st)
{

    // need to deal with values outside of range; probably limit
    // at unit interval?
    panPos_ = pMap_[aw::pNamePanLeftRight]->getValueAtSample(st);
    amp_ = pMap_[aw::pNameOperand1]->getValueAtSample(st);

    // always operate on two values
    workingArray_[0] = amp_ * std::sqrt((1 - panPos_)); // left value
    workingArray_[1] = amp_ * std::sqrt(panPos_);

    return workingArray_;
}