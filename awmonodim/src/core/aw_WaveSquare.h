/*! \file aw_WaveSquare.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_WAVE_SQUARE_H
#define AW_WAVE_SQUARE_H

#include <boost/shared_ptr.hpp>
#include <string>

#include "aw_Generator.h"
#include "aw_Common.h"



namespace aw {


    class WaveSquare; // forward declarator
    typedef boost::shared_ptr<WaveSquare> WaveSquarePtr;
    
    
    //! Generator of a square wave
    class WaveSquare: public aw::Generator 
    {
    public:
    
        // constructor; args passed to base class
        WaveSquare(aw::SystemPtr sys); 
        virtual ~WaveSquare();
    
        virtual std::string getName();
    
        void init();
    
        void reset();
    
        virtual double getValueAtSample(aw::SampleTimeType sampleTime);
    
    
    private:
    
        double sampleTime_;
        double periodSamples_;
        double amp_;
            
    }; // end class WaveSquare


} // end namespace aw


#endif



