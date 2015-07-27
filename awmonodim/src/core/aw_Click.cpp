/*! \file aw_WaveSquare.cpp
    \brief Wave Square Generator
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <cmath>
#include <stdexcept>
#include <boost/shared_ptr.hpp>

#include "aw_Click.h"
#include "aw_Common.h"




namespace aw {


// =============================================================================
// constructor
// :: defines a member of a class from outside the class definition itself
Click :: Click(SystemPtr o)
    : aw::Generator::Generator(o) // call base class constructor
{
    init();
}

// =============================================================================
//! Destroyer, virtual
Click :: ~Click()
{
}

// =============================================================================
//! Perform basic setup
void Click :: init()
{   

    pValid_.push_back(aw::pNameRate);
    pTypeMap_[aw::pNameRate] = aw::gTypeMono;

    pValid_.push_back(aw::pNameAmplitude);
    pTypeMap_[aw::pNameAmplitude] = aw::gTypeMono;

    // setup some default generators 
    Generator::setParameter(aw::pNameRate, 1,
                    aw::pContextNamePeriodSeconds);
    Generator::setParameter(aw::pNameAmplitude, 1,
                    aw::pContextNameNone);

    reset();
}

// =============================================================================
//! Reset all values used in value generation
void Click :: reset()
{
}


// =============================================================================
//! Return a string name
std::string Click :: getName()
{
    static const std::string name("Click");
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
double Click :: getValueAtSample(aw::SampleTimeType st)
{
    
    sampleTime_ = st;
    sr_ = sys_->getSamplingRate();

    periodSamples_ = pMap_[aw::pNameRate]->getValueAtSample(sampleTime_);
    periodSamples_ = getValueWithContextToPeriodSamples(periodSamples_,
                     aw::pNameRate, sr_);

    // take mod of period
    sampleTime_ = fmod(sampleTime_, periodSamples_);
    
    // only update amplitude, and provide output, at sample time zero, 
    // or the beginning of each period
    if (sampleTime_ < 1.0) {
        // need to pass original sample time here!
        value_ = pMap_[aw::pNameAmplitude]->getValueAtSample(st);
    }
    else
        value_ = 0;

    // normalize and scale
    return value_;
};    


} // end namespace aw

