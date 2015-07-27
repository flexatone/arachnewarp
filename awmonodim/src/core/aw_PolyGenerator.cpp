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


// note: all methods on poly generator must be overridden methods of a generator
// otherwise, pointer usage does not work properly

// =============================================================================
PolyGenerator :: PolyGenerator(aw::SystemPtr o)
    : aw::Generator::Generator(o) // call base class constructor
{
    init();
}

// =============================================================================
PolyGenerator :: ~PolyGenerator() {
    // now, using a heap-based array, need to delete workingArray
    delete[] workingArray_;
    //std::cout << "PolyGenerator deleted working array" <<  std::endl;
}

// =============================================================================
void PolyGenerator :: init() {   
    gt_ = aw::gTypePoly;
    defaultFoldDownMethod_ = aw::fdMethodFirst;
    polyDepth_ = 1; // default init size

    // assign WorkingArrayPtr 
    workingArray_ = new double[aw::defaultPolyDepthAllocated];

    // as we just reset array, its size is the default; store allocated
    polyDepthAllocated_ = aw::defaultPolyDepthAllocated;
    clearWorkingArray();
}

// =============================================================================
void PolyGenerator :: reset()
{
}

// =============================================================================
std::string PolyGenerator :: getName()
{
    static const std::string name("PolyGenerator");
    return name;
}

// =============================================================================
void PolyGenerator :: clearWorkingArray()
{
    // need to initialize working array over all allocated values, not just
    // those in use
    for (unsigned int i=0; i<polyDepthAllocated_; i++) {
        workingArray_[i] = 0; 
    };
}

// =============================================================================
// Resize the working array only if necessary.
void PolyGenerator :: resizeWorkingArray(unsigned int size) {
    // delete the old and create new size if larger than exisiting
    // assume that the requested size is also the 
    if (size > polyDepthAllocated_) {
        // working array never stores anything we need; it is just used
        // for passing as an output butter from each generator
        polyDepthAllocated_ = size;
        delete[] workingArray_;
        workingArray_ = new double[size];
        clearWorkingArray(); // not sure this is necessary
        //std::cout << "=== PolyGenerator :: resizeWorkingArray: " << "new size " << size << " new polyDepthAllocated_ " << polyDepthAllocated_ << std::endl;
    }
}


// =============================================================================
// TODO: must deal with active array and combine/average data into on form
// possibly look at a parameter to determine fold-down method
// this method is not overridden by other PolyGenerators, as behavior can be defined completely here
double PolyGenerator :: getValueAtSample(aw::SampleTimeType st)
{
    //sr_ = sys_->getSamplingRate();
    // must call to update value
    // here, return the first value
    // ultimately, create a FoldDown generator with defaultFoldDownMethod_
    return getPolyAtSample(st)[0];
}


// =============================================================================
// this behavior must be specialized for each subclass
aw::WorkingArrayPtr PolyGenerator :: getPolyAtSample(aw::SampleTimeType st)
{
    // basic behavior is to return pointer to double array
    return workingArray_;
};    



} // end namespace aw




// getPolyDepth is defined in Generator
// int Generator :: getPolyDepth()
