
#include <stdexcept>
#include <iostream>

#include "aw_common.h"



namespace aw {


//==============================================================================
// display

void print(SAMPLE_T* out, FRAME_SIZE_T size) {
    std::cout << "<array ";
    for (FRAME_SIZE_T i=0; i<size; ++i) {
        std::cout << out[i] << ' ';
    }

    // TODO: figure out how to do this idiom    
    //while (out < out+size) {
        //std::cout << *out++ << ' ';
    //}    
    std::cout << '>' << std::endl;
}



//void denormalize(SAMPLE_T* in, SAMPLE_T* out, FRAME_SIZE_T size) {
    //for (FRAME_SIZE_T i=0; i<size; ++i) {
        //out[i] = in[i] + 1; // temporary 
    //}
//}


} // end namespace aw



