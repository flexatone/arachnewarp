#ifndef _AW_COMMON_H_
#define _AW_COMMON_H_


#include <string>
#include <vector>

#include <tr1/cstdint> // has uint32_t



namespace aw {

//enumerations =================================================================




//typedefs =====================================================================

// define the sample format
typedef double SAMPLE_T; // sample value type

// frame size type: probadbly under 10,000, 0 to 65535
typedef std::tr1::uint16_t FRAME_SIZE_T;

// dimension probably never more than 200!
typedef std::tr1::uint8_t FRAME_DIM_T; 

typedef std::tr1::uint8_t UINT8_T; 

// needs to be very large
typedef std::tr1::uint64_t FRAME_COUNT_T; 

// never have more thean 200 parameter types
typedef std::tr1::uint8_t PARAMETER_INDEX_T; 

SAMPLE_T  const PI(3.14159265358979323846264338);
SAMPLE_T const PI2(3.14159265358979323846264338*2.0);

//! Defined for all hierarchical displays. 
std::tr1::uint8_t const INDENT_SIZE(2);


// functions ===================================================================
//! Print an arry of SAMPLE_T of size type FRAME_SIZE_T.
void print(SAMPLE_T* out, FRAME_SIZE_T size);


//std::string gen_id_to_name(GeneratorID q);
//GeneratorID gen_name_to_id(const std::string& q);



} // end namespace
#endif // ends _AW_COMMON_H_
















