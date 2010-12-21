/*! \file aw_WaveSquare.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_POLY_ADD_H
#define AW_POLY_ADD_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "aw_Generator.h"
#include "aw_PolyGenerator.h"
#include "aw_Common.h"




namespace aw {



class PolyAdd; // forward declarator
typedef boost::shared_ptr<PolyAdd> PolyAddPtr;

//! PolyGenerator that adds two PolyGenerators at parallel positions in the working array. Note that this PolyAdd can work just like an Add, and suggests its replacement. 
class PolyAdd: public aw::PolyGenerator 
{
public:

    PolyAdd(aw::SystemPtr sys); 

    ~PolyAdd();

    void init();

    void reset();

    std::string getName();

    aw::WorkingArrayPtr getPolyAtSample(aw::SampleTimeType);


private:

    // needs to be propery sample
    aw::SampleTimeType sampleTime_;

    aw::MagnitudeType sum_; // for storing sum

    int srcSize1_;
    int srcSize2_;
    int srcSize3_;
    int srcSize4_;

    // pointer to working arrays obtained from stored PolyGenerators
    aw::WorkingArrayPtr workingArrayOperand1_;
    aw::WorkingArrayPtr workingArrayOperand2_;
    aw::WorkingArrayPtr workingArrayOperand3_;
    aw::WorkingArrayPtr workingArrayOperand4_;

   
} ; // end class PolyAdd


} // end namespace aw



#endif



