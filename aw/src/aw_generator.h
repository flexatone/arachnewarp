#ifndef _AW_GENERATOR_H_
#define _AW_GENERATOR_H_


#include <memory>
#include <vector>
#include <string>

#include "aw_common.h"

namespace aw {


class Generator;
// define shared ptr
typedef std::shared_ptr<Generator> GeneratorShared;

class Generator {
    private:    
    FRAME_DIM_T _output_frame_dimension;
    FRAME_SIZE_T _output_size; // can use frame size, as is 16 bit
    FRAME_SIZE_T _frame_size; // if changed, need to rebuild output
    
    //! The number of rames that have passed since the last reset
    FRAME_COUNT_T _frame_count;
    

    public:
    Generator();
    ~Generator();
    


    // gen routine is defined in derived class and is not inherited
    // need a variable list of input, each of which is a generator
    
    //! A std::vector if shared Generators that are the inputs to this function.
    std::vector<GeneratorShared> inputs;

    //! Can store dictionary of input type classes: these can store all sorts of specialized metadata data about the inputs needed for this Generator
    std::vector<std::string> input_names;    
    
    //! A linear array of samples, which may include multiple dimensions in series. 
    SAMPLE_T* output;

    //! Initialize the Generator, allocating the output array.
    void init();
    //! Reset all parameters, and zero out the output array.
    void reset();
    
    //! Print the output buffer for all dimensions at the current sample.
    void print_output();
    
};


} // end namespace aw


#endif // ends _AW_GENERATOR_H_
