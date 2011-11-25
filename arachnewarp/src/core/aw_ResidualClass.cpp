/*
 *  ResidualClass.cpp
 *  arachnewarpTest
 *
 *  Created by Christopher Ariza on 6/30/10.
 *  Copyright 2010 Flexatone HFP. All rights reserved.
 *
 */

#include <vector>
#include <algorithm>
#include <string>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "aw_ResidualClass.h"




namespace aw {




// =============================================================================
// constructor
ResidualClass::ResidualClass(int m, int s)
{
    setModulusAndShift(m, s);
}

// =============================================================================
// set modulus and shift with integers
void ResidualClass::setModulusAndShift(int m, int s)
{
    // TODO: add range checking and raise an exception
    modulus_ = m;
    shift_ = s;
}    

// =============================================================================
// get modulus and shift with integers
// read-only access
int ResidualClass::getModulus() const
{
    return modulus_;
}    

int ResidualClass::getShift() const
{
    return shift_;
}   

// =============================================================================
// set modulus and shift with strings
// const std::string& b/c string will not be modified
// & is passing by reference here
void ResidualClass::setModulusAndShift(const std::string& s)
{
    std::vector<std::string> tokens;
    boost::split(tokens, s, boost::is_any_of("@"));

    // TODO: need to check that there are only two elements
    // in tokens; need to be sure that they are numbers

    int mTemp; // temporary storage
    int sTemp; 

    // see: http://www.parashift.com/c++-faq-lite/misc-technical-issues.html#faq-39.2
    // here we use the >> operator to extract i into x

    std::istringstream i(tokens[0]);
    i >> mTemp; // convert to number

    std::istringstream j(tokens[1]);
    j >> sTemp;

    // call method that does boundary checking
    setModulusAndShift(mTemp, sTemp);
}    

// =============================================================================
// get modulus and shift as string representation
const std::string ResidualClass::getString()
{
    std::stringstream out;
    out << modulus_ << "@" << shift_; // adds to stream
    return out.str(); // returns as a string
}

// =============================================================================
// do equality comparison
// this uses this and that object pointers
bool ResidualClass::operator==(ResidualClass& that)
{
    return this->getModulus() == that.getModulus() && this->getShift() == that.getShift(); 
}





// =============================================================================
// second arg is passing an integer vector z by reference
// alternatively, could pass in a vector by reference and update values
// in that vector
std::vector<int> ResidualClass::getSegment(int n, std::vector<int>& z)
{
    // create a variable named post that is a vector; init w/ array
    // initialize with zeros
    std::vector<int> post;
    post.reserve(z.size());

    // for temporary storage of value
    int nLocal = 0;

    // iterate over values in the input z vector
    for (int i=0; i<z.size(); i++)
    {
        // check that n+shift is w/n modulus
        // N is the start value 
        nLocal = (n + shift_) % modulus_;

        // if the current z value taken to the mod is == to nLocal, keep
        if (z[i] % modulus_ == nLocal)
            post.push_back(z[i]); 
    };
    // return the newly created vector
    // see http://www.velocityreviews.com/forums/t285675-a-function-which-returns-a-vector.html
    return post;
}    



} // end namespace aw
