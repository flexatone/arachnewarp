/*! \file aw_WaveSquare.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_POLY_GENERATOR_H
#define AW_POLY_GENERATOR_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "aw_Generator.h"


class PolyGenerator; // forward declarator

typedef boost::shared_ptr<PolyGenerator> PolyGeneratorPtr;


//! Base class of PolyGenerators, objects that are specialized to return/provide multiple values. These values are passed as  a pointer to an array, where array values are refreshed with each call. 

class PolyGenerator: public Generator 
{
public:

    // constructor
    PolyGenerator(SystemPtr sys); 

    virtual ~PolyGenerator();

    virtual std::string getName();

    //! Set Generator type as aw::gTypePoly; set default fold-down method. 
    virtual void init();

    virtual void reset();


    //! Return a sinlge value, as a double, from the PolyGenerator representation, simulating a Generator. This overrides the normal behavior or Generator. 
    virtual double getValueAtSample(aw::SampleTimeType st);

    //! Main method for getting values from a PolyGenerator, customized in subclass. Returns a pointer to workingArray_; active size is found on polySize_
    virtual double* getPolyAtSample(aw::SampleTimeType);


protected:

    //! Working array of double values for passing by pointer from 
    double workingArray_[aw::maximumPolySize];

    //! Store a default fold-down method for moving from a poly representation to a single value. 
    aw::FoldDownMethod defaultFoldDownMethod_;
    
} ;



#endif



