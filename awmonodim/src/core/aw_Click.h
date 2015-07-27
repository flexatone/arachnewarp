/*! \file aw_WaveSquare.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_CLICK_H
#define AW_CLICK_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "aw_Generator.h"
#include "aw_Common.h"




namespace aw {


    class Click; // forward declarator
    typedef boost::shared_ptr<Click> ClickPtr;
    
    
    //! Generator of a square wave
    class Click: public aw::Generator 
    {
    public:
    
        // constructor; args passed to base class
        Click(aw::SystemPtr sys); 
    
        virtual ~Click(); // TODO: probably does not need to be virtual
    
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
    
            
    } ; // end class Click


} // end namespace aw


#endif



