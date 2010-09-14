/*! \file aw_WaveSquare.cpp
    \brief Wave Square Generator
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <math.h>


#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "aw_FilterHighPass.h"
#include "aw_Common.h"



// =============================================================================
// constructor
// :: defines a member of a class from outside the class definition itself
FilterHighPass :: FilterHighPass(SystemPtr o)
    : Generator :: Generator(o) // call base class constructor
{
    init();
}

// =============================================================================
//! Destroyer, virtual
FilterHighPass :: ~FilterHighPass()
{
}

// =============================================================================
//! Perform basic setup
void FilterHighPass :: init()
{   

    pValid_.push_back(aw::pNameOperand1);
    pTypeMap_[aw::pNameOperand1] = aw::gTypeMono;

    pValid_.push_back(aw::pNameRate);
    pTypeMap_[aw::pNameRate] = aw::gTypeMono;


    // setup some default generators 
    Generator::setParameter(aw::pNameOperand1, 0,
                    aw::pContextNameNone);
    Generator::setParameter(aw::pNameRate, 220,
                    aw::pContextNameFrequency);

    reset();
}

// =============================================================================
//! Reset all values used in value generation
void FilterHighPass :: reset()
{
}

// =============================================================================
//! Return a string name
std::string FilterHighPass :: getName()
{
    static const std::string name("FilterHighPass");
    return name;
}


// =============================================================================
double FilterHighPass :: getValueAtSample(aw::SampleTimeType st)
{
    sr_ = sys_->getSamplingRate();

    value_ = pMap_[aw::pNameOperand1]->getValueAtSample(st);
    fqCutoff_ = pMap_[aw::pNameRate]->getValueAtSample(st);


    fqCutoff_ = getValueWithContextToFrequency(fqCutoff_, aw::pNameRate, sr_);
    fqCutoff_ *= 2.0 * aw::pi;

    w_ = 2.0 * sr_;
    norm_ = 1.0 / (fqCutoff_ + w_);
    a0_ = w_ * norm_;
    a1_ = -a0_;
    b1_ = (w_ - fqCutoff_) * norm_;

    value_ = value_*a0_ + zNeg1_*a1_ + zNeg1_*b1_;
    zNeg1_ = value_;

    return value_;
};    



// http://www.musicdsp.org/showone.php?id=117
// void SetHPF(float fCut, float fSampling)
// {
//     float w = 2.0 * fSampling;
//     float Norm;
// 
//     fCut *= 2.0F * PI;
//     Norm = 1.0 / (fCut + w);
//     a0 = w * Norm;
//     a1 = -a0;
//     b1 = (w - fCut) * Norm;
// }

// out[n] = in[n]*a0 + in[n-1]*a1 + out[n-1]*b1;
// 


