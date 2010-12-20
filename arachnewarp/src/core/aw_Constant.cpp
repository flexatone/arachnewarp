/*! \file aw_Constant.cpp
    \brief A constant value Generator.
    
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <sstream>

#include "aw_Constant.h"




namespace aw {


// =============================================================================
//! Constructor
Constant :: Constant(aw::SystemPtr o)
    : aw::Generator::Generator(o) // call base class Constructor
{      
    init();
}

// =============================================================================
//! Destroyer, virtual
Constant :: ~Constant()
{
}

// =============================================================================
//! Perform basic setup
void Constant :: init()
{    
    pValid_.push_back(aw::pNameValue);
    pTypeMap_[aw::pNameValue] = aw::gTypeNone;

    value_ = 0;
}

// =============================================================================
//! Perform basic setup
void Constant :: reset()
{
}


// =============================================================================
//! Return a string name
std::string Constant :: getName()
{
    static const std::string name("Constant");
    return name;
}


// =============================================================================
//! Return a string name
// This method must override the behavior of other Generators as it is not made of Generators
std::string Constant :: getParameterString(bool compact)
{
    // just return number
    if (compact) {
        return aw::numberToString(value_);
    }
    else {
        std::string str;
        str += getName();
        str += "{";   
    
        // abbreviated version; just have number alone
        str += aw::numberToString(value_);
    
        // only one parameter, so give condensed form
    //     str += aw::parameterNameToString(pValid_[0]);
    //     str += "{";
    // 
    //     // see function template here for alternative way to do this
    //     // http://notfaq.wordpress.com/2006/08/30/c-convert-int-to-string/
    //     std::stringstream out;
    //     out << value_; // convert value to string
    //     str += out.str();
    // 
    //     str += "}";
    
        str += "}";
        return str;
    }
}



// =============================================================================
// Set the parameter of this Generator
// As a Constant, this method must override Generator behavior to assing
// value_ directly
void Constant :: setParameter(const aw::ParameterName p, double v, 
                              aw::ParameterContext pc)
{
    if (!isValidParameter(p)) {
        throw std::invalid_argument("invalid parameter for this generator");
    }
    bool match(false);
    switch (p) {                       
        case aw::pNameValue:     
            if (isValidContext(p, pc)) {
            // note: this Generator does not use the pMap_ map
            value_ = v;   
            match = true;
            break;              
            }
    }
    if (!match) {
        throw std::invalid_argument("invalid parameter name or parameter context");
    }    
}    



// -----------------------------------------------------------------------------
// called from Generator, overloaded for Constant
void Constant :: readParameterString(std::string& parameterString) {

    std::string str(parameterString); // get a copy of this string
    //std::cout << "Constant.readParameterString(): " << str << std::endl;

    // in some cases we will get just a number as parmeter string
    // if the are in the form value{3}
    bool pStrIsArgs = aw::canConvertFromStringToDouble(str); 

    if (pStrIsArgs) {
        setParameter(aw::pNameValue, 
                     aw::stringToDouble(parameterString));
    }
    else {
        // read in parameters; there can be only one
        std::string name(""); 
        std::string arguments("");
        std::string context("");
        std::string working("");
    
        aw::getFirstParameterTrio(str, name, arguments, context, working);
        // names here are parameter names; as strings, these will be converted
        // set parameter as double
        setParameter(aw::stringToParameterName(name), 
                     aw::stringToDouble(arguments));
    }
    
}



// =============================================================================
//! Get a value for a given time point.
double Constant :: getValueAtSecond(double secondTime)
{
    // std::cout << "Constant: current value of value: " << value << std::endl;
    return value_;
}

// =============================================================================
//! Get a value for a given sample point.
double Constant :: getValueAtSample(aw::SampleTimeType sampleTime)
{
    // std::cout << "Constant.getValueAtSample: current value of value: " << value << std::endl;
    return value_;
}    



} // end namespace aw

