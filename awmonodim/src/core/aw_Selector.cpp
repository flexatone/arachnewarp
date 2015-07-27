/*! \file aw_WaveSquare.cpp
    \brief Wave Square Generator
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <math.h>


#include <iostream>
#include <iomanip>
#include <stdexcept>

#include <boost/shared_ptr.hpp>


#include "aw_Selector.h"
#include "aw_Common.h"


using namespace aw;


namespace aw {


// =============================================================================
Selector :: Selector(aw::SystemPtr o)
    : aw::PolyGenerator::Generator(o) // call base class constructor
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
    // 0 (ordered cyclic forward), 1 (ordered cyclic reverse), 2 (ordered oscillate), 3 (random choice), 4 (random permutate), 5 (random walk)

    pValid_.push_back(aw::pNameSelectionMethod);   
    pTypeMap_[aw::pNameSelectionMethod] = aw::gTypeMono;

    pValid_.push_back(aw::pNameRefresh);
    pTypeMap_[aw::pNameRefresh] = aw::gTypeMono;

    pValid_.push_back(aw::pNameStride);
    pTypeMap_[aw::pNameStride] = aw::gTypeMono;

    // set some defaults
    // note: missing default for aw::pNameValueList

    // default selection method of 1
    Generator::setParameter(aw::pNameSelectionMethod, 0,
                    aw::pContextNameNone);

    Generator::setParameter(aw::pNameRefresh, 1,
                    aw::pContextNameTrigger);

    Generator::setParameter(aw::pNameStride, 1,
                           aw::pContextNameNone);


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
    oscillateSwitch_ = 1; 
    // used for random walk storage of random value
    walkSwitch_ = 1; 
    // setting this to zero will force updating on first call and a refresh of 
    // permutationIndices_
    srcSizeLast_ = 0;
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
    //std::cout << "=== Selector :: getValueAtSample:" << "about to get value form sample: " << st << std::endl;

    // sampleTime_ is defined as a double; this may be converting from the 
    // int sample time type
    sampleTime_ = st;
    sr_ = sys_->getSamplingRate();

    // get period from rate generator, then convert 
    periodSamples_ = pMap_[aw::pNameRefresh]->getValueAtSample(st);
    periodSamples_ = getValueWithContextToPeriodSamples(periodSamples_,
                     aw::pNameRefresh, sr_);

    // get the last selection method
    selectionMethod_ = pMap_[aw::pNameSelectionMethod]->getValueAtSample(st);
    // stride form another generator, w/ probabilistic rounding
    strideMagnitude_ = aw::doubleToIntProabilistic(
                       pMap_[aw::pNameStride]->getValueAtSample(st));


    // get current size; need to identify when size changes
    srcSize_ = pMap_[aw::pNameValueList]->getPolyDepth();

    //std::cout << "=== Selector :: getValueAtSample:" << "srcSize_ obtained from value lists polyDepth: " << srcSize_ << std::endl;


    if (srcSize_ != srcSizeLast_) {
        // only when size changes
        srcSizeLast_ = srcSize_;
        // initialize vector
        permutationIndices_.clear(); // remove any contents
        for (int i=0; i<srcSize_; i++) {
            // this is a vector, and will grow as needed
            permutationIndices_.push_back(i);
        }
        // setting to source size will force shuffling and init below
        permutationIndex_ = srcSize_;
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

        //std::cout << "sampleTimeLastRefresh " << sampleTimeLastRefresh_ << " sampleTime_ " << sampleTime_ << " periodSamples_ " << periodSamples_ << std::endl;


        // if 1, ordered cyclic reverse, increment before, so that when starting
        // at 0, first value is -1
        if (selectionMethod_ == 1) i_ -= strideMagnitude_;

        // if 3, random choice, get a random number within srcSize
        // strideMagnitude_ is not used here
        else if (selectionMethod_ == 3) {
            // range is inclusive of last value
            i_ = aw::randomIntRange(0, srcSize_-1);
        }

        // if 4, random permutation
        else if (selectionMethod_ == 4) {
            // range is inclusive of last value
            // need to store a list of index values; step through this list
            // and refresh after each complete usage

            // always shuffle when all indices have been used
            if (permutationIndex_ > srcSize_ - 1) {
                aw::shuffleVectorInt(permutationIndices_);
                permutationIndex_ = 0;
            }
            // set to value from permutation
            i_ = permutationIndices_[permutationIndex_];
            // increment for each usage
            permutationIndex_ += 1;

            //std::cout << "created  permutationIndices_" << std::endl;
            //aw::printVector(permutationIndices_);

        }

        // if 5, random walk, take a random step size and perform modulo
        else if (selectionMethod_ == 5) {
            walkSwitch_ = aw::randomUnit();
            // range is inclusive of last value
            //std::cout << "Selector: random walk " << walkSwitch_ << " pre i_ " << i_  << std::endl;
            if (walkSwitch_ > 0.5) {
                i_ -= aw::randomIntRange(1, strideMagnitude_);
            } else {
                i_ += aw::randomIntRange(1, strideMagnitude_);
            }
            i_ = aw::mod(i_, srcSize_);

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
            i_ += oscillateSwitch_;
            if (i_ >= srcSize_) {
                // flip direction; get negative of magnitude
                oscillateSwitch_ = -strideMagnitude_;
                // last index was top of array; next need second to top
                i_ = srcSize_ - 2;
            }
            else if (i_ < 0) { // if at zero or less
                // flip direction
                oscillateSwitch_ = strideMagnitude_;
                // last index was 0; next need 1
                i_ = 1;
            }
        }

    }

    return value_;
}




} // end namespace aw


        // std::clog << "pre aw::denormalize: " << "refreshed!" << std::endl;

