/*! \file aw_WaveSquare.cpp
    \brief Wave Square Generator
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <math.h>


#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "aw_PolyConstant.h"
#include "aw_Common.h"




namespace aw {


// =============================================================================
// constructor
// :: defines a member of a class from outside the class definition itself
//! Doc in cpp
/// longer doc in cpp
PolyConstant :: PolyConstant(aw::SystemPtr o)
    : aw::PolyGenerator::PolyGenerator(o) // call base class constructor
{
    init();
}

// =============================================================================
//! Destroyer, virtual
PolyConstant :: ~PolyConstant()
{
    // delete value_ b/c it is created on the heap
    // delete[] value_;

}

// =============================================================================
//! Perform basic setup
void PolyConstant :: init()
{
    // this might be better as pNameList
    pValid_.push_back(aw::pNameValue);
    pTypeMap_[aw::pNameValue] = aw::gTypeNone;

    // create on heap with default size
    // value_ = new double[aw::defaultPolyDepthAllocated];

    // setup some default values; this must be done directly here to the
    // value parameter; do not try to set a contstant, as that will create
    // recursive object creation
    value_.push_back(0.0); 
    polyDepth_ = 1; // default init size
    //clearValueArray();


}

// =============================================================================
//! Reset all values used in value generation
void PolyConstant :: reset()
{
}

// =============================================================================
//! Return a string name
std::string PolyConstant :: getName()
{
    static const std::string name("PolyConstant");
    return name;
}



// =============================================================================
//! Return a string name
// This method must override the behavior of other Generators as it is not made of Generators
std::string PolyConstant :: getParameterString(bool compact)
{
    // just return list
    if (compact) {
        return aw::vectorDoubleToString(value_);
    }
    else {
        std::string str;
        str += getName();
        str += "{";   
        // abbreviated version; just have number alone
        str += aw::vectorDoubleToString(value_);    
        str += "}";
        return str;
    }
}



// =============================================================================
// Set the parameter of this Generator
// As a PolyConstant, this method must override Generator behavior to set
// value_ directly
// value_ is defined as private in PolyConstant.h
// this is the main method used to load values
void PolyConstant :: setParameter(const aw::ParameterName p, 
                                  std::vector<double>& v, 
                                  aw::ParameterContext pc)
{
    // checks parameters defined on pValid
    if (!isValidParameter(p)) {
        throw std::invalid_argument("invalid parameter for this generator");
    }
    bool match(false);
    switch (p) {                       
        case aw::pNameValue:   
            if (isValidContext(p, pc)) {
                // the workingArray_ may need to be adjusted 
                resizeWorkingArray(v.size());
                // the polyDepth_ value must be adjusted
                polyDepth_ = v.size();

                value_.clear();
                // note: this Generator does not use the pMap_ map
                // transfer/copy values to local vector
                for (unsigned int i=0; i<v.size(); i++) {
                    value_.push_back(v[i]); 
                };
            match = true;
            break;              
            }
        default: // handle other cases
            break;
    }
    if (!match) {
        throw std::invalid_argument("invalid parameter name or parameter context");
    }    

    //std::cout << "=== PolyConstant :: setParameter: " << "polyDepth_ " << polyDepth_ << " getPolyDepth(): " << getPolyDepth() << std::endl;


}    

// -----------------------------------------------------------------------------
// String overload, calls vector overload
void PolyConstant :: setParameter(const aw::ParameterName p, 
                                  std::string& vString, 
                                  aw::ParameterContext pc)
{
    std::vector<double> vVector = aw::stringToVectorDouble(vString);
    // pass on vector to vector version
    setParameter(p, vVector, pc);
}
// -----------------------------------------------------------------------------
//! Char array overload, calls vector overload.
void PolyConstant :: setParameter(const aw::ParameterName p, 
                                  char* const vCharArray, 
                                  aw::ParameterContext pc)
{
    std::vector<double> vVector = aw::stringToVectorDouble(vCharArray);
    // pass on vector to vector version
    setParameter(p, vVector, pc);
}

// -----------------------------------------------------------------------------
//! All string array overload, calls vector overload.
// must not call Generator version
void PolyConstant :: setParameter(std::string& pString, 
                                  std::string& vString, 
                                  std::string& pcString)
{
    // pass on vector to vector version
    setParameter(aw::stringToParameterName(pString), 
                 vString, 
                 aw::stringToParameterContext(pcString));
}




// -----------------------------------------------------------------------------
// called from Generator, overloaded for PolyConstant
void PolyConstant :: readParameterString(std::string& parameterString) {

    std::string str(parameterString); // get a copy of this string
    //std::cout << "PolyConstant.readParameterString(): " << str << std::endl;

    // in some cases we will get just a number as parmeter string
    // if the are in the form value{3}
    bool pStrIsArgs = aw::canConvertFromStringToVectorDouble(str); 

    if (pStrIsArgs) {
        setParameter(aw::pNameValue, 
                     parameterString,
                     aw::pContextNameNone);
    }
    else {
        // read in parameters; there can be only one
        std::string name(""); 
        std::string arguments("");
        std::string context("");
        std::string working("");
    
        aw::getFirstParameterTrio(str, name, arguments, context, working);
        // names here are parameter names; as strings, these will be converted
        setParameter(aw::stringToParameterName(name), 
                     arguments, // leave as string; easer function matching
                     aw::pContextNameNone);
    }
    
}



// =============================================================================
// return a pointer to a working array
aw::WorkingArrayPtr PolyConstant :: getPolyAtSample(aw::SampleTimeType) {
    // update values in working vector; the values may be changed
    // in later processing; thus, at the top of the call chain
    // muse refresh

    for (unsigned int i=0; i<polyDepth_; i++) {
        workingArray_[i] = value_[i]; 
    };
    // returning the pointer to the vector
    return workingArray_;
}



} // end namespace aw

