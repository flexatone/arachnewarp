#ifndef _AW_COMMON_H_
#define _AW_COMMON_H_


#include <string>
#include <vector>

#include <tr1/cstdint> // has uint32_t



namespace aw {

// define the sample format
typedef double SAMPLE_T; // sample value type

// frame size type: probadbly under 10,000, 0 to 65535
typedef std::tr1::uint16_t FRAME_SIZE_T;
// dimension probably never more than 200!
typedef std::tr1::uint8_t FRAME_DIM_T; 
// needs to be very large
typedef std::tr1::uint64_t FRAME_COUNT_T; 


SAMPLE_T const PI(3.14159265358979323846264338);
SAMPLE_T const PI2(3.14159265358979323846264338*2.0);


//! Print an arry of SAMPLE_T of size type FRAME_SIZE_T.
void print(SAMPLE_T* out, FRAME_SIZE_T size);

//! Denormalize a value within the unit interval to the range specified by a and b.
SAMPLE_T denormalize_unit(SAMPLE_T value, SAMPLE_T a=0, SAMPLE_T b=1);



} // end namespace


#endif // ends _AW_COMMON_H_




