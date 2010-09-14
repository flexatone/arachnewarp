/*! \file aw_WaveSquare.cpp
    \brief Wave Square Generator
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <math.h>


#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "aw_Selector.h"
#include "aw_Common.h"


// TODO: consider adding a step-size parameter, or stride, that is used to 
// to set how far each step is taken for oscillation, cyclic, and random walk
// do not use sign of value, just take abs/magnitude

// =============================================================================
Selector :: Selector(SystemPtr o)
    : PolyGenerator :: Generator(o) // call base class constructor
{
    init();
}

// =============================================================================
Selector :: ~Selector()
{
}

// =============================================================================
void Selector :: init()
{
    // value list needs to be a poly generator
    pValid_.push_back(aw::pNameValueList);   
    pTypeMap_[aw::pNameValueList] = aw::gTypePoly;

    // selection method is a number, translated to a string
    // 1 (ordered cyclic forward), 2 (ordered cyclic reverse), 3 (ordered oscillate), 4 (random choice), 5 (random permutate), 6 (random walk)

    pValid_.push_back(aw::pNameSelectionMethod);   
    pTypeMap_[aw::pNameSelectionMethod] = aw::gTypeMono;

    pValid_.push_back(aw::pNameRefresh);
    pTypeMap_[aw::pNameRefresh] = aw::gTypeMono;

    // set some defaults
    // default selection method of 1
    Generator::setParameter(aw::pNameSelectionMethod, 1,
                    aw::pContextNameNone);

    Generator::setParameter(aw::pNameRefresh, 1,
                    aw::pContextNameTrigger);


    // init should always call reset
    reset();
}

// =============================================================================
void Selector :: reset()
{
    // reset index to zero
    i_ = 0; 
    value_ = 0;
    // set init value to negative one to force initial refresh at sample 0
    sampleTimeLastRefresh_ = 0;
    // used for ordered oscillate; flip between -1 and 1
    stride_ = 1; 
}

// =============================================================================
std::string Selector :: getName()
{
    static const std::string name("Selector");
    return name;
}



// =============================================================================
double Selector :: getValueAtSample(aw::SampleTimeType st)
{
    // update values in working vector; the values may have been changed
    // in later processing

    // sampleTime_ is defined as a double; this may be converting from the 
    // int sample time type
    sampleTime_ = st;
    sr_ = sys_->getSamplingRate();

    // get period from rate generator, then convert 
    periodSamples_ = pMap_[aw::pNameRefresh]->getValueAtSample(st);
    periodSamples_ = getValueWithContextToPeriodSamples(periodSamples_,
                     aw::pNameRate, sr_);

    // get the last selection method
    selectionMethod_ = pMap_[aw::pNameSelectionMethod]->getValueAtSample(st);
    // this might be controlled by another generator
    // TODO: get a double and return an int, w/ probabilistic rounding
    strideMagnitude_ = 1;


    // get current size; need to identify when size changes
    srcSize_ = pMap_[aw::pNameValueList]->getPolySize();
    if (srcSize_ != srcSizeLast_) {
        srcSizeLast_ = srcSize_;
    }

    // 0 (ordered cyclic forward), 1 (ordered cyclic reverse), 2 (ordered oscillate), 3 (random choice), 4 (random permutate), 5 (random walk)

    // get a pointer to working array
    workingArray_ = pMap_[aw::pNameValueList]->getPolyAtSample(st);



    // if source size is 1, nothing to do, always return the same value
    if (srcSize_ == 1) {
        value_ = workingArray_[0];
    }   
    // determine if it is time to update output value
    else if ((sampleTime_ - sampleTimeLastRefresh_ >= periodSamples_) ||
        sampleTime_ == 0) {

        sampleTimeLastRefresh_ = sampleTime_; // update to this time

        // if 1, ordered cyclic reverse, increment before, so that when starting
        // at 0, first value is -1
        if (selectionMethod_ == 1) i_ -= strideMagnitude_;

        // if 3, random choice, get a random number within srcSize
        // strideMagnitude_ is not used here
        else if (selectionMethod_ == 3) {
            // range is inclusive of last value
            i_ = aw::randomIntegerRange(0, srcSize_-1);
        }

        // set new value; return after iteration
        // using aw::mod of size keeps all values within appropriate index
        // negative values (-1, -2, etc), will count down through valid indices
        value_ = workingArray_[aw::mod(i_, srcSize_)];

        //std::cout << "current i: " << i_ << " i_ % srcSize_: " << i_ % srcSize_ << " resultant value: " << value_ << std::endl;

        // if 0, ordered cyclic, increment after
        if (selectionMethod_ == 0) i_ += 1;

        // if ordered oscillate
        else if (selectionMethod_ == 2) {
            // stride_ flips b/n -1, 1, modify i_ after usage
            i_ += stride_;
            if (i_ >= srcSize_) {
                // flip direction; get negative of magnitude
                stride_ = -strideMagnitude_;
                // last index was top of array; next need second to top
                i_ = srcSize_ - 2;
            }
            else if (i_ < 0) { // if at zero or less
                // flip direction
                stride_ = strideMagnitude_;
                // last index was 0; next need 1
                i_ = 1;
            }
        }

    }

    return value_;
}



        // std::clog << "pre aw::denormalize: " << "refreshed!" << std::endl;

