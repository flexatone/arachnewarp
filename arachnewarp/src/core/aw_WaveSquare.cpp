/*! \file aw_WaveSquare.cpp
    \brief Wave Square Generator
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include "aw_WaveSquare.h"
#include "aw_Common.h"

#include <math.h>
#include <string>
#include <stdexcept>

#include <boost/shared_ptr.hpp>




namespace aw {


// =============================================================================
// constructor
// :: defines a member of a class from outside the class definition itself
WaveSquare :: WaveSquare(aw::SystemPtr o)
    : aw::Generator::Generator(o) // call base class constructor
{
    init();
}

// =============================================================================
//! Destroyer, virtual
WaveSquare :: ~WaveSquare()
{
}

// =============================================================================
//! Perform basic setup
void WaveSquare :: init()
{   

    pValid_.push_back(aw::pNameRate);
    pTypeMap_[aw::pNameRate] = aw::gTypeMono;

    pValid_.push_back(aw::pNameDutyCycle);
    pTypeMap_[aw::pNameDutyCycle] = aw::gTypeMono;

    pValid_.push_back(aw::pNameMinimum);
    pTypeMap_[aw::pNameMinimum] = aw::gTypeMono;

    pValid_.push_back(aw::pNameMaximum);
    pTypeMap_[aw::pNameMaximum] = aw::gTypeMono;

    // setup some default generators 
    Generator::setParameter(aw::pNameMinimum, 0,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameMaximum, 1,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameRate, 6,
                    aw::pContextNamePeriodSeconds);
    Generator::setParameter(aw::pNameDutyCycle, .3);

    reset();
}

// =============================================================================
//! Reset all values used in value generation
void WaveSquare :: reset()
{
}


// =============================================================================
//! Return a string name
std::string WaveSquare :: getName()
{
    static const std::string name("WaveSquare");
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
double WaveSquare :: getValueAtSample(aw::SampleTimeType st)
{
    sampleTime_ = st;
    sr_ = sys_->getSamplingRate();

    periodSamples_ = pMap_[aw::pNameRate]->getValueAtSample(st);
    periodSamples_ = getValueWithContextToPeriodSamples(periodSamples_,
                     aw::pNameRate, sr_);

    // phase might be a parameter
    double phase = 0; // between 0 and 1

    // shift the desired time by the phase-scaled period
    if (phase != 0) {
        sampleTime_ += periodSamples_ * phase;
    };

    // find time position within the period 
    // if our period is 2, and our initial secondTime is 2.5, this
    // returns .5 
    sampleTime_ = fmod(sampleTime_, periodSamples_);

    if (sampleTime_ < periodSamples_ * 
            pMap_[aw::pNameDutyCycle]->getValueAtSample(st))
        amp_ = 0;
    else
        amp_ = 1;

    return aw::denormalize(amp_, 
        pMap_[aw::pNameMinimum]->getValueAtSample(st),
        pMap_[aw::pNameMaximum]->getValueAtSample(st)
        );
};    




} // end namespace aw

