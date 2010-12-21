/*! \file aw_Convert.cpp
    \brief Conversion, transformation, and similar functional routines.
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


// utility functions for processing unit intervals spacings

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <cstdlib>

#include <boost/algorithm/string.hpp>
#include <boost/random.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "aw_Common.h"
#include "aw_System.h"



namespace aw {

// =============================================================================
//! Convert a aw::ParameterContext enum integer to a string representation. 
std::string parameterContextToString(aw::ParameterContext pc)
{
    std::string pcName;
    switch (pc) {     
        case aw::pContextNameNone:
            pcName += "none";
            break;
        case aw::pContextNameFrequency:
            pcName += "frequency";
            break;
        case aw::pContextNamePeriodSeconds:
            pcName += "seconds";
            break;
        case aw::pContextNamePeriodSamples:
            pcName += "samples";
            break;
        case aw::pContextNameTrigger:
            pcName += "trigger";
            break;
        case aw::pContextNamePitch:
            pcName += "pitch";
            break;
        case aw::pContextNameBeatsPerMinute:
            pcName += "bpm";
            break;
        default:
            throw std::out_of_range("parameterContextToString(): the given ParameterContext matches no known ParameterContext"); 
    };

    return pcName;
}

// -----------------------------------------------------------------------------
//! Convert a string representation to an aw::ParameterContext enumeration. 
// pass a copy of the string, not a reference
aw::ParameterContext stringToParameterContext(std::string str)
{
    // copy of a string is pased, not a reference, so can edit in place
    // remove extra space, lower case
    aw::scrubString(str);
    aw::ParameterContext pc; // an integer

    if (str == "none" || str == "") // an empty string is none
        pc = aw::pContextNameNone;
    else if (str == "frequency" || str == "fq" || str == "hertz" || str == "hz")
        pc = aw::pContextNameFrequency;
    else if (str == "periodseconds" || str == "seconds" || str == "sec")
        pc = aw::pContextNamePeriodSeconds;
    else if (str == "periodsamples" || str == "samples" || str == "samp")
        pc = aw::pContextNamePeriodSamples;
    else if (str == "trigger")
        pc = aw::pContextNameTrigger;
    else if (str == "pitch")
        pc = aw::pContextNamePitch;
    else if (str == "beatsperminute" || str == "bpm")
        pc = aw::pContextNameBeatsPerMinute;
    else
        throw std::out_of_range("stringToParameterContext(): the given string (" + str + ")  matches no known ParameterContext"); 
    return pc;
}

// -----------------------------------------------------------------------------
//! Convert a aw::ParameterName enum integer to a string representation. 
std::string parameterNameToString(aw::ParameterName pn)
{
    std::string pName;
    switch (pn) {     
        case aw::pNameMinimum:
            pName += "minimum";
            break;
        case aw::pNameMaximum:
            pName += "maximum";
            break;
        case aw::pNameRefresh:
            pName += "refresh";
            break;
        case aw::pNameRate: // used for frequency values
            pName += "rate";
            break;
        case aw::pNameValue:
            pName += "value";
            break;
        case aw::pNameDutyCycle:
            pName += "dutyCycle";
            break;
        case aw::pNameOperand1:
            pName += "operand1";
            break;
        case aw::pNameOperand2:
            pName += "operand2";
            break;
        case aw::pNameOperand3:
            pName += "operand3";
            break;
        case aw::pNameOperand4:
            pName += "operand4";
            break;
        case aw::pNameAmplitude:
            pName += "amplitude";
            break;
        case aw::pNameValueList:
            pName += "valueList";
            break;
        case aw::pNameSelectionMethod:
            pName += "selectionMethod";
            break;
        case aw::pNamePanLeftRight:
            pName += "panLeftRight";
            break;
        case aw::pNameStride: // stride magnitude; integer step size
            pName += "stride";
            break;

        default:
            throw std::out_of_range("parameterNameToString(): the given ParameterName matches no known ParameterName"); 
    };
    return pName;
}

// -----------------------------------------------------------------------------
aw::ParameterName stringToParameterName(std::string str)
{
    // copy of a string is pased, not a reference, so can edit in place
    // remove extra space, lower case
    aw::scrubString(str);
    
    ParameterName pn; // an integer

    if (str == "minimum" || str == "min")
        pn = aw::pNameMinimum;
    else if (str == "maximum" || str == "max")
        pn = aw::pNameMaximum;
    else if (str == "refresh")
        pn = aw::pNameRefresh;
    else if (str == "rate")
        pn = aw::pNameRate;
    else if (str == "value")
        pn = aw::pNameValue;
    else if (str == "dutycycle")
        pn = aw::pNameDutyCycle;
    else if (str == "operand1" || str == "op1" || str == "op")
        pn = aw::pNameOperand1;
    else if (str == "operand2" || str == "op2")
        pn = aw::pNameOperand2;
    else if (str == "operand3" || str == "op3")
        pn = aw::pNameOperand3;
    else if (str == "operand4" || str == "op4")
        pn = aw::pNameOperand4;
    else if (str == "amplitude" || str == "amp")
        pn = aw::pNameAmplitude;
    else if (str == "valuelist")
        pn = aw::pNameValueList;
    else if (str == "selectionmethod")
        pn = aw::pNameSelectionMethod;
    else if (str == "panleftright" || str == "pan")
        pn = aw::pNamePanLeftRight;
    else if (str == "stride")
        pn = aw::pNameStride;

    else
        throw std::out_of_range("stringToParameterName(): the given string (" + str + ") matches no known ParameterName"); 
    return pn;
}



// -----------------------------------------------------------------------------
//! Convert a string representation to an aw::GeneratorName enumeration. 
// move this to generator factory?
aw::GeneratorName stringToGeneratorName(std::string str)
{
    // copy of a string is pased, not a reference, so can edit in place
    // remove extra space, lower case
    aw::scrubString(str);
    GeneratorName gn; // an integer

    if (str == "add")
        gn = aw::gNameAdd;
    else if (str == "multiply")
        gn = aw::gNameMultiply;
    else if (str == "constant")
        gn = aw::gNameConstant;
    else if (str == "randomuniform")
        gn = aw::gNameRandomUniform;
    else if (str == "wavesine")
        gn = aw::gNameWaveSine;
    else if (str == "wavesquare")
        gn = aw::gNameWaveSquare;
    else if (str == "filterlowpass")
        gn = aw::gNameFilterLowPass;
    else if (str == "filterhighpass")
        gn = aw::gNameFilterHighPass;
    else if (str == "click")
        gn = aw::gNameClick;
    else if (str == "polyconstant")
        gn = aw::gNamePolyConstant;
    else if (str == "selector")
        gn = aw::gNameSelector;
    else if (str == "panstereo")
        gn = aw::gNamePanStereo;
    else if (str == "polyadd")
        gn = aw::gNamePolyAdd;
    else
        throw std::out_of_range("stringToGeneratorName(): the given string (" + str + ") matches no known GeneratorName"); 
    return gn;
}




// =============================================================================
// unit interval transformations and value mapping
double denormalize(double value, double a, double b)
{
    // need to handle value beyond 0 and 1
    if (value < 0 || value > 1) 
        throw std::out_of_range("value must be between 0 and 1"); 

    // if no range, return boundary
    if (a == b) return a;
    // general form: value * (max - min) + max;
    // a is min, b is max
    else if (a < b) return (value * (b - a)) + a;
    // b is min, a is max
    else return (value * (a - b)) + b;   
}


// -----------------------------------------------------------------------------
// modulus that is always positive; mod of negative values keeps sign
//http://stackoverflow.com/questions/1082917/mod-of-negative-number-is-melting-my-brain
int mod(int x, int m) {
    return (x % m + m) % m;
}



// -----------------------------------------------------------------------------
// need to fit value in range; need to snap to integer range with probabilistic mapping
// int fitIntegerRange(double v, double a, double b)




// =============================================================================
double sampleTimeToSecondTime(aw::SampleTimeType sampleTime, int sr)
{
    // given a number of samples into the future, determine how manuy 
    // seconds this takes
    // simply divide by sampling reate
    return static_cast<double>(sampleTime) / sr;
}

// this did not work; would not match to pointer
// Convert sample time to seconds time with an System object
// double aw::sampleTimeToSecondTime(aw::SampleTimeType sampleTime, 
//                                   boost::shared_ptr<System> o)
// {
//     return static_cast<double>(sampleTime) / o->getSamplingRate();
// }


// -----------------------------------------------------------------------------
double bpmToSecondTime(double bpm)
{
    return (60.0 / bpm);
}

// -----------------------------------------------------------------------------
double secondTimeToSampleTime(double secondTime, int sr)
{
    // as this takes double second values, this must return doubles
    return secondTime * static_cast<double>(sr);
}




// =============================================================================
// need global-level instance of BoostRandomEngine
static BoostRandomEngine randomEngine(std::time(0));                 

// random double uniform values within the unit interval. 
double randomUnit()
{
    // assuming static here makes this more efficient
    static double const min = 0;
    static double const max = 1;
    //static aw::BoostRandomEngine base(std::time(0));
    static aw::DistributionUniformReal dist(min, max);
    static aw::GeneratorUniformReal rng(randomEngine, dist);
    return rng();
}

// get within range [0, n)
aw::Int32Signed randomInteger(int a)
{    
    // create the distribution with the supplied args
    // as inclusive, need to reduce upper boundary by 1
    // no error check for negative values
    DistributionUniformInt dist(0, a-1);
    aw::GeneratorUniformInt rng(randomEngine, dist);    
    return rng();
}



aw::Int32Signed randomIntegerRange(int a, int b)
{    
    // create the distribution with the supplied args
    DistributionUniformInt dist(a, b);
    aw::GeneratorUniformInt rng(randomEngine, dist);    
    return rng();
}

// see  for shuffle
// http://stackoverflow.com/questions/147391/using-boostrandom-as-the-rng-for-stdrandom-shuffle
// for ranges: std::swap()

// Shuffle a vector of integers.
void shuffleIntegerVector(std::vector<aw::Int32Signed>& v)
{    
    // shuffle in place, as passing by reference
    std::random_shuffle(v.begin(), v.end(), aw::randomInteger); 
}


// -----------------------------------------------------------------------------
//! Return a random integer between a half range of values
// based on Koenig, p135
// aw::Int32Signed aw::randomRange(aw::Int32Signed a, aw::Int32Signed b)
// {
//     // for now, assume that a is min and b is max
//     aw::Int32Signed shift = 0 - a; // get negative offset
//     // aw::Int32Signed min = a + shift; // starts at zero now
//     aw::Int32Signed max = b + shift; 
// 
//     // integer division truncates
//     if (max > RAND_MAX) 
//         throw std::domain_error("max aw::randomRange argument is out of range");
// 
//     const aw::Int32Signed bucketSize = RAND_MAX / max;
//     aw::Int32Signed r;
// 
//     do r = std::rand() / bucketSize;
//     while (r >= max);
// 
//     return r - shift; // restore shift value
// }


// =============================================================================
// number conversions


aw::Int32Signed doubleToIntProabilistic(double n)
{
    double up = std::ceil(n);
    double down = std::floor(n);
    double fraction = n - down; // just the floating point value

    double seed = aw::randomUnit(); // get random value b/n 0 and 1
    aw::Int32Signed x;

    // if no fraction, return lower unaltered
    if (fraction == 0) {
        x = aw::Int32Signed(down);
    }
    // favor going up, more likely if fraction is large
    // 2.8, w/ a fraction of .8, has a good chance of being > tn rand(0,1)
    else if (seed <= fraction) { 
        x = aw::Int32Signed(up);
    }
    // 2.2, w/ a fraction of .2, has a good chance of being < tn rand(0,1)
    // favor going down, likely if fraction is small 
    else { 
        x = aw::Int32Signed(down);
    }
    
    return x;
}



// =============================================================================
// string conversions and printing


// remove leading and trailing spaces and make lower case
// pass by reference; used as a proc routine before string comparisons
void scrubString(std::string& str)
{
    boost::to_lower(str);
    boost::erase_all(str, " ");
    boost::erase_all(str, "\n");
    boost::erase_all(str, "\t");
    //boost::trim(str);
}

// -----------------------------------------------------------------------------
// http://notfaq.wordpress.com/2006/08/30/c-convert-int-to-string/
std::string numberToString(double n) 
{
    std::stringstream out;
    out << n;
    return out.str();
}

// -----------------------------------------------------------------------------
std::string arrayDoubleToString(double* a, int size)
{
    std::vector<std::string> tokens; // for storage of string conversion

//     for (std::vector<double>::const_iterator iter = v.begin(); 
//         iter != v.end(); 
//         ++iter) {

    for (int i=0; i<size; i++) {
        std::stringstream out;
        out << a[i]; 
        tokens.push_back(out.str()); 
    }
    std::string out = boost::join(tokens, ",");
    return out;
}

// -----------------------------------------------------------------------------
std::string vectorDoubleToString(std::vector<double> v)
{
    // first, convert array to vector string
    std::vector<std::string> tokens; // for storage of string conversion
    for (int i=0; i<v.size(); i++) {
        std::stringstream out;
        out << v[i]; // counverts to a string
        tokens.push_back(out.str()); 
    }
    std::string out = boost::join(tokens, ",");
    return out;
}


// -----------------------------------------------------------------------------
double stringToDouble(const std::string& str) 
{
    std::istringstream i(str);
    double x;
    if (!(i >> x))
        throw std::domain_error("convertToDouble(\"" + str + "\")");
    return x;
}

// -----------------------------------------------------------------------------
std::vector<double> stringToVectorDouble(const std::string& str) 
{
    // split string by commas, do post scrubbing
    std::vector<std::string> pre = aw::splitString(str, ",", true);
    // for output
    std::vector<double> out;

    for (std::vector<std::string>::const_iterator iter = pre.begin(); 
        iter != pre.end(); 
        ++iter) {
        // could use string conversion with stringstream or similar
//         std::stringstream out;
//         out << *iter; 
        out.push_back(boost::lexical_cast<double>(*iter)); 
    };
    // can return vector; cannot return an array
    return out;
}


// -----------------------------------------------------------------------------
// need to check that all chars are either numbers or point values
// TODO: this method is not complete: need better condition checking
bool canConvertFromStringToDouble(const std::string& str) {
    bool post; // if the are in the form value{3}
    double test;

    // if any comma delimiters are found, cannot use (a list likely)
    int foundDelimit = aw::countSubStrings(str, &aw::charListDelimit);
    if (foundDelimit > 0) return false;

    try {
        test = aw::stringToDouble(str);
        post = true;
    }
    catch (std::domain_error) {
        post = false;
    }
    return post;
}

// -----------------------------------------------------------------------------
// TODO: this method is not complete: need better conditions
// need to make sure that only characters are numerals or ,
bool canConvertFromStringToVectorDouble(const std::string& str) {
    bool post; // if the are in the form value{3}
    std::vector<double> test;

    // need to pass reference to character
    int foundDelimit = aw::countSubStrings(str, &aw::charListDelimit);
    if (foundDelimit == 0) return false;

    try {
        test = aw::stringToVectorDouble(str);
        post = true;
    }
    catch (boost::bad_lexical_cast) {
        post = false;
    }
    catch (std::domain_error) {
        post = false;
    }

    return post;
}



// -----------------------------------------------------------------------------
std::vector<std::string> splitString(const std::string& srcRaw, 
                                         const std::string& find, 
                                         bool postScrub) 
{
    std::vector<std::string> out;

    // need to make a copy of src string, as must not be a const reference
    std::string src(srcRaw);

    for(StringSplitIterator it = boost::make_split_iterator(
            src, boost::first_finder(find, boost::is_iequal())); 
        it!=StringSplitIterator(); 
        ++it) { 
        std::string sub = boost::copy_range<std::string>(*it);
        // scrub string in place
        if (postScrub) aw::scrubString(sub); 
        out.push_back(sub);
    } 
    return out;
}




// -----------------------------------------------------------------------------
int countSubStrings(const std::string& srcRaw, const std::string& find)
{
    int count(0); 

    // need to make a copy of src string, as must not be a const reference
    std::string src(srcRaw);

    // make first finder that looks for find string, use this to 
    // to create a find iterator
    for(aw::StringFindIterator it=boost::make_find_iterator(
            src, boost::first_finder(find, boost::is_iequal())); 
        it != StringFindIterator(); 
        ++it) { 
        count++;
        } 
    return count;
}


// -----------------------------------------------------------------------------
bool stringWellFormed(const std::string& str, const std::string& open, const std::string& close) 
{
    int countOpen(0); 
    int countClose(0); 
    // see if open and close are the same
    countOpen = aw::countSubStrings(str, open);
    countClose = aw::countSubStrings(str, close);
    if (countOpen == countClose) return true;
    return false;
}






// =============================================================================

//! Get the first parameter name
void getFirstParameterName(std::string& src, 
                               std::string& out,
                               std::string& working) {
    int openCount(0);
    int start(0);
    int end(0);
    int targetCount(0);
    bool writing(false);
//     std::cout << src << ": out: " << out << " working: " << working << std::endl;    
    for(int i = 0; i < src.length(); i++) {
        if (src[i] == '{') openCount++;
        else if (src[i] == '}') openCount--;
        // if we are at open 0 level, writing has not started, and this is the
        // first 0 level found, start writing
        if (openCount == 0 && writing == false && targetCount == 0) {
            writing = true;
            start = i;
            targetCount++; // increment how many times at level 1
        }
        // as soon as open count move off of zero, stop writing; do not include
        // this point (its a brace)
        else if (openCount == 1 && writing == true)  {
            writing = false;
            end = i; // do not include this point
        }
        // if this is the last character of the source
        else if (openCount == 0 && writing == true && i == src.length()-1) {
            writing = false;
            end = i+1; // include last point
        }

//         std::cout << src[i] << ": open: " << openCount << " writing: " << writing << std::endl;    

    }

//     std::cout << "start, end " << start << " : " << end << std::endl;    

    // need source, start, and length to copy
    out.append(src, start, end-start);
    // assign to working everything after the removed section
    if (src.length()-end > 1) working.append(src, end, src.length()-end);

}

// -----------------------------------------------------------------------------
//! Get the first delimited range of a parameter statements.
void getFirstParameterArguments(std::string& src, 
                                    std::string& out, 
                                    std::string& working) {
    int openCount(0);
    int start(0);
    int end(0);
    int targetCount(0);
    bool writing(false);
//     std::cout << src << ": out: " << out << " working: " << working << std::endl;    
    for(int i = 0; i < src.length(); i++) {
        if (src[i] == '{') openCount++;
        else if (src[i] == '}') openCount--;
        // if we are at open 0 level, writing has not started, and this is the
        // first 1 level found, start writing
        if (openCount == 1 && writing == false && targetCount == 0) {
            writing = true;
            start = i;
            targetCount++; // increment how many times at level 1
        }
        // when we next get open count to zero, the boundary has been closed
        // however, we need to include this point
        else if (openCount == 0 && writing == true) {
            writing = false;
            end = i+1; // want to include this point
        }
//         std::cout << src[i] << ": open: " << openCount << " writing: " << writing << std::endl;    
    }

//     std::cout << "start, end " << start << " : " << end << std::endl;    

    if (start != 0 && end != 0) { 
        // need source, start, and length to copy
        // need to remove outermost braces
        out.append(src, start+1, end-start-2);
        // assign to working everything after the removed section
        // working.append(src, 0, start);
        working.append(src, end, src.length()-end);
    }
    // if both start and end are zero, append all of of src to working
    else working.append(src); 

//     if (src.length()-end > 1) working.append(src, end, src.length()-end);

}

// -----------------------------------------------------------------------------
//! Get the first parameter context string.
// TODO: this may rely on there being no spaces between braces; need to scrub
void getFirstParameterContext(std::string& src, 
                                  std::string& out, 
                                  std::string& working) {
    int openCount(0);
    int start(0);
    int end(0);
    int targetCount(0);
    int openCountLast(0); // store last open count
    int openCountNext(-1); // store next open count

    int targetTransitions(0); // count transitions from 0 to 1

    bool writing(false);
//     std::cout << src << ": out: " << out << " working: " << working << std::endl;    
    for(int i = 0; i < src.length(); i++) {
        if (src[i] == '{') openCount++;
        else if (src[i] == '}') openCount--;

        // count how many times we go from 0 to 1; can only accept 2nd
        if (openCountLast == 0 && openCount == 1) targetTransitions++;
        
        // look ahead one character; this assumes there are no spaces
        openCountNext = openCount; // assign to present
        if (i < src.length()-1) {
            if (src[i+1] == '{') openCountNext++;
            else if (src[i+1] == '}') openCountNext--;
        }

        // if we are at open 0 level for the second time moving from 1
        // then we have the context 
        // first 1 level found, start writing
        // next char is a level increase
        // this marks the close of the last group, so need to shift 1
        if (openCount == 0 && writing == false && openCountLast == 1 && openCountNext == 1 && targetTransitions == 1 && targetCount == 0) {
            writing = true;
            start = i+1;
            targetCount++; // increment how many times at level 1

        }
        // when we next get open count to zero, the boundary has been closed
        else if (openCount == 0 && writing == true) {
            writing = false;
            end = i+1; // want to include this point
        }
        openCountLast = openCount;

//         std::cout << src[i] << ": open: " << openCount << " writing: " << writing << " targetTransitions: " << targetTransitions << std::endl;    
    }
    // only get if set
    if (start != 0 && end != 0) { 
        // need source, start, and length to copy
        // need to remove outermost braces
        out.append(src, start+1, end-start-2);
        // assign to working everything after the removed section
        // working.append(src, 0, start);
        working.append(src, end, src.length()-end);
    }
    else working.append(src); 

}


// -----------------------------------------------------------------------------
//! Get the first group of three parameter components. The working string returns the remaining string. 
void getFirstParameterTrio(std::string& src, // input
                               std::string& name, // output to write to
                               std::string& arguments,
                               std::string& context,
                               std::string& working) {

    // remove white space; TODO need to remove intermingled white space 
    //aw::scrubString(src);

    std::string workingPostName; 
    std::string workingPostArguments; 
    std::string workingPostContext; 

    aw::getFirstParameterName(src, name, workingPostName);
    aw::getFirstParameterArguments(src, arguments, workingPostArguments);
    aw::getFirstParameterContext(src, context, workingPostContext);
    
    // if no context, and no parameters, return name working
    if (name != "" && arguments == "" && context == "") {
        working = workingPostName;
        //std::cout << "getFirstParameterTrio: setting working from working post name " << std::endl;
    }
    // if not context, assign working after parameters
    else if (context == "") {
        working = workingPostArguments;
        //std::cout << "getFirstParameterTrio: setting working from working post arguments " << std::endl;
    }
    else if (context != "") {
        working = workingPostContext;
        //std::cout << "getFirstParameterTrio: setting working from working post context " << std::endl;
    }
}




// =============================================================================

void printDebug(const char* c)
{
    std::cout << "aw_Common::printDebug(): " << c << std::endl;
}


void printDebug(std::string& str)
{
    std::cout << "aw_Common::printDebug(): " << str << std::endl;
}

void printDebug(double v)
{
    std::cout << "aw_Common::printDebug(): " << numberToString(v) << std::endl;
}




// =============================================================================
//! Given an array, print to standard output.
// we must provide a length argument
void printArray (double arg[], aw::Int32Signed length, const std::string& header) 
{
    std::vector<std::string> tokens; // for storage of string conversion

    // convert to a vector of strings
    for (aw::Int32Signed i=0; i<length; i++) {
        std::stringstream out;
        out << arg[i]; // adds to stream
        tokens.push_back(out.str()); 
    };

    // using boost::join, returns a string
    // return boost::join(tokens, " | ");
    std::cout << header << ": " << boost::join(tokens, " | ") << std::endl;
}


//! Given a vector, print to standard out. A `header` argyment can be included. 
template <typename T>
void printVector (const std::vector<T>& src, const std::string& header) 
{
    std::vector<std::string> tokens; // for storage of string conversion
    typedef typename std::vector<T>::const_iterator IterType;

    // use a const_iterator to get values out of src
    // go from .begin to .end
    for (IterType iter = src.begin(); iter != src.end(); ++iter) {
        std::stringstream out;
        // w/o iterator, would be src[i]
        // if access a member of structure named 'name', would use (*iter).name
        // parens are req due to precedence of .
        // this is the same as iter->name
        out << *iter; 
        tokens.push_back(out.str()); 
    };
    // using boost::join, returns a string
    std::cout << header << ": " << boost::join(tokens, " | ") << std::endl;
}
// these are required for each class of the tempalte
// necessary b/c of template definition in header: http://www.parashift.com/c++-faq-lite/templates.html#faq-35.13
// solve linking errors by providing samples
template void printVector<int> (const std::vector<int>& src, const std::string& header);
//template void aw::printVector<aw::Int32Signed> (const std::vector<aw::Int32Signed>& src, const std::string& header);
template void printVector<double> (const std::vector<double>& src, const std::string& header);



//! Given a vector of floats, print to standard output.
// passing vector by reference
// void aw::printVector (const std::vector<double>& src, const std::string& header) 
// {
//     std::vector<std::string> tokens; // for storage of string conversion
// 
// 
//     // use a const_iterator to get values out of src
//     // go from .begin to .end
//     for (std::vector<double>::const_iterator iter=src.begin();
//             iter != src.end(); ++iter) {
//         std::stringstream out;
//         // w/o iterator, would be src[i]
//         // if access a member of structure named 'name', would use (*iter).name
//         // parens are req due to precedence of .
//         // this is the same as iter->name
//         out << *iter; 
//         tokens.push_back(out.str()); 
//     };
// 
//     // using boost::join, returns a string
//     // return boost::join(tokens, " | ");
//     std::cout << header << ": " << boost::join(tokens, " | ") << std::endl;
// }
// 
// //! Given a vector of ints, print to standard output.
// void aw::printVector (const std::vector<aw::Int32Signed>& src, const std::string& header) 
// {
//     std::vector<std::string> tokens; // for storage of string conversion
//     for (std::vector<aw::Int32Signed>::const_iterator iter=src.begin();
//             iter != src.end(); ++iter) {
//         std::stringstream out;
//         out << *iter; 
//         tokens.push_back(out.str()); 
//     };
//     std::cout << header << ": " << boost::join(tokens, " | ") << std::endl;
// }



} // end namespace aw



