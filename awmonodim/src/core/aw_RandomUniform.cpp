/*! \file aw_RandomUniform.cpp
    \brief Random Uniform Generator.
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <time.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>

#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>


#include "aw_RandomUniform.h"
#include "aw_Common.h"
#include "aw_Constant.h"
#include "aw_System.h"




namespace aw {


// =============================================================================
// constructor
RandomUniform :: RandomUniform(aw::SystemPtr o)
        : aw::Generator::Generator(o), // call base class Constructor
        sampleTimeLastRefresh_(0),
        value_(0)
{      
    init();
}

// =============================================================================
//! Destroyer, virtual
RandomUniform :: ~RandomUniform()
{ // base class destructor will reset all references in localGenerators_
}

// =============================================================================
//! Perform basic setup
void RandomUniform :: init()
{   

    pValid_.push_back(aw::pNameRefresh);
    pTypeMap_[aw::pNameRefresh] = aw::gTypeMono;

    pValid_.push_back(aw::pNameMinimum);
    pTypeMap_[aw::pNameMinimum] = aw::gTypeMono;

    pValid_.push_back(aw::pNameMaximum);
    pTypeMap_[aw::pNameMaximum] = aw::gTypeMono;


    // setup some default generators 
    Generator::setParameter(aw::pNameMinimum, 0,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameMaximum, 1,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameRefresh, 1,
                    aw::pContextNameTrigger);

    // init should always call reset
    reset();
}

// =============================================================================
//! Reset all values used in value generation
void RandomUniform :: reset()
{
    value_ = aw::randomUnit();
    sampleTimeLastRefresh_ = 0;
}

// =============================================================================
//! Return a string name
std::string RandomUniform :: getName()
{
    static const std::string name("RandomUniform");
    return name;
}



// =============================================================================
// get a value for a time/sample point
double RandomUniform :: getValueAtSample(aw::SampleTimeType st)
{

    sampleTime_ = st;
    sr_ = sys_->getSamplingRate();

    periodSamples_ = pMap_[aw::pNameRefresh]->getValueAtSample(st);
    periodSamples_ = getValueWithContextToPeriodSamples(periodSamples_,
                     aw::pNameRefresh, sr_);

    // need to look for and deal with values over
    // % LONG_MAX

    // sampleTimeLastRefresh set to zero on init; if true then get new 
    // value
    if (sampleTime_ - sampleTimeLastRefresh_ >= periodSamples_) {
        sampleTimeLastRefresh_ = sampleTime_; // update to this time
        // get new value
        value_ = aw::randomUnit();
        // std::clog << "pre aw::denormalize: " << "refreshed!" << std::endl;
    }

    return aw::denormalize(value_, 
        pMap_[aw::pNameMinimum]->getValueAtSample(st),
        pMap_[aw::pNameMaximum]->getValueAtSample(st)
        );
}    


} // end namespace aw


