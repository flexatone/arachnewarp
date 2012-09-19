
#include <iostream>
#include <iomanip>

#include "aw_generator.h"


namespace aw {

// have a common base class that does not define functions

Generator :: Generator()
    : _output_frame_dimension(2), _frame_size(10), _frame_count(0) {
    init();
    reset(); // initialize values
}

// dtor
Generator :: ~Generator() {
    delete output;
}

void Generator :: init() {
    std::cout << "calling Generator init" << std::endl;
    // size is always dim * _frame_size
    // only do this once to avoid repeating
    _output_size = _output_frame_dimension * _frame_size;
    output = new SAMPLE_T[_output_size];
}

void Generator :: reset() {
    std::cout << "calling Generator reset" << std::endl;
    // do not need to partion by diminsons
    for (int i=0; i<_output_size; ++i) {
        output[i] = 0.0; // set to zero; float or int?
    }
    // always reset frame count?
    _frame_count = 0;
}


void Generator :: print_output() {
    std::cout << "<output@" << _frame_count << ": ";            
    for (int i=0; i<_output_size; ++i) {
        if (i % _frame_size == 0 and i != 0) {
            std::cout << std::endl << "           ";        
        }
        std::cout << std::setprecision(8) << output[i] << '|';
    }
    std::cout << '>' << std::endl;
}



} // end namespaces aw



