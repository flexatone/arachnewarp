
#include <stdexcept>

#include "aw_common.h"



namespace aw {


//==============================================================================
// unit interval transformations and value mapping
double denormalize(double value, double a, double b) {
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



