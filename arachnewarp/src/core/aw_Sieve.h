/*! \file aw_Sieve.cpp
    \brief The Xenakis Sieve
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/



#ifndef AW_SIEVE_H
#define AW_SIEVE_H

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

#include "aw_ResidualClass.h"
#include "aw_Common.h"



//
// possible binary output through STL #include <bitset>
// also boost::dynamic_bitset<>

// STL set_union(), set_intersection(), and set_difference()

// this object may be used in a polygenerator with a refresh attribute

// Create and return Segment objects that wrap a vector, can be used for z and other segments, can return alternative representations (binary, width, etc)

namespace aw {

class Sieve; // forward declaration
typedef boost::shared_ptr<Sieve> SievePtr;


    //! Model of a Xenakis sieve.
    class Sieve
    {
    public:
        Sieve(std::string s); // constructor
    
        // parse string
        void readString(std::string s);
    
        // segment method; return a vector; supply z by reference
        std::vector<int> getSegment(int n, std::vector<int>& z);
    
        // get a string version
        const std::string getString();
    
    
    private:
        std::string inputString_;
    
        //! Internal storage for residual classes
        std::vector<ResidualClassPtr> rcStorage_;
    
        // cannot create just a floating instance
        // ResidualClass q("3@2");
    } ; // end class Sieve



} // end namespace aw


#endif






