/*! \file aw_WaveSquare.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_POLY_GENERATOR_H
#define AW_POLY_GENERATOR_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>

#include "aw_Generator.h"
#include "aw_Common.h"





namespace aw {



class PolyGenerator; // forward declarator
typedef boost::shared_ptr<PolyGenerator> PolyGeneratorPtr;


//! Base class of PolyGenerators, objects that are specialized to return/provide multiple values from an array of static size. These values are passed as a pointer to an array, where array values are refreshed from the source (usually a PolyConstant) with each call. 

class PolyGenerator: public aw::Generator 
{
public:

    // constructor
    PolyGenerator(aw::SystemPtr sys); 

    virtual ~PolyGenerator();

    virtual std::string getName();

    //! Set Generator type as aw::gTypePoly; set default fold-down method. 
    virtual void init();

    virtual void reset();

    //! Initialize all working array values to zero.
    virtual void clearWorkingArray();


    //! Return a sinlge value, as a double, from the PolyGenerator representation, simulating a Generator. This overrides the normal behavior or Generator. 
    virtual double getValueAtSample(aw::SampleTimeType st);

    //! Main method for getting values from a PolyGenerator, customized in subclass. Returns a pointer to workingArray_; active size is found on polySize_
    virtual aw::WorkingArrayPtr getPolyAtSample(aw::SampleTimeType);


protected:

    //! Working array of double values for passing by pointer to callers. Note that this array does not have to be used; instead, a working array pointer received from an embedded parameter object can be used. Default working array size is 128. 
    double workingArray_[aw::maximumPolySize];

    // TODO: might use this instead in the future
    // BoostWorkingArray workingArray_;


    //! Store a default fold-down method for moving from a poly representation to a single value. 
    aw::FoldDownMethod defaultFoldDownMethod_;
    
} ; // end class PolyGenerator




} // end namespace aw



#endif



