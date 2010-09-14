/*! \file aw_WaveSquare.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_WAVE_SINE_H
#define AW_WAVE_SINE_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "aw_Generator.h"


class WaveSine; // forward declarator

typedef boost::shared_ptr<WaveSine> WaveSinePtr;


//! Generator of a square wave
class WaveSine: public Generator 
{
public:

    // constructor; args passed to base class
    WaveSine(SystemPtr sys); 
    virtual ~WaveSine();

    virtual std::string getName();

    void init();

    void reset();

    virtual double getValueAtSample(aw::SampleTimeType sampleTime);


private:

    double value_;
    double sampleTime_;
    double periodSamples_;


//     double valueLast_;
//     double sampleTimeLast_;

//     double direction_;
//     double phaseShiftSamples_;

        
} ;



#endif



