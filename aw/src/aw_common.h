#ifndef _AW_COMMON_H_
#define _AW_COMMON_H_


#include <string>
#include <vector>

#include <stdint.h> // has uint32_t



namespace aw {

// define the sample format
typedef double SVT; // sample value type
typedef int AST; // arary size type: TODO: use a smaller data size


SVT const pi(3.14159265358979323846264338);
SVT const pi2(3.14159265358979323846264338*2.0);


//! Print an arry of SVT of size type AST.
void print(SVT* out, AST size);

//! Denormalize a value within the unit interval to the range specified by a and b.
SVT denormalize_unit(SVT value, SVT a=0, SVT b=1);



} // end namespace


#endif // ends _AW_COMMON_H_




