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


//! Model of a Xenakis sieve.
class Sieve
{
public:
    Sieve(std::string s); // constructor

    // parse string
    void parse(std::string s);

    // segment method; return a vector; supply z by reference
    std::vector<int> segment(int n, std::vector<int> &z);

    // get a string version
    const std::string represent();


private:
    std::string inputString;

    // want to store an array of ResidualClasses
    // could strore array of pointers to rc w/ vector<ResidualClass*>
    std::vector<ResidualClass> rcStorage;

    // cannot create just a floating instance
    // ResidualClass q("3@2");
} ;


#endif






