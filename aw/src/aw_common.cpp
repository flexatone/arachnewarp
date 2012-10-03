
#include <stdexcept>
#include <iostream>

#include "aw_common.h"



namespace aw {


//==============================================================================
// display

void print(SampleType* out, FrameSizeType size) {
    std::cout << "<array ";
    for (FrameSizeType i=0; i<size; ++i) {
        std::cout << out[i] << ' ';
    }

    // TODO: figure out how to do this idiom    
    //while (out < out+size) {
        //std::cout << *out++ << ' ';
    //}    
    std::cout << '>' << std::endl;
}



//void denormalize(SampleType* in, SampleType* out, FrameSizeType size) {
    //for (FrameSizeType i=0; i<size; ++i) {
        //out[i] = in[i] + 1; // temporary 
    //}
//}


} // end namespace aw



