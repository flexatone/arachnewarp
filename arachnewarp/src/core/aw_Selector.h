/*! \file aw_WaveSquare.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_SELECTOR_H
#define AW_SELECTOR_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "aw_Generator.h"
#include "aw_PolyGenerator.h"


class Selector; // forward declarator

typedef boost::shared_ptr<Selector> SelectorPtr;

//! Generator that selects values from a PolyGenerator providing a value list. 
class Selector: public Generator 
{
public:

    Selector(SystemPtr sys); 

    ~Selector();

    std::string getName();

    void init();

    void reset();

    double getValueAtSample(aw::SampleTimeType st);


private:

    double sampleTime_;

    //! Store when the output value was last refreshed; based on the refresh Generator.
    aw::SampleTimeType sampleTimeLastRefresh_;

    //! Stores the period derived from the rate generator
    double periodSamples_;

    //! Stores the last-selected value
    double value_; 

    //! Store the selection method
    double selectionMethod_; 
    
    // store direction shift
    int stride_;
    // get as a double; will need to round to an int
    int strideMagnitude_;

    // need to store index values used
    // know that the max number of value to be selected from is 
    // std::vector<double> stockpile_;

    // index value to access values
    // as 1_ is negative at times, all values need to be unsigned for comparison
    int i_; 
    int srcSize_; 
    //! Store last size for comparison
    int srcSizeLast_; 

    // pointer to working array is obtained from stored PolyConstant
    double* workingArray_;
        
   
} ;



#endif


