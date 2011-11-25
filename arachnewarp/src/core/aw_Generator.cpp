
/*! \file aw_Generator.cpp
    \brief Base class for all Generators
    
Copyright 2010 Flexatone HFP. All rights reserved.
*/

#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <algorithm>


#include "aw_Generator.h"
#include "aw_System.h"
#include "aw_Common.h"
#include "aw_Constant.h"
#include "aw_PolyConstant.h"
#include "aw_GeneratorFactory.h"




namespace aw {


// =============================================================================
// constructor
Generator :: Generator(aw::SystemPtr o)
    // references cannot be assigned implicitly; must initialize
    // there, must pass system ptr to variable here
    : sys_(o) // initialize
    // default polySize is one

{
    Generator::init();
}


// =============================================================================
// destructor
Generator :: ~Generator()
{
    for (int i = 0; i < localGenerators_.size(); ++i) {
        // here, use -> on smart pointer to access method
        if (aw::PRINT_LOG) {
            std::clog<< "destroying a local generator: " << i << " name: " << localGenerators_[i]->getName() << std::endl;
        }
        // here, use dot operator on pointer
        localGenerators_[i].reset();
    }
}

// =============================================================================
void Generator :: init()
{    
    gt_ = aw::gTypeMono;
    // for Generators, connect the working value to the pointer; this is only
    // present to simulate PolyGenerator behavior. 
    workingValuePtr_ = &workingValue_;

    // defaults for both parameters
    polyDepth_ = 1;
    framesDefined_ = 1; 
}

// =============================================================================
void Generator :: reset()
{
}

// =============================================================================
std::string Generator :: getName()
{
    static const std::string name = "Generator";
    return name;
}

// =============================================================================
aw::GeneratorType Generator :: getGeneratorType()
{
    return gt_;
}

// =============================================================================
//! Get the size of the active region of this Generator
int Generator :: getPolyDepth()
{
    return polyDepth_;
}

// =============================================================================
int Generator :: getFramesDefined()
{
    return framesDefined_;
}

// =============================================================================
// only relevant in PolyGenerator subclass
void Generator :: clearWorkingArray()
{
}

// =============================================================================
// only relevant in PolyGenerator subclass
void Generator :: resizeWorkingArray(int size)
{
}


// =============================================================================
std::string Generator :: getParameterString(bool compact)
{
    // "refresh{30}{frequency}"
    std::string str;

    str += getName();
    str += "{";
    
    //result = std::find(pValid_.begin(), pValid_.end(), p);
    for (ParameterNameVector::iterator it = pValid_.begin(); 
        it != pValid_.end(); 
        ++it)  // need to pre-increment here
    {
        //std::cout << *it << std::endl;
        // parameter names are actually just Integers, as the are defined
        // in an enum; need to convert
        str += aw::parameterNameToString(*it); // dereference string
        str += "{";
        // provide value set to this Parmeter
        //GeneratorPtr subParameter = getParameter(*it);
        str += getParameter(*it)->getParameterString(compact);
        str += "}";

        // might only show context if it is not none
        aw::ParameterContext localContext = getContext(*it);
        
        if (localContext != aw::pContextNameNone) {
            str += "{";
            str += aw::parameterContextToString(localContext);
            str += "}";
        }
    }
    str += "}";

    return str;
}


// =============================================================================
bool Generator :: isValidParameter(const aw::ParameterName p)
{
    ParameterNameVector::iterator result;
    // find the parameter in the vector of parameters to determine validity
    result = std::find(pValid_.begin(), pValid_.end(), p);
    if(result == pValid_.end()) return false;
    return true;
}

// =============================================================================
bool Generator :: isValidGenerator(const aw::ParameterName p, 
                                   aw::GeneratorType gt)
{
    if (pTypeMap_[p] == aw::gTypeAny)
        return true;
    else if (pTypeMap_[p] == gt) 
        return true;
    return false;
}

// -----------------------------------------------------------------------------
bool Generator :: isValidGenerator(const aw::ParameterName p, GeneratorPtr g)
{
    // call alternate form
    return isValidGenerator(p, g->getGeneratorType());
}



// =============================================================================
// a result is given whether or not this Generator or Generator subclass supports this particular parameter. 
// this could be moved into Common?
// note: every time a new parameter type is added, it must be updated here
bool Generator :: isValidContext(const aw::ParameterName p,
                                           aw::ParameterContext pc)
{
    switch (p) {                       
        case aw::pNameValue:       
            if (pc == aw::pContextNameNone) return true;
            else return false;

        case aw::pNameRefresh:       
            switch (pc) {     
                case aw::pContextNameFrequency:
                    return true;
                case aw::pContextNamePeriodSeconds:
                    return true;
                case aw::pContextNamePeriodSamples:
                    return true;
                case aw::pContextNameTrigger:
                    return true;
                case aw::pContextNameBeatsPerMinute:
                    return true;
                default: return false;
            };

        case aw::pNameRate:       
            switch (pc) {     
                case aw::pContextNameFrequency:
                    return true;
                case aw::pContextNamePeriodSeconds:
                    return true;
                case aw::pContextNamePeriodSamples:
                    return true;
                case aw::pContextNamePitch:
                    return true;
                case aw::pContextNameBeatsPerMinute:
                    return true;
                default: return false;
            };

        case aw::pNameDutyCycle:       
            if (pc == aw::pContextNameNone) return true;
            else return false;

        case aw::pNameMinimum:       
            if (pc == aw::pContextNameNone) return true;
            else return false;

        case aw::pNameMaximum:                         
            if (pc == aw::pContextNameNone) return true;
            else return false;

        case aw::pNameOperand1:                         
            if (pc == aw::pContextNameNone) return true;
            else return false;

        case aw::pNameOperand2:                         
            if (pc == aw::pContextNameNone) return true;
            else return false;

        case aw::pNameOperand3:                         
            if (pc == aw::pContextNameNone) return true;
            else return false;

        case aw::pNameOperand4:                         
            if (pc == aw::pContextNameNone) return true;
            else return false;

        // could have context in unitInterval or dB
        case aw::pNameAmplitude:                         
            if (pc == aw::pContextNameNone) return true;
            else return false;

        case aw::pNameValueList:                         
            if (pc == aw::pContextNameNone) return true;
            else return false;

        case aw::pNameSelectionMethod:                         
            if (pc == aw::pContextNameNone) return true;
            else return false;

        // contexts might be unitInterval (0-1), mirrorUnitInterval()
        // mirror percent (-100 to 100)?
        case aw::pNamePanLeftRight:                         
            if (pc == aw::pContextNameNone) return true;
            else return false;

        case aw::pNameStride:                         
            if (pc == aw::pContextNameNone) return true;
            else return false;

        case aw::pNameFilePath:                         
            if (pc == aw::pContextNameNone) return true;
            else return false;


    } // end case   
    return false;   
}



// =============================================================================
aw::ParameterContext Generator :: getContext(aw::ParameterName pn)
{
    return pContextMap_[pn];
}    
// -----------------------------------------------------------------------------
aw::ParameterContext Generator :: getContext(std::string pnString)
{
    return pContextMap_[aw::stringToParameterName(pnString)];
}    



// =============================================================================
GeneratorPtr Generator :: getParameter(aw::ParameterName pn)
{
    return pMap_[pn];
}    
// -----------------------------------------------------------------------------
GeneratorPtr Generator :: getParameter(std::string pnString)
{
    return pMap_[aw::stringToParameterName(pnString)];
}    


// =============================================================================
SystemPtr Generator :: getSystem()
{
    return sys_;
}    







// =============================================================================
// the primary setParameter methods are here: each use a true type
void Generator :: setParameter(const aw::ParameterName p, 
                               GeneratorPtr g, 
                               aw::ParameterContext pc)
{
    // first, make sure this is a valid parameter
    if (!isValidParameter(p)) {
        throw std::invalid_argument("invalid parameter for this generator: " + getName());
    }
    if (!isValidContext(p, pc)) {
        throw std::invalid_argument("invalid context for this parameter: " + aw::parameterNameToString(p) + " context:" + aw::parameterContextToString(pc));
    }
    // check that a valid generator type is being provided
    if (!isValidGenerator(p, g->getGeneratorType())) {
        throw std::invalid_argument("invalid generator type for this parameter: " + aw::parameterNameToString(p));
    }

    pMap_[p] = g;
    // for many parameters this may be none
    pContextMap_[p] = pc; 
} 
// -----------------------------------------------------------------------------
// this is only done for parameter values that are provided as doubles, not for parameter values that are provided by Generators. 
void Generator :: setParameter(const aw::ParameterName p, 
                               double v, 
                               aw::ParameterContext pc)
{
    // create a local generator
    ConstantPtr g(new Constant(sys_));
    // p1 = new Constant(sys_);
    g->setParameter(aw::pNameValue, v);
    localGenerators_.push_back(g);
    // set the parameter here
    // call version with Generator: in derived class!

    if (aw::PRINT_LOG) {
        std::clog<< "calling base class setParameter with a double: " << v << " name: " << g->getName() << std::endl;
        }

    setParameter(p, g, pc);
}

// case of providing a plain int: need to explicitly cast to a double and call the right form; this is important for disintinguishing from a char*. 
void Generator :: setParameter(const aw::ParameterName p, 
                               int v, 
                               aw::ParameterContext pc)
{
    double vDouble(v);
    setParameter(p, vDouble, pc);

}


// -----------------------------------------------------------------------------
// vector loading version
void Generator :: setParameter(const aw::ParameterName p, 
                                  std::vector<double>& v, 
                                  aw::ParameterContext pc)
{
    // create a local generator
    PolyConstantPtr g(new PolyConstant(sys_));
    // calls overridden method
    g->setParameter(aw::pNameValue, v);
    localGenerators_.push_back(g);
    // set the parameter here
    // call version with Generator: in derived class!

    if (aw::PRINT_LOG) {
        std::clog<< "calling base class setParameter with a vector: name: " << g->getName() << std::endl;
        }
    // possibly need to do dynamic cast here?
    setParameter(p, g, pc);
}



// forms where the generator parameter is provided as a string
// to be overloaded in subclasses that need string arguments
// -----------------------------------------------------------------------------
void Generator :: setParameter(const aw::ParameterName p, 
                               std::string& s, 
                               aw::ParameterContext pc)
{
    // here, convert values to numbers; will create a constant generator
    double v = aw::stringToDouble(s);
    setParameter(p, v, pc);
} 
// -----------------------------------------------------------------------------
void Generator :: setParameter(const aw::ParameterName p, 
                               const char* s, 
                               aw::ParameterContext pc)
{
    // here, convert values to numbers; will create a constant generator
    // TODO: not tested
    double v = aw::stringToDouble(s);
    setParameter(p, v, pc);
} 








// alternative setParameter methods 
// -----------------------------------------------------------------------------
void Generator :: setParameter(const std::string& pString, 
                               GeneratorPtr g, 
                               const std::string& pcString)
{
    // pass on to whichever method is appropriate
    // if this is Constant, may call a overridten method
    setParameter(aw::stringToParameterName(pString), 
                g, 
                aw::stringToParameterContext(pcString));
} 
// -----------------------------------------------------------------------------
void Generator :: setParameter(const char* pCharArray, 
                               GeneratorPtr g, 
                               const char* pcCharArray)
{
    // character arrays are converted to strings
    setParameter(aw::stringToParameterName(pCharArray), 
                g, 
                aw::stringToParameterContext(pcCharArray));
} 







// -----------------------------------------------------------------------------
void Generator :: setParameter(const std::string& pString, 
                               double v, 
                               const std::string& pcString)
{
    // pass on to whichever method is appropriate
    // if this is Constant, may call a overridden method
    setParameter(aw::stringToParameterName(pString), 
                v, 
                aw::stringToParameterContext(pcString));
} 
// -----------------------------------------------------------------------------
void Generator :: setParameter(const char* pCharArray, 
                               double v, 
                               const char* pcCharArray)
{
    // character arrays are converted to strings
    setParameter(aw::stringToParameterName(pCharArray), 
                v, 
                aw::stringToParameterContext(pcCharArray));
} 








// -----------------------------------------------------------------------------
// all string passed by reference
// strings here should be const?
// PolyConstant overload this form
void Generator :: setParameter(std::string& pString, 
                               std::string& generatorString, 
                               std::string& pcString)
{
    std::string name(""); 
    std::string arguments("");
    std::string context("");
    std::string working("");

    //std::cout << "Generator.setParameter(): pString: " << pString << std::endl;
    //std::cout << "Generator.setParameter(): generatorString: " << generatorString << std::endl;

    aw::getFirstParameterTrio(generatorString, name, arguments, context,
                              working);

    // call create from GeneratorFactory
    GeneratorFactory gf(sys_); // one instance
    GeneratorPtr g = gf.create(generatorString);

    // pass on to whichever method is appropriate
    // if this is Constant, may call a overridden method
    setParameter(aw::stringToParameterName(pString), 
                g, 
                aw::stringToParameterContext(pcString));
} 

// -----------------------------------------------------------------------------
// all char array version
// PolyConstant overload this form
void Generator :: setParameter(const char* pCharArray, 
                               const char* generatorCharArray, 
                               const char* pcCharArray)
{
    std::string pString(pCharArray);
    std::string generatorString(generatorCharArray);
    std::string pcString(pcCharArray);
    // character arrays are converted to strings
    setParameter(pString, 
                 generatorString, 
                 pcString);
} 







// =============================================================================
// this is called from GeneratorFactory.create
void Generator :: readParameterString(std::string& parameterString) {

    std::string str(parameterString); // get a copy of this string

    // read in parameters in a loop
    while (str != "") {
        std::string name(""); 
        std::string arguments("");
        std::string context("");
        std::string working("");

        // set name, etc from values in str
        //std::cout << "Generator.readParameterString(): " << str << std::endl;

        aw::getFirstParameterTrio(str, name, arguments, context, working);
        // std::cout << "name: " << name << std::endl;
        // std::cout << "arguments: " << arguments << std::endl;

        // look here for arguments that are simply numbers: these
        // are Constant objects, and should be created here (simply by using setParameter and a number for arguments!
        bool pStrIsDoubleArgs = aw::canConvertFromStringToDouble(arguments); 
        bool pStrIsVectorDoubleArgs = aw::canConvertFromStringToVectorDouble(
            arguments); 

        if (pStrIsDoubleArgs) {
            //std::cout << "Generator.readParameterString(): evaluating args as double" << std::endl;
            setParameter(name, aw::stringToDouble(arguments), context);
        }
        else if (pStrIsVectorDoubleArgs) {
            //std::cout << "Generator.readParameterString(): evaluating args as vector double" << std::endl;

            std::vector<double> v = aw::stringToVectorDouble(arguments);
            setParameter(aw::stringToParameterName(name),
                         v, 
                         aw::stringToParameterContext(context));
        }
        else {
        // names here are parameter names; as strings, these will be converted
        // this calls the all-string version of this, only on non Constant gens
        // arguments may define other internal Generators
            setParameter(name, arguments, context);
        }

//         std::cout << "got name, arguments, context: " << name << ": " << arguments << ": " << context << ": " << std::endl;
//         std::cout << "got working: " << working << std::endl;

        str.clear();
        str.append(working); // append the remaining parameters
    }
}




// =============================================================================
double Generator :: getValueWithContextToFrequency(double v, 
                    aw::ParameterName pn, 
                    aw::SampleRateType sr)
{
    switch (pContextMap_[pn]) {       
        case aw::pContextNameFrequency:
            // do nothing
            break;
        case aw::pContextNamePeriodSeconds:
            // convert from period to frequency
            v = 1.0 / v;
            break;
        case aw::pContextNamePeriodSamples:
            // convert from period in samples to frequency
            v = 1.0 / aw::sampleTimeToSecondTime(v);
            break;
        case aw::pContextNamePitch:
            // convert from midi pitch to fq
            v = 440.0 * pow(2, ((v-69)/12.0));
            break;
        case aw::pContextNameBeatsPerMinute:
            // convert from period in samples to frequency
            v = 1.0 / aw::bpmToSecondTime(v);
            break;
    };
    return v;
}    

// -----------------------------------------------------------------------------
double Generator :: getValueWithContextToPeriodSamples(double v, 
                    aw::ParameterName pn, 
                    aw::SampleRateType sr)
{
    // the context of the input value is determined by the context alone
    // whatever the context, return the value in sample
    switch (pContextMap_[pn]) {       
        case aw::pContextNameFrequency:
            // from fq to period in samples
            v = aw::secondTimeToSampleTime(1./v, sr);
            break;
        case aw::pContextNamePeriodSeconds:
            // from seconds to samples
            v = aw::secondTimeToSampleTime(v, sr);
            break;
        case aw::pContextNamePeriodSamples:
            break; // nothing to do!
        case aw::pContextNameTrigger:
            if (v != 0) {
                v = 1; // if not 0 return a period of 1
                }
            else {
                // not sure if this is the best approach
                // if 0, return the longest period possible
                v = LONG_MAX;
            }
            break;
        case aw::pContextNamePitch:
            // convert from midi pitch to fq, then fq to period in samples
            v = 440.0 * pow(2, ((v-69)/12.0));
            v = aw::secondTimeToSampleTime(1./v, sr);
            break;

        case aw::pContextNameBeatsPerMinute:
            v = aw::secondTimeToSampleTime(aw::bpmToSecondTime(v), sr);
            break;


    };

    //std::cout << "getValueWithContextToPeriodSamples() " << " pContextMap_[pn] " << pContextMap_[pn] << " pn " << pn <<  " v " << v << std::endl;


    return v;
}    


// =============================================================================
double Generator :: getValueAtSample(aw::SampleTimeType sampleTime)
{
    // std::cout << "calling base class getValueAtSample" << std::endl;
    return 0;
}    

// =============================================================================
double Generator :: getValueAtSecond(double secondTime)
{
    // std::cout << "calling base class getValueAtSecond" << std::endl;
    return getValueAtSample(aw::secondTimeToSampleTime(secondTime, 
                            sys_->getSamplingRate()));
}    




// =============================================================================
aw::WorkingArrayPtr Generator :: getPolyAtSample(aw::SampleTimeType st)
{
    // call get value to set working value ptr
    workingValue_ = getValueAtSample(st);
    // working value pointer is already linked to workingValue_
    return workingValuePtr_;
}

// =============================================================================
//! Convert second to sample, call getPolyAtSample
aw::WorkingArrayPtr Generator :: getPolyAtSecond(double secondTime)
{
    return getPolyAtSample(aw::secondTimeToSampleTime(secondTime, 
                            sys_->getSamplingRate()));
}





// =============================================================================
// get a value for a number of samples into the future
// sampleTime is a integer count into the future, that may repeat
// void Generator :: getBlock(aw::SampleTimeType sampleTime, 
//                             double outputSamples[])
// {
//     // simulate: create an array 
// 	// double outputSamples[this->blockSize]; 
// 	aw::Int32Signed numSamplesToProcess = sys_->getBlockSize();
// 
// 	//while(numSamplesToProcess--)
//     //while (--numSamples >= 0)
//     for (int i=0; i<numSamplesToProcess; i++)
//     {
//         outputSamples[i] = getValueAtSample(sampleTime + i);
//     }
// 
//     // return aw::denormalize(rng(), min, max);
// }    
// 


// =============================================================================
//! Print a range of sample values at integer increments of t
/// aw::TimeContext enum value determines if this uses getValueAtSecond or getValueAtSample. The `scalar` parameter multiplies the number of samples `n` in production, and causes output to only be collected at the modulus of this scalar. 

void Generator :: printSample(int n, double t, int scalar, aw::TimeContext tc)
{
    //std::cout << "calling printSample from within Generator: " << this->getName() << std::endl;

    double v;     
    std::vector<double> values; // store values
    for (int i=0; i < n*scalar; i++) { 
        switch (tc) {                       
            case aw::timeContextNameSample:       
                v = getValueAtSample(t+i); // adds to stream
                break;              
            case aw::timeContextNameSecond:                         
                v = getValueAtSecond(t+i); // adds to stream
                break;     
            // TODO: need default case here         
        }; // end case    
        if (i % scalar == 0) values.push_back(v); 
    };
    aw::printVector(values, getName());
}



// =============================================================================
//! Print Generator diagnostic

void Generator :: printParameterDiagnostic()
{
    ParameterGeneratorMap::iterator p;

    std::clog<< "// parameter diagnostic: pMap_" << std::endl;
    for(p = pMap_.begin(); p != pMap_.end(); p++) {
        std::clog << "// key: " << p->first;
        std::clog << " // Generator name: " << p->second->getName();
        std::clog << " // sample value: " << p->second->getValueAtSample(0) << std::endl;
    }

    ParameterContextMap::iterator r;

    std::clog<< "// parameter diagnostic: pContextMap_" << std::endl;
    for(r = pContextMap_.begin(); r != pContextMap_.end(); r++) {
        std::clog << "// key: " << r->first;
        std::clog << " // value: " << r->second<< std::endl;
    }

    GeneratorVector::iterator s;

    std::clog<< "// parameter diagnostic: localGenerators_" << std::endl;
    for(s = localGenerators_.begin(); s != localGenerators_.end(); s++) {
        std::clog << "// Generator name: " << (*s)->getName();
        std::clog << " // sample value: " << (*s)->getValueAtSample(0) << std::endl;
    }
}



} // end namespace aw



