/*! \file aw_WaveSquare.cpp
    \brief Wave Square Generator
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <math.h>


#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "aw_PolyGenerator.h"
#include "aw_Common.h"




namespace aw {



// =============================================================================
PolyGenerator :: PolyGenerator(aw::SystemPtr o)
    : aw::Generator::Generator(o) // call base class constructor
{
    init();
}

// =============================================================================
PolyGenerator :: ~PolyGenerator()
{
    // assume i do not need to delete workingArray b/c its size is static
    // and it is created at compile time
}

// =============================================================================
void PolyGenerator :: init()
{   
    gt_ = aw::gTypePoly;
    defaultFoldDownMethod_ = aw::fdMethodFirst;
    clearWorkingArray();
}

// =============================================================================
void PolyGenerator :: reset()
{
}

// =============================================================================
void PolyGenerator :: clearWorkingArray()
{
    // need to initialize working array
    for (int i=0; i<aw::maximumPolySize; i++) {
        workingArray_[i] = 0; 
    };
}


// =============================================================================
std::string PolyGenerator :: getName()
{
    static const std::string name("PolyGenerator");
    return name;
}

// =============================================================================
// TODO: must deal with active array and combine/average data into on form
// possibly look at a parameter to determine fold-down method
double PolyGenerator :: getValueAtSample(aw::SampleTimeType st)
{
    //sr_ = sys_->getSamplingRate();

    // must call to update value
    // return the first value
    // ultimately, create a FoldDown generator with defaultFoldDownMethod_
    return getPolyAtSample(st)[0];
};    


// =============================================================================
aw::WorkingArrayPtr PolyGenerator :: getPolyAtSample(aw::SampleTimeType st)
{
    // basic behavior is to return pointer to double array
    return workingArray_;
};    



} // end namespace aw

