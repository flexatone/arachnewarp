
#include <stdexcept>
#include <iostream>

#include "aw_common.h"



namespace aw {


//==============================================================================
// display

void print(SVT* out, AST size) {
    std::cout << "<array ";
    for (AST i=0; i<size; ++i) {
        std::cout << out[i] << ' ';
    }

    // TODO: figure out how to do this idiom    
    //while (out < out+size) {
        //std::cout << *out++ << ' ';
    //}    
    std::cout << '>' << std::endl;
}


//==============================================================================
// unit interval transformations and value mapping
SVT denormalize_unit(SVT value, SVT a, SVT b) {
    // need to handle value beyond 0 and 1
    if (value < 0 || value > 1) 
        throw std::out_of_range("value must be between 0 and 1"); 
    // if no range, return boundary
    if (a == b) return a;
    // general form: value * (max - min) + max;
    // a is min, b is max
    else if (a < b) return (value * (b - a)) + a;
    // b is min, a is max
    else return (value * (a - b)) + b;   
}



} // end namespace aw



