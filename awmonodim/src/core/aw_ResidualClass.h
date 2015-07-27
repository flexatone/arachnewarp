/*! \file aw_RandomUniform.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_RESIDUAL_CLASS_H
#define AW_RESIDUAL_CLASS_H

#include <vector>
#include <boost/shared_ptr.hpp>

#include "aw_Common.h"




namespace aw {

class ResidualClass; // forward declaration
typedef boost::shared_ptr<ResidualClass> ResidualClassPtr;


    //! Model of a Xenakis residual class.
    class ResidualClass
    {
    public:
    
        //! Create a residual class with two integers for modulus and shift. Defaults are provided
        ResidualClass(int m=1, int s=1); // constructor
    
        //! Set modulus and shift with two integer arguments
        void setModulusAndShift(int m=1, int s=1);
    
        //! Read a modulus and shift from a string based by reference.
        void setModulusAndShift(const std::string &s);
    
        //! Return the modulus.
        int getModulus() const;
    
        //! Return the shift.
        int getShift() const;
    
        //! Equality comparison
        bool operator==(ResidualClass& that);
    
        //! Return a string representation
        const std::string getString();
    
    
        // return a vector
        std::vector<int> getSegment(int n, std::vector<int>& z);
    
    private:
        int modulus_;
        int shift_;
    
    } ; // end class ResidualClass


} // end namespace aw


#endif






