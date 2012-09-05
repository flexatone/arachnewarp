

// #ifndef AW_COMMON_H
// #define AW_COMMON_H


#include <string>
#include <vector>
#include <stdint.h> // has uint32_t



namespace aw {

double const pi(3.14159265358979323846264338);
double const pi2(3.14159265358979323846264338*2.0);



//! Denormalize a value within the unit interval to the range specified by a and b.
double denormalize (double value, double a=0, double b=1);



} // end namespace

