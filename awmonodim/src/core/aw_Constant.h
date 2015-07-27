/*! \file aw_Constant.h
    \brief Header.
    
Copyright 2010 Flexatone HFP. All rights reserved.
*/

#ifndef AW_CONSTANT_H
#define AW_CONSTANT_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "aw_System.h"
#include "aw_Generator.h"
#include "aw_Common.h"




namespace aw {


class Constant; // forward declaration
typedef boost::shared_ptr<Constant> ConstantPtr;


//! Base class of all generators
class Constant: public aw::Generator
{
public:

    // constructors
    Constant(aw::SystemPtr o); 

    virtual ~Constant();

    virtual std::string getName();

    // must override behavior of Generator
    std::string getParameterString(bool compact=false);

    void init();

    void reset();

    // most basic getValue
    virtual double getValueAtSecond(double secondTime);
    virtual double getValueAtSample(aw::SampleTimeType);

    // provide string name of argument, value (or object), value domain
    void setParameter(const aw::ParameterName p, 
                              double v, // not a Generator!
                              aw::ParameterContext pc=aw::pContextNameNone);

    // must override all-string parameter setting, as this is the terminus
//     void setParameter(std::string& pString, 
//                       std::string& generatorString, 
//                       std::string& pcString);

    void readParameterString(std::string& parameterString);

    // could provide a virtual function to handle setting with a generator
    // just get a sample value as a double

private:

    //! Convention to use this variable as the destination of generated values; this value is not in base-class, but only last-inherited 
    double value_;
        
} ; // end class Constant



} // end namespace aw



#endif


// old approach to get name
//     static const std::string& getName() {
//        static const std::string sOut("Constant");
//        return sOut;      
//     }
