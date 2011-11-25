/*
 *  aw_Generator.h
 *  arachnewarpTest
 *
 *  Created by Christopher Ariza on 7/10/10.
 *  Copyright 2010 Flexatone HFP. All rights reserved.
 *
 */

#ifndef AW_GENERATOR_H
#define AW_GENERATOR_H

#include <string>
#include <vector>
#include <map>

#include <boost/shared_ptr.hpp>

#include "aw_System.h"
#include "aw_Common.h"




namespace aw {

class Generator; // forward declarator

typedef boost::shared_ptr<Generator> GeneratorPtr;

typedef std::map<aw::ParameterName, GeneratorPtr> ParameterGeneratorMap;
typedef std::map<aw::ParameterName, aw::ParameterContext> ParameterContextMap;
typedef std::map<aw::ParameterName, aw::GeneratorType> ParameterTypeMap;

typedef std::vector<aw::ParameterName> ParameterNameVector;

//! Used, as localGenerators_, to store Generators built automatically by this Generator. These may be constants or other values. References to these Generators are then passed to setParameter and go in a ParameterGeneratorMap. This permits being able to tell the difference between Generators created locally and those that were passed in as an argument and created somewhere else.
typedef std::vector<GeneratorPtr> GeneratorVector;


//! Base class of all one-dimensional Generators. Returns a single double, or a pointer to a double. PolySize of a Generator is always 1.
class Generator
{
public:

    //! Constructor; sets PolySize to 1. PolySize will be altered in PolyGenerator subclases. 
    Generator(aw::SystemPtr o); 

    //! Destructor. All destructors need to be virtual, even if not specialized.
    virtual ~Generator();

    //! Initialization. Called by each each constructor, and always called for the base-class constructor. For Generators, sets gt_ to gTypeMono
    virtual void init();

    //! Rreturn generator to initial conditions. Specialized in subclasses.
    virtual void reset();

    //! Return the Generator name as a string.
    virtual std::string getName();


    // -------------------------------------------------------------------------
    //! Return the Generator type, either aw::gTypeMono or aw::gTypePoly
    aw::GeneratorType getGeneratorType();

    // -------------------------------------------------------------------------
    // These methods provide compatibility with PolyGenerator, but are not implemented for normal Generators.

    //! Return active polySize. For Generators this is always 1; for PolyGenerators this may be 1 or greater.
    virtual int getPolyDepth();

    int getFramesDefined();

    //! Initialize all working array values to zero.
    virtual void clearWorkingArray();

    // Only applicable to PolyGenerators
    virtual void resizeWorkingArray(int size);





    // -------------------------------------------------------------------------
    //! Return a string representation of this Generator, including embeded parameter Generators. The Constant subclass of this method will need to override this method
    // as it needs to get a constant value. 
    virtual std::string getParameterString(bool compact=false);

    //! For a named parameter, return true if a this Generator supports this parameter.
    bool isValidParameter(const aw::ParameterName p);

    //! For a named parameter, return true if the supplied aw::GeneratorType is compatible with this parameter slot. Parameter types are either aw::gTypeMono or aw::gTypePoly.
    bool isValidGenerator(const aw::ParameterName p, aw::GeneratorType gt);

    //! Calls isValidParameter() with supplied Generator
    bool isValidGenerator(const aw::ParameterName p, GeneratorPtr g);

    //! For a named parameter, return true if this aw::ParameterContext is supported. 
    bool isValidContext(const aw::ParameterName p, aw::ParameterContext pc);

    // -------------------------------------------------------------------------
    //! Return the context value stored in pContextMap_ for this parameter
    aw::ParameterContext getContext(aw::ParameterName pn);    

    //! Return the context value stored in pContextMap_ for this parameter string
    aw::ParameterContext getContext(std::string pnString);

    //! Return the active Generator for a named parameter slot
    GeneratorPtr getParameter(aw::ParameterName pn);    

    //! Return the active Generator for a parameter string
    GeneratorPtr getParameter(std::string pnString);

    //! Return a pointer to the System object contained in this Generator.
    SystemPtr getSystem();


    // -------------------------------------------------------------------------
    //! Set a parameter with a Generator. This is the main function called by all other overloaded function calls
    virtual void setParameter(const aw::ParameterName p, 
                              GeneratorPtr g, 
                              aw::ParameterContext pc=aw::pContextNameNone);

    //! Set any parameter with a double; will create a local Constant in this Generator.
    virtual void setParameter(const aw::ParameterName p, 
                              double v, 
                              aw::ParameterContext pc=aw::pContextNameNone);

    //! Set any parameter with a int; explicitly convert to a double and call the setParameter(). This is necessary to avoid confusion with char* values. This is not a virtual function as no need to specialize. 
    void setParameter(const aw::ParameterName p, 
                      int v, 
                      aw::ParameterContext pc=aw::pContextNameNone);

    //! Set a PolyGenerator parameter with a list; will create a local PolyConstant in this Generator
    virtual void setParameter(const aw::ParameterName p, 
                              std::vector<double>& v, 
                              aw::ParameterContext pc=aw::pContextNameNone);


    //! Set a Generator with a string value; overridden by PolyTableFile
    virtual void setParameter(const aw::ParameterName p, 
                      std::string& s, 
                      aw::ParameterContext pc=aw::pContextNameNone);
    //! Set a Generator with a char* value; overridden by PolyTableFile
    virtual void setParameter(const aw::ParameterName p, 
                      const char* s, 
                      aw::ParameterContext pc=aw::pContextNameNone);




    // -------------------------------------------------------------------------
    // secondary forms using different types

    //! Set any parameter with a Generator using string arguments for ParameterName and ParameterContext. 
    void setParameter(const std::string& pString, 
                       GeneratorPtr g, 
                       const std::string& pcString="none");

    //! Set any parameter with a Generator using character array for ParameterName and ParameterContext
    void setParameter(const char* pCharArray, 
                       GeneratorPtr g, 
                       const char* pcCharArray="none");


    //! Set any parameter with a double using string arguments for ParameterName and ParameterContext. Calls setParameter() with a double.
    void setParameter(const std::string& pString, 
                       double v, 
                       const std::string& pcString="none");

    //! Set any parameter with a double using a character array for ParameterName and ParameterContext. Calls setParameter() with a double.
    void setParameter(const char* pCharArray, 
                       double v, 
                       const char* pcCharArray="none");

    //! Set parameter based on std::string arguments for all values. Parameter value may be a string defining an embedded Generator. May need to be overridden.
    virtual void setParameter(std::string& pString, 
                              std::string& generatorString, 
                              std::string& pcString);

    //! Set parameter based on character array arguments for all values. Calls setParameter() with strings. 
    virtual void setParameter(const char* pCharArray, 
                              const char* generatorCharArray, 
                              const char* pcCharArray="none");


    //! Read in a parameter string and set the parameters in this object to the specified Generators. This sets all parameters defined in the string; this will create any internal objects as necessary. Constant and PolyConstant must override. 
    virtual void readParameterString(std::string& parameterString);


    // -------------------------------------------------------------------------
    // utility conversions
    double getValueWithContextToFrequency(double v, 
                    aw::ParameterName pn, 
                    aw::SampleRateType sr);

    double getValueWithContextToPeriodSamples(double v, 
                    aw::ParameterName pn, 
                    aw::SampleRateType sr);


    // -------------------------------------------------------------------------
    //! Return the value at a given sample.
    virtual double getValueAtSample(aw::SampleTimeType);

    //! Return value at a second. base version of this function calls getValueAtSample() with time conversion
    // might be overridden in base class if optimization is available. 
    double getValueAtSecond(double secondTime);


    //! Return a pointer to either a double (when a Generator) or a double array (when a PolyGenerator). PolyGenerator overrides this in subclass
    virtual aw::WorkingArrayPtr getPolyAtSample(aw::SampleTimeType st);

    aw::WorkingArrayPtr getPolyAtSecond(double st);


    // -------------------------------------------------------------------------
    //! Get a block of values; need to know starting time point.
    // passing in a reference to an array
//     void getBlock(aw::SampleTimeType sampleTime, double outputSamples[]);


    // -------------------------------------------------------------------------
    // this does not need to be void, as only found in base class
    void printSample(int n, double t, int scalar=1, 
                     aw::TimeContext tc=aw::timeContextNameSecond);

    void printParameterDiagnostic();



protected:

    //! Reference to System object contained within this Generator.
    SystemPtr sys_;

    //! The current sampling rate. Updated from sys_ for each call.
    aw::SampleRateType sr_; // current sampling rate

    //! The generator type (aw::GeneratorType) of this Generator or PolyGenerator
    // this parameter be not be necessary with a unification of mono/poly types
    aw::GeneratorType gt_;

    //! The active depth used (active data) on the poly array. For all Generators this is 1. For PolyGenerators this is 1 or larger
    int unsigned polyDepth_;
    //! Store current size of workingArray as actually allocated, not just what is used as defined by polyDepth_.
    int unsigned polyDepthAllocated_;


    ///! All generators can optionally define a number of frames defined as samples; for dynamic generators, this will always be 1; for tables, this will be the number of frames

    int unsigned framesDefined_;

    // -------------------------------------------------------------------------

    //! A vector of parameter names (aw::ParameterName) specific to this subclass.
    ParameterNameVector pValid_;

    //! A map of aw::ParameterName to the active GeneratorPtr
    ParameterGeneratorMap pMap_; 

    //! A map of aw::ParameterName to the active context (aw::ParameterContext)
    ParameterContextMap pContextMap_; 

    //! A map of aw::ParameterName to the active generator type (aw::GeneratorType)
    ParameterTypeMap pTypeMap_; 

    //! A vector of GeneratorPtr created dynamically within this Generator to supply constant, default, or string-inherited parameter values. 
    GeneratorVector localGenerators_;

    // -------------------------------------------------------------------------

    //! Double type storage of a value resulting from a processing called. Used to simulate PolyGenerator functionality in a Generator. Not relevant for PolyGenerators.
    double workingValue_; // current working value

    //! Pointer to workingValue_; provided to simulate functionality of PolyGenerators. Not relevant for PolyGenerators.
    aw::WorkingArrayPtr workingValuePtr_;



private:


        
};



} // end namespace aw



#endif

