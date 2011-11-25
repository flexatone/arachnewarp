/*! \file aw_WaveSquare.cpp
    \brief Wave Square Generator
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <math.h>


#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "aw_PolyTableFile.h"
#include "aw_Common.h"
#include "aw_Input.h"



namespace aw {


// =============================================================================
// constructor
PolyTableFile :: PolyTableFile(aw::SystemPtr o)
    : aw::PolyGenerator::PolyGenerator(o), // call base class constructor
    // note: must call new here; cannot call new in init() below
    input_(new Input(sys_)) 
{
    init();
}

// =============================================================================
//! Destroyer, virtual
PolyTableFile :: ~PolyTableFile()
{
    // need to release table, though this is not necsesary
}

// =============================================================================
//! Perform basic setup
void PolyTableFile :: init()
{

    pValid_.push_back(aw::pNameFilePath);
    pTypeMap_[aw::pNameFilePath] = aw::gTypeNone;

    // default is None
    fp_ = "none";
    //std::cout << "PolyTableFile :: init(): fp_: " << fp_ << std::endl;

    polyDepth_ = 1; // this is the channel size, and will be updatedat on load

}

// =============================================================================
//! Reset all values used in value generation
void PolyTableFile :: reset()
{
}

// =============================================================================
//! Return a string name
std::string PolyTableFile :: getName()
{
    static const std::string name("PolyTableFile");
    return name;
}



// =============================================================================
//! Return a string name
// This method must override the behavior of other Generators as it is not made of Generators
std::string PolyTableFile :: getParameterString(bool compact)
{
    //std::cout << "PolyTableFile :: getParameterString(): fp_: " << fp_ << std::endl;
    // just return string
    if (compact) {
        return fp_;
    }
    else {
        std::string str;
        str += getName();
        str += "{";   
        // abbreviated version; just have number alone
        str += fp_;    
        str += "}";
        return str;
    }
}

  

// -----------------------------------------------------------------------------
// String overload, calls vector overload
void PolyTableFile :: setParameter(const aw::ParameterName p, 
                                  std::string& fpString, 
                                  aw::ParameterContext pc)
{
    //std::cout << "PolyTableFile :: setParameter: " << fpString << std::endl;
    if (!isValidParameter(p)) {
        throw std::invalid_argument("invalid parameter for this generator");
    }
    bool match(false);

    switch (p) {                       
        case aw::pNameFilePath:  
            if (isValidContext(p, pc)) {

                // TODO: need to make sure that the file exists
                fp_ = fpString;
                // channel number, returned here, becomes the poly depth
                // table will be cleared and updated

                //std::cout << "PolyTableFile :: setParameter: " << "about to call input_->read(fp_, table_): " << "table_.size(): " << table_.size() << std::endl;

                polyDepth_ = input_->read(fp_, table_);

                // the workingArray_ may need to be adjusted; this will only adjust size if greater than the minimum
                resizeWorkingArray(polyDepth_);

                // need to set number frames based on file
                // the size divided by the depth will always be the number of frames
                framesDefined_ = table_.size() / polyDepth_; 

            match = true;
            break;              
            }
    }
    if (!match) {
        throw std::invalid_argument("invalid parameter name or parameter context");
    }    

}
// -----------------------------------------------------------------------------
//! Char array overload, calls vector overload.
void PolyTableFile :: setParameter(const aw::ParameterName p, 
                                  char* const fpCharArray, 
                                  aw::ParameterContext pc)
{
    // convert char array to string.
    std::string fpString(fpCharArray);
    setParameter(p, fpString, pc);
}

// -----------------------------------------------------------------------------
//! All string array overload, calls local method.
void PolyTableFile :: setParameter(std::string& pString, 
                                  std::string& fpString, 
                                  std::string& pcString)
{
    setParameter(aw::stringToParameterName(pString), 
                 fpString, 
                 aw::stringToParameterContext(pcString));
}




// -----------------------------------------------------------------------------
// called from Generator, overloaded for PolyTableFile
void PolyTableFile :: readParameterString(std::string& parameterString) {

    std::cout << "PolyTableFile.readParameterString(): " << parameterString << std::endl;

    std::string str(parameterString); // get a copy of this string

    // assume string file path is only argument; this does not define embedded 
    // parameters
    setParameter(aw::pNameFilePath, 
                 parameterString,
                 aw::pContextNameNone);

}



// =============================================================================
// return a pointer to a working array
aw::WorkingArrayPtr PolyTableFile :: getPolyAtSample(aw::SampleTimeType st)
{
    // update values in working vector; the values may be changed
    // in later processing; thus, at the top of the call chain
    // muse refresh

    // take modulus of sample time to auto loop
    // this assumes that st will never be negative
    st = st % framesDefined_;

    //aw::printVector(table_, "raw table");

    for (int i=0; i<polyDepth_; i++) {

        //std::cout << "PolyTableFile :: getPolyAtSample: " << i << " st: " << st << " adding value from table: " << table_[(st*polyDepth_)+i] << std::endl;

        // for each depth unit, get that much more from value
        // must multiply depth by sample time to add bass value, then add 
        // each index within that multiple
        workingArray_[i] = table_[(st*polyDepth_)+i]; 
    };
    // returning the pointer to the vector
    return workingArray_;
}



} // end namespace aw

