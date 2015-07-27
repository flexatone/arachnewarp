/*! \file aw_WaveSquare.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_POLY_CONSTANT_H
#define AW_POLY_CONSTANT_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "aw_Generator.h"
#include "aw_PolyGenerator.h"
#include "aw_Common.h"




namespace aw {


class PolyConstant; // forward declarator
typedef boost::shared_ptr<PolyConstant> PolyConstantPtr;

//! PolyGenerator of a constant list of values, where the list is returned as a whole for each sample call. Values are stored within this object in the static value_ array. When called on by other Generators, all values in the value_ array are copied into the workingArray_, and a reference to this array is passed to the caller. The workingArray_ is defined and stored in PolyGenerator; all PolyGenerators have a fixed, stack-based workingArray. 
class PolyConstant: public aw::PolyGenerator 
{
public:

    PolyConstant(aw::SystemPtr sys); 

    ~PolyConstant();

    std::string getName();

    std::string getParameterString(bool compact=false);


    void init();

    void reset();

    //! overridden from Generator
    void setParameter(const aw::ParameterName p, 
                      std::vector<double>& v, 
                      aw::ParameterContext pc=aw::pContextNameNone);

    //! not found in any base class:
    void setParameter(const aw::ParameterName p, 
                      std::string& vString, 
                      aw::ParameterContext pc=aw::pContextNameNone);

    //! not found in any base class:
    void setParameter(const aw::ParameterName p, 
                      char* const vCharArray, 
                      aw::ParameterContext pc=aw::pContextNameNone);

    //! overload Generator form
    void setParameter(std::string& pString, 
                      std::string& vString, 
                      std::string& pcString);


    // overload Generator form
    void readParameterString(std::string& ps);


    aw::WorkingArrayPtr getPolyAtSample(aw::SampleTimeType);


private:



    // Internal storage for values is a double array; these values are loaded into workingArray_ for each call. 

    // if an array pointer
    // aw::WorkingArrayPtr value_;

    // old approach: statically defined size
    //double value_[aw::defaultPolyDepthAllocated];

    // as vector: safest
    std::vector<double> value_;

   
} ; // end class PolyConstant



} // end namespace aw



#endif



