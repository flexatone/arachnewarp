/*! \file aw_RandomUniform.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/



#ifndef AW_RANDOM_UNIFORM_H    
#define AW_RANDOM_UNIFORM_H

#include <boost/random.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "aw_System.h"
#include "aw_Generator.h"
#include "aw_Common.h"




namespace aw {


    class RandomUniform; // forward declarator
    typedef boost::shared_ptr<RandomUniform> RandomUniformPtr;
    
    
    //! Generator of random uniformity.
    class RandomUniform: public aw::Generator 
    {
    public:    
        // constructor
        RandomUniform(aw::SystemPtr sys); 
    
        virtual ~RandomUniform();
    
        virtual std::string getName();
    
        void init();
    
        void reset();
    
        virtual double getValueAtSample(aw::SampleTimeType sampleTime);
    
    
    private:
    
        aw::SampleTimeType sampleTimeLastRefresh_;
        double sampleTime_;
        double periodSamples_;
        double value_; 
    
    } ; // end class RandomUniform


} // end namespace aw



#endif



