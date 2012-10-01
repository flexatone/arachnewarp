
#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "aw_generator.h"


namespace aw {

//==============================================================================
ParameterType :: ParameterType() {
    _class_name = "ParameterType";
}

ParameterType :: ~ParameterType() {
}

std::ostream& operator<<(std::ostream& output, const ParameterType& pt) {
    output << "<Pmtr: " << pt._class_name << ": " << pt._instance_name << ">";
    return output; 
}


//==============================================================================
ParameterTypeValue :: ParameterTypeValue() {
    _class_name = "ParameterTypeValue";
}

ParameterTypeValue :: ~ParameterTypeValue() {
}




//==============================================================================
GeneratorShared  Generator :: make(GeneratorID q){
    // default will be an empty shared generator
    aw::GeneratorShared g;    

    if (q == ID_Constant) {
        g = ConstantShared(new Constant);
    } 
    else if (q == ID_Add) {
        g = AddShared(new Add);    
    }
    else {
        throw std::invalid_argument("no matching GeneratorID: " + q);
    }
    // automatically call init
    g->init();
    return g;
}



Generator :: Generator()
    : _class_name("Generator"), _output_frame_dimension(2), _frame_size(20), _resizable_output(true), 
    _frame_count(0), 
    _input_parameter_count(0), output(NULL) {
    // constructor
        
    // do not call virtual functions in constructors!!!

}

Generator :: ~Generator() {
    // deconstructor 
    // if gen was not initialized, do not delete
    if (output != NULL) delete[] output;
}


void Generator :: init() {
    // virtual method, specialized in subclass; 
    // this base class method is explicitly called when needed by base classes
    std::cout << *this << " Generator::init()" << std::endl;
    
    // since this might be called more than once in the life of an object, need to repare storage units that are general set in the init() routine
    _input_parameter_type.clear();
    _inputs.clear();
    _input_parameter_count = 0;
    
    // _register_input_parameter_type is called to add inputs here
    _resize_output();
    reset(); // zero output; zero frame count
}

void Generator :: _resize_output() {
    // _output_size is set here
    std::cout << *this << " Generator::_resize_output()" << std::endl;
    // must delete if set to something else
    if (output != NULL) delete[] output;
    // size is always dim * _frame_size
    // only do this once to avoid repeating
    _output_size = _output_frame_dimension * _frame_size;
    output = new SAMPLE_T[_output_size];
    // TODO: check that alloc was sucesful
    if (output == NULL) {
        throw std::bad_alloc();    
    }
}


void Generator :: _register_input_parameter_type(ParameterTypeShared pts) {
    // storing in an unordered map
    _input_parameter_type[_input_parameter_count] = pts;

    // store a vector in the position to accept inputs
    V_GEN_SHARED_T vInner;  
    // a place holder is not necessary
    //vInner.push_back(GeneratorShared());
    _inputs.push_back(vInner); // extr copy made here
    
    // add vector to output size as well 
    V_FRAME_DIM_T vSizeInner;  
    _inputs_output_size.push_back(vSizeInner);
    
    _input_parameter_count += 1;
}



FRAME_DIM_T Generator :: _find_max_input_dimension(FRAME_DIM_T d) {
    // pass in max value found so far
    FRAME_DIM_T dSub(1);
    for (PARAMETER_INDEX_T i = 0; i<_input_parameter_count; ++i) {
        // inputs are a vector of Generators        
        for (PARAMETER_INDEX_T j=0; j<_inputs[i].size(); ++j) {
            dSub = _inputs[i][j]->_find_max_input_dimension(d);
            if (dSub > d) {
                d = dSub;
            }
        }
    }    
    return d;
}


void Generator :: _update_for_new_input() {
    // get all the output sizes of all inputs to reduce function calls during rendering.     
    // not using iterators here os as to be able to ref both VVs
    for (PARAMETER_INDEX_T i = 0; i<_input_parameter_count; ++i) {
        // inputs are a vector of Generators        
        for (PARAMETER_INDEX_T j=0; j<_inputs[i].size(); ++j) {
            _inputs_output_size[i][j] = _inputs[i][j]->get_output_size();
        }
    } 
    
    // this is recursive, which previous methos are not:
    FRAME_DIM_T maxDim = _find_max_input_dimension();
    std::cout << *this << " Found max input dim: @" << (int)maxDim << std::endl;
}





void Generator :: _render_inputs(FRAME_COUNT_T f) {
    // for each of the inputs, need to render up to this frame
    // this is "pulling" lower-level values up to date
    // this seems somewhat wasteful as in most cases we just will be advancing by 1 more than the previous frame count value 
    // this is inlined in the header
    V_GEN_SHARED_T :: const_iterator j; // vector of generators    
    for (PARAMETER_INDEX_T i = 0; i<_input_parameter_count; ++i) {
        // inputs are a vector of Generators
        for (j=_inputs[i].begin(); 
             j!=_inputs[i].end(); ++j) {
            // this is a shared generator
            (*j)->render(f);
        }
    }
}


void Generator :: set_dimension(FRAME_DIM_T d) {
    // only change if different; assume already created
    if (d != _output_frame_dimension) {
        _output_frame_dimension = d;
        _resize_output(); // _output_size is set here with dimension
        reset(); // must reset values to zero 
    }
    // when we set the dimension, should we set it for inputs?
}


void Generator :: reset() {
    std::cout << *this << " Generator::reset()" << std::endl;
    // do not need to partion by diminsons
    for (int i=0; i<_output_size; ++i) {
        output[i] = 0.0; // set to zero; float or int?
    }
    // always reset frame count?
    _frame_count = 0;
}

std::ostream& operator<<(std::ostream& output, const Generator& g) {
    output << "<Gen: " << g._class_name << " @" << 
        (int)g._output_frame_dimension << ">";
    return output; 
}

void Generator :: print_output() {
    // provide name of generator first by dereferencing this
    std::string space1 = std::string(INDENT_SIZE*2, ' ');
    
    std::cout << *this << " Output@" << _frame_count << ": ";
    for (int i=0; i<_output_size; ++i) {
        if (i % _frame_size == 0) {
            std::cout << std::endl << space1;        
        }
        std::cout << std::setprecision(8) << output[i] << '|';
    }
    std::cout << std::endl;
}

void Generator :: print_inputs(bool recursive, UINT8_T recurse_level) {
    //std::cout << "recurse_level: " << (int)recurse_level << std::endl;
    
    std::string space1 = std::string(recurse_level*INDENT_SIZE, ' ');
    std::string space2 = std::string((recurse_level+1)*INDENT_SIZE, ' '); 
    std::string space3 = std::string((recurse_level+2)*INDENT_SIZE, ' '); 
    
    std::cout << space1 << *this << " Inputs:" << std::endl;

    V_GEN_SHARED_T :: const_iterator j;
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
    // this is a dummy render method just for demonstartion
    _frame_count += 1;
}


PARAMETER_INDEX_T Generator :: get_parameter_index_from_name(
    const std::string& s) {
    // match the string to the name returned by get_instance_name; 
    // this the instance name
    Generator :: MapIndexToParameterTypeShared ::const_iterator k;
    for (k=_input_parameter_type.begin(); 
        k != _input_parameter_type.end(); 
        ++k) {
        // each value (second( is a ParameterTypeShared)
        if (s == k->second->get_instance_name()) return k->first;
    }
    // TODO:
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
    _inputs_output_size[i].clear();
    
    _inputs[i].push_back(gs);    
    _inputs_output_size[i].push_back(gs->get_output_size());
    
    _update_for_new_input();
}

void Generator :: set_parameter_by_index(PARAMETER_INDEX_T i, 
                                        SAMPLE_T v){
    // overridden method for setting a value: generates a constant
    
	aw::GeneratorShared c = aw::ConstantShared(new aw::Constant);
    c->init();
    c->set_parameter_by_index(0, v); // this will call Constant::reset()

    set_parameter_by_index(i, c); // other overloaded
//    _inputs[i].clear();    
//    _inputs[i].push_back(c);    
//    _inputs_output_size[i].push_back(c->get_output_size());
    
}


void Generator :: add_parameter_by_index(PARAMETER_INDEX_T i, 
                                        GeneratorShared gs){
    // adding additiona, with a generator
    _inputs[i].push_back(gs);    
    _inputs_output_size[i].push_back(gs->get_output_size());    
    
    _update_for_new_input();
    
}

void Generator :: add_parameter_by_index(PARAMETER_INDEX_T i, 
                                        SAMPLE_T v){
    // adding additional as a constant value
    // note that no one else will have a handle on this constatn
    // overridden method for setting a sample value: adds a constant

	aw::GeneratorShared c = aw::ConstantShared(new aw::Constant);
    c->init();
    c->set_parameter_by_index(0, v); // this will call Constant::reset()
    
    add_parameter_by_index(i, c); // other overloaded
    
//    _inputs[i].push_back(c);    
//    _inputs_output_size[i].push_back(c->get_output_size());

    
}




//==============================================================================
Constant :: Constant() {
    _class_name = "Constant"; 
}
// dtor
Constant :: ~Constant() {
}

FRAME_DIM_T Constant :: _find_max_input_dimension(FRAME_DIM_T d) {
    // overriden virtual method b/c this is terminal of recusive search
    // just return dimensionality
    return get_dimension();
}


// the int routie must configure the names and types of parameters
void Constant :: init() {
    std::cout << *this << " Constant::init()" << std::endl;
    // resize and reset after setting parameters
    // this will call the virtual reset 
    Generator::init();
    
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
    // need overidden reset because have to transfer stored value to the output array; normal reset sets the output to zer. 
    std::cout << *this << " Constant::reset()" << std::endl;
    // sum values first, then reseset all output to values
    SAMPLE_T v(0);
    VV_SAMPLE_T :: const_iterator j;
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
    // need overridden print because this is terminal in recursive searches
    std::string space1 = std::string(recurse_level*INDENT_SIZE, ' ');
    std::string space2 = std::string((recurse_level+1)*INDENT_SIZE, ' '); 
    std::string space3 = std::string((recurse_level+2)*INDENT_SIZE, ' ');     
    // this cannot recurse so nothing to do with parameters; just so that
    // they are the same as Generator
    std::cout << space1 << *this << " Inputs:" << std::endl;
    // iterative over inputs
    for (PARAMETER_INDEX_T k=0; k!=_inputs.size(); ++k) {   
        // is this doing a copy?
        ParameterTypeShared pts = _input_parameter_type[k];
        std::cout << space2 << *pts << std::endl;
        // need to iterate over stored _values
        VV_SAMPLE_T :: const_iterator j;
        for (j = _values.begin(); j!=_values.end(); ++j) {
            std::cout << space3 << "Value: " << *j << std::endl;
        }
    }
}

void Constant :: render(FRAME_COUNT_T f) {
    // do not need to iterate, as the output array always stores the proper values
    _frame_count = f;
}







void Constant :: set_parameter_by_index(PARAMETER_INDEX_T i, 
                                        GeneratorShared gs) {
    throw std::invalid_argument("invalid to set a GeneratoreShared as a value to a Constatn");                                        
}

void Constant :: set_parameter_by_index(PARAMETER_INDEX_T i, SAMPLE_T v){
    // store the passed SAMPLE_T value in the _values vector
    _values.clear();
    _values.push_back(v);    
    reset(); 

    // does not make sense to do this, as there are no stored generators
    // _inputs_output_size[i].clear()
    
    // do not need to call     _update_for_new_input();
    
}

void Constant :: add_parameter_by_index(PARAMETER_INDEX_T i, 
                                    GeneratorShared gs) {
    throw std::invalid_argument("invalid to set a GeneratoreShared as a value to a Constatn");
}


void Constant :: add_parameter_by_index(PARAMETER_INDEX_T i, SAMPLE_T v){
    _values.push_back(v);    
    reset();  
    
    // do not need to call     _update_for_new_input();
    
}





//==============================================================================
Add :: Add()
    : _input_index_opperands(0), _sum_opperands(0) {
    _class_name = "Add"; 
}
// dtor
Add :: ~Add() {
}

void Add :: init() {
    // the int routie must configure the names and types of parameters
    std::cout << *this << " Add::init()" << std::endl;
    // call base init, allocates and resets()
    Generator::init();    
    // register some parameters
    aw::ParameterTypeValueShared pt1 = aw::ParameterTypeValueShared(new 
                                       aw::ParameterTypeValue);
    pt1->set_name("Opperands");
    _register_input_parameter_type(pt1);
    // we can store ahead our only known input for performance
    _input_index_opperands = 0; // stored for speed
}


void Add :: render(FRAME_COUNT_T f) {
    FRAME_SIZE_T i;
    PARAMETER_INDEX_T j;
    
    // called once for efficiency
    FRAME_DIM_T len_at_num_value = _inputs[_input_index_opperands].size();
    
    while (_frame_count < f) {
        // calling render inputs updates the output of all inputs by calling their render functions; after doing so, the outputs are ready for reading
        _render_inputs(f);        
        
        // i is the position in output; we need to read through this in order, as it is the order of time
        for (i=0; i<_output_size; ++i) {
            // must sum teh value at this sample in time found across all Generators for each parameter type employed  
            _sum_opperands = 0;
            // we know we only have one input parameter type, but we do not know how many Generators reside in this input, so look at outputs and sum

            // what if the input has a smaller output than this output? an easy solution is to always take the modulus of the generator's size. This will cause a wrap-around of sorts of values. Different Gens can deal with this situation in different ways. 

            for (j=0; j<len_at_num_value; ++j) {
                _sum_opperands += _inputs[_input_index_opperands][j]->
                    output[i % _inputs_output_size[_input_index_opperands][j]];
            }
            output[i] = _sum_opperands; 
        }
        _frame_count += 1;
    }    
}






} // end namespaces aw



