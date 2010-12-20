/*! \file aw_RandomUniform.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_RESIDUAL_CLASS_H
#define AW_RESIDUAL_CLASS_H

#include <vector>
#include "aw_Common.h"




namespace aw {


//! Model of a Xenakis residual class.
class ResidualClass
{
public:
    ResidualClass(int m=1, int s=1); // constructor

    void setModulusAndShift(int m=1, int s=1);
    // set a string version
    void setModulusAndShift(const std::string &s);


    // get modulus and shift, const delivers read-only access
    int getModulus() const;
    int getShift() const;

    // equality comparison
    // note: this seems like it should be const ResidualClass
    bool operator==(ResidualClass& that);

    // get a string version
    const std::string represent();


    // return a vector
    std::vector<int> segment(int n, std::vector<int> &z);

private:
    int modulus;
    int shift;

} ; // end class ResidualClass


} // end namespace aw


#endif






