/*! \file aw_WaveSquare.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_FILTER_LOW_PASS_H
#define AW_FILTER_LOW_PASS_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "aw_Generator.h"


class FilterLowPass; // forward declarator
typedef boost::shared_ptr<FilterLowPass> FilterLowPassPtr;


//! Generator of multiplied inputs
class FilterLowPass: public Generator 
{
public:

    // constructor; args passed to base class
    FilterLowPass(SystemPtr sys); 
    virtual ~FilterLowPass();

    virtual std::string getName();

    void init();

    void reset();

    virtual double getValueAtSample(aw::SampleTimeType sampleTime);


private:

    double fqCutoff_;
    double w_;
    double norm_;
    double zNeg1_;
    double value_;
    double a0_;
    double a1_;
    double b1_;
        
} ;



#endif



