
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <cassert>

// needed for Buffer
#include <sndfile.hh>

#include "aw_generator.h"
#include "aw_illustration.h"


namespace aw {

//=============================================================================
// ParameterType

ParameterTypeShared ParameterType :: make(ParameterTypeID q){
    ParameterTypeShared p;
    // just get defailt environment
    if (q == ID_Value) {
        p = ParameterTypeValueShared(new ParameterTypeValue);
    }
    else if (q == ID_Frequency) {
        p = ParameterTypeFrequencyShared(new ParameterTypeFrequency);    
    }
    else if (q == ID_Duration) {
        p = ParameterTypeDurationShared(new ParameterTypeDuration);    
    }    
    else if (q == ID_Phase) {
        p = ParameterTypePhaseShared(new ParameterTypePhase);    
    }   
    else if (q == ID_Channels) {
        p = ParameterTypeChannelsShared(new ParameterTypeChannels);    
    }
    else if (q == ID_Trigger) {
        p = ParameterTypeTriggerShared(new ParameterTypeTrigger);    
    }
    else if (q == ID_LowerBoundary) {
        p = ParameterTypeLowerBoundaryShared(new ParameterTypeLowerBoundary);    
    }    
    else if (q == ID_UpperBoundary) {
        p = ParameterTypeUpperBoundaryShared(new ParameterTypeUpperBoundary);    
    }
    
    else {
        throw std::invalid_argument("no matching ParaameterTypeID");
    }
    return p;
}

ParameterType :: ParameterType() {
    _class_name = "ParameterType";
}

std::ostream& operator<<(std::ostream& ostream, const ParameterType& pt) {
    ostream << "<Pmtr: " << pt._class_name << ": " << pt._instance_name << ">";
    return ostream; 
}

//-----------------------------------------------------------------------------
ParameterTypeValue :: ParameterTypeValue() {
    _class_name = "ParameterTypeValue";
    _class_id = ID_Value;
}

//-----------------------------------------------------------------------------
ParameterTypeFrequency :: ParameterTypeFrequency() {
    _class_name = "ParameterTypeFrequency";
    _class_id = ID_Frequency;
}

//-----------------------------------------------------------------------------
ParameterTypeDuration :: ParameterTypeDuration() {
    _class_name = "ParameterTypeDuration";
    _class_id = ID_Duration;
}

//-----------------------------------------------------------------------------
ParameterTypePhase :: ParameterTypePhase() {
    _class_name = "ParameterTypePhase";
    _class_id = ID_Phase;
}

//-----------------------------------------------------------------------------
ParameterTypeChannels :: ParameterTypeChannels() {
    _class_name = "ParameterTypeChannels";
    _class_id = ID_Channels;
}

//-----------------------------------------------------------------------------
ParameterTypeTrigger :: ParameterTypeTrigger() {
    _class_name = "ParameterTypeTrigger";
    _class_id = ID_Trigger;
}

//-----------------------------------------------------------------------------
ParameterTypeLowerBoundary :: ParameterTypeLowerBoundary() {
    _class_name = "ParameterTypeLowerBoundary";
    _class_id = ID_LowerBoundary;
}

ParameterTypeUpperBoundary :: ParameterTypeUpperBoundary() {
    _class_name = "ParameterTypeUpperBoundary";
    _class_id = ID_UpperBoundary;
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
GeneratorShared  Generator :: make_with_environment(GeneratorID q, 
            EnvironmentShared e) {                        
    GeneratorShared g;    
    if (q == ID_Constant) {
        g = ConstantShared(new Constant(e));
    }
    else if (q == ID_Add) {
        g = AddShared(new Add(e));    
    }
    else if (q == ID_Multiply) {
        g = MultiplyShared(new Multiply(e));    
    }    
    else if (q == ID_Buffer) {
        g = BufferShared(new Buffer(e));    
    }	
    else if (q == ID_Phasor) {
        g = PhasorShared(new Phasor(e));    
    }
    else if (q == ID_Sine) {
        g = SineShared(new Sine(e));    
    }
    else if (q == ID_Map) {
        g = MapShared(new Map(e));
    }
    else if (q == ID_AttackDecay) {
        g = AttackDecayShared(new AttackDecay(e));
    }
    else {
        throw std::invalid_argument("no matching GeneratorID");
    }
    // call subclass init, which calls baseclass init
    g->init();
    return g;
}


GeneratorShared  Generator :: make(GeneratorID q){
	// just get default environment
    EnvironmentShared e = EnvironmentShared(new Environment);
    return make_with_environment(q, e);
}

//.............................................................................
Generator :: Generator(EnvironmentShared e) 
	// this is the only constructor for Generator; the passed-in GenertorConfigShared is stored in the object and used to set init frame frame size.
	: _class_name("Generator"), 
    //gid(-1), // want this to not match any known value
	_output_count{0}, // always init to 1; can change in init
	_outputs_size{0}, // will be updated after resizing
    _input_count{0},
    _slot_count{0},
	_environment{e}, // replace with Environment
    
	// protected ...
	_frame_size{1}, // set in init;
	_sampling_rate{0}, // set from Envrionment in init()
	_nyquist{0}, // set from Envrionment in init()

    _frame_size_is_resizable{false},
    _render_count{0} {
	//outputs(NULL) {
    //std::cout << "Generator (1 arg): Constructor" << std::endl;
}


void Generator :: init() {
    // we only set sampling reate at init; thus, we can call init to reset SR
    // the frame size is eithe common or larger; thus, we generally only read as many as we have from a client object.
    _frame_size = _environment->get_common_frame_size();    
    _sampling_rate = _environment->get_sampling_rate();
	_nyquist = _sampling_rate / 2; // let floor
    	
    aw::ParameterTypeShared pt1 = aw::ParameterType::make( 
            aw::ParameterType::ID_Value);
    pt1->set_instance_name("Generator default output");
    _register_output_parameter_type(pt1);
}

void Generator :: reset() {
    ParameterIndexType i;
    FrameSizeType j;
    SampleType n(0);
    for (i=0; i<_output_count; ++i) {
        for (j=0; j < _frame_size; ++j) {
            outputs[i][j] = n;
        }
    }
    // always reset render count?
    _render_count = 0;
	// should reset reset inputs?
}

void Generator :: set_default() {
    // empty base class; override in derived classes and called in init() routines
}

//.............................................................................
// private methods
void Generator :: _resize_outputs() {
    // size is always dim * _frame_size
    // only do this once to avoid repeating
    _outputs_size = _output_count * _frame_size;
    
    ParameterIndexType i;
    VVSampleType::iterator j; // not a const       
    
    // add output vectors if necessary
    if (outputs.size() < _output_count) {
        // if size is 2, need 4, iter 2, 3 to get 4
        for (i=outputs.size(); i< _output_count; ++i) {
            VSampleType vs;
            outputs.push_back(vs);
        }
    }     
    else if (outputs.size() > _output_count) {
        j = outputs.begin();
        outputs.erase(j+_output_count, j+outputs.size());
    }
    
    assert(outputs.size() == _output_count);
    
    SampleType n(0);
    // reserve each component; set value to 0
	// must be sure all have the same _frame_size
    for (j=outputs.begin(); j!=outputs.end(); ++j) {
		// this will also set all values to 0.0
		(*j).resize(_frame_size, n);
    }
	// reset to set all values to zero and reset render count
	reset();
    
}

void Generator :: _register_output_parameter_type(ParameterTypeShared pts) {
	// called in derived init() to setup a output types; this does not preprae storage ; 
    _output_parameter_type[_output_count] = pts;
    _output_count += 1;
	_resize_outputs(); // will use _output_count value
}

void Generator :: _clear_output_parameter_types() {
    _output_count = 0;
    outputs.clear();
}

void Generator :: _register_input_parameter_type(ParameterTypeShared pts) {
	// called in derived init() to setup a input types and prepare storage
    // storing in an unordered map
    _input_parameter_type[_input_count] = pts;

    // store a vector in the position to accept inputs
    VGenSharedOutPair vInner;  
    _inputs.push_back(vInner); // extr copy made here
    	
	VSampleType vSampleTypeInner;
	_summed_inputs.push_back(vSampleTypeInner);
    // must do this after pushing_back, as makes a copy
    // resize to store initialzied values and use like an array
    // could user reserve here
	SampleType n(0);	
    _summed_inputs[_input_count].resize(get_common_frame_size(), n);    
    _input_count += 1;
}

void Generator :: _clear_input_parameter_types() {
    _input_count = 0;
    _inputs.clear();
    _summed_inputs.clear();
}

void Generator :: _register_slot_parameter_type(ParameterTypeShared pts) {
	// called in derived init()
	// set dictionary directly
    _slot_parameter_type[_slot_count] = pts;		
	_slots.push_back(GeneratorShared()); // store empty to hold position
    _slot_count += 1;
}

void Generator :: _update_for_new_slot() {
}

void Generator :: _render_inputs(RenderCountType f) {
    // for each of the inputs, need to render up to this frame
    // this is "pulling" lower-level values up to date
    // passing render count seems somewhat wasteful as in most cases we just will be advancing by 1 more than the previous frame count value
    // this is inlined in the header
    ParameterIndexType j;
    ParameterIndexType gen_count_at_input(0);
    for (ParameterIndexType i = 0; i<_input_count; ++i) {
        // inputs are a vector of Generators
        gen_count_at_input = _inputs[i].size();        
        for (j=0; j<gen_count_at_input; ++j) {
            // this is a pair of shared generator, outputs index
            _inputs[i][j].first->render(f);
        }
    }
}

void Generator :: _reset_inputs() {
    // NOTE: this is not called on reset(), and thus this is not yet recurssive
    VGenSharedOutPair :: const_iterator j; // vector of generators    
    for (ParameterIndexType i = 0; i<_input_count; ++i) {
        // inputs are a vector of Generators
        for (j=_inputs[i].begin(); j!=_inputs[i].end(); ++j) {
            // this is a shared generator
            (*j).first->reset();
        }
    }
}

void Generator :: _sum_inputs(FrameSizeType fs) {
    // this is inlined in render() calls
    // note that this is nearly identical to Add :: render(); here we store the results in _summed_inputs, not in outputs; we also do not render inputs
    // when reading inputs, we must assume they are the common frame size, not necessarily our frame size (like when we are a buffer)
    ParameterIndexType i;
    FrameSizeType k;
    ParameterIndexType j;
    ParameterIndexType gen_count_at_input(0);
	SampleType sum(0);
    
	// iterate over each input parameter type
    for (i=0; i<_input_count; ++i) {
        // do not need to _summed_inputs[i].clear(), as we are repalcing with a new SampleTyple sum; _summed_inputs[i].capacity() == frameSize due to reserving in _register_input_parameter_type
        gen_count_at_input = _inputs[i].size();        
		// for each frame, read across all input
		for (k=0; k < fs; ++k) {
			sum = 0;
			// now iterate over each gen in this input
			for (j=0; j<gen_count_at_input; ++j) {
				sum += _inputs[i][j].first->outputs[
                            _inputs[i][j].second // the output to read
                        ][k];
			}
			// sum of all gens at this sample frame
            _summed_inputs[i][k] = sum;
		}
	}
}

//..............................................................................
// public methods

void Generator :: render(RenderCountType f) {
    // this is a dummy render method just for demonstartion; at the end of a render cycle _render_count must be the same as `f`
    _render_count = f;
}

//..............................................................................
SampleType Generator :: get_outputs_average() const {
    SampleType sum(0);
    for (ParameterIndexType i=0; i<_output_count; ++i) {
        for (FrameSizeType j=0; j < _frame_size; ++j) {
            sum += fabs(outputs[i][j]);
        }
    }
    return sum / _outputs_size;
}

SampleType Generator :: get_output_average(ParameterIndexType d) const {
    // this does single output and whole outputs averaging.
    // if d is zero, get all frames/channels, otherwise, just get rquested dimension (i.e., 2d gets vector pos 1
	if (d > _output_count) {
		throw std::invalid_argument("a dimension greater than the number available has been requested");
	}	
    VParameterIndexT dims; 
    OutputsSizeT count(0); 
    ParameterIndexType p;
    
	if (d == 0) {
        // get all
        for (p=0; p<_output_count; ++p) {
            dims.push_back(p);
        }
        count = _outputs_size; // will be complete size
    }
    else {
        // need to request from 1 less than submitted positio, as dim 0 is the first dimension; we know tha td is greater tn zero
        ParameterIndexType dPos = d - 1;
        if (dPos >= _output_count) {
            // this should not happend due to check above
            throw std::invalid_argument("requested dimension is greater than that supported");
        }
        // just store the start position in the outputs for the dimension requested
        dims.push_back(dPos);
        count = _frame_size; // will be one frame size
    }
    SampleType sum(0);    
    // get average of one dim or all 
    for (VParameterIndexT::const_iterator i=dims.begin(); i!=dims.end(); ++i) {
        // from start of dim to 1 less than frame plus start
        for (FrameSizeType j=0; j < _frame_size; ++j) {
            sum += outputs[*i][j];
        }
    }
    return sum / count; // cast count to 
}

void Generator :: _set_frame_size(FrameSizeType f) {
    // protected
    // only change if different; assume already created
	if (!_frame_size_is_resizable) {
		throw std::domain_error("this generator does not support frame size setting");
	}	
	if (f == 0) {
		throw std::invalid_argument("a frame size of 0 is not supported");
	}	
	// must always set and resize, as our output number might be different
	_frame_size = f;
	_resize_outputs(); // _outputs_size is set here with dimension
	reset(); // must reset values to zero 
    // when we set the dimension, should we set it for inputs?
}


//void Generator :: _set_output_count(ParameterIndexType d) {
	//// this public method is not used during Generator::__init__
    //// only change if different; assume already created
	//if (d == 0) {
		//throw std::invalid_argument("a dimension of 0 is not supported");
	//}	
	//_output_count = d;
	//_resize_outputs(); // _outputs_size is set here with dimension
	//reset(); // must reset values to zero 
    //// when we set the dimension, should we set it for inputs?
//}

//..............................................................................
// display methods

std::string Generator :: get_name_address() const {
    std::stringstream s;
    // get address of self, dereferenced, without delimiters
    s << _class_name << &(*this); 
    return s.str();
} 

std::string Generator :: get_label_address() const {
    std::stringstream s;
    // get address of self, dereferenced
    s << "Gen::" << _class_name << "{" << &(*this) << "}"; 
    return s.str();
} 

std::string Generator :: get_label() const {
    std::stringstream s;
    s << "<" << get_label_address() << 
        " x{" << static_cast<int>(_slot_count) << "}" <<     
        "i{" << static_cast<int>(_input_count) << "}" <<    
        "o{" << static_cast<int>(_output_count) << "}" <<         
        ">";    
    return s.str();
} 

std::ostream& operator<<(std::ostream& ostream, const Generator& g) {
    ostream << g.get_label();
    return ostream; 
}

std::ostream& operator<<(std::ostream& ostream, const GeneratorShared g) {
    ostream << g->get_label(); // get from shared pointer
    return ostream; 
}

void Generator :: print_outputs(FrameSizeType start, FrameSizeType end) {
    // provide name of generator first by dereferencing this
    std::string space1 = std::string(INDENT_SIZE*2, ' ');
    if (end == 0 || end < start) {
        end = _frame_size;
    }
    if (start > end) {
        start = 0;
    }
    std::cout << *this << " Output rc{" << _render_count << "} f{" << start << "-" << end << "}: ";    
    ParameterIndexType i;
    FrameSizeType j;
    for (i=0; i<_output_count; ++i) {
        std::cout << std::endl << space1;            
        for (j=start; j < end; ++j) {
            std::cout << std::setprecision(8) << outputs[i][j] << '|';
        }
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

// illustrate outputs
void Generator :: illustrate_outputs() {
	TimeDomainGraph p;
    // pass a version this instancea as a Shared Pointer
	p.draw(shared_from_this());
	p.pipe(); // pipe to gnu plot
}

void Generator :: illustrate_network() {
	NetworkGraph p;
    // pass a version this instancea as a Shared Pointer
	p.draw(shared_from_this());
    //p.print(); // for debugging
	p.pipe(); // pipe to gnu plot
}

//..............................................................................
// loading and writing outputs

void Generator :: write_outputs_to_vector(VSampleType& vst) const {
    // writing out to a flat vector
    
    vst.clear(); // may not be necessary, but insures consistancy
    vst.resize(_outputs_size, SampleType(0));    
    ParameterIndexType i;
    FrameSizeType j;
    OutputsSizeT n(0);
            
    for (i=0; i<_output_count; ++i) {
        for (j=0; j < _frame_size; ++j) {        
            vst[n] = outputs[i][j];
            n += 1;
        }
    }
}

void Generator :: write_output_to_fp(const std::string& fp, 
                                    ParameterIndexType d) const {
    throw std::domain_error("not implemented on base class");
}

void Generator :: set_outputs_from_array(SampleType* v, OutputsSizeT s, 
								ParameterIndexType ch, bool interleaved){
    // low level method of a raw, interleaved array as input; need size;
    // we need this b/c libsndfile uses raw arrays
	// caller is responsible for releasing passed in array
	// we will read in only as many channels as we currently have as outputs
	if (s < 1) {
        //std::cout << "passed size: " << s << std::endl;
		throw std::domain_error(
            "the supplied vector must have size greater than 0");
	}
    
	bool reset_needed(true); 
	// if possible, we will resize the frame size; outs will not be changed
	if (_frame_size_is_resizable) {
		// frame size is the number of outpuots divided by channel
		_set_frame_size(s/ch);
		reset_needed = false;
	}
	// must reset values to zero (if not done above) as s may be smaller than outputsize, and we would get mixed content
	if (reset_needed) reset(); 

	// determine how many outputs to read; try to take all unless greater than output
	ParameterIndexType out_count_to_take(ch);
	if (ch > _output_count) {
		out_count_to_take = _output_count;
	}
	    
	// assuming interleaved source to non-interleved destination; audio inputs are normally interleaved
	ParameterIndexType j(0);
	OutputsSizeT i(0); // for each sample in source
	OutputsSizeT k(0); // for each sample we write
	
	// we will never run over larger area than outputs size, s is less than outputs; we also must not run over v, so we must iterate by s. 
    while (i < s) {
		// step over channels in source
		for (j=0; j<ch; ++j) {
			//if (i >= s) break; // dont overflow outputs
			if (j >= out_count_to_take) {
				i += 1;				
				continue; // dont write more than we can take				
			}
			// can check here if write pos in outputs is out of bounds
	        //outputs[k + out_to_matrix_offset[j]] = v[i];
            outputs[j][k] = v[i];
			//std::cout << i << std::endl;
			i += 1;
		}
		k += 1; // increment only once for each bundle of channel infor written
	}
}

void Generator :: set_outputs_from_vector(VSampleType& vst, 
								ParameterIndexType ch, bool interleaved) {
	// this is set outputs as we will try to set all channels available
    // size is a long; cast ot std::uint32_t
	OutputsSizeT s = static_cast<OutputsSizeT>(vst.size());
    // pointer to first element simulating an array
    SampleType* v = &vst[0]; // get a pointer to this vector
    // this will resize and reset if possible in dervied Generator
	set_outputs_from_array(v, s, ch, interleaved);
}

void Generator :: set_outputs_from_fp(const std::string& fp) {
    // vitual method overridden in Buffer (so as to localize use of libsndfile
    throw std::domain_error("not implemented on base class");

}


void Generator :: set_outputs(const BufferInjectorShared bi) {
    // vitual method overridden in Buffer
    throw std::domain_error("not implemented on base class");
}

void Generator :: set_outputs(const std::string& fp) {
    // vitual method overridden in Buffer
    throw std::domain_error("not implemented on base class");

}


//..............................................................................
// parameter translating

ParameterIndexType Generator :: get_input_index_from_parameter_name(
    const std::string& s) {
    // match the string to the name returned by get_instance_name; 
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

ParameterIndexType Generator :: get_input_index_from_class_id(const
        ParameterType::ParameterTypeID ptid) {
    // match the string to the name returned by get_instance_name; 
    Generator :: MapIndexToParameterTypeShared ::const_iterator k;
    for (k = _input_parameter_type.begin();
        k != _input_parameter_type.end(); 
        ++k) {
        // each value (second( is a ParameterTypeShared)
        if (ptid == k->second->get_class_id()) return k->first;
    }
	throw std::invalid_argument("no matching parameter type");
}

ParameterIndexType Generator :: get_slot_index_from_parameter_name(    
    const std::string& s) {
    // not yet implemented
	throw std::invalid_argument("no matching parameter name: " + s);
}

//..............................................................................
// parameter setting and adding; all overloaded for taking generator or sample type values, whcich auto-creates constants.

Generator::VGenSharedOutPair Generator :: get_input_gen_shared_by_index(
                        ParameterIndexType i) {
    if (_input_count <= 0 or i >= _input_count) {
        throw std::invalid_argument("Parameter index is not available.");		
    }
    // NOTE: this is copying all contents, but probably not a problem b/c just shared pointers
    Generator::VGenSharedOutPair post(_inputs[i]);
    return post;
}

void Generator :: set_input_by_index(
        ParameterIndexType i,
        GeneratorShared gs,
        ParameterIndexType pos){
    // if zero, none are set; current value is next available slot for registering
    if (_input_count <= 0 or i >= _input_count) {
        throw std::invalid_argument("Parameter index is not available.");                                        
    }
    if (pos >= gs->get_output_count()) {
        throw std::invalid_argument("position exceeds output count on passed input");    
    }    
    // this removes all stored values
    _inputs[i].clear();    
    GenSharedOutPair gsop(gs, pos);  
    _inputs[i].push_back(gsop);    
}

void Generator :: set_input_by_index(
        ParameterIndexType i,
        SampleType v,
        ParameterIndexType pos){
    // overridden method for setting a value: generates a constant
	// pass the GeneratorConfig to produce same dimensionality requested
    aw::GeneratorShared c = aw::Generator::make_with_environment(ID_Constant, 
            _environment);
    c->set_input_by_index(0, v); // this will call Constant::reset()
    set_input_by_index(i, c, pos); // call overloaded
}


void Generator :: set_input_by_class_id(
        ParameterType::ParameterTypeID ptid,
        GeneratorShared gs,
        ParameterIndexType pos){
    ParameterIndexType i = get_input_index_from_class_id(ptid);
    set_input_by_index(i, gs, pos); // call overloaded
}

void Generator :: set_input_by_class_id(
        ParameterType::ParameterTypeID ptid,
        SampleType v,
        ParameterIndexType pos){
    ParameterIndexType i = get_input_index_from_class_id(ptid);
    set_input_by_index(i, v, pos); // call overloaded
}


void Generator :: add_input_by_index(ParameterIndexType i, 
        GeneratorShared gs, ParameterIndexType pos){
    // pos is the output of the generator to connect into the specified input
    if (_input_count <= 0 or i >= _input_count) {
        throw std::invalid_argument("Parameter index is not available.");
	}
    if (pos >= gs->get_output_count()) {
        throw std::invalid_argument("position exceeds output count on passed input");    
    }
    // adding additiona, with a generator
    GenSharedOutPair gsop(gs, pos);     
    _inputs[i].push_back(gsop);    
}

void Generator :: add_input_by_index(ParameterIndexType i, SampleType v, 
        ParameterIndexType pos){
    // adding additional as a constant value
    // note that no one else will have a handle on this constant
    // overridden method for setting a sample value: adds a constant	
	// pass the EnvironmentShared to inner Generator 
    aw::GeneratorShared c = aw::Generator::make_with_environment(ID_Constant, 
            _environment);    
    c->set_input_by_index(0, v); // this will call Constant::reset()
    add_input_by_index(i, c, pos); // other overloaded
}


//..............................................................................
// public slot control 
void Generator :: set_slot_by_index(ParameterIndexType i, GeneratorShared gs, 
									bool update){
    // if zero, none are set; current value is next available slot for registering]
	// updat defaults to true in header	
    if (_slot_count <= 0 or i >= _slot_count) {
        throw std::invalid_argument("Slot index is not available.");		
    }        
    _slots[i] = gs; // direct assignment; replaces default if not there
    // store in advance the outputs size of the input 
	if (update) {
		_update_for_new_slot();
	}
}

void Generator :: set_slot_by_index(ParameterIndexType i, SampleType v, 
									bool update){
    // overridden method for setting a value: generates a constant
	// pass the GeneratorConfig to produce same dimensionality requested
	// updat defaults to true in header
    aw::GeneratorShared c = aw::Generator::make_with_environment(ID_Constant, 
            _environment);
    c->set_input_by_index(0, v); // this will call Constant::reset()
    set_slot_by_index(i, c, update); // call overloaded
	// _update_for_new_slot called in set_slot_by_index
}


GeneratorShared Generator :: get_slot_gen_shared_at_index(
										  ParameterIndexType i) {
    if (_slot_count <= 0 or i >= _slot_count) {
        throw std::invalid_argument("Parameter index is not available.");		
    }
    return _slots[i];
}









//------------------------------------------------------------------------------
Constant :: Constant(EnvironmentShared e) 
	// must initialize base class with passed arg
	: Generator(e) {
	_class_name = "Constant"; 
    _class_id = ID_Constant;
}

void Constant :: init() {
    //std::cout << *this << " Constant::init()" << std::endl;
    // resize and reset after setting parameters
    // this will call the virtual reset 
    Generator::init();
    _clear_output_parameter_types(); // must clear the default set by Gen init
	
    // register some parameters
//    aw::ParameterTypeValueShared pt1 = aw::ParameterTypeValueShared(new 
//                                       aw::ParameterTypeValue);
    aw::ParameterTypeShared pt1 = aw::ParameterType::make(
            aw::ParameterType::ID_Value);
    pt1->set_instance_name("Constant numerical value");
    _register_output_parameter_type(pt1);	
    _register_input_parameter_type(pt1);
	
	assert(get_output_count() == 1);
    // do not define any slots (would require an internal Constant)  
    // _we store one value for each "gen" assigned to input  
    _values.push_back(0);           
}

void Constant :: reset() {
    // need overidden reset because have to transfer stored value to the outputs array; normal reset sets the outputs to zer. 
    //std::cout << *this << " Constant::reset()" << std::endl;
    // sum values first, then reseset all outputs to values
    SampleType v(0);
    VSampleType :: const_iterator k;
    // sum stored values to find how outputs should be represented
    for (k = _values.begin(); k!=_values.end(); ++k) {
        v += *k;
    }
    ParameterIndexType i;
    ParameterIndexType outs = get_output_count();
    FrameSizeType j;
    FrameSizeType frames = get_frame_size();
    
    for (i=0; i<outs; ++i) {
        for (j=0; j < frames; ++j) {
            outputs[i][j] = v;
        }
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
    // do nothing, as outputs is already set
    _render_count = f;
}


Generator::VGenSharedOutPair Constant :: get_input_gen_shared_by_index(
										  ParameterIndexType i) {
	// overridden virtual method
    // return an empty vector for clients to iterate over
    Generator::VGenSharedOutPair post;
    return post;
}

void Constant :: set_input_by_index(ParameterIndexType i, GeneratorShared gs, ParameterIndexType pos) {
    throw std::invalid_argument("invalid to set a GeneratoreShared as a value to a Constant");                                        
}

void Constant :: set_input_by_index(ParameterIndexType i, SampleType v, ParameterIndexType pos){
    if (get_input_count() <= 0 or i >= get_input_count()) {
        throw std::invalid_argument("Parameter index is not available.");                                        
    }
    // store the passed SampleType value in the _values vector
    _values.clear();
    _values.push_back(v);    
    reset(); 
}

void Constant :: add_input_by_index(ParameterIndexType i, 
                                    GeneratorShared gs, ParameterIndexType pos) {
    throw std::invalid_argument("invalid to add a GeneratoreShared as a value to a Constatn");
}

void Constant :: add_input_by_index(ParameterIndexType i, SampleType v, ParameterIndexType pos){
    if (get_input_count() <= 0 or i >= get_input_count()) {
        throw std::invalid_argument("Parameter index is not available.");
	}
    _values.push_back(v);    
    reset();  
}






//------------------------------------------------------------------------------
_BinaryCombined :: _BinaryCombined(EnvironmentShared e) 
	// must initialize base class with passed arg
	: Generator(e), 
	_n_opperands(0) { // end intitializer list
    _frame_size_is_resizable = false;
    // the following need to be set in derived classes
//	_class_name = "Add";
//    _class_id = ID_Add;
    _op_switch = '+';
    _n_opperands_init = 0; // must be 1
}

void _BinaryCombined :: init() {
    // the int routine must configure the names and types of parameters
    Generator::init();
    // must clear the default set by Gen init because slot will set directly    
    _clear_output_parameter_types();
    
    aw::ParameterTypeShared so1 = aw::ParameterType::make(
            aw::ParameterType::ID_Channels);
                                           
    so1->set_instance_name("Channels");
	_register_slot_parameter_type(so1);	// create deafult constant, update
    // set value to 1; will call _update_for_new_slot    
    set_slot_by_index(0, 1);
}


void _BinaryCombined :: _update_for_new_slot() {
    // this is a small int; might overflow of trying to create large number of outs
    ParameterIndexType outs = static_cast<ParameterIndexType>(_slots[0]->outputs[0][0]);
    if (outs <= 0) {
        throw std::invalid_argument("outputs must be greater than or equal to zero");
    }
    _clear_output_parameter_types();	
    _clear_input_parameter_types();
	
    std::stringstream s;
    aw::ParameterTypeShared pt;    
    // set inputs; this will clear any existing connections
    for (ParameterIndexType i=0; i<outs; ++i) {
        //pt = aw::ParameterTypeValueShared(new aw::ParameterTypeValue);
        pt = aw::ParameterType::make(aw::ParameterType::ID_Value);
        s.str(""); // clears contents; not the same as .clear()
        s << "Opperands " << i+1;
        pt->set_instance_name(s.str());
		// we can use the same pt for both i/o
	    _register_output_parameter_type(pt);			
        _register_input_parameter_type(pt);        
    }
	assert(get_output_count() == outs);
	assert(get_input_count() == outs);	
}


void _BinaryCombined :: render(RenderCountType f) {

    ParameterIndexType i;
    ParameterIndexType input_count(get_input_count());
    FrameSizeType k;
    ParameterIndexType j;    

    ParameterIndexType gen_count_at_input(0);
    ParameterIndexType out(0);
        
    while (_render_count < f) {
        // calling render inputs updates the outputs of all inputs by calling their render functions; after doing so, the outputs are ready for reading
        _render_inputs(f);
        
        // for each parameter input we have an output
        for (i = 0; i < input_count; ++i) {
            gen_count_at_input = _inputs[i].size();
            // step through each frame             
            for (k=0; k < _frame_size; ++k) {
                _n_opperands = _n_opperands_init; // declared in class
                // add across for each Gen found in this input
                for (j=0; j<gen_count_at_input; ++j) {
                    out = _inputs[i][j].second;
                    //std::cout << _inputs[i][j].first << " got output: " << (int)out << " value: " << _inputs[i][j].first->outputs[out][k] <<  std::endl;
                    if (_op_switch == '+') {
                        _n_opperands += _inputs[i][j].first->outputs[out][k];
                    } else if (_op_switch == '*') {
                        _n_opperands *= _inputs[i][j].first->outputs[out][k];                    
                    }
                }
                // store in out channel for each input
                outputs[i][k] = _n_opperands;
            }
		}
        _render_count += 1;
    }    
}



//------------------------------------------------------------------------------
Add :: Add(EnvironmentShared e)
	// must initialize base class with passed arg
	: _BinaryCombined(e) {
	_class_name = "Add";  // override what is set in Add
    _class_id = ID_Add;
    _op_switch = '+';
    _n_opperands_init = 0;
}

void Add :: init() {
    _BinaryCombined::init(); // must call base init; calls Generator::init()
}

//------------------------------------------------------------------------------
Multiply :: Multiply(EnvironmentShared e) 
	// must initialize base class with passed arg
	: _BinaryCombined(e) {
	_class_name = "Multiply";  // override what is set in Add
    _class_id = ID_Multiply;            
    _op_switch = '*';
    _n_opperands_init = 1; // must be 1    
}

void Multiply :: init() {
    _BinaryCombined::init(); // must call base init; calls Generator::init()
}


//------------------------------------------------------------------------------
// comparison





//------------------------------------------------------------------------------
Buffer :: Buffer(EnvironmentShared e) 
	// must initialize base class with passed arg
	: Generator(e) {
	_class_name = "Buffer";
    _class_id = ID_Buffer;        
	// this is the unique difference of the Buffer class 
    _frame_size_is_resizable = true;
}


void Buffer :: init() {
    // the int routie must configure the names and types of parameters
    // std::cout << *this << " Buffer::init()" << std::endl;
    // call base init, allocates and resets()
    Generator::init();    
    _clear_output_parameter_types(); // must clear the default set by Gen init

    // register some slots: 
    // register slots
    aw::ParameterTypeShared so1 = aw::ParameterType::make( 
            aw::ParameterType::ID_Channels);                                           
    so1->set_instance_name("Channels");
	_register_slot_parameter_type(so1);
    set_slot_by_index(0, 1, false); // false so as to not update until dur is set
	    
    aw::ParameterTypeShared so2 = aw::ParameterType::make( 
            aw::ParameterType::ID_Duration);
    so2->set_instance_name("Duration in seconds");
	_register_slot_parameter_type(so2);
    // set value; will call _update_for_new_slot    
    set_slot_by_index(1, 1, true); // one second, update now 
}

void Buffer :: _update_for_new_slot() {
	// slot 0: channels
    // this is a small int; might overflow of trying to create large number of outs
    ParameterIndexType outs = static_cast<ParameterIndexType>(_slots[0]->outputs[0][0]);
    if (outs <= 0) {
        throw std::invalid_argument("outputs must be greater than or equal to zero");
    }
    // reset i/o, dropping all contained input connections;
    _clear_input_parameter_types();
    _clear_output_parameter_types(); // must clear the default set by Gen init
	
    std::stringstream s;
    aw::ParameterTypeValueShared pt_i;
    aw::ParameterTypeValueShared pt_o;    	
    // set inputs; this will clear any existing connections
    for (ParameterIndexType i=0; i<outs; ++i) {        
        aw::ParameterTypeShared pt_i = aw::ParameterType::make( 
                aw::ParameterType::ID_Value);        
        s.str(""); // clears contents; not the same as .clear()
        s << "Input " << i+1;
        pt_i->set_instance_name(s.str());
        _register_input_parameter_type(pt_i);        
		
        aw::ParameterTypeShared pt_o = aw::ParameterType::make( 
                aw::ParameterType::ID_Value);
        s.str(""); // clears contents; not the same as .clear()
        s << "Output " << i+1;
        pt_o->set_instance_name(s.str());
		_register_output_parameter_type(pt_o);
    }
	assert(get_output_count() == outs);
	assert(get_input_count() == outs);	
	// slot 1: duration 	
	_set_frame_size(_slots[1]->outputs[0][0] * _sampling_rate);
}


void Buffer :: render(RenderCountType f) {
	// render count must be ignored; instead, we render until we have filled our buffer; this means that the components will have a higher counter than render; need to be reset at beginning and end

	// must reset; might advance to particular render count
	_reset_inputs(); 
	
	// we assume that all inputs have the same frame size as standard frame size
	FrameSizeType cfs = get_common_frame_size();
    FrameSizeType fs = get_frame_size();
	RenderCountType rc(0); // must start with request for frame 1
	OutputsSizeT i(0);
	ParameterIndexType j(0);
	ParameterIndexType out_count(get_output_count()); // out is always same as in 
	RenderCountType pos(0);
    
	// ignore render count for now; just fill buffer
    while (true) {
        _render_inputs(rc+1); // render count must start at 1
		_sum_inputs(cfs); // we use common frame size, not our frame size
		for (i=0; i < cfs; ++i) {
            pos = i + (rc * cfs); // where we write in the buffer
            if (pos >= fs) break; // we end when we are full
            
			for (j=0; j<out_count; ++j) { // iter over inputs/outputs
				//outputs[i + (rc * cfs) + out_to_matrix_offset[j]] = _summed_inputs[j][i];
				outputs[j][pos] = _summed_inputs[j][i];                
			}
		}
        if (pos >= fs) break; // we end when we are full        
		rc++;
    }
	// we reset after to retrun components to starting state in case someone else uses this later. 	
	_reset_inputs(); 
	
}

void Buffer :: write_output_to_fp(const std::string& fp, 
                                    ParameterIndexType d) const {
    // default is d is 0, which is all 
    // if want ch 2 of stereo, d is 2
    if (d > get_output_count()) {
		throw std::invalid_argument("dimension greater than that supported is requested");
    }
    // can select format here
	int format = SF_FORMAT_AIFF | SF_FORMAT_PCM_16;
                                    
    VParameterIndexT dims; 
    OutputsSizeT count(0);
    ParameterIndexType p;    
    
    ParameterIndexType reqDim(1); // one if requested a specific dim
    if (d==0) {
        reqDim = get_output_count(); // number of dims
        //dims = out_to_matrix_offset; //copy entire vector     
        for (p=0; p<get_output_count(); ++p) {
            dims.push_back(p);
        }        
        count = get_outputs_size();        
    } 
    else { // just write the single dim specified 
        ParameterIndexType dPos = d - 1; // if request dim 2, want offset for 1
        //dims.push_back(out_to_matrix_offset[dPos]);    
        dims.push_back(dPos);        
        count = get_frame_size(); // only need one frame
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
    OutputsSizeT i(0);
    OutputsSizeT j(0);
    
    FrameSizeType frameSize = get_frame_size();

    // interleave dimensions if more than one requested
    // iterate over frame size; for each frame, we will write that many dimensiosn
    for (i = 0; i < frameSize; ++i) {
        for (VParameterIndexT::const_iterator p = dims.begin(); p != dims.end(); ++p) {
            // if we are stereo, for each frame point we write two points, one at each offset position
            v[j] = outputs[*p][i];
            ++j; // abs position
        }
    }
    assert(j==count);
    outfile.write(v, count);
    delete[] v;
    
}

void Buffer :: set_outputs_from_fp(const std::string& fp) {
    // vitual method overridden in Buffer (so as to localize use of libsndfile
    // an exception to call on base class
    // libsndfile nomenclature is different than used here: For a sound file with only one channel, a frame is the same as a item (ie a single sample) while for multi channel sound files, a single frame contains a single item for each channel.
	SndfileHandle sh(fp);
    // read into temporary array; this means that we use 2x the memory that we really need, but it means that we can un-interleave the audio file when passing in the array; ideally we would pass in our outputs directly
    OutputsSizeT s =  static_cast<OutputsSizeT>(sh.frames() * sh.channels());
    // we need to use a double, as that seems to be what we get out of sndfile
    double* v = new double[s]; // why not use SampleType instaed of double
    sh.readf(v, sh.frames());
    // this call will resize frame if necessary
    set_outputs_from_array(v, s, sh.channels());
    delete[] v;
}



void Buffer :: set_outputs(const BufferInjectorShared bi) {
    // vitual method overridden in Buffer
    std::cout << "Buffer: set_outputs: " << bi->get_frame_size() << " channels: " << bi->get_channels() << std::endl;
    
    VSampleType vst;
    bi->fill_interleaved(vst);
    ParameterIndexType ch = bi->get_channels();
    set_slot_by_index(0, ch); // set channels
    // will get a pointer to vst and pass to set from array
    set_outputs_from_vector(vst, ch);
    // vector will be auto destroyed here
}

void Buffer :: set_outputs(const std::string& fp) {
    // vitual method overridden in Buffer
    set_outputs_from_fp(fp);
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
    _class_id = ID_Phasor;
}


void Phasor :: init() {
    // the int routie must configure the names and types of parameters
    // std::cout << *this << " Phasor::init()" << std::endl;
    Generator::init();
    _clear_output_parameter_types(); // must clear the default set by Gen init
	
    // register some parameters
    aw::ParameterTypeShared pt1 = aw::ParameterType::make( 
            aw::ParameterType::ID_Frequency);                                       
    pt1->set_instance_name("Frequency");
    _register_input_parameter_type(pt1);
	_input_index_frequency = 0;
	
    aw::ParameterTypeShared pt2 = aw::ParameterType::make( 
            aw::ParameterType::ID_Phase);
    pt2->set_instance_name("Phase");
    _register_input_parameter_type(pt2);	
	_input_index_phase = 1;	
	
	// register outputs
    aw::ParameterTypeShared pt3 = aw::ParameterType::make( 
            aw::ParameterType::ID_Value);
    pt3->set_instance_name("Output");
    _register_output_parameter_type(pt3);	

    aw::ParameterTypeShared pt4 = aw::ParameterType::make( 
            aw::ParameterType::ID_Trigger);
    pt4->set_instance_name("Trigger");
    _register_output_parameter_type(pt4);	

    reset();
}


void Phasor :: reset() {
    Generator::reset();
    _amp = 0;
    _amp_prev = 1;
}

void Phasor :: render(RenderCountType f) {
	// given a frequency and a sample rate, we can calculate the number of samples per cycle
	// 1 / fq is time in seconds
	// sr is samples per sec
	// this does not need to be done each render call, but if so, ensure continuity
	// this is the abs positoin in this frame
	//_abs_sample_pos = f * fs; 		
	// this is the relative postion in this frame
	OutputsSizeT i(0);
	
    while (_render_count < f) {
        // calling render inputs updates the outputs of all inputs by calling their render functions; after doing so, the outputs are ready for reading
        _render_inputs(f);
		// get a vector  for each input summing accross all dimensions
		_sum_inputs(_frame_size);
		
		for (i=0; i < _frame_size; ++i) {
			// for each frame position, must get sum across all frequency values calculated.
			_sum_frequency = _summed_inputs[_input_index_frequency][i];
			// we might dither this to increase accuracy over time; we floor+.5 to get an int period for now; period must not be zero or 1 (div by zero)
			_period_samples = floor((static_cast<SampleType>(_sampling_rate) /
					frequency_limiter(_sum_frequency, _nyquist)) + 0.5);
                     
			// add amp increment to previous amp; do not care about where we are in the cycle, only that we get to 1 and reset amp
			_amp = _amp_prev + (1.0 / static_cast<SampleType>(
					(_period_samples - 1)));
			// if amp is at or above 1, set to zero
            // TODO: need to formalize this min gap
			if (_amp > 1.00001) {
                _amp = 0.0;
                outputs[1][i] = 1; // set trigger           
            }
            else {
                outputs[1][i] = 0; // set trigger  
            }
			//std::cout << "i" << i << " : _amp " << _amp << std::endl;
			outputs[0][i] = _amp;            
			_amp_prev = _amp;
		}
        //std::cout << "_period_samples: " << _period_samples << std::endl;        
        _render_count += 1;
    }
    
}




//-----------------------------------------------------------------------------
Sine :: Sine(EnvironmentShared e) 
	// must initialize base class with passed arg
	: Generator(e)
	{
	_class_name = "Sine"; 
    _class_id = ID_Sine;
}

Sine :: ~Sine() {
}

void Sine :: init() {
    // the init must configure the names and types of parameters
    Generator::init();
    _clear_output_parameter_types(); // must clear the default set by Gen init
	
    // register some parameters
    aw::ParameterTypeShared pt1 = aw::ParameterType::make( 
            aw::ParameterType::ID_Frequency);                                       
    pt1->set_instance_name("Frequency");
    _register_input_parameter_type(pt1);
	_input_index_frequency = 0;
	
    aw::ParameterTypeShared pt2 = aw::ParameterType::make( 
            aw::ParameterType::ID_Phase);
    pt2->set_instance_name("Phase");
    _register_input_parameter_type(pt2);	
	_input_index_phase = 1;
    
	// register output
    aw::ParameterTypeShared pt_o1 = aw::ParameterType::make(
            aw::ParameterType::ID_Value);
    pt_o1->set_instance_name("Output");
    _register_output_parameter_type(pt_o1);
    
    set_default();
    reset();
}

void Sine :: set_default() {
    // default configuration sets phase at zero
    set_input_by_index(_input_index_phase, 0);
}


void Sine :: reset() {
    Generator::reset();
    // will get set on first render call
    _cur_phase = 0;
    _cur_fq = 0;
    _phase_increment = 0;
}


//void Sine :: render(RenderCountType f) {
//    while (_render_count < f) {
//        _render_inputs(f);
//		_sum_inputs(_frame_size);
//        // a running count of samples
//        _sample_count = _render_count * _frame_size;
//		// iterate over one frame
//		for (_i=0; _i < _frame_size; ++_i) {
//			//_sum_frequency = ;
//			//_sum_phase = _summed_inputs[_input_index_phase][i];
//            // 2*pi is one cycle, times the number of samples per cycle
//            _angle_increment = PI2 * (
//                    _summed_inputs[_input_index_frequency][_i] /
//                    _sampling_rate);
//            // mult angle increment by cumulative running of samples
//			outputs[0][_i] = sin(_angle_increment * (_sample_count + _i));            
//		}
//        //std::cout << "_period_samples: " << _period_samples << std::endl;        
//        _render_count += 1;
//    }
//}



void Sine :: render(RenderCountType f) {
    while (_render_count < f) {
        _render_inputs(f);
		_sum_inputs(_frame_size);
        
		for (_i=0; _i < _frame_size; ++_i) {

            // whne phase input changes, set cur value to that value; not sure this is the right way to do this but maybe, as an phasor driving 1 to 2IP would oscillate
            if (_summed_inputs[_input_index_phase][_i] != _phase_increment) {
                _phase_increment = _summed_inputs[_input_index_phase][_i];
                _cur_phase = _phase_increment; 
            }
            phase_limiter(_cur_phase); // inllined, in place
        
            // phase input is a phase offset                 
            outputs[0][_i] = sin(_cur_phase);
            
            if (_summed_inputs[_input_index_frequency][_i] != _cur_fq) {
                _cur_fq = _summed_inputs[_input_index_frequency][_i];
                // could pre-callc 2 pi over sr
                _angle_increment = PI2 * _cur_fq / _sampling_rate;
            }
            _cur_phase += _angle_increment;
            
            phase_limiter(_cur_phase); // inllined, in place
        }
        
        _render_count += 1;
    }
}




//-----------------------------------------------------------------------------
Map :: Map(EnvironmentShared e)
	// must initialize base class with passed arg
	: Generator(e)
	{
	_class_name = "Map";
    _class_id = ID_Map;
}


void Map :: init() {
    // the int route must configure the names and types of parameters
    // std::cout << *this << " Phasor::init()" << std::endl;
    Generator::init();
    _clear_output_parameter_types(); // must clear the default set by Gen init
	
    // register some parameters
    aw::ParameterTypeShared pt1 = aw::ParameterType::make( 
            aw::ParameterType::ID_Value);
    pt1->set_instance_name("Source");
    _register_input_parameter_type(pt1);
	_input_index_src = 0;


    aw::ParameterTypeShared pt4 = aw::ParameterType::make(
            aw::ParameterType::ID_LowerBoundary);
    pt4->set_instance_name("Source Lower");
    _register_input_parameter_type(pt4);
	_input_index_src_lower = 1;

    aw::ParameterTypeShared pt5 = aw::ParameterType::make(
            aw::ParameterType::ID_UpperBoundary);
    pt5->set_instance_name("Source Upper");
    _register_input_parameter_type(pt5);
	_input_index_src_upper = 2;
	

    aw::ParameterTypeShared pt2 = aw::ParameterType::make(
            aw::ParameterType::ID_LowerBoundary);
    pt2->set_instance_name("Destination Lower");
    _register_input_parameter_type(pt2);	
	_input_index_dst_lower = 3;

    aw::ParameterTypeShared pt3 = aw::ParameterType::make(
            aw::ParameterType::ID_UpperBoundary);
    pt3->set_instance_name("Destination Upper");
    _register_input_parameter_type(pt3);
	_input_index_dst_upper = 4;


	// register output
    aw::ParameterTypeShared pt_out = aw::ParameterType::make(
            aw::ParameterType::ID_Value);
    pt_out->set_instance_name("Output");
    _register_output_parameter_type(pt_out);
    
    // set default
    set_default();
    reset();

}

void Map :: set_default() {
    // default configuration is to set source lower/upper
    set_input_by_index(_input_index_src_lower, 0);
    set_input_by_index(_input_index_src_upper, 1);
}

void Map :: reset() {
    Generator::reset();
}

void Map :: render(RenderCountType f) {
    
    while (_render_count < f) {
        _render_inputs(f);
		_sum_inputs(_frame_size);

		for (_i=0; _i < _frame_size; ++_i) {
            // must get true min max
            aw::true_min_max(
                    _summed_inputs[_input_index_src_lower][_i],
                    _summed_inputs[_input_index_src_upper][_i],
                    &_min_src,
                    &_max_src
                    );
            aw::true_min_max(
                    _summed_inputs[_input_index_dst_lower][_i],
                    _summed_inputs[_input_index_dst_upper][_i],
                    &_min_dst,
                    &_max_dst
                    );
            // if input is beyond min/max defined as source, it is clipped
			_limit_src = aw::double_limiter(
                    _summed_inputs[_input_index_src][_i], _min_src, _max_src
                    );
            _range_src = _max_src - _min_src; // no abs necessary
            _range_dst = _max_dst - _min_dst; // no abs necessary
            // _limit_src needs to be shifted to start from zero; if range is -3, 1 then need to add 3; if range is 3, 10, need to subtact 3; thus, -(min)
            if (_range_src != 0 && _range_dst != 0) {
                // get percentage of src, apply to range of dst + shift
                outputs[0][_i] = (
                        ((_limit_src - _min_src) / _range_src) *
                        _range_dst) + _min_dst;
            }
            else {
                // if we have no range on input, it means that our source boundaries are the same, which means that we have clipped to a constant; there is no sensible mapping (dst lower, upper, middle are all vialable / if we have no range on output, it menans dst boundaries are the same, so we should output that value; thus in either case, simply returning the dst min is acceptable.
                outputs[0][_i] = _min_dst;
            }
		}
        //std::cout << "_period_samples: " << _period_samples << std::endl;        
        _render_count += 1;
    }
    
}



//-----------------------------------------------------------------------------
AttackDecay :: AttackDecay(EnvironmentShared e)
	// must initialize base class with passed arg
	: Generator(e)
	{
	_class_name = "AttackDecay";
    _class_id = ID_AttackDecay;
}

void AttackDecay :: init() {
    // the int route must configure the names and types of parameters
    // std::cout << *this << " Phasor::init()" << std::endl;
    Generator::init();
    _clear_output_parameter_types(); // must clear the default set by Gen init
	
    // register some parameters
    aw::ParameterTypeShared pt_i1 = aw::ParameterType::make(
            aw::ParameterType::ID_Trigger);
    pt_i1->set_instance_name("Trigger");
    _register_input_parameter_type(pt_i1);
	_input_index_trigger = 0;

    aw::ParameterTypeShared pt_i2 = aw::ParameterType::make(
            aw::ParameterType::ID_Duration);
    pt_i2->set_instance_name("Attack time");
    _register_input_parameter_type(pt_i2);
	_input_index_attack = 1;

    aw::ParameterTypeShared pt_i3 = aw::ParameterType::make(
            aw::ParameterType::ID_Duration);
    pt_i3->set_instance_name("Decay time");
    _register_input_parameter_type(pt_i3);
	_input_index_decay = 2;

    aw::ParameterTypeShared pt_i4 = aw::ParameterType::make(
            aw::ParameterType::ID_Value);
    pt_i4->set_instance_name("Exponent");
    _register_input_parameter_type(pt_i4);
	_input_index_exponent = 3;

    aw::ParameterTypeShared pt_i5 = aw::ParameterType::make(
            aw::ParameterType::ID_Value);
    pt_i5->set_instance_name("Cycle");
    _register_input_parameter_type(pt_i5);
	_input_index_cycle = 4;
	

	// register output
    aw::ParameterTypeShared pt_o1 = aw::ParameterType::make(
            aw::ParameterType::ID_Value);
    pt_o1->set_instance_name("Output");
    _register_output_parameter_type(pt_o1);
    
    // add outputs: EOA, EOD!
    aw::ParameterTypeShared pt_o2 = aw::ParameterType::make(
            aw::ParameterType::ID_Value);
    pt_o2->set_instance_name("EOA");
    _register_output_parameter_type(pt_o2);
    _output_index_eoa = 1;

    aw::ParameterTypeShared pt_o3 = aw::ParameterType::make(
            aw::ParameterType::ID_Value);
    pt_o3->set_instance_name("EOD");
    _register_output_parameter_type(pt_o3);
    _output_index_eod = 2;
    
    // set default
    set_default();
    reset();
}

void AttackDecay :: set_default() {
    // set trigger to zero in case user just assigns cycle
    set_input_by_index(_input_index_trigger, 0);
    set_input_by_index(_input_index_exponent, 4);
    set_input_by_index(_input_index_cycle, 0);
    
}

void AttackDecay :: reset() {
    Generator::reset();
    _progress_samps = 0;
    _env_stage = 0;
    _last_amp = 0;
    _stage_amp_range = 1;
    _trigger_a = false;
}

void AttackDecay :: render(RenderCountType f) {
    // TODO: have this support gates; sustain if fall is > creater than attack; never do less than attack if gate falls before end of attack
    
    while (_render_count < f) {
        _render_inputs(f);
		_sum_inputs(_frame_size);
		for (_i=0; _i < _frame_size; ++_i) {

            // alway set to zero unless we have a change
            outputs[_output_index_eoa][_i] = 0.0;
            outputs[_output_index_eod][_i] = 0.0;

            // convert to samples; will truncate to int; might need to round
            _a_samps = fabs(_summed_inputs[_input_index_attack][_i] *
                    static_cast<SampleType>(_sampling_rate));
            _d_samps = fabs(_summed_inputs[_input_index_decay][_i] *
                    static_cast<SampleType>(_sampling_rate));


            // attack can be triggered by two conditions: if in cycle mode and envl stage is 0 (after completion of release), or if we get a normal trigger. 
            if (_summed_inputs[_input_index_cycle][_i] > TRIG_THRESH &&
                    _env_stage == 0) {
                _trigger_a = true;
            }
            else if (_summed_inputs[_input_index_trigger][_i] > TRIG_THRESH) {
                _trigger_a = true;
            }
            // determine stage: these are called once
            // if in any stage, and we find a trig:
            if (_trigger_a) {
                _env_stage = 1; // go to attack
                _progress_samps = 0; // should already by zero
                _stage_amp_range = 1 - _last_amp;
                _trigger_a = false;
            }
            // only do once, when env stage is 1 and have enough attack, move stage
            if (_progress_samps > _a_samps && _env_stage == 1)  { // and < A+D
                _env_stage = 2; // in decay
                _progress_samps = 0;
                _stage_amp_range = 1 - _last_amp;                
                // trig end of attack
                outputs[_output_index_eoa][_i] = 1.0;
            }            
            // if we are in stage 2 and our progress is > decay samples, move to silence
            if (_progress_samps > _d_samps && _env_stage == 2) {
                _env_stage = 0; // in silence; if auto will re-trigger
                _progress_samps = 0;
                // if we get to here we reached zero
                _stage_amp_range = 1; // decay is always from 1
                 // trig end of decay
                outputs[_output_index_eod][_i] = 1.0;
            }
            
            // the following are called repeatedly within a selected stage
            // write outputs
            if (_env_stage == 0) { // in silence
                _stage_amp_range = 1;
                outputs[0][_i] = 0.0;
            }
            else if (_env_stage == 1) { // attack
                // 1-pow((dur-x)/float(dur),4)
                // inverse exponential in ascent, scaled over range
                outputs[0][_i] = 1 - pow(
                        ((_a_samps - _progress_samps) / _a_samps),
                        _summed_inputs[_input_index_exponent][_i]
                        ) * _stage_amp_range;
            }
            else if (_env_stage == 2) { // decay
                // pow((dur-x)/float(dur),4)
                outputs[0][_i] = pow(
                        ((_d_samps - _progress_samps) / _d_samps),
                        _summed_inputs[_input_index_exponent][_i]
                        );
            }
            else { // should never happen
                throw "bad stage"; // provide proper or errror
            }
            
            _last_amp = outputs[0][_i];
            // always increment; just reset when we get a trigger
            ++_progress_samps;
            
		}
        //std::cout << "_period_samples: " << _period_samples << std::endl;        
        _render_count += 1;
    }
    
}






} // end namespaces aw



