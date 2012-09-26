
#include <iostream>
#include <iomanip>

#include "aw_generator.h"


namespace aw {

//==============================================================================
ParameterType :: ParameterType() {
}

ParameterType :: ~ParameterType() {
}

std::ostream& operator<<(std::ostream& output, const ParameterType& pt) {
    output << "<PmtrType: " << pt._instance_name << ">";
    return output; 
}



//==============================================================================
// have a common base class that does not define functions

Generator :: Generator()
    : _output_frame_dimension(2), _frame_size(20), _frame_count(0), _input_parameter_count(0), output(NULL) {

    _class_name = "Generator"; // will be reset in derived classes
    _resize_output();
    _init();
    reset(); // initialize values

}

// dtor
Generator :: ~Generator() {
    delete[] output; // remove dyn alloc memeory
}

void Generator :: _init() {
    std::cout << *this << ": init()" << std::endl;
}

void Generator :: _resize_output() {
    std::cout << "calling " << *this << " _resize_output" << std::endl;
    // must delete if set to something else
    if (output != NULL) delete[] output;
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

std::ostream& operator<<(std::ostream& output, const Generator& g) {
    output << "<Gen: " << g._class_name << ">";
    return output; 
}

void Generator :: print_output() {
    // provide name of generator first by dereferencing this
    std::cout << *this << " Output@" << _frame_count << ": ";
    for (int i=0; i<_output_size; ++i) {
        if (i % _frame_size == 0) {
            std::cout << std::endl << "    ";        
        }
        std::cout << std::setprecision(8) << output[i] << '|';
    }
    std::cout << std::endl;
}

void Generator :: print_inputs(bool recursive, UINT8_T recurse_level) {
    //std::cout << "recurse_level: " << (int)recurse_level << std::endl;
    
    std::string space1 = std::string(recurse_level*4, ' ');
    std::string space2 = std::string((recurse_level+1)*4, ' '); 
    std::string space3 = std::string((recurse_level+2)*4, ' '); 
    
    std::cout << space1 << *this << " Inputs:" << std::endl;
    VGeneratorShared :: const_iterator j;
    // need an interger as key for _input_parameter_type
    for (PARAMETER_INDEX_T k=0; k!=_inputs.size(); ++k) {   
        ParameterTypeShared pts = _input_parameter_type[k];
        // need to iterate over each sub vector
        std::cout << space2 << *pts << std::endl;
        for (j=_inputs[k].begin(); j!=_inputs[k].end(); ++j) {
            // deref
            GeneratorShared g = *j;
            if (g == NULL) { // an empty GeneratorShared
                std::cout << space3 << "<Empty>" << std::endl;                
            }
            else {
                if (recursive) {
                    g->print_inputs(recursive, recurse_level+2);
                }
                else { // need to deref the shared generator
                    std::cout << "        " << *g << std::endl;
                }
            }
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


void Generator :: _register_input_parameter_type(ParameterTypeShared pts) {
    // storing in an unordered map
    _input_parameter_type[_input_parameter_count] = pts;

    // put a place holder for the GeneratorShared here; empty
    // does not alloc
    // Note: this makes many copies, but is a very small reference obj
    VGeneratorShared vInner;  
    //vInner.push_back(GeneratorShared());
    _inputs.push_back(vInner); // extr copy made here
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
    _inputs[i].clear();
    _inputs[i].push_back(gs);    
}

// TODO: create a Constant
// overridden method for setting a sample value: generates a constant
void Generator :: set_parameter_by_index(PARAMETER_INDEX_T i, 
                                        SAMPLE_T v){
    _inputs[i].clear();
    // TODO: create a constant at value v
    _inputs[i].push_back(GeneratorShared());    
}


void Generator :: add_parameter_by_index(PARAMETER_INDEX_T i, 
                                        GeneratorShared gs){
    _inputs[i].push_back(gs);    
}

// TODO: create a Constant
// overridden method for setting a sample value: adds a constant
void Generator :: add_parameter_by_index(PARAMETER_INDEX_T i, 
                                        SAMPLE_T v){
    _inputs[i].clear();
    _inputs[i].push_back(GeneratorShared());    
}



//==============================================================================
Constant :: Constant() {
    _class_name = "Constant"; 
    // Generator :: _init auto called
    _init(); // call constants init
}
// dtor
Constant :: ~Constant() {
}

// the int routie must configure the names and types of parameters
void Constant :: _init() {
    std::cout << *this << ": init()" << std::endl;
    // register some parameters
    aw::ParameterTypeValueShared pt1 = aw::ParameterTypeValueShared(new 
                                       aw::ParameterTypeValue);
    pt1->set_name("Constant numerical value");
    // when this is called, the inputs vectors are filled with GeneratorShared
    // these will the inputs vector will be filled with 1 empty placeholder
    _register_input_parameter_type(pt1);
    // by default, add a zero value
    _values.push_back(0);    
    
}

void Constant :: reset() {
    std::cout << "calling "<< *this << " reset" << std::endl;
    // sum values first, then rest all
    SAMPLE_T v(0);
    
    VSample :: const_iterator j;
    for (j = _values.begin(); j!=_values.end(); ++j) {
        v += *j;
    }
    // do not need to partion by diminsons
    for (int i=0; i<_output_size; ++i) {
        output[i] = v; // set to the value
    }
    // always reset frame count?
    _frame_count = 0;
}


void Constant :: print_inputs(bool recursive, UINT8_T recurse_level) {
    std::string space1 = std::string(recurse_level*4, ' ');
    std::string space2 = std::string((recurse_level+1)*4, ' '); 
    std::string space3 = std::string((recurse_level+2)*4, ' ');     
    // this cannot recurse so nothing to do with parameters; just so that
    // they are the same as Generator
    std::cout << space1 << *this << " Inputs:" << std::endl;
    // the size here is correct
    for (PARAMETER_INDEX_T k=0; k!=_inputs.size(); ++k) {   
        ParameterTypeShared pts = _input_parameter_type[k];
        std::cout << space2 << *pts << std::endl;
        // need to iterate over values in _values
        VSample :: const_iterator j;
        for (j = _values.begin(); j!=_values.end(); ++j) {
            std::cout << space3 << "<Value: " << *j << " >" << std::endl;
        }
    }
}

void Constant :: render(FRAME_COUNT_T f) {
    // do not need to iterate, as we are always at the necessary value
    _frame_count = f;
}

void Constant :: set_parameter_by_index(PARAMETER_INDEX_T i, SAMPLE_T v){
    _values.clear();
    _values.push_back(v);    
    reset(); 
}

void Constant :: add_parameter_by_index(PARAMETER_INDEX_T i, SAMPLE_T v){
    _values.push_back(v);    
    reset();  
}





//==============================================================================
Add :: Add()
    : _input_index_num_value(0), _sum(0) {
    _class_name = "Add"; 
    _init();
}
// dtor
Add :: ~Add() {
}

// the int routie must configure the names and types of parameters
void Add :: _init() {
    std::cout << *this << ": init()" << std::endl;
    // register some parameters
    aw::ParameterTypeValueShared pt1 = aw::ParameterTypeValueShared(new 
                                       aw::ParameterTypeValue);
    pt1->set_name("Opperands");
    _register_input_parameter_type(pt1);
    _input_index_num_value = 0; // stored for speed
}


void Add :: render(FRAME_COUNT_T f) {

    VGeneratorShared :: const_iterator j; // vector of generators

    while (_frame_count < f) {
        // for each of the inputs, need to render up to this frame
        for (j=_inputs[_input_index_num_value].begin(); 
             j!=_inputs[_input_index_num_value].end(); ++j) {
            // this is a shared generator
            (*j)->render(f);
        }
        // i is the position in output; assume that all outputs are the same 
        // size. 
        for (int i=0; i<_output_size; ++i) {
            _sum = 0;
            for (j=_inputs[_input_index_num_value].begin(); 
                 j!=_inputs[_input_index_num_value].end(); ++j) {
                _sum += (*j)->output[i];
            }
            output[i] = _sum; 
        }
        _frame_count += 1;
    }    
}


} // end namespaces aw



