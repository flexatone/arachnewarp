
#include <stdexcept>

#include "aw_common.h"
#include "aw_array_map.h"



namespace aw {

// given an input and output vector, right
// need to find a way to pass in a an array or dictionary of parameters vectors

// it would be nice if these were in a single argument
void denormalize(SAMPLE_T* in, SAMPLE_T* out, FRAME_SIZE_T size) {
    for (FRAME_SIZE_T i=0; i<size; ++i) {
        out[i] = in[i] + 1; // temporary 
    }
}
    

} // end namespace aw




