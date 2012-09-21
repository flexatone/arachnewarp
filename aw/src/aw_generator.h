#ifndef _AW_GENERATOR_H_
#define _AW_GENERATOR_H_


#include <vector>
#include <string>

//#include <memory> # only with -std=c++0x
// #include <boost/shared_ptr.hpp>
#include <tr1/memory>

#include "aw_common.h"

namespace aw {



// todo: add mechanism to tag generators

class Generator;
// define shared ptr
typedef std::tr1::shared_ptr<Generator> GeneratorShared;

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
    
    
    //! A std::vector if shared Generators that are the inputs to this function.
    std::vector<GeneratorShared> inputs;

    //! Can store dictionary of input type classes: these can store all sorts of specialized metadata data about the inputs needed for this Generator
    std::vector<std::string> input_names;    
    
    //! A linear array of samples, which may include multiple dimensions in series. This probably should be private.
    SAMPLE_T* output;

    //! Initialize the Generator, allocating the output array.
    void init();
    //! Reset all parameters, and zero out the output array.
    void reset();
    
    //! Print the output buffer for all dimensions at the current sample.
    void print_output();

    //! Render the requested frame if not already rendered
    void render(FRAME_COUNT_T f); 
  
    
};


} // end namespace aw



/*class Constant: public Generator {*/
/*};*/






#endif // ends _AW_GENERATOR_H_
