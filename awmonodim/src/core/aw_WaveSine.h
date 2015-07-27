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
#include "aw_Common.h"



namespace aw {


    class WaveSine; // forward declarator
    typedef boost::shared_ptr<WaveSine> WaveSinePtr;
    
    
    //! Generator of a sine wave, not based on a wave table. Parameters are rate, min, and max.
    class WaveSine: public aw::Generator 
    {
    public:
    
        // constructor; args passed to base class
        WaveSine(aw::SystemPtr sys); 
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
    
            
    } ; // end class WaveSine


} // end namespace aw


#endif



