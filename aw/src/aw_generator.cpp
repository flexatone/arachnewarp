
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

std::ostream& operator<<(std::ostream& matrix, const ParameterType& pt) {
    matrix << "<Pmtr: " << pt._class_name << ": " << pt._instance_name << ">";
    return matrix; 
}


//------------------------------------------------------------------------------
ParameterTypeValue :: ParameterTypeValue() {
    _class_name = "ParameterTypeValue";
}

//------------------------------------------------------------------------------
ParameterTypeFrequency :: ParameterTypeFrequency() {
    _class_name = "ParameterTypeFrequency";
}

//------------------------------------------------------------------------------
ParameterTypeDuration :: ParameterTypeDuration() {
    _class_name = "ParameterTypeDuration";
}

//------------------------------------------------------------------------------
ParameterTypePhase :: ParameterTypePhase() {
    _class_name = "ParameterTypePhase";
}
//------------------------------------------------------------------------------
ParameterTypeChannels :: ParameterTypeChannels() {
    _class_name = "ParameterTypeChannels";
}





//==============================================================================
// GeneratorConfig
// intentionally not using overridden make functions here to make code as clear as possible

// TODO: this class can go away: replace with Environment directly

//GeneratorConfigShared GeneratorConfig :: make_default() {
//	EnvironmentShared e = EnvironmentShared(new Environment);
//    GeneratorConfigShared gc = GeneratorConfigShared(new GeneratorConfig(e));
//    return gc;
//}

//GeneratorConfigShared GeneratorConfig :: make_with_dimension(
//	OutputCountType d) {
//	EnvironmentShared e = EnvironmentShared(new Environment);
//    GeneratorConfigShared gc = GeneratorConfigShared(new GeneratorConfig(e));
//    gc->set_init_frame_dimension(d);
//    return gc;
//}


//..............................................................................
//GeneratorConfig :: GeneratorConfig(EnvironmentShared e) 
//    : 
//	_init_frame_size(64), // this is the default for all generators; 
//	_environment(e) {
//    // set standard generator defaults here: 1 d, size 128
//	// what determiens what comes from environment, what comes from GenteratorConfig?
//}
//
//GeneratorConfig :: ~GeneratorConfig() {
//}


//==============================================================================
// Generator

// removed: make_with_dimension


//GeneratorShared  Generator :: make_with_dimension(GeneratorID q, 
//	OutputCountType d){
//    // static factory method
//	GeneratorConfigShared gc = GeneratorConfig :: make_with_dimension(d);
//	
//    // ids are defined as public enum in generator.h
//    GeneratorShared g;    
//    if (q == ID_Constant) {
//        g = ConstantShared(new Constant(gc));
//    }
//    else if (q == ID_Add) {
//        g = AddShared(new Add(gc));    
//    }
//    else if (q == ID_BufferFile) {
//        g = BufferFileShared(new BufferFile(gc));    
//    }	
//    else if (q == ID_Phasor) {
//        g = PhasorShared(new Phasor(gc));    
//    }
//    else {
//        throw std::invalid_argument("no matching GeneratorID: " + q);
//    }
//    // automatically call init; this will call subclass init, which calls baseclass init
//    g->init();
//    return g;
//}


GeneratorShared  Generator :: make(GeneratorID q){
	// just get defailt vaules
    EnvironmentShared e = EnvironmentShared(new Environment);

    GeneratorShared g;    
    if (q == ID_Constant) {
        g = ConstantShared(new Constant(e));
    }
    else if (q == ID_Add) {
        g = AddShared(new Add(e));    
    }
    else if (q == ID_BufferFile) {
        g = BufferFileShared(new BufferFile(e));    
    }	
    else if (q == ID_Phasor) {
        g = PhasorShared(new Phasor(e));    
    }
    else {
        throw std::invalid_argument("no matching GeneratorID: " + q);
    }
    // automatically call init; this will call subclass init, which calls baseclass init
    g->init();
    return g;
}

//..............................................................................
Generator :: Generator(EnvironmentShared e) 
	// this is the only constructor for Generator; the passed-in GenertorConfigShared is stored in the object and used to set init frame frame size.
	: _class_name("Generator"), 
	_output_count(1), // always init to 1; can change in init
	_frame_size(e->get_common_frame_size()),
	_matrix_size(1), // will be updated after resizing
    _input_parameter_count(0), 	
    _slot_parameter_count(0), 	
    
	_environment(e), // replace with Environment
    
	// protected ...
	_sampling_rate(0), // set from Envrionment in init()
	_nyquist(0), // set from Envrionment in init()

    _frame_size_is_resizable(false), 	
    _render_count(0),	
	matrix(NULL) {
    //std::cout << "Generator (1 arg): Constructor" << std::endl;
}

Generator :: ~Generator() {
    // deconstructor 
    // if gen was not initialized, do not delete
    if (matrix != NULL) delete[] matrix;
}

void Generator :: init() {
    // the frame size is eithe common or larger; thus, we generally only read as many as we have from a client object. 
    _frame_size = _environment->get_common_frame_size();    
    _sampling_rate = _environment->get_sampling_rate();
	_nyquist = _sampling_rate / 2; // let floor
    
    // since this might be called more than once in the life of an object, need to repare storage units that are general set in the init() routine
    _input_parameter_type.clear();
    _inputs.clear();
    _input_parameter_count = 0;
    
    // _register_input_parameter_type is called to add inputs here
	// cant use _set_output_count() here because we compare to the value _output_count and only change if it is different; thus, here we must manually set for the initial size. 
    _resize_matrix();
    reset(); // zero matrix; zero frame count
}


//..............................................................................
// private methods

void Generator :: _resize_matrix() {
    // _matrix_size is set here, memory is allocated
    // std::cout << *this << " Generator::_resize_matrix()" << std::endl;
    // must delete if set to something else
    if (matrix != NULL) delete[] matrix;
    // size is always dim * _frame_size
    // only do this once to avoid repeating
    _matrix_size = _output_count * _frame_size;
	// alloc!
    matrix = new SampleType[_matrix_size];
    // check that alloc was sucesful
    if (matrix == NULL) {
        throw std::bad_alloc();    
    }
	// whenever this called, need to update out_to_matrix_offset; this is done once per resizing for efficiency
	out_to_matrix_offset.clear();
	out_to_matrix_offset.reserve(_output_count);
	for (OutputCountType d=0; d<_output_count; ++d) {
		out_to_matrix_offset.push_back(d * _frame_size);
	}
}

void Generator :: _register_input_parameter_type(ParameterTypeShared pts) {
	// called in derived init() to setup a input types and prepare storage
    // storing in an unordered map
    _input_parameter_type[_input_parameter_count] = pts;

    // store a vector in the position to accept inputs
    VGenSharedOutPair vInner;  
    _inputs.push_back(vInner); // extr copy made here
    
    // add vector to matrix size as well 
//    VFrameSize vSizeInner;  
//    _inputs_frame_size.push_back(vSizeInner);
	
	VSampleType vSampleTypeInner;
	_summed_inputs.push_back(vSampleTypeInner);
    _input_parameter_count += 1;
}

void Generator :: _register_slot_parameter_type(ParameterTypeShared pts) {
	// called in derived init()
	// set dictionary directly
    _slot_parameter_type[_slot_parameter_count] = pts;		
	_slots.push_back(GeneratorShared()); // store empoty to hold position
    _slot_parameter_count += 1;
    
}


void Generator :: _update_for_new_input() {
    // pass
}


void Generator :: _update_for_new_slot() {
    // thi is virtual: override to update
	// when a slot is chagned, we might need to reset/clear data
}


void Generator :: _render_inputs(RenderCountType f) {
    // for each of the inputs, need to render up to this frame
    // this is "pulling" lower-level values up to date
    // this seems somewhat wasteful as in most cases we just will be advancing by 1 more than the previous frame count value 
    // this is inlined in the header
    VGenSharedOutPair :: const_iterator j; // vector of generators    
    for (ParameterIndexType i = 0; i<_input_parameter_count; ++i) {
        // inputs are a vector of Generators
        for (j=_inputs[i].begin(); j!=_inputs[i].end(); ++j) {
            // this is a pair of shared generator, matrix index
            // we just render the generator; do not care about which matrix we are looking at until later; will not render multiple times for multiple calls because of the render count id. 
            (*j).first->render(f);
        }
    }
}

void Generator :: _reset_inputs() {
    VGenSharedOutPair :: const_iterator j; // vector of generators    
    for (ParameterIndexType i = 0; i<_input_parameter_count; ++i) {
        // inputs are a vector of Generators
        for (j=_inputs[i].begin(); 
             j!=_inputs[i].end(); ++j) {
            // this is a shared generator
            (*j).first->reset();
        }
    }
}

void Generator :: _sum_inputs() {

	// sum all inputs for all 1 frame of each dimension, up to the max dimension found;
	//std::cout << "_sum_inputs(): " << std::endl;
	SampleType sum(0);

    // we only read and/or sum up to our own frame size
    FrameSizeType frameSize = get_frame_size();
    
	// iterate over each input parameter type
    for (ParameterIndexType i = 0; i<_input_parameter_count; ++i) {

		_summed_inputs[i].clear();
		// only call reserve() if necessary
		if (_summed_inputs[i].capacity() != frameSize) {
			_summed_inputs[i].reserve(frameSize);
			// std::cout << "_sum_inputs(): reserving: " << maxFound << std::endl
        }
		// go up to the max for each input slot, but stop reading from an input when necessary
		for (unsigned int k=0; k < frameSize; ++k) {
            // TODO: store _inputs[i].size()
			sum = 0;
			// now iterate over each input in this slot
            // TODO: replace wiht reused iterator over _inputs; .size() is slow
			for (ParameterIndexType j=0; j<_inputs[i].size(); ++j) {
				// if (k >= _inputs_frame_size[i][j]) continue;
				// get the stored matrix value @k and sum it into position k
				sum += _inputs[i][j].first->matrix[k];
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

SampleType Generator :: get_matrix_average() const {
    SampleType sum(0);
    for (MatrixSizeType i=0; i < _matrix_size; ++i) {
        sum += fabs(matrix[i]);
    }
    return sum / _matrix_size;
}

SampleType Generator :: get_output_average(OutputCountType d) const {
    // this does output and whole matrix averaging.
    // if d is zero, get all frames/channels, otherwise, just get rquested dimension (i.e., 2d gets vector pos 1
	if (d > _output_count) {
		throw std::invalid_argument("a dimension greater than the number available has been requested");
	}	
    VFrameSizeType dims; 
    MatrixSizeType count(0); 
    
	if (d == 0) {
        // this makes a copy
        dims = out_to_matrix_offset;  // do all 
        count = _matrix_size; // will be complete size
    }
    else {
        // need to request from 1 less than submitted positio, as dim 0 is the first dimension; we know tha td is greater tn zero
        OutputCountType dPos = d - 1;
        if (dPos >= out_to_matrix_offset.size()) {
            // this should not happend due to check above
            throw std::invalid_argument("requested dimension is greater than that supported");
        }
        // just store the start position in the matrix for the dimension requested
        dims.push_back(out_to_matrix_offset[dPos]);
        count = _frame_size; // will be one frame size
    }
    SampleType sum(0);    
    // get average of one dim or all 
    for (VFrameSizeType::const_iterator i=dims.begin(); i!=dims.end(); ++i) {
        // from start of dim to 1 less than frame plus start
        for (MatrixSizeType j=(*i); j < (*i) + _frame_size; ++j) {
            sum += matrix[j];
        }
    }
    return sum / count; // cast count to 
}



// should be protected
void Generator :: _set_frame_size(FrameSizeType f) {
    // only change if different; assume already created
	if (!_frame_size_is_resizable) {
		throw std::domain_error("this generator does not support frame size setting");
	}	
	if (f == 0) {
		throw std::invalid_argument("a frame size of 0 is not supported");
	}		
    if (f != _frame_size) {
        _frame_size = f;
        _resize_matrix(); // _matrix_size is set here with dimension
        reset(); // must reset values to zero 
    }
    // when we set the dimension, should we set it for inputs?
}

// protected
void Generator :: _set_output_count(OutputCountType d) {
	// this public method is not used during Generator::__init__
    // only change if different; assume already created
	if (d == 0) {
		throw std::invalid_argument("a dimension of 0 is not supported");
	}	
    if (d != _output_count) {
        _output_count = d;
        _resize_matrix(); // _matrix_size is set here with dimension
        reset(); // must reset values to zero 
    }
    // when we set the dimension, should we set it for inputs?
}

void Generator :: reset() {
    //std::cout << *this << " Generator::reset()" << std::endl;
    // do not need to partion by diminsons
    for (MatrixSizeType i=0; i<_matrix_size; ++i) {
        matrix[i] = 0.0; // set to zero; float or int?
    }
    // always reset frame count?
    _render_count = 0;
	// should reset reset inputs?
}

//..............................................................................
// display methods

std::ostream& operator<<(std::ostream& ostream, const Generator& g) {
    // replace with static cast
    ostream << "<Gen: " << g._class_name << " @" << 
        (int)g._output_count << ">";
    return ostream; 
}

// RENAME: print marix
void Generator :: print_output() {
    // provide name of generator first by dereferencing this
    std::string space1 = std::string(INDENT_SIZE*2, ' ');
    
    std::cout << *this << " Output frame @" << _render_count << ": ";
    for (MatrixSizeType i=0; i<_matrix_size; ++i) {
        if (i % _frame_size == 0) {
            std::cout << std::endl << space1;        
        }
        std::cout << std::setprecision(8) << matrix[i] << '|';
    }
    std::cout << std::endl;
}

void Generator :: print_inputs(bool recursive, UINT8 recurse_level) {
    //std::cout << "recurse_level: " << (int)recurse_level << std::endl;
    
    std::string space1 = std::string(recurse_level*INDENT_SIZE, ' ');
    std::string space2 = std::string((recurse_level+1)*INDENT_SIZE, ' '); 
    std::string space3 = std::string((recurse_level+2)*INDENT_SIZE, ' '); 
    std::cout << space1 << *this << " Inputs:" << std::endl;

    VGenSharedOutPair :: const_iterator j;
    // need an interger as key for _input_parameter_type
    for (ParameterIndexType k=0; k!=_inputs.size(); ++k) {   
        ParameterTypeShared pts = _input_parameter_type[k];
        // need to iterate over each sub vector
        std::cout << space2 << *pts << std::endl;
        for (j=_inputs[k].begin(); j!=_inputs[k].end(); ++j) {
            // deref
            GeneratorShared g = (*j).first;
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


void Generator :: plot_output() {
	Plotter p;
	VSampleType v;
	write_output_to_vector(v); // load matrix into this vecotr
	p.plot(v, get_output_count());
	p.pipe(); // pipe to gnu plot
}



//..............................................................................
// loading and writing matrix

// write matrix
void Generator :: write_output_to_vector(VSampleType& vst) const {
    vst.clear(); // may not be necessary, but insures consistancy
    vst.reserve(_matrix_size);
    for (MatrixSizeType i=0; i<_matrix_size; ++i) {
        vst.push_back(matrix[i]); 
    }
}


// write matrix
void Generator :: write_output_to_fp(const std::string& fp, 
                                    OutputCountType d) const {
    throw std::domain_error("not implemented on base class");                                    
}


// set matrix
void Generator :: set_output_from_array(SampleType* v, MatrixSizeType s, 
								OutputCountType ch, bool interleaved){
	// caller is responsible for releasing called array
	if (s < 1) {
		throw std::domain_error(
            "the supplied vector must have size greater than 0");
	}	
    
	// TODO: add method to do both of these at the same time: set_frame_size_and_or_dimension
	bool reset_needed(true); 
	if (_frame_size_is_resizable) {
		// frame size is the number of outpuots divided by channel
		_set_frame_size(s/ch);
		reset_needed = false;
	}
        
	// must reset values to zero (if not done above) as s may be smaller than outputsize, and we would get mixed content
	if (reset_needed) reset(); 

	// if we did not resize dimensions, limit at max
	if (s > _matrix_size) {
		s = _matrix_size;
	}
	// if we did not resize dimensions, limit at max
	if (ch > _output_count) {
		ch = _output_count;
	}
	
//    std::cout << "set_output_from_array: ch: " << (int)ch << " s: " << s << " matrix size: " << get_matrix_size() << " frame size: " << get_frame_size() << " out_to_matrix_offset[0]] " << (int)out_to_matrix_offset[0] << "out_to_matrix_offset[1]] " << (int)out_to_matrix_offset[1]  << std::endl;
    
	// assuming interleaved source to non-inter dest
	OutputCountType j(0);
	MatrixSizeType i(0);
	MatrixSizeType k(0);
	
	// we will never run over larger area than matrix size, s is less than matrix; we also must not run over v, so we must iterate by s. 
    while (i < s) {
		// step over dimensions
		for (j=0; j<ch; ++j) {
			// i+j maybe greater than s; and not caught above
			if (i >= s) break;
	        matrix[k+out_to_matrix_offset[j]] = v[i];
			//std::cout << i << std::endl;
			i += 1;
		}
		k += 1; // will only reach total/ch
    }
}


// RENAME: set matrix from vector
void Generator :: set_output_from_vector(const VSampleType& vst, 
								OutputCountType ch, bool interleaved) {
    // size is a long; cast ot std::tr1::uint32_t
	MatrixSizeType s = static_cast<MatrixSizeType>(vst.size());
	// pack this in an array for reuse of the same function
    SampleType* v = new SampleType[s];
	// cant use iterator b/c need number
    for (MatrixSizeType i=0; i<s; ++i) {
		v[i] = vst[i];
	}
	set_output_from_array(v, s, ch, interleaved);
	// clean up temporary vector 
	delete[] v;
		
}


// set matrix form fp
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
//    _inputs_frame_size[i].clear();
    
    GenSharedOutPair gsop(gs, 0); // TEMPORARY until we assign otuptus to  
    _inputs[i].push_back(gsop);    
    // store in advance the matrix size of the input
    // _inputs_frame_size[i].push_back(gs->get_matrix_size());
    _update_for_new_input();
}

void Generator :: set_input_by_index(ParameterIndexType i, SampleType v){
    // overridden method for setting a value: generates a constant
	// pass the GeneratorConfig to produce same dimensionality requested
	aw::GeneratorShared c = aw::ConstantShared(
							new aw::Constant(_environment));
    c->init();
    //std::cout << "Generator :: set_input_by_index(" << std::endl;
    
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
    GenSharedOutPair gsop(gs, 0); // TEMPORARY until we assign otuptus to      
    _inputs[i].push_back(gsop);    
    //_inputs_frame_size[i].push_back(gs->get_matrix_size());    
    _update_for_new_input();
}

void Generator :: add_input_by_index(ParameterIndexType i, SampleType v){
    // adding additional as a constant value
    // note that no one else will have a handle on this constant
    // overridden method for setting a sample value: adds a constant	
	// pass the EnvironmentShared to inner Generator 
	aw::GeneratorShared c = aw::ConstantShared(
							new aw::Constant(_environment));
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
    // store in advance the matrix size of the input    
    _update_for_new_slot();
}

void Generator :: set_slot_by_index(ParameterIndexType i, SampleType v){
    // overridden method for setting a value: generates a constant
	// pass the GeneratorConfig to produce same dimensionality requested
	aw::GeneratorShared c = aw::ConstantShared(
							new aw::Constant(_environment));
    c->init();
    c->set_input_by_index(0, v); // this will call Constant::reset()
    set_slot_by_index(i, c); // call overloaded
	// _update_for_new_slot called in set_slot_by_index
}











//------------------------------------------------------------------------------
Constant :: Constant(EnvironmentShared e) 
	// must initialize base class with passed arg
	: Generator(e) {
	_class_name = "Constant"; 
}

Constant :: ~Constant() {
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
    _register_input_parameter_type(pt1);
    
    // do not define any slots (would require an internal Constant an inf  
    // _we store one value for each input  
    _values.push_back(0);           
}

void Constant :: reset() {
    // need overidden reset because have to transfer stored value to the matrix array; normal reset sets the matrix to zer. 
    //std::cout << *this << " Constant::reset()" << std::endl;
    // sum values first, then reseset all matrix to values
    SampleType v(0);
    VSampleType :: const_iterator j;
    // sum stored values to find how matrix should be represented
    for (j = _values.begin(); j!=_values.end(); ++j) {
        v += *j;
    }
    // do not need to partion by diminsons
    MatrixSizeType m_len(get_matrix_size()); 
    for (MatrixSizeType i=0; i<m_len; ++i) {
        matrix[i] = v; // set to the value
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
    // do nothing, as matrix is already set
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
}






//------------------------------------------------------------------------------
Add :: Add(EnvironmentShared e) 
	// must initialize base class with passed arg
	: Generator(e), 
	_sum_opperands(0) { // end intitializer list
    _frame_size_is_resizable = false;		
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
    
    // register slots
//    aw::ParameterTypeChannelsShared so1 = aw::ParameterTypeChannelsShared(new 
//										   aw::ParameterTypeChannels);
//    so1->set_instance_name("Channels");
//    // this will set the 
//	_register_slot_parameter_type(so1);	// create deafult constant, update
//    
//    // set value; will call update for new slot     
//    set_slot_by_index(0, 1);
    
}

// TODO: can permit ADD to have slot change the number of inputs in this case

//void Add :: _update_for_new_slot() {
//	// dimensions already set to largest found in inputs
//	// set frame size based on slot 1; mult sr by seconds requested
//	_set_output_count(static_cast<OutputCountType>(_slots[0]->matrix[0]));
//}


void Add :: render(RenderCountType f) {
    ParameterIndexType i;
    FrameSizeType k;
    ParameterIndexType j;
    
	// matrix size is private, so must get once per render call; if this is a performance problem can expose matrix size
    
    // TODO: not the right way to do this now
    FrameSizeType frameSize = get_frame_size();
    OutputCountType output_count = get_output_count();
    std::size_t gen_count_at_input = 0;
    OutputCountType out(0);
        
    while (_render_count < f) {
        // calling render inputs updates the matrix of all inputs by calling their render functions; after doing so, the outputs are ready for reading
        _render_inputs(f);        
        // for each parameter input; might be more than one; we should have an output dim for each input
        for (i = 0; i<output_count; ++i) {
            // TODO: this should be pre-stored 
            gen_count_at_input = _inputs[i].size();
            // step through each frame             
            for (k=0; k < frameSize; ++k) {
                _sum_opperands = 0;
                // add across for each Gen found in this input
                for (j=0; j<gen_count_at_input; ++j) {
                    out = _inputs[i][j].second;
                    _sum_opperands += _inputs[i][j].first->matrix[
                        // read from this frame plus the offset for out defined; 
                        k + _inputs[i][j].first->out_to_matrix_offset[out]
                        ];
                }
                matrix[(i*frameSize) + k] = _sum_opperands;
            }
		}
        _render_count += 1;
    }    
}



//------------------------------------------------------------------------------
BufferFile :: BufferFile(EnvironmentShared e) 
	// must initialize base class with passed arg
	: Generator(e) {
	_class_name = "BufferFile";
	// this is the unique difference of the BufferFile class 
    _frame_size_is_resizable = true;
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
	_set_frame_size(_slots[0]->matrix[0] * _sampling_rate * get_output_count());
}


void BufferFile :: render(RenderCountType f) {
	// render count must be ignored; instead, we render until we have filled our buffer; this means that the components will have a higher counter than render; need to be reset at beginning and end

	// must reset; might advance to particular render count
	_reset_inputs(); 
	
	// TODO Is there a better way to get input frame size?
	// Need a get smalles from size calculation pre-done
	FrameSizeType input_frame_size = _inputs[0][0].first->get_frame_size();
	
	// assume that all inputs have the saem init frame size
	RenderCountType render_cycles_max = get_frames_per_dimension() / input_frame_size;
	RenderCountType render_count(0);
	
	MatrixSizeType i(0);		
	
	// ignore render count for now; just fill buffer
    while (render_count < render_cycles_max) {

        _render_inputs(render_count);		
		_sum_inputs();
		
		// TODO need to handle multidemsional inputs to dom outputs
		for (i=0; i < input_frame_size; ++i) {
			matrix[i + (render_count * input_frame_size)] = _summed_inputs[0][i];
		}
		
		render_count++;
		
    }

	// we reset after to retrun components to starting state in case someone else uses this later. 	
	_reset_inputs(); 
	
}

void BufferFile :: write_output_to_fp(const std::string& fp, 
                                    OutputCountType d) const {
    // default is d is 0
    // if want ch 2 of stereo, d is 2
    if (d > get_output_count()) {
        // this should not happend due to check above
        throw std::invalid_argument("requested dimension is greater than that supported");
    }
    // can select format here
	int format = SF_FORMAT_AIFF | SF_FORMAT_PCM_16;
                                    
    VFrameSizeType dims;     
    MatrixSizeType count(0);
    
    OutputCountType reqDim(1); // one if requested a specific dim
    if (d==0) {
        reqDim = get_output_count(); // number of dims
        dims = out_to_matrix_offset; //copy         
        count = get_matrix_size();        
    } 
    else { // just write a single dim specified 
        OutputCountType dPos = d - 1;
        if (dPos >= out_to_matrix_offset.size()) {
            // this should not happen due to check above
            throw std::invalid_argument("requested dimension is greater than that supported");
        }
        dims.push_back(out_to_matrix_offset[dPos]);    
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
    MatrixSizeType i(0);
    MatrixSizeType j(0);
    
    // interleave dimensions if more than one requested
    // iterate over frame size; for each frame, we will write that many dimensiosn
    for (i = 0; i < get_frame_size(); ++i) {
        for (VFrameSizeType::const_iterator p = dims.begin(); 
            p != dims.end(); ++p) {
            // if we are stereo, for each frame point we write two points, one at each offset position
            v[j] = matrix[i + (*p)];
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
    MatrixSizeType s =  static_cast<MatrixSizeType>(sh.frames() * sh.channels());
    double* v = new double[s]; // why not use SampleType instaed of double
    sh.readf(v, sh.frames());
    // this call will resize frame/dim
    set_output_from_array(v, s, sh.channels());
    delete[] v;

}






//------------------------------------------------------------------------------
Phasor :: Phasor(EnvironmentShared e) 
	// must initialize base class with passed arg
	: Generator(e),
		_amp(0),
		_amp_prev(1) // init amp previous to 1 so to force reset to 0 on start
		//_period_start_sample_pos(0)
	{
	_class_name = "Phasor"; 
}

Phasor :: ~Phasor() {
}

void Phasor :: init() {
    // the int routie must configure the names and types of parameters
    std::cout << *this << " Phasor::init()" << std::endl;
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
	MatrixSizeType fs = get_frame_size();
	// this is the abs positoin in this frame
	//_abs_sample_pos = f * fs; 		
	// this is the relative postion in this frame
	MatrixSizeType i(0);
	
    while (_render_count < f) {
        // calling render inputs updates the matrix of all inputs by calling their render functions; after doing so, the outputs are ready for reading
        _render_inputs(f);
		
		// get a vector  for each input summing accross all dimensions
		_sum_inputs();
		
		for (i=0; i < fs; ++i) {
			// for each frame position, must get sum across all frequency values calculated.
						
			// iterate over vector of Generators, get matrix value only for first dimension for now; 
			
			_sum_frequency = _summed_inputs[_input_index_frequency][i];

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
			matrix[i] = _amp;
			
		}
		// can use out_to_matrix_offset; stores index position of start of each dimension		        
        _render_count += 1;
    }    
}





} // end namespaces aw



