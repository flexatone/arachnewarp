/*! \file aw_WaveSquare.cpp
    \brief Wave Square Generator
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <math.h>


#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "aw_FilterLowPass.h"
#include "aw_Common.h"



// =============================================================================
// constructor
// :: defines a member of a class from outside the class definition itself
FilterLowPass :: FilterLowPass(SystemPtr o)
    : Generator :: Generator(o) // call base class constructor
{
    init();
}

// =============================================================================
//! Destroyer, virtual
FilterLowPass :: ~FilterLowPass()
{
}

// =============================================================================
//! Perform basic setup
void FilterLowPass :: init()
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
void FilterLowPass :: reset()
{
}

// =============================================================================
//! Return a string name
std::string FilterLowPass :: getName()
{
    static const std::string name("FilterLowPass");
    return name;
}


// =============================================================================
double FilterLowPass :: getValueAtSample(aw::SampleTimeType st)
{
    sr_ = sys_->getSamplingRate();

    value_ = pMap_[aw::pNameOperand1]->getValueAtSample(st);
    fqCutoff_ = pMap_[aw::pNameRate]->getValueAtSample(st);

    fqCutoff_ = getValueWithContextToFrequency(fqCutoff_, aw::pNameRate, sr_);

//     if (st % 4410 == 0) {
//         std::cout << "sample time: " << (aw::SampleTimeType) sampleTime << " fqCutoff_: " << fqCutoff_ << std::endl;
//         };

    fqCutoff_ *= 2.0 * aw::pi;

    w_ = 2.0 * sr_;
    norm_ = 1.0 / (fqCutoff_ + w_);
    b1_ = (w_ - fqCutoff_) * norm_;
    a1_ = a0_ = fqCutoff_ * norm_;

    value_ = value_*a0_ + zNeg1_*a1_ + zNeg1_*b1_;
    zNeg1_ = value_;

    return value_;
};    



// http://www.musicdsp.org/showone.php?id=117
// void SetLPF(float fCut, float fSampling)
// {
//     float w = 2.0 * fSampling;
//     float Norm;
// 
//     fCut *= 2.0F * PI;
//     Norm = 1.0 / (fCut + w);
//     b1 = (w - fCut) * Norm;
//     a0 = a1 = fCut * Norm;
// }
// 

// out[n] = in[n]*a0 + in[n-1]*a1 + out[n-1]*b1;
// 



// r  = rez amount, from sqrt(2) to ~ 0.1
// f  = cutoff frequency
// (from ~0 Hz to SampleRate/2 - though many
// synths seem to filter only  up to SampleRate/4)
// 
// The filter algo:
// out(n) = a1 * in + a2 * in(n-1) + a3 * in(n-2) - b1*out(n-1) - b2*out(n-2)
// 
// Lowpass:
//       c = 1.0 / tan(pi * f / sample_rate);
// 
//       a1 = 1.0 / ( 1.0 + r * c + c * c);
//       a2 = 2* a1;
//       a3 = a1;
//       b1 = 2.0 * ( 1.0 - c*c) * a1;
//       b2 = ( 1.0 - r * c + c * c) * a1;