/*! \file aw_WaveSquare.cpp
    \brief Wave Square Generator
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/



#include "aw_WaveSine.h"
#include "aw_Common.h"

#include <cmath>
#include <stdexcept>
#include <boost/shared_ptr.hpp>



// =============================================================================
// constructor
// :: defines a member of a class from outside the class definition itself
WaveSine :: WaveSine(SystemPtr o)
    : Generator :: Generator(o) // call base class constructor
{
    init();
}

// =============================================================================
//! Destroyer, virtual
WaveSine :: ~WaveSine()
{
}

// =============================================================================
//! Perform basic setup
void WaveSine :: init()
{   

    pValid_.push_back(aw::pNameRate);
    pTypeMap_[aw::pNameRate] = aw::gTypeMono;

    pValid_.push_back(aw::pNameMinimum);
    pTypeMap_[aw::pNameMinimum] = aw::gTypeMono;

    pValid_.push_back(aw::pNameMaximum);
    pTypeMap_[aw::pNameMaximum] = aw::gTypeMono;

    // setup some default generators 
    Generator::setParameter(aw::pNameMinimum, 0,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameMaximum, 1,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameRate, .2,
                    aw::pContextNameFrequency);

    reset();
}

// =============================================================================
//! Reset all values used in value generation
void WaveSine :: reset()
{
}


// =============================================================================
//! Return a string name
std::string WaveSine :: getName()
{
    static const std::string name("WaveSine");
    return name;
}


// =============================================================================
// set frequency

// if our frequency is .5 hz, it means that our period is 2 seconds
// if our frequency is 2 hz, it means tt our period is .5 second
// if fq is 4 hz, period is .25 seconds
// p == 1 / fq
// fq == 1/ p




// =============================================================================
double WaveSine :: getValueAtSample(aw::SampleTimeType st)
{
    
    sampleTime_ = st;
    sr_ = sys_->getSamplingRate();

    periodSamples_ = pMap_[aw::pNameRate]->getValueAtSample(st);
    periodSamples_ = getValueWithContextToPeriodSamples(periodSamples_,
                     aw::pNameRate, sr_);

    // phase might be a parameter
    // aw::SampleTimeType samplePhase = 0; // between 0 and 1

    value_ = sin(sampleTime_ * aw::pi2 / periodSamples_);

    // alternative arrangement gives same results
    // value_ = sin((double) sampleTime_ / periodSamples_ * aw::pi2);

    // normalize and scale
    return aw::denormalize((1.0 + value_) * 0.5, 
        pMap_[aw::pNameMinimum]->getValueAtSample(st),
        pMap_[aw::pNameMaximum]->getValueAtSample(st)
        );
};    




//     if ((aw::SampleTimeType) st % 4410 == 0) {
//         std::cout << "sample time: " << (aw::SampleTimeType) sampleTime_ << " raw rate value: " << periodSamples_ << std::endl;
//         };

