
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
    : _output_frame_dimension(2), _frame_size(20), _frame_count(0), _input_parameter_count(0), output(NULL) {

    _name = "Generator"; 
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
    // in derived classes this can prepare parameters
    std::cout << "calling "<< *this << " init" << std::endl;
}

void Generator :: _resize_output() {
    std::cout << "calling " << *this << " _resize_output" << std::endl;
    // must delete if set to something else
    if (output != NULL) delete output;
    // size is always dim * _frame_size
    // only do this once to avoid repeating
    _output_size = _output_frame_dimension * _frame_size;
    output = new SAMPLE_T[_output_size];
}

void Generator :: reset() {
    std::cout << "calling "<< *this << " reset" << std::endl;
    // do not need to partion by diminsons
    for (int i=0; i<_output_size; ++i) {
        output[i] = 0.0; // set to zero; float or int?
    }
    // always reset frame count?
    _frame_count = 0;
}

std::ostream &operator<<(std::ostream& output, const Generator& g) {
    output << "<Gen: " << g._name << ">";
    return output; 
}

void Generator :: print_output() {
    // provide name of generator first by dereferencing this
    std::cout << *this << std::endl;
    // TODO: estimate string size for uniform spacing
    std::cout << "<output@" << _frame_count << ": ";            
    for (int i=0; i<_output_size; ++i) {
        if (i % _frame_size == 0 and i != 0) {
            std::cout << std::endl << "           ";        
        }
        std::cout << std::setprecision(8) << output[i] << '|';
    }
    std::cout << '>' << std::endl;
}

void Generator :: print_inputs() {
    // provide name of generator first by dereferencing this
    std::cout << *this << " inputs:" << std::endl;

    VVGeneratorShared :: const_iterator k;
    VGeneratorShared :: const_iterator j;

    for (k=inputs.begin(); k!=inputs.end(); ++k) {
        // need to iterate over each sub vector
        std::cout << "    " << "input" << std::endl;
        VGeneratorShared sub = *k; // get vector handle
        for (j=sub.begin(); j!=sub.end(); ++j) {
            // deref
            GeneratorShared g = *j;
            // deref one more time to print
            std::cout << "        " << g << std::endl;
        }
    }
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


// this is a critical method in that it both adds a parameter type instance but also sets up the inputs vectors
void Generator :: _register_input_parameter_type(ParameterTypeShared pts) {

    // storing in an unordered map
    _input_parameter_type[_input_parameter_count] = pts;

    // put a place holder for the GeneratorShared here; empty
    // does not alloc
    // Note: this makes many copies, but is a very small reference obj
    std::vector<GeneratorShared> vInner;  
    vInner.push_back(GeneratorShared());
    inputs.push_back(vInner);
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

    // this removes all stored values
    inputs[i].clear();
    inputs[i].push_back(gs);    
}


void Generator :: add_parameter_by_index(PARAMETER_INDEX_T i, 
                                        GeneratorShared gs){
    inputs[i].push_back(gs);    
}


//==============================================================================
Constant :: Constant() 
    : _value(0) {
    _name = "Constant"; 
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

    // when this is called, the inputs vectors are filled with GeneratorShared
    _register_input_parameter_type(pt1);

}




} // end namespaces aw



