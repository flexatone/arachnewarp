
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>

// needed for BufferFile
#include <sndfile.hh>


#include "aw_generator.h"
#include "aw_plotter.h"



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

//------------------------------------------------------------------------------
ParameterTypeFrequency :: ParameterTypeFrequency() {
    _class_name = "ParameterTypeFrequency";
}
ParameterTypeFrequency :: ~ParameterTypeFrequency() {
}

//------------------------------------------------------------------------------
ParameterTypeDuration :: ParameterTypeDuration() {
    _class_name = "ParameterTypeDuration";
}
ParameterTypeDuration :: ~ParameterTypeDuration() {
}

//------------------------------------------------------------------------------
ParameterTypePhase :: ParameterTypePhase() {
    _class_name = "ParameterTypePhase";
}
ParameterTypePhase :: ~ParameterTypePhase() {
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


//..............................................................................
GeneratorConfig :: GeneratorConfig(EnvironmentShared e) 
    : _init_frame_dimension(1), 
	_init_frame_size(64), // this is the default for all generators; 
	_environment(e) {
    // set standard generator defaults here: 1 d, size 128
	// what determiens what comes from environment, what comes from GenteratorConfig?
}

GeneratorConfig :: ~GeneratorConfig() {
}


//==============================================================================
// Generator

GeneratorShared  Generator :: make_with_dimension(GeneratorID q, 
	FrameDimensionType d){
    // static factory method
	GeneratorConfigShared gc = GeneratorConfig :: make_with_dimension(d);
	
    // ids are defined as public enum in generator.h
    GeneratorShared g;    
    if (q == ID_Constant) {
        g = ConstantShared(new Constant(gc));
    }
    else if (q == ID_Add) {
        g = AddShared(new Add(gc));    
    }
    else if (q == ID_BufferFile) {
        g = BufferFileShared(new BufferFile(gc));    
    }	
    else if (q == ID_Phasor) {
        g = PhasorShared(new Phasor(gc));    
    }
    //else if (q == ID_Recorder) {
        //g = RecorderShared(new Recorder(gc));    
    //}	
    else {
        throw std::invalid_argument("no matching GeneratorID: " + q);
    }
    // automatically call init; this will call subclass init, which calls baseclass init
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
	_output_size(1), // will be updated after resizing
    _input_parameter_count(0), 	
	_generator_config(gc),
	// protected ...
	_sampling_rate(0), // set from Envrionment in init()
	_nyquist(0), // set from Envrionment in init()
    _dimension_dynamics(DD_ResizableFreely), 
    _frame_size_is_resizable(false), 	
    _render_count(0),	
	output(NULL) {
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
	if (_dimension_dynamics == DD_FixedMono && _frame_dimension != 1) {
		throw std::invalid_argument("the requested dimension is not supported (and can be only 1)");			
	}
    _frame_size = _generator_config->get_init_frame_size();

	// Get relevant data from the environment and store in the Generator when assumed to be static. 
	EnvironmentShared e = _generator_config->get_environment();
    
    _sampling_rate = e->get_sampling_rate();
	_nyquist = _sampling_rate / 2; // let floor
    
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
	
	VSampleType vSampleTypeInner;
	_summed_inputs.push_back(vSampleTypeInner);
	
    _input_parameter_count += 1;
}

void Generator :: _register_slot_parameter_type(ParameterTypeShared pts) {
	// called in derived init()
	// set dictionary directly
    _slot_parameter_type[_slot_parameter_count] = pts;		
	// unlike with parameters, we need to add a place holder in the slots vector so that we have random access; best to do is set a default constant
	aw::GeneratorShared c = aw::ConstantShared(new aw::Constant(_generator_config));	
    c->init();
    c->set_input_by_index(0, 0.0); // this will call Constant::reset()
	_slots.push_back(c); // store default constant to hold position
    _slot_parameter_count += 1;
	
}

FrameDimensionType Generator :: _find_max_input_dimension(FrameDimensionType d) {
	// recursively find the max dimension in all inputs and their input
    // pass in max value found so far
	
	// get the dim of this Generator, which might be bigger than its inputs
    FrameDimensionType dSub(get_dimension());
	if (dSub > d) {
		d = dSub;
	}
    for (ParameterIndexType i = 0; i<_input_parameter_count; ++i) {
        // inputs are a vector of Generators        
        for (ParameterIndexType j=0; j<_inputs[i].size(); ++j) {
			// if this is a Constant, it will return the constant dimensionality stored
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
    if (_dimension_dynamics == DD_ResizableFreely && _frame_dimension < maxDim) {
		// set_dimension() checks _dimension_dynamics too
        set_dimension(maxDim); // will resize and reset
    }
}


void Generator :: _update_for_new_slot() {
	// when a slot is chagned, we might need to reset/clear data
}


void Generator :: _render_inputs(RenderCountType f) {
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

void Generator :: _reset_inputs() {
    VGenShared :: const_iterator j; // vector of generators    
    for (ParameterIndexType i = 0; i<_input_parameter_count; ++i) {
        // inputs are a vector of Generators
        for (j=_inputs[i].begin(); 
             j!=_inputs[i].end(); ++j) {
            // this is a shared generator
            (*j)->reset();
        }
    }
}

void Generator :: _sum_inputs() {
	// sum all inputs for all 1 frame of each dimension, up to the max dimension found;
	//std::cout << "_sum_inputs(): " << std::endl;
	SampleType sum(0);
	// iterate over each input parameter type
    for (ParameterIndexType i = 0; i<_input_parameter_count; ++i) {
		// resize to the max output size found in this input type position
		// need the max of all values in _inputs_output_size[i]
	    OutputSizeType maxFound(1);		
        for (ParameterIndexType j=0; j<_inputs[i].size(); ++j) {
			// note that if ther is a Buffer or something with a large size it could appear as being very large
			if (_inputs_output_size[i][j] > maxFound) {
				maxFound = _inputs_output_size[i][j];
			}		
		}
		// not: we are still doing a pus_back call here; might be faster to 
		_summed_inputs[i].clear();
		// only call reserve() if necessary
		if (_summed_inputs[i].capacity() != maxFound) {
			_summed_inputs[i].reserve(maxFound);
			// std::cout << "_sum_inputs(): reserving: " << maxFound << std::endl;			
		}
		// go up to the max for each input slot, but stop reading from an input when necessary
		for (unsigned int k=0; k < maxFound; ++k) {
			sum = 0;
			// now iterate over each input in this slot
			for (ParameterIndexType j=0; j<_inputs[i].size(); ++j) {
				if (k >= _inputs_output_size[i][j]) continue;
				// get the stored output value @k and sum it into position k
				sum += _inputs[i][j]->output[k];
			}	
			// index of value at push_back is k
			_summed_inputs[i].push_back(sum);
		}
	}
}

//..............................................................................
// public methods

void Generator :: render(RenderCountType f) {
    // this is a dummy render method just for demonstartion
    _render_count += 1;
}


//..............................................................................

void Generator :: set_dimension(FrameDimensionType d) {
	// this public method is not used during Generator::__init__
    // only change if different; assume already created
	if (_dimension_dynamics != DD_ResizableFreely) {
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

SampleType Generator :: get_output_abs_average() const {
    SampleType sum(0);
    for (OutputSizeType i=0; i < _output_size; ++i) {
        sum += fabs(output[i]);
    }
    return sum / _output_size;
}

SampleType Generator :: get_output_average(FrameDimensionType d) const {
    // if d is zero, get all dimensions, otherwise, just get rquested dimension (i.e., 2d gets vector pos 1
	if (d > _frame_dimension) {
		throw std::invalid_argument("a dimension greater than the number available has been requested");
	}	
    VFrameSizeType dims; 
    OutputSizeType count(0); 
    
	if (d == 0) {
        // this makes a copy
        dims = _dimension_offsets;  // do all 
        count = _output_size; // will be complete size
    }
    else {
        // need to request from 1 less than submitted positio, as dim 0 is the first dimension; we know tha td is greater tn zero
        FrameDimensionType dPos = d - 1;
        if (dPos >= _dimension_offsets.size()) {
            // this should not happend due to check above
            throw std::invalid_argument("requested dimension is greater than that supported");
        }
        // just store the start position in the output for the dimension requested
        dims.push_back(_dimension_offsets[dPos]);
        count = _frame_size; // will be one frame size
    }
    SampleType sum(0);    
    // get average of one dim or all 
    for (VFrameSizeType::const_iterator i=dims.begin(); i!=dims.end(); ++i) {
        // from start of dim to 1 less than frame plus start
        for (OutputSizeType j=(*i); j < (*i) + _frame_size; ++j) {
            sum += output[j];
        }
    }
    return sum / count; // cast count to 
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
    _render_count = 0;
	// should reset reset inputs?
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
    
    std::cout << *this << " Output frame @" << _render_count << ": ";
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


// this is no longer necessary, but if we ever want to plot to a file, we might need something similar
//void Generator :: plot_output_to_temp_fp(bool open) {
//	// grab a shared handle to envirionment
//	EnvironmentShared e = _generator_config->get_environment();
//	Plotter p;
//	// assume not interleaved
//	VSampleType v;
//	write_output_to_vector(v); // load output into this vecotr
//	p.plot(v, get_dimension());
//	// get the default plot directory
//	std::string fp(e->get_fp_plot());
//	p.write(fp);
//	
//	if (open) {
//		// make system call to gnuplot
//		std::string cmd = "gnuplot " + fp;
//		system(cmd.c_str());
//	}
//}


void Generator :: plot_output() {
	Plotter p;
	VSampleType v;
	write_output_to_vector(v); // load output into this vecotr
	p.plot(v, get_dimension());
	p.pipe(); // pipe to gnu plot
}



//..............................................................................
// loading and writing output

void Generator :: write_output_to_vector(VSampleType& vst) const {
    vst.clear(); // may not be necessary, but insures consistancy
    vst.reserve(_output_size);
    for (OutputSizeType i=0; i<_output_size; ++i) {
        vst.push_back(output[i]); 
    }
}

void Generator :: write_output_to_fp(const std::string& fp, 
                                    FrameDimensionType d) const {
    throw std::domain_error("not implemented on base class");                                    
}



void Generator :: set_output_from_array(SampleType* v, OutputSizeType s, 
								FrameDimensionType ch, bool interleaved){
	// caller is responsible for releasing called array
	if (s < 1) {
		throw std::domain_error(
            "the supplied vector must have size greater than 0");
	}	
    
	// TODO: add method to do both of these at the same time: set_frame_size_and_or_dimension
	bool reset_needed(true); 
	if (_frame_size_is_resizable) {
		// frame size is the number of outpuots divided by channel
		set_frame_size(s/ch);
		reset_needed = false;
	}
	if (_dimension_dynamics == DD_ResizableFreely) {
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
	
//    std::cout << "set_output_from_array: ch: " << (int)ch << " s: " << s << " output size: " << get_output_size() << " frame size: " << get_frame_size() << " _dimension_offsets[0]] " << (int)_dimension_offsets[0] << "_dimension_offsets[1]] " << (int)_dimension_offsets[1]  << std::endl;
    
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


void Generator :: set_output_from_fp(const std::string& fp) {
    // vitual method overridden in BufferFile (so as to localize use of libsndfile
    // an exception to call on base class
    throw std::domain_error("not implemented on base class");

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

void Generator :: set_input_by_index(ParameterIndexType i, 
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

void Generator :: set_input_by_index(ParameterIndexType i, SampleType v){
    // overridden method for setting a value: generates a constant
	// pass the GeneratorConfig to produce same dimensionality requested
	aw::GeneratorShared c = aw::ConstantShared(
							new aw::Constant(_generator_config));
    c->init();
    c->set_input_by_index(0, v); // this will call Constant::reset()
    set_input_by_index(i, c); // call overloaded
	// do not call _update_for_new_input here b/c called in set_input_by_index
}


void Generator :: add_input_by_index(ParameterIndexType i, 
                                        GeneratorShared gs){
    if (_input_parameter_count <= 0 or i >= _input_parameter_count) {
        throw std::invalid_argument("Parameter index is not available.");                                        
    }
    // adding additiona, with a generator
    _inputs[i].push_back(gs);    
    _inputs_output_size[i].push_back(gs->get_output_size());    
    _update_for_new_input();
}

void Generator :: add_input_by_index(ParameterIndexType i, 
                                        SampleType v){
    // adding additional as a constant value
    // note that no one else will have a handle on this constant
    // overridden method for setting a sample value: adds a constant	
	// pass the GeneratorConfig to produce same dimensionality requested	
	aw::GeneratorShared c = aw::ConstantShared(
							new aw::Constant(_generator_config));
    c->init();
    c->set_input_by_index(0, v); // this will call Constant::reset()
    add_input_by_index(i, c); // other overloaded
	// do not call _update_for_new_input here b/c called in set_input_by_index
}


//..............................................................................
void Generator :: set_slot_by_index(ParameterIndexType i, GeneratorShared gs){
    // if zero, none are set; current value is next available slot for registering
    if (_slot_parameter_count <= 0 or i >= _slot_parameter_count) {
    }        
    _slots[i] = gs; // direct assignment; replaces default if not there
    // store in advance the output size of the input    
    _update_for_new_slot();
}

void Generator :: set_slot_by_index(ParameterIndexType i, SampleType v){
    // overridden method for setting a value: generates a constant
	// pass the GeneratorConfig to produce same dimensionality requested
	aw::GeneratorShared c = aw::ConstantShared(
							new aw::Constant(_generator_config));
    c->init();
    c->set_input_by_index(0, v); // this will call Constant::reset()
    set_slot_by_index(i, c); // call overloaded
	// _update_for_new_slot called in set_slot_by_index
}











//------------------------------------------------------------------------------
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
    for (OutputSizeType i=0; i<get_output_size(); ++i) {
        output[i] = v; // set to the value
    }
    // always reset frame count?
    _render_count = 0;
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

void Constant :: render(RenderCountType f) {
    // do not need to iterate, as the output array always stores the proper values
    _render_count = f;
}

void Constant :: set_input_by_index(ParameterIndexType i, 
                                        GeneratorShared gs) {
    throw std::invalid_argument("invalid to set a GeneratoreShared as a value to a Constant");                                        
}

void Constant :: set_input_by_index(ParameterIndexType i, SampleType v){
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

void Constant :: add_input_by_index(ParameterIndexType i, 
                                    GeneratorShared gs) {
    throw std::invalid_argument("invalid to add a GeneratoreShared as a value to a Constatn");
}

void Constant :: add_input_by_index(ParameterIndexType i, SampleType v){
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
	_sum_opperands(0) { // end intitializer list
    _frame_size_is_resizable = false;		
	_dimension_dynamics = DD_ResizableFreely; 	
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

void Add :: render(RenderCountType f) {
    FrameSizeType i;
    ParameterIndexType j;
    
    // called once for efficiency; this is the number of operrands that live in this position
    FrameDimensionType len_at_num_value = _inputs[_input_index_opperands].size();
	// output size is private, so must get once per render call; if this is a performance problem can expose output size
    OutputSizeType output_size = get_output_size();
		
    while (_render_count < f) {
        // calling render inputs updates the output of all inputs by calling their render functions; after doing so, the outputs are ready for reading
        _render_inputs(f);        
        
		// NOTE: not using _sum_inputs() and related tools to demontrate approach for other similar operator process that will not necessarily summ all inputs in the same position.         
        
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
					// j is iterating across different inputs; we must just check the next one to add; we might find a way to mark this j value as no longer needed
					continue; 
				}
                _sum_opperands += _inputs[_input_index_opperands][j]->output[i];
            }
            output[i] = _sum_opperands; 
        }
        _render_count += 1;
    }    
}




//------------------------------------------------------------------------------
BufferFile :: BufferFile(GeneratorConfigShared gc) 
	// must initialize base class with passed arg
	: Generator(gc) {
	_class_name = "BufferFile";
	// this is the unique difference of the BufferFile class 
    _frame_size_is_resizable = true;
	// dimension is resizable not because of embedded generators, but because we will always read in all channels from an audio file; 
	_dimension_dynamics = DD_ResizableFreely; 
}

BufferFile :: ~BufferFile() {
}

void BufferFile :: init() {
    // the int routie must configure the names and types of parameters
    std::cout << *this << " BufferFile::init()" << std::endl;
    // call base init, allocates and resets()
    Generator::init();    
    // register some parameters:
    aw::ParameterTypeValueShared pt1 = aw::ParameterTypeValueShared(new 
                                       aw::ParameterTypeValue);
    pt1->set_instance_name("Inputs");
    _register_input_parameter_type(pt1);	
    // register some slots: 
    aw::ParameterTypeDurationShared so1 = aw::ParameterTypeDurationShared(new 
										   aw::ParameterTypeDuration);
    so1->set_instance_name("Duration in seconds");
	_register_slot_parameter_type(so1);	
}

void BufferFile :: _update_for_new_slot() {
	// dimensions already set to largest found in inputs
	// set frame size based on slot 1; mult sr by seconds requested
	set_frame_size(_slots[0]->output[0] * _sampling_rate * get_dimension());
}


void BufferFile :: render(RenderCountType f) {
	// render count must be ignored; instead, we render until we have filled our buffer; this means that the components will have a higher counter than render; need to be reset at beginning and end

	// must reset; might advance to particular render count
	_reset_inputs(); 
	
	// TODO Is there a better way to get input frame size?
	// Need a get smalles from size calculation pre-done
	FrameSizeType input_frame_size = _inputs[0][0]->get_frame_size();
	
	// assume that all inputs have the saem init frame size
	RenderCountType render_cycles_max = get_frames_per_dimension() / input_frame_size;
	RenderCountType render_count(0);
	
	OutputSizeType i(0);		
	
	// ignore render count for now; just fill buffer
    while (render_count < render_cycles_max) {

        _render_inputs(render_count);		
		_sum_inputs();
		
		// TODO need to handle multidemsional inputs to dom outputs
		for (i=0; i < input_frame_size; ++i) {
			output[i + (render_count * input_frame_size)] = _summed_inputs[0][i];
		}
		
		render_count++;
		
    }

	// we reset after to retrun components to starting state in case someone else uses this later. 	
	_reset_inputs(); 
	
}

void BufferFile :: write_output_to_fp(const std::string& fp, 
                                    FrameDimensionType d) const {
    // default is d is 0
    // if want ch 2 of stereo, d is 2
    if (d > get_dimension()) {
        // this should not happend due to check above
        throw std::invalid_argument("requested dimension is greater than that supported");
    }
    // can select format here
	int format = SF_FORMAT_AIFF | SF_FORMAT_PCM_16;
                                    
    VFrameSizeType dims;     
    OutputSizeType count(0);
    
    FrameDimensionType reqDim(1); // one if requested a specific dim
    if (d==0) {
        reqDim = get_dimension(); // number of dims
        dims = _dimension_offsets; //copy         
        count = get_output_size();        
    } 
    else { // just write a single dim specified 
        FrameDimensionType dPos = d - 1;
        if (dPos >= _dimension_offsets.size()) {
            // this should not happen due to check above
            throw std::invalid_argument("requested dimension is greater than that supported");
        }
        dims.push_back(_dimension_offsets[dPos]);    
        count = get_frame_size();
    }

    FrameSizeType sr(_sampling_rate);
	SndfileHandle outfile(fp, SFM_WRITE, format, reqDim, sr);
	if (not outfile){
        throw std::bad_alloc();    
    }

    double* v = new SampleType[count];
	if (not v) {
        throw std::bad_alloc();    
    }
    OutputSizeType i(0);
    OutputSizeType j(0);
    
    // interleave dimensions if more than one requested
    // iterate over frame size; for each frame, we will write that many dimensiosn
    for (i = 0; i < get_frame_size(); ++i) {
        for (VFrameSizeType::const_iterator p = dims.begin(); 
            p != dims.end(); ++p) {
            // if we are stereo, for each frame point we write two points, one at each offset position
            v[j] = output[i + (*p)];
            ++j;
        }
    }
    // TODO: j should equal count, check
    outfile.write(v, count);
    delete[] v;
    
}

void BufferFile :: set_output_from_fp(const std::string& fp) {
    // vitual method overridden in BufferFile (so as to localize use of libsndfile
    // an exception to call on base class
    std::cout << *this << ": set_output_from_fp" << std::endl;

    // libsndfile nomenclature is different than used here: For a sound file with only one channel, a frame is the same as a item (ie a single sample) while for multi channel sound files, a single frame contains a single item for each channel.
	SndfileHandle sh(fp);
    // read into temporary array; this means that we use 2x the memory that we really need, but it means that we can un-interleave the audio file when passing in the array
    OutputSizeType s(sh.frames() * sh.channels());
    double* v = new double[s];
    sh.readf(v, sh.frames());
    // this call will resize frame/dim
    set_output_from_array(v, s, sh.channels());
    delete[] v;

}


//------------------------------------------------------------------------------
//Recorder :: Recorder(GeneratorConfigShared gc) 
	//// must initialize base class with passed arg
	//: Generator(gc) {
	//_class_name = "Recorder";
	//// frame size is not used; its the Buffer's that is used
    //_frame_size_is_resizable = false;
	//// dimension is fixed at mono; does not matter, as we can rely on stored buffer; 
	//_dimension_dynamics = DD_FixedMono; 
//}

//Recorder :: ~Recorder() {
//}

//void Recorder :: init() {
    //// the int routie must configure the names and types of parameters
    //std::cout << *this << " Recorder::init()" << std::endl;
    //// call base init, allocates and resets()
    //Generator::init();    
    //// register some parameters: none to register here
	//// register some slots
    //aw::ParameterTypeDurationShared pt1 = aw::ParameterTypeDurationShared(new 
										   //aw::ParameterTypeDuration);
    //pt1->set_instance_name("Duration in seconds");
	//_register_slot_parameter_type(pt1);
//}

//void Recorder :: write_output_to_fp(const std::string& fp, 
                                    //FrameDimensionType d) const {
	//// write ouptut of Buffer
//}

//void Recorder :: render(RenderCountType f) {
	//// have Buffer store output of inputs
//}





//------------------------------------------------------------------------------
Phasor :: Phasor(GeneratorConfigShared gc) 
	// must initialize base class with passed arg
	: Generator(gc),
		_amp(0),
		_amp_prev(1) // init amp previous to 1 so to force reset to 0 on start
		//_period_start_sample_pos(0)
	{
	_class_name = "Phasor"; 
	// use a fixed size of 1, regardless of inputs (which are summed only for the relevent dimension
	_dimension_dynamics = DD_FixedMono; 	
}

Phasor :: ~Phasor() {
}

void Phasor :: init() {
    // the int routie must configure the names and types of parameters
    std::cout << *this << " Phasor::init()" << std::endl;
    // call base init, sets _frame_dimension, _frame_size, allocates and resets(); this is called after the constructor, so the _dimension_dynamics parameter is already set
    Generator::init();    
    // register some parameters
    aw::ParameterTypeFrequencyShared pt1 = aw::ParameterTypeFrequencyShared(new 
                                       aw::ParameterTypeFrequency);
    pt1->set_instance_name("Frequency");
    _register_input_parameter_type(pt1);
	_input_index_frequency = 0;
	
    aw::ParameterTypePhaseShared pt2 = aw::ParameterTypePhaseShared(new 
                                       aw::ParameterTypePhase);
    pt2->set_instance_name("Phase");
    _register_input_parameter_type(pt2);	
	_input_index_phase = 1;	
}


void Phasor :: render(RenderCountType f) {
	// given a frequency and a sample rate, we can calculate the number of samples per cycle
	// 1 / fq is time in seconds
	// sr is samples per sec

	// must access _frame_size ; each render is a frame worth
	// this does not need to be done each render call, but if so, ensure continuity
	OutputSizeType fs = get_frame_size();
	// this is the abs positoin in this frame
	//_abs_sample_pos = f * fs; 		
	// this is the relative postion in this frame
	OutputSizeType i(0);
	
    while (_render_count < f) {
        // calling render inputs updates the output of all inputs by calling their render functions; after doing so, the outputs are ready for reading
        _render_inputs(f);
		
		// get a vector  for each input summing accross all dimensions
		_sum_inputs();
		
		for (i=0; i < fs; ++i) {
			// for each frame position, must get sum across all frequency values calculated.
						
			// iterate over vector of Generators, get output value only for first dimension for now; 
			
			_sum_frequency = _summed_inputs[_input_index_frequency][i];
			//VGenShared :: const_iterator j;
			//VGenShared vf = _inputs[_input_index_frequency];
			//for (j=vf.begin(); j!=vf.end(); ++j) {
				//// always getting the first dimension only here
				//_sum_frequency += (*j)->output[i];
			//}

			// we might dither this to increase accuracy over time
			_period_samples = floor((_sampling_rate / 
							frequency_limiter(_sum_frequency, _nyquist)) + 0.5);
			// sample period must not be 0 or 1

			// add amp increment to previou amp; do not care about where we are in the cycle, only that we get to 1 and reset amp
			_amp = _amp_prev + (1.0 / (_period_samples - 1));

			// if amp is at or above 1, set to zero
			if (_amp > 1) {
				_amp = 0.0;
			}
			_amp_prev = _amp;
			//std::cout << "i" << i << " : _amp " << _amp << std::endl;
			output[i] = _amp;
			
		}
		// can use _dimension_offsets; stores index position of start of each dimension		        
        _render_count += 1;
    }    
}





} // end namespaces aw



