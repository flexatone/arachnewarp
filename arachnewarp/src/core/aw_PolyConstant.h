/*! \file aw_WaveSquare.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_POLY_CONSTANT_H
#define AW_POLY_CONSTANT_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "aw_Generator.h"
#include "aw_PolyGenerator.h"


class PolyConstant; // forward declarator

typedef boost::shared_ptr<PolyConstant> PolyConstantPtr;

//! PolyGenerator of a constant list of values.
class PolyConstant: public PolyGenerator 
{
public:

    PolyConstant(SystemPtr sys); 

    ~PolyConstant();

    std::string getName();

    std::string getParameterString(bool compact=false);


    void init();

    void reset();

    // overridden from Generator
    void setParameter(const aw::ParameterName p, 
                      std::vector<double>& v, 
                      aw::ParameterContext pc=aw::pContextNameNone);

    // not found in any base class:
    void setParameter(const aw::ParameterName p, 
                      std::string& vString, 
                      aw::ParameterContext pc=aw::pContextNameNone);

    // not found in any base class:
    void setParameter(const aw::ParameterName p, 
                      char* const vCharArray, 
                      aw::ParameterContext pc=aw::pContextNameNone);

    // overload Generator form
    void setParameter(std::string& pString, 
                      std::string& vString, 
                      std::string& pcString);


    // overload Generator form
    void readParameterString(std::string& ps);


    aw::WorkingArrayPtr getPolyAtSample(aw::SampleTimeType);


private:

    // Internal storage for values; loaded into workingArray_ for each call. This is necessary as values in the _workingArray may be manipulated by callers. 
       
    double value_[aw::maximumPolySize];
   
} ;



#endif



