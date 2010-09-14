/*! \file aw_WaveSquare.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_MULTIPLY_H
#define AW_MULTIPLY_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "aw_Generator.h"


class Multiply; // forward declarator
typedef boost::shared_ptr<Multiply> MultiplyPtr;


//! Generator of multiplied inputs
class Multiply: public Generator 
{
public:

    // constructor; args passed to base class
    Multiply(SystemPtr sys); 
    virtual ~Multiply();

    virtual std::string getName();

    void init();

    void reset();

    virtual double getValueAtSample(aw::SampleTimeType sampleTime);


private:

    double value1_;
    double value2_;
    double value3_;
    double value4_;
        
} ;



#endif



