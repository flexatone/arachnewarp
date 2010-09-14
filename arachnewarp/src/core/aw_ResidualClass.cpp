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


// =============================================================================
// constructor
ResidualClass::ResidualClass(int m, int s)
    : modulus(m), shift(s)
{
    // setModulusAndShift(m, s);
}

// =============================================================================
// set modulus and shift with integers
void ResidualClass::setModulusAndShift(int m, int s)
{
    modulus = m;
    shift = s;
}    

// =============================================================================
// get modulus and shift with integers
// read-only access
int ResidualClass::getModulus() const
{
    return modulus;
}    

int ResidualClass::getShift() const
{
    return shift;
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

    // see: http://www.parashift.com/c++-faq-lite/misc-technical-issues.html#faq-39.2
    // here we use the >> operator to extract i into x
    std::istringstream i(tokens[0]);
    i >> modulus;

    std::istringstream j(tokens[1]);
    j >> shift;

}    

// =============================================================================
// get modulus and shift as string representation
const std::string ResidualClass::represent()
{
    std::stringstream out;
    out << modulus << "@" << shift; // adds to stream
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

std::vector<int> ResidualClass::segment(int n, std::vector<int> &z)
{
    // create a variable named post that is a vector; init w/ array
    // initialize with zeros
    std::vector<int> post;
    post.reserve(z.size());

    for (int i=0; i<z.size(); i++)
    {
        int nLocal = 0;
        // check that n+shift is w/n modulus
        // not sure why i have to do (int) type casting here
        nLocal = (n + shift) % modulus;
        if (z[i] % modulus == nLocal)
            // should have enough room to append b/c allocated based on 
            // size of z
            post.push_back(z[i]); 
    };
    // we return the vector
    // see http://www.velocityreviews.com/forums/t285675-a-function-which-returns-a-vector.html
    return post;
}    



