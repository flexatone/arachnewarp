/*! \file aw_WaveSquare.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_POLY_TABLE_H
#define AW_POLY_TABLE_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "aw_Generator.h"
#include "aw_PolyGenerator.h"
#include "aw_Common.h"




namespace aw {


class PolyTable; // forward declarator
typedef boost::shared_ptr<PolyTable> PolyConstantPtr;

//! PolyGenerator of a constant list of values.
class PolyTable: public aw::PolyGenerator 
{
public:

    PolyTable(aw::SystemPtr sys); 

    ~PolyTable();

    std::string getName();

    std::string getParameterString(bool compact=false);


    void init();

    void reset();

    //! overridden from Generator
    void setParameter(const aw::ParameterName p, 
                      std::vector<double>& v, 
                      aw::ParameterContext pc=aw::pContextNameNone);

    //! not found in any base class:
    void setParameter(const aw::ParameterName p, 
                      std::string& vString, 
                      aw::ParameterContext pc=aw::pContextNameNone);

    //! not found in any base class:
    void setParameter(const aw::ParameterName p, 
                      char* const vCharArray, 
                      aw::ParameterContext pc=aw::pContextNameNone);

    //! overload Generator form
    void setParameter(std::string& pString, 
                      std::string& vString, 
                      std::string& pcString);


    // overload Generator form
    void readParameterString(std::string& ps);


    aw::WorkingArrayPtr getPolyAtSample(aw::SampleTimeType);


private:



    // Internal storage for values is a double array; these values are loaded into workingArray_ for each call. 

    // if an array pointer
    // aw::WorkingArrayPtr value_;

    // old approach: statically defined size
    //double value_[aw::defaultPolyDepthAllocated];

    // as vector: safest
    std::vector<double> value_;

   
} ; // end class PolyTable



} // end namespace aw



#endif



