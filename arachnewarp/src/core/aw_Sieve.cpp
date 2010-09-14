/*
 *  Sieve.cpp
 *  arachnewarpTest
 *
 *  Created by Christopher Ariza on 7/1/10.
 *  Copyright 2010 Flexatone HFP. All rights reserved.
 *
 */

#include <iostream>
#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>

#include "aw_Sieve.h"
// #include "ResidualClass.h" // in Sieve.h

// =============================================================================
// constructor
Sieve::Sieve(std::string s)
{
    parse(s);
}


// =============================================================================
// parse and load a string representation
void Sieve::parse(std::string s)
{
    inputString = s; // cannot get this to work as a constructor
    // print out input string
    // std::cout << inputString << std::endl;

    // a vector of strings
    std::vector<std::string> tokens;
    // the boost split, assigns values to tokens
    boost::split(tokens, inputString, boost::is_any_of("|"));

    for (int i=0; i<tokens.size(); i++) {
        // std::cout << tokens[i] << std::endl;
        // create a Residual class
        ResidualClass rc;
        rc.setModulusAndShift(tokens[i]);
        // print  modulus and shifts
        // std::cout << a.represent() << std::endl;
        rcStorage.push_back(rc);
    };


//     std::vector<int> post;
//     post.reserve(z.size());
// 
//     for (int i=0; i<z.size(); i++)
//     {
//         int nLocal = 0;
//         // check that n+shift is w/n modulus
//         // not sure why i have to do (int) type casting here
//         nLocal = (n + shift) % modulus;
//         if (z[i] % modulus == nLocal)
//             // should have enough room to append b/c allocated based on 
//             // size of z
//             post.push_back(z[i]); 
//     };

}    



// =============================================================================
// get a string representation
const std::string Sieve::represent()
{
//     std::stringstream out;
// 
//     for (int i=0; i<rcStorage.size(); i++) {
//         // call represent on residual class
//         out << rcStorage[i].represent() << "|" ; 
//     };
//     return out.str(); // returns as a string


    std::vector<std::string> tokens;

    for (int i=0; i<rcStorage.size(); i++) {
        // call represent on residual class
        tokens.push_back(rcStorage[i].represent()); 
    };

    // using boost::join, returns a string
    return boost::join(tokens, "|");
}




// =============================================================================
// second arg is passing an integer vector z by reference
// return a vector
std::vector<int> Sieve::segment(int n, std::vector<int> &z)
{
    // create a variable named post that is a vector; init w/ array
    // initialize with zeros
    std::vector<int> post;
    post.reserve(z.size());

    return post;
}    






