
#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "aw_generator.h"
#include "aw_plotter.h"

// TODO: get rid of default constructors with no args
// require all generators to use either factor or explicit args


namespace aw {

//==============================================================================
// ParameterType
ParameterType :: ParameterType() {
    _class_name = "ParameterType";
}

ParameterType :: ~ParameterType() {
}

std::ostream& operator<<(std::ostream& output, const ParameterType& pt) {
    output << "<Pmtr: " << pt._class_name << ": " << pt._instance_name << ">";
    return output; 
}


//------------------------------------------------------------------------------
ParameterTypeValue :: ParameterTypeValue() {
    _class_name = "ParameterTypeValue";
}

ParameterTypeValue :: ~ParameterTypeValue() {
}




//==============================================================================
// GeneratorConfig
// intentionally not using overridden make functions here to make code as clear as possible

GeneratorConfigShared GeneratorConfig :: make_default() {
	EnvironmentShared e = EnvironmentShared(new Environment);
    GeneratorConfigShared gc = GeneratorConfigShared(new GeneratorConfig(e));
    return gc;
}

GeneratorConfigShared GeneratorConfig :: make_with_dimension(
	FrameDimensionType d) {
	EnvironmentShared e = EnvironmentShared(new Environment);
    GeneratorConfigShared gc = GeneratorConfigShared(new GeneratorConfig(e));
    gc->set_init_frame_dimension(d);
    return gc;
}

// TODO: can add make that takes an EnvironmeShared 

//..............................................................................
GeneratorConfig :: GeneratorConfig(EnvironmentShared e) 
    : _init_frame_dimension(1), 
	_init_frame_size(64),
	_environment(e) {
    // set standard generator defaults here: 1 d, size 128
}

GeneratorConfig :: ~GeneratorConfig() {
}


//==============================================================================
// Generator

GeneratorShared  Generator :: make_with_dimension(GeneratorID q, 
	FrameDimensionType d){
    // static factory method
	GeneratorConfigShared gc = GeneratorConfig :: make_with_dimension(d);
	
    GeneratorShared g;    
    if (q == ID_Constant) {
        g = ConstantShared(new Constant(gc));
    }
    else if (q == ID_Add) {
        g = AddShared(new Add(gc));    
    }
    else if (q == ID_Buffer) {
        g = BufferShared(new Buffer(gc));    
    }	
    else {
        throw std::invalid_argument("no matching GeneratorID: " + q);
    }
    // automatically call init; this will subclass init, which calls baseclass init
    g->init();
    return g;
}

GeneratorShared  Generator :: make(GeneratorID q){
	// provide a default dimensionality of 1, pass to make_with_dimension
	FrameDimensionType d(1);
	return Generator :: make_with_dimension(q, d);
}

//..............................................................................
Generator :: Generator(GeneratorConfigShared gc) 
	// this is the only constructor for Generator; the passed-in GenertorConfigShared is stored in the object and used to set init frame dimension and frame size.
	: _class_name("Generator"), 
	_frame_dimension(1), 
	_frame_size(64),
	_output_size(1),
    _input_parameter_count(0), 	
    _dimension_is_resizable(true), 
    _frame_size_is_resizable(false), 	
    _frame_count(0), 	
	output(NULL),
    _generator_config(gc) {
    //std::cout << "Generator (1 arg): Constructor" << std::endl;
}


Generator :: ~Generator() {
    // deconstructor 
    // if gen was not initialized, do not delete
    if (output != NULL) delete[] output;
}


void Generator :: init() {
    // virtual method, specialzied in subclass, but the base class is explicitly called.
    //std::cout << *this << " Generator::init()" << std::endl;

	// read values from GeneratorConfig
    _frame_dimension = _generator_config->get_init_frame_dimension();
    _frame_size = _generator_config->get_init_frame_size();
    
    // since this might be called more than once in the life of an object, need to repare storage units that are general set in the init() routine
    _input_parameter_type.clear();
    _inputs.clear();
    _input_parameter_count = 0;
    
    // _register_input_parameter_type is called to add inputs here
	// cant use set_dimension() here because we compare to the value _frame_dimension and only change if it is different; thus, here we must manually set for the initial size. 
    _resize_output();
    reset(); // zero output; zero frame count
}


//..............................................................................
// private methods

void Generator :: _resize_output() {
    // _output_size is set here, memory is allocated
    // std::cout << *this << " Generator::_resize_output()" << std::endl;
    // must delete if set to something else
    if (output != NULL) delete[] output;
    // size is always dim * _frame_size
    // only do this once to avoid repeating
    _output_size = _frame_dimension * _frame_size;
	// alloc!
    output = new SampleType[_output_size];
    // check that alloc was sucesful
    if (output == NULL) {
        throw std::bad_alloc();    
    }
	// whenever this called, need to update _dimension_offsets; this is done once per resizing for efficiency
	_dimension_offsets.clear();
	_dimension_offsets.reserve(_frame_dimension);
	for (FrameDimensionType d=0; d<_frame_dimension; ++d) {
		_dimension_offsets.push_back(d * _frame_size);
	}
}

void Generator :: _register_input_parameter_type(ParameterTypeShared pts) {
	// called in derived init() to setup a input types and prepare storage
    // storing in an unordered map
    _input_parameter_type[_input_parameter_count] = pts;

    // store a vector in the position to accept inputs
    VGenShared vInner;  
    _inputs.push_back(vInner); // extr copy made here
    
    // add vector to output size as well 
    VOutputSize vSizeInner;  
    _inputs_output_size.push_back(vSizeInner);
    
    _input_parameter_count += 1;
}

FrameDimensionType Generator :: _find_max_input_dimension(FrameDimensionType d) {
	// recursively find the max dimension in all inputs and their input
    // pass in max value found so far
    FrameDimensionType dSub(1);
    for (ParameterIndexType i = 0; i<_input_parameter_count; ++i) {
        // inputs are a vector of Generators        
        for (ParameterIndexType j=0; j<_inputs[i].size(); ++j) {
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
    
    // these values are set when the input is added; probably not necessary to search through all here, unless an input (or an input's input) could have changed since it was added; this does not seem likely, but is possible

    // this is recursive, which previous methods are not:
    FrameDimensionType maxDim = _find_max_input_dimension();
    std::cout << *this << " Found max input dim: @" << (int)maxDim << std::endl;

    // if resizable, and size is less, resize    
    // if size is greater, no need to resize (it seems)
    if (_dimension_is_resizable && _frame_dimension < maxDim) {
		// set_dimension() checks _dimension_is_resizable too
        set_dimension(maxDim); // will resize and reset
    }
}

void Generator :: _render_inputs(FrameCountType f) {
    // for each of the inputs, need to render up to this frame
    // this is "pulling" lower-level values up to date
    // this seems somewhat wasteful as in most cases we just will be advancing by 1 more than the previous frame count value 
    // this is inlined in the header
    VGenShared :: const_iterator j; // vector of generators    
    for (ParameterIndexType i = 0; i<_input_parameter_count; ++i) {
        // inputs are a vector of Generators
        for (j=_inputs[i].begin(); 
             j!=_inputs[i].end(); ++j) {
            // this is a shared generator
            (*j)->render(f);
        }
    }
}


//..............................................................................
// public methods


void Generator :: render(FrameCountType f) {
    // this is a dummy render method just for demonstartion
    _frame_count += 1;
}


//..............................................................................

void Generator :: set_dimension(FrameDimensionType d) {
    // only change if different; assume already created
	if (!_dimension_is_resizable) {
		throw std::domain_error("this generator does not support dimension setting");
	}
	if (d == 0) {
		throw std::invalid_argument("a dimension of 0 is not supported");
	}	
    if (d != _frame_dimension) {
        _frame_dimension = d;
        _resize_output(); // _output_size is set here with dimension
        reset(); // must reset values to zero 
    }
    // when we set the dimension, should we set it for inputs?
}

void Generator :: set_frame_size(FrameSizeType f) {
    // only change if different; assume already created
	if (!_frame_size_is_resizable) {
		throw std::domain_error("this generator does not support frame size setting");
	}	
	if (f == 0) {
		throw std::invalid_argument("a frame size of 0 is not supported");
	}		
    if (f != _frame_size) {
        _frame_size = f;
        _resize_output(); // _output_size is set here with dimension
        reset(); // must reset values to zero 
    }
    // when we set the dimension, should we set it for inputs?
}

// TODO: might have method to set both dim and frame size with the same method; this would be faster
//void Generator :: set_frame_size_and_or_dimension(FrameSizeType f, FrameDimensionType d) {}



void Generator :: reset() {
    //std::cout << *this << " Generator::reset()" << std::endl;
    // do not need to partion by diminsons
    for (OutputSizeType i=0; i<_output_size; ++i) {
        output[i] = 0.0; // set to zero; float or int?
    }
    // always reset frame count?
    _frame_count = 0;
}

//..............................................................................
// display methods

std::ostream& operator<<(std::ostream& output, const Generator& g) {
    // replace with static cast
    output << "<Gen: " << g._class_name << " @" << 
        (int)g._frame_dimension << ">";
    return output; 
}

void Generator :: print_output() {
    // provide name of generator first by dereferencing this
    std::string space1 = std::string(INDENT_SIZE*2, ' ');
    
    std::cout << *this << " Output frame @" << _frame_count << ": ";
    for (OutputSizeType i=0; i<_output_size; ++i) {
        if (i % _frame_size == 0) {
            std::cout << std::endl << space1;        
        }
        std::cout << std::setprecision(8) << output[i] << '|';
    }
    std::cout << std::endl;
}

void Generator :: print_inputs(bool recursive, UINT8 recurse_level) {
    //std::cout << "recurse_level: " << (int)recurse_level << std::endl;
    
    std::string space1 = std::string(recurse_level*INDENT_SIZE, ' ');
    std::string space2 = std::string((recurse_level+1)*INDENT_SIZE, ' '); 
    std::string space3 = std::string((recurse_level+2)*INDENT_SIZE, ' '); 
    std::cout << space1 << *this << " Inputs:" << std::endl;

    VGenShared :: const_iterator j;
    // need an interger as key for _input_parameter_type
    for (ParameterIndexType k=0; k!=_inputs.size(); ++k) {   
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


void Generator :: plot_output_to_temp_fp(bool open) {
	// grab a shared handle to envirionment
	EnvironmentShared e = _generator_config->get_environment();
	Plotter p;
	// assume not interleaved
	VSampleType v;
	write_output_to_vector(v); // load output into this vecotr
	p.plot(v, get_dimension());
	// get the default plot directory
	std::string fp(e->get_fp_plot());
	p.write(fp);
	
	if (open) {
		// make system call to gnuplot
		std::string cmd = "gnuplot " + fp;
		system(cmd.c_str());
	}
}



//..............................................................................
// loading writing output

void Generator :: write_output_to_vector(VSampleType& vst) const {
    vst.clear(); // may not be necessary, but insures consistancy
    vst.reserve(_output_size);
    for (OutputSizeType i=0; i<_output_size; ++i) {
        vst.push_back(output[i]); 
    }
}


void Generator :: set_output_from_array(SampleType* v, OutputSizeType s, 
								FrameDimensionType ch, bool interleaved){
	// caller is responsible for releasing called array
	// TODO: add method to do both of these at the same time: set_frame_size_and_or_dimension
	bool reset_needed(true); 
	if (_frame_size_is_resizable) {
		// frame size is the number of outpuots divided by channel
		set_frame_size(s/ch);
		reset_needed = false;
	}
	if (_dimension_is_resizable) {
		set_dimension(ch);
		reset_needed = false;		
	}
	// must reset values to zero (if not done above) as s may be smaller than outputsize, and we would get mixed content
	if (reset_needed) reset(); 

	// if we did not resize dimensions, limit at max
	if (s > _output_size) {
		s = _output_size;
	}
	// if we did not resize dimensions, limit at max
	if (ch > _frame_dimension) {
		ch = _frame_dimension;
	}
	
	// assuming interleaved source to non-inter dest
	FrameDimensionType j(0);
	OutputSizeType i(0);
	OutputSizeType k(0);
	
	// we will never run over larger area than output size, s is less than output; we also must not run over v, so we must iterate by s. 
    while (i < s) {
		// step over dimensions
		for (j=0; j<ch; ++j) {
			// i+j maybe greater than s; and not caught above
			if (i >= s) break;
	        output[k+_dimension_offsets[j]] = v[i];
			//std::cout << i << std::endl;
			i += 1;
		}
		k += 1; // will only reach total/ch
    }
}

void Generator :: set_output_from_vector(const VSampleType& vst, 
								FrameDimensionType ch, bool interleaved) {

	OutputSizeType s = vst.size();
	// pack this in an array for reuse of the same function
    SampleType* v = new SampleType[s];
	// cant use iterator b/c need number
    for (OutputSizeType i=0; i<s; ++i) {
		v[i] = vst[i];
	}
	set_output_from_array(v, s, ch, interleaved);
	// clean up temporary vector 
	delete[] v;
		
}


//..............................................................................
// parameter translating

ParameterIndexType Generator :: get_parameter_index_from_name(
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
	// raise an exception, or return error code?
	throw std::invalid_argument("no matching parameter name: " + s);
}


//..............................................................................
// parameter setting and adding; all overloaded for taking generator or sample type values, whcich auto-creates constants.

void Generator :: set_parameter_by_index(ParameterIndexType i, 
                                        GeneratorShared gs){
    // if zero, none are set; current value is next available slot for registering
    if (_input_parameter_count <= 0 or i >= _input_parameter_count) {
        throw std::invalid_argument("Parameter index is not available.");                                        
    }        
    // this removes all stored values
    _inputs[i].clear();
    _inputs_output_size[i].clear();
    
    _inputs[i].push_back(gs);    
    // store in advance the output size of the input
    _inputs_output_size[i].push_back(gs->get_output_size());
    
    _update_for_new_input();
}

void Generator :: set_parameter_by_index(ParameterIndexType i, 
                                        SampleType v){
    // overridden method for setting a value: generates a constant
	// pass the GeneratorConfig to produce same dimensionality requested
	aw::GeneratorShared c = aw::ConstantShared(
							new aw::Constant(_generator_config));
    c->init();
    c->set_parameter_by_index(0, v); // this will call Constant::reset()
    set_parameter_by_index(i, c); // call overloaded
}


void Generator :: add_parameter_by_index(ParameterIndexType i, 
                                        GeneratorShared gs){
    if (_input_parameter_count <= 0 or i >= _input_parameter_count) {
        throw std::invalid_argument("Parameter index is not available.");                                        
    }
    // adding additiona, with a generator
    _inputs[i].push_back(gs);    
    _inputs_output_size[i].push_back(gs->get_output_size());    
    _update_for_new_input();
}

void Generator :: add_parameter_by_index(ParameterIndexType i, 
                                        SampleType v){
    // adding additional as a constant value
    // note that no one else will have a handle on this constant
    // overridden method for setting a sample value: adds a constant	
	// pass the GeneratorConfig to produce same dimensionality requested	
	aw::GeneratorShared c = aw::ConstantShared(
							new aw::Constant(_generator_config));
    c->init();
    c->set_parameter_by_index(0, v); // this will call Constant::reset()
    add_parameter_by_index(i, c); // other overloaded

}




//------------------------------------------------------------------------------
//Constant :: Constant() {
//	// will call no argument default constructor
//    _class_name = "Constant"; 
//}

Constant :: Constant(GeneratorConfigShared gc) 
	// must initialize base class with passed arg
	: Generator(gc) {
	_class_name = "Constant"; 
}

Constant :: ~Constant() {
}

FrameDimensionType Constant :: _find_max_input_dimension(FrameDimensionType d) {
    // overriden virtual method b/c this is terminal of recusive search
    // just return dimensionality of this generator
    return get_dimension();
}


// the int routie must configure the names and types of parameters
void Constant :: init() {
    //std::cout << *this << " Constant::init()" << std::endl;
    // resize and reset after setting parameters
    // this will call the virtual reset 
    Generator::init();
    
    // register some parameters
    aw::ParameterTypeValueShared pt1 = aw::ParameterTypeValueShared(new 
                                       aw::ParameterTypeValue);
    pt1->set_instance_name("Constant numerical value");
    // when this is called, the inputs vectors are filled with GeneratorShared
    // these will the inputs vector will be filled with 1 empty placeholder
    _register_input_parameter_type(pt1);
    // by default, add a zero value
    _values.push_back(0);           
}

void Constant :: reset() {
    // need overidden reset because have to transfer stored value to the output array; normal reset sets the output to zer. 
    //std::cout << *this << " Constant::reset()" << std::endl;
    // sum values first, then reseset all output to values
    SampleType v(0);
    VSampleType :: const_iterator j;
    for (j = _values.begin(); j!=_values.end(); ++j) {
        v += *j;
    }
    // do not need to partion by diminsons
    for (int i=0; i<get_output_size(); ++i) {
        output[i] = v; // set to the value
    }
    // always reset frame count?
    _frame_count = 0;
}

void Constant :: print_inputs(bool recursive, UINT8 recurse_level) {
    // need overridden print because this is terminal in recursive searches
    std::string space1 = std::string(recurse_level*INDENT_SIZE, ' ');
    std::string space2 = std::string((recurse_level+1)*INDENT_SIZE, ' '); 
    std::string space3 = std::string((recurse_level+2)*INDENT_SIZE, ' ');     
    // this cannot recurse so nothing to do with parameters; just so that
    // they are the same as Generator
    std::cout << space1 << *this << " Inputs:" << std::endl;
    // iterative over inputs
    for (ParameterIndexType k=0; k!=_inputs.size(); ++k) {   
        // is this doing a copy?
        ParameterTypeShared pts = _input_parameter_type[k];
        std::cout << space2 << *pts << std::endl;
        // need to iterate over stored _values
        VSampleType :: const_iterator j;
        for (j = _values.begin(); j!=_values.end(); ++j) {
            std::cout << space3 << "Value: " << *j << std::endl;
        }
    }
}

void Constant :: render(FrameCountType f) {
    // do not need to iterate, as the output array always stores the proper values
    _frame_count = f;
}



void Constant :: set_parameter_by_index(ParameterIndexType i, 
                                        GeneratorShared gs) {
    throw std::invalid_argument("invalid to set a GeneratoreShared as a value to a Constant");                                        
}

void Constant :: set_parameter_by_index(ParameterIndexType i, SampleType v){
    if (get_parameter_count() <= 0 or i >= get_parameter_count()) {
        throw std::invalid_argument("Parameter index is not available.");                                        
    }
    // store the passed SampleType value in the _values vector
    _values.clear();
    _values.push_back(v);    
    reset(); 
    // does not make sense to do this, as there are no stored generators
    // _inputs_output_size[i].clear()
    // do not need to call     _update_for_new_input();
}

void Constant :: add_parameter_by_index(ParameterIndexType i, 
                                    GeneratorShared gs) {
    throw std::invalid_argument("invalid to add a GeneratoreShared as a value to a Constatn");
}

void Constant :: add_parameter_by_index(ParameterIndexType i, SampleType v){
    if (get_parameter_count() <= 0 or i >= get_parameter_count()) {
        throw std::invalid_argument("Parameter index is not available.");
	}
    _values.push_back(v);    
    reset();  
    // do not need to call     _update_for_new_input();
}




//------------------------------------------------------------------------------
Add :: Add(GeneratorConfigShared gc) 
	// must initialize base class with passed arg
	: Generator(gc), 
	_input_index_opperands(0), 
	_sum_opperands(0) {
	_class_name = "Add"; 
}

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
    pt1->set_instance_name("Opperands");
    _register_input_parameter_type(pt1);
    // we can store ahead our only known input for performance
    _input_index_opperands = 0; // stored for speed
}


void Add :: render(FrameCountType f) {
    FrameSizeType i;
    ParameterIndexType j;
    
    // called once for efficiency; this is the number of operrands that live in this position
    FrameDimensionType len_at_num_value = _inputs[_input_index_opperands].size();
	// output size is private, so must get once per render call; if this is a performance problem can expose output size
    OutputSizeType output_size = get_output_size();
	
    while (_frame_count < f) {
        // calling render inputs updates the output of all inputs by calling their render functions; after doing so, the outputs are ready for reading
        _render_inputs(f);        
        
        // i is the position in output; we need to read through this in order, as for each dimension it is the order of time
        for (i=0; i < output_size; ++i) {
            // must sum teh value at this sample in time found across all Generators for each parameter type employed  
            _sum_opperands = 0;
            // we know we only have one input parameter type, but we do not know how many Generators reside in this input, so look at outputs and sum

            // what if the input has a smaller output than this output? an easy solution is to always take the modulus of the generator's size. This will cause a wrap-around of sorts of values. Different Gens can deal with this situation in different ways. 

            for (j=0; j<len_at_num_value; ++j) {
				// i may be larger than permitted on a given input; 
				//std::cout << _inputs_output_size[_input_index_opperands][j] << std::endl;
				if (i >= _inputs_output_size[_input_index_opperands][j]) {
					// j is iterating across different inputs; we must jsut check the next one to add; we might find a way to mark this j value as no longer needed
					continue; 
				}
                _sum_opperands += _inputs[_input_index_opperands][j]->output[i];
            }
            output[i] = _sum_opperands; 
        }
        _frame_count += 1;
    }    
}






//------------------------------------------------------------------------------
Buffer :: Buffer(GeneratorConfigShared gc) 
	// must initialize base class with passed arg
	: Generator(gc) {
	_class_name = "Buffer";
	_frame_size_is_resizable = true; // this is the only difference
	// we can require this to be set at init; then, on import, handle how we map this
	_dimension_is_resizable = false; 
}

Buffer :: ~Buffer() {
}

void Buffer :: init() {
    // the int routie must configure the names and types of parameters
    std::cout << *this << " Buffer::init()" << std::endl;
    // call base init, allocates and resets()
    Generator::init();    
    // register some parameters: none to register here
}








} // end namespaces aw



