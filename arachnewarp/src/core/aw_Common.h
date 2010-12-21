/*! \file aw_Convert.h
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_CONVERT_H
#define AW_CONVERT_H


#include <string>
#include <vector>
#include <stdint.h> // has uint32_t

#include <boost/random.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/array.hpp>

#include "aw_System.h"







// =============================================================================
namespace aw {

double const pi(3.14159265358979323846264338);
double const pi2(3.14159265358979323846264338*2.0);

//! Turn debug information print on or off.
double const PRINT_LOG(false);

//! The maximum size of PolyGenerator arrays. This determines the initial size of all poly arrays. These arrays will reallocate to larger sizes if necessary. This size is similar to bit depth
int const defaultPolyDepthAllocated(32);

// characters
char const charListDelimit = ',';
char const charPmtrOpen = '{';
char const charPmtrClose = '}';

// =============================================================================
// used for sample time
typedef uint32_t unsigned Int32Unsigned;
// 0	4,294,967,295

typedef int32_t signed Int32Signed;
// -2,147,483,648	2,147,483,647

typedef uint64_t unsigned Int64Unsigned;
// 0	18,446,744,073,709,551,615

//! All sample times units are passed in this type. Presently, this is a 32 bit int, but could be increased if needed.
typedef uint32_t unsigned SampleTimeType;
// 0	4,294,967,295


//! Used for all seconds values. 
typedef double SecondsTimeType;

//! Used for all sample rate values. 
typedef int unsigned SampleRateType;

//! This type is used for all amplitude values, or other values calculated in processing. 
typedef double MagnitudeType;


typedef double* WorkingArrayPtr;


// =============================================================================
// http://www.brainbell.com/tutorials/C++/Generating_Random_Numbers.htm
typedef boost::mt19937 BoostRandomEngine;

typedef boost::uniform_real<double> DistributionUniformReal;
// note the & following; needs to bass by reference

typedef boost::variate_generator<BoostRandomEngine&, DistributionUniformReal> GeneratorUniformReal;

typedef boost::uniform_int<Int32Signed> DistributionUniformInt;
// note the & following; needs to bass by reference
typedef boost::variate_generator<BoostRandomEngine&, DistributionUniformInt> GeneratorUniformInt;


// boost arrays: this one is not yet used
// typedef boost::array<MagnitudeType, aw::defaultPolyDepthAllocated> BoostWorkingArray;

// string processing
typedef boost::find_iterator<std::string::iterator> StringFindIterator;
typedef boost::split_iterator<std::string::iterator> StringSplitIterator; 





// =============================================================================
//! aw::TimeContext are used in cases where we need to specify if values/presentation is by sample or seconds.
enum TimeContext {
    timeContextNameSample = 0, 
    timeContextNameSecond = 1,
};



//! Some parameters can have a context to determine how a value is interpreted. 
enum ParameterContext {
    // none; no context necessary
    pContextNameNone = 0,  // hz
    // values specify hertz frequency
    pContextNameFrequency = 1,  // hz
    // values specify period in seconds
    pContextNamePeriodSeconds = 2, // seconds
    // values specify period in seconds
    pContextNamePeriodSamples = 3, // seconds
    // values specify a trigger, where non-zero is a trigger
    pContextNameTrigger = 4, // trigger
    // alternate pitch representation: midi pitch values
    pContextNamePitch = 5, 
    // beats per minute
    pContextNameBeatsPerMinute = 6, 

};

//! Converter from enumeration to string
std::string parameterContextToString(ParameterContext pc);

//! Converter from string to ParameterContext
ParameterContext stringToParameterContext(std::string str);


//! All parameters can be accessed by name
// names here should reflect there use in a generator or a family of generators
// and not be general to any context
enum ParameterName {
    pNameMinimum = 0,
    pNameMaximum = 1,
    pNameRefresh = 2,
    pNameRate = 3, // frequency in hertz, or period in sec
    pNameValue = 4, // a constant value, used for PolyConstant and Constant

    // this parameter name may be redundant with unit interval?
    pNameDutyCycle = 5,

    // operands can be signals or other general data sources
    // possible change to Control, Signal?
    pNameOperand1 = 6,
    pNameOperand2 = 7,
    pNameOperand3 = 8,
    pNameOperand4 = 9,

    pNameAmplitude = 10,
    pNameValueList = 11,
    pNameSelectionMethod = 12,

    // a unit interval control; possible rename position, horizontalPosition
    // xPosition
    pNamePanLeftRight = 13,

    // step size for selecting from lists
    pNameStride = 14,

    };

//! Converter from enumeration to string
std::string parameterNameToString(ParameterName pn);

//! Converter from string to ParameterName
ParameterName stringToParameterName(std::string str);


// -----------------------------------------------------------------------------
//! Define types of Generators; each parameter must define one and only one type of Generator
enum GeneratorType {
    gTypeMono = 0,
    gTypePoly = 1,
    gTypeNone = 2, // for Constant and PolyConstant: 
    gTypeAny = 3, // any form : 
};


//! All Generators can be accessed by name
enum GeneratorName {
    gNameAdd = 0,
    gNameMultiply = 1,
    gNameConstant = 2,
    gNameRandomUniform = 3, 
    gNameWaveSine = 4, 
    gNameWaveSquare = 5,
    gNameFilterLowPass = 6,
    gNameFilterHighPass = 7,
    gNameClick = 8,
    gNamePolyConstant = 9,
    gNameSelector = 10,
    gNamePanStereo = 11,
    gNamePolyAdd = 12,
    };


//! Converter from a string to enumeration
GeneratorName stringToGeneratorName(std::string str);


//! Fold down method used by various PolyGenerator objects.
enum FoldDownMethod {
    fdMethodFirst = 0, 
    fdMethodLast = 1, 
    fdMethodAverage = 2, 
};


// =============================================================================
//! Denormalize a value within the unit interval to the range specified by a and b.
double denormalize (double value, double a=0, double b=1);

//! Modules of integers that assures always positive values. 
int mod(int x, int m);

//! Convert sample time to seconds time
double sampleTimeToSecondTime(aw::SampleTimeType sampleTime, int sr=44100);

// this did not work
// double sampleTimeToSecondTime(aw::SampleTimeType sampleTime,
//        boost::shared_ptr<System> o);

//! Convert bpm to second time.
double bpmToSecondTime(double bpm);

//! Convert second time to samples
double secondTimeToSampleTime(double secondTime, int sr=44100);

// =============================================================================


//! Random uniform double values within the unit interval. 
double randomUnit();

//! Random integers within a range, inclusive.
aw::Int32Signed randomIntegerRange(int a, int b);

//! Random integers within the range [0, n)
aw::Int32Signed randomInteger(int a);

void shuffleIntegerVector(std::vector<aw::Int32Signed>& v);


// aw::Int32Signed randomRange(aw::Int32Signed a, aw::Int32Signed b);


// =============================================================================
// number conversions

aw::Int32Signed doubleToIntProabilistic(double n);


// =============================================================================
// string conversions and printing

//! Remove leading and trailing white spaces, and make all lower case. Modifies the supplied string in place.
void scrubString(std::string& str);

//! Convert a number to a string.
std::string numberToString(double n);

//! Convert a double array to a string representation.
std::string arrayDoubleToString(double* a, int size);

//! Convert a vector array to a string representation.
std::string vectorDoubleToString(std::vector<double>);

//! Convert a string to a double.
double stringToDouble(const std::string& str);

//! Convert a comma-delimited string to a vector of doubles.
std::vector<double> stringToVectorDouble(const std::string& str);

//! Determine if the supplied string can be converted to a double
bool canConvertFromStringToDouble(const std::string& str);

//! Determine if the supplied string can be converted to a double array
bool canConvertFromStringToVectorDouble(const std::string& str);

//! Split a string into sub-strings based on a find string that is one or more characters. Return a vector of strings.
std::vector<std::string> splitString(const std::string& str, 
                                     const std::string& find,
                                     bool postScrub=true);


//! Count number of substrings in the provided string. A string or character can be passed by reference. 
int countSubStrings(const std::string& src, const std::string& find);


// -----------------------------------------------------------------------------
//! Retrun boolean if a parameter string is well formed, or of all braces are matching pairs.
bool stringWellFormed(const std::string& str, 
                      const std::string& open, 
                      const std::string& close);

void getFirstParameterName(std::string& src, std::string& out, 
                           std::string& working);

void getFirstParameterArguments(std::string& src, std::string& out, 
                                std::string& working);

void getFirstParameterContext(std::string& src, std::string& out,
                              std::string& working);


void getFirstParameterTrio(std::string& src, // input
                               std::string& name, // output to write to
                               std::string& arguments,
                               std::string& context,
                               std::string& working);


// =============================================================================
void printDebug(const char* c);

void printDebug(std::string& str);

void printDebug(double v);


// =============================================================================
// defaults must be specified here
void printArray (double arg[], aw::Int32Signed length, const std::string& 
                 header="aw::printArray output");


// template for handling vectors of other data
template <typename T>
void printVector (const std::vector<T>& src, const std::string& header="aw::printVector output");

// void aw::printVector (const std::vector<double>&, const std::string& 
//                  header="aw::printVector<double> output");
// 
// void aw::printVector (const std::vector<aw::Int32Signed>&, const std::string& 
//                  header="aw::printVector<aw::Int32Signed> output");

} // end namespace aw

#endif