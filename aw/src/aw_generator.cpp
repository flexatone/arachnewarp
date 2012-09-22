
#include <iostream>
#include <iomanip>

#include "aw_generator.h"


namespace aw {

//==============================================================================
ParameterType :: ParameterType() {
}

ParameterType :: ~ParameterType() {
}



//==============================================================================
// have a common base class that does not define functions

Generator :: Generator()
    : _output_frame_dimension(2), _frame_size(20), _frame_count(0), 
    _input_parameter_count(0), output(NULL) {
    // always create this first
    _resize_output();
    _init();
    reset(); // initialize values
}

// dtor
Generator :: ~Generator() {
    delete output; // remove dyn alloc memeory
}

void Generator :: _init() {
    std::cout << "calling Generator init" << std::endl;
}

void Generator :: _resize_output() {
    std::cout << "calling Generator _resize_output" << std::endl;
    // must delete if set to something else
    if (output != NULL) delete output;
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

void Generator :: render(FRAME_COUNT_T f) {
    while (_frame_count < f) {
        // do a processing run and increment the frame count
        for (int i=0; i<_output_size; ++i) {
            // sum of previous values
            output[i] = output[i] + .2; 
        }
        _frame_count += 1;
    }
}


void Generator :: _register_input_parameter_type(ParameterTypeShared pts) {
    _input_parameter_type[_input_parameter_count] = pts;
    // put a place holder for the GeneratorShared here; empty
    // does not alloc
    inputs.push_back(GeneratorShared());
    _input_parameter_count += 1;
}


PARAMETER_INDEX_T Generator :: get_parameter_index_from_name(
    const std::string& s) {
    Generator :: MapIndexToParameterTypeShared ::const_iterator k;
    for (k=_input_parameter_type.begin(); 
        k != _input_parameter_type.end(); 
        ++k) {
        // each value (second( is a ParameterTypeShared)
        if (s == k->second->get_name()) return k->first;
    }
    return 200; // raise an exception, or return error code?
}


void Generator :: set_parameter_by_index(PARAMETER_INDEX_T i, 
                                        GeneratorShared gs){
    // should _input_parameter_type be examined for anything?
    // could check value types or range
    // _input_parameter_type[i]

    // a given input can be examined here to determine if dimensionality 
    // should change

    // assign
    inputs[i] = gs;    
}



//==============================================================================
Constant :: Constant() {
    // base _init auto called
    _init(); // call constants init
}
// dtor
Constant :: ~Constant() {
}

void Constant :: _init() {
    std::cout << "Constant::init()" << std::endl;
    // register some parameters
    aw::ParameterTypeValueShared pt1 = aw::ParameterTypeValueShared(new 
                                  aw::ParameterTypeValue);
    pt1->set_name("Constant numerical value");
    _register_input_parameter_type(pt1);

}




} // end namespaces aw



