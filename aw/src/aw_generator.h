#ifndef _AW_GENERATOR_H_
#define _AW_GENERATOR_H_

#include <vector>
#include <string>
#include <utility> // has pair
#include <unordered_map>
#include <memory>

#include "aw_common.h"

namespace aw {

//=============================================================================
class ParameterType;

//! Shared ParameterType. 
typedef std::shared_ptr<ParameterType> ParameterTypePtr;

//! The ParameterType, based on subclass definition, defines the meaning of an input slot that can be filled by a Gen. While subclass defines the meaning of the parameter, parameters can have instance names for the particular usage of a Gen. 
class ParameterType {
    public://------------------------------------------------------------------

    //! Identifiers for Parameter types. 
    enum ParameterTypeID {
        ID_Value,
        ID_Frequency,
        ID_Duration,
        ID_Phase,
        ID_Channels,
        // what about inputs that take trigger or gate in same input?
        ID_Trigger,
        ID_LowerBoundary,
        ID_UpperBoundary,
        ID_Table // for slot in breakpoint, wavetable
    };

    //! Primary constructor static method for creating share Parameter types. 
    static ParameterTypePtr make(ParameterTypeID);

    protected://---------------------------------------------------------------
    //! Class name set on creation. 
    std::string _class_name;
    
    //! Public parameter id, can be used for class matching.
    ParameterTypeID _class_id;
    
    //static const char _class_name_alt[];

    //! The name of the parameter in the context of a specific generator. 
    std::string _instance_name;

    //! Additional description in the context of the assigned-to generator. 
    std::string _description;

    // can also define, for a particular instance, an expected context; that is, if fq is required, this can be defined here. then, when given another generator at a different context, conversion can happen?
	
    public://------------------------------------------------------------------
    explicit ParameterType();

	//! Output stream processor.
	friend std::ostream& operator<<(std::ostream& outputs, const ParameterType& pt);

    //! Set the name of this ParameterType. Passed by const reference as assignment makes the necessary copy. This name is set by the Gen and may not be the same even if its the same ParameterType subclass. 
    void set_instance_name(const std::string& s) {_instance_name = s;};

    std::string get_class_name() const {return _class_name;};

    //! Return the name as a string. 
    std::string get_instance_name() const {return _instance_name;};
    
    // Return the Parameter type id. 
    ParameterTypeID get_class_id() const {return _class_id;};
    
};


class ParameterTypeValue;
//! Shared ParameterTypeValue.
typedef std::shared_ptr<ParameterTypeValue> ParameterTypeValuePtr;
//! A subclass of ParameterType that specifies a value; the value can be one of many sorts of things such as a constant or an opperand. 
class ParameterTypeValue: public ParameterType {
    public://------------------------------------------------------------------
    explicit ParameterTypeValue();
};

class ParameterTypeFrequency;
typedef std::shared_ptr<ParameterTypeFrequency> ParameterTypeFrequencyPtr;
//! A subclass of ParameterType that specifies a frequency; this is assumed presently to be in Hertz.
class ParameterTypeFrequency: public ParameterType {
    public://------------------------------------------------------------------
    explicit ParameterTypeFrequency();
};

class ParameterTypeDuration;
typedef std::shared_ptr<ParameterTypeDuration> ParameterTypeDurationPtr;
//! A subclass of ParameterType that specifies a duration in seconds.
class ParameterTypeDuration: public ParameterType {
    public://------------------------------------------------------------------
    explicit ParameterTypeDuration();
};

class ParameterTypePhase;
typedef std::shared_ptr<ParameterTypePhase> ParameterTypePhaseShared;
//! A subclass of ParameterType that specifies a phase; this is assumed presently to be in floating-point values.
class ParameterTypePhase: public ParameterType {
    public://------------------------------------------------------------------
    explicit ParameterTypePhase();
};

//! Used by some Gen to configure slots for both input and output channels, or dimensionality (frame/output count) of the internal marix. 
class ParameterTypeChannels;
typedef std::shared_ptr<ParameterTypeChannels> ParameterTypeChannelsShared;
//! A subclass of ParameterType that specifies a phase; this is assumed presently to be in floating-point values.
class ParameterTypeChannels: public ParameterType {
    public://------------------------------------------------------------------
    explicit ParameterTypeChannels();
};

class ParameterTypeTrigger;
typedef std::shared_ptr<ParameterTypeTrigger> ParameterTypeTriggerShared;
//! An impulse or trigger, or a single sample at 1.
class ParameterTypeTrigger: public ParameterType {
    public://------------------------------------------------------------------
    explicit ParameterTypeTrigger();
};

class ParameterTypeLowerBoundary;
typedef std::shared_ptr<ParameterTypeLowerBoundary>
        ParameterTypeLowerBoundaryShared;
//! A lower boundary, likely a minimum, for dynamic sizing of a generator. 
class ParameterTypeLowerBoundary: public ParameterType {
    public://------------------------------------------------------------------
    explicit ParameterTypeLowerBoundary();
};

class ParameterTypeUpperBoundary;
typedef std::shared_ptr<ParameterTypeUpperBoundary>
        ParameterTypeUpperBoundaryShared;
//! A lower boundary, likely a maximum, for dynamic sizing of a generator. 
class ParameterTypeUpperBoundary: public ParameterType {
    public://------------------------------------------------------------------
    explicit ParameterTypeUpperBoundary();
};

class ParameterTypeTable;
typedef std::shared_ptr<ParameterTypeTable>
        ParameterTypeTableShared;
//! A table, or a Buffer with 2 channels of data. 
class ParameterTypeTable: public ParameterType {
    public://------------------------------------------------------------------
    explicit ParameterTypeTable();
};



//=============================================================================
class Gen;
typedef std::shared_ptr<Gen> GenPtr;
//! Gen class. Base-class of all Generators. A Gen has inputs and outputs. Inputs are a vector of vectors of Generators/ out number pairs. The number of types, and types of inputs, are defined by the mapping _input_parameter_type; the Gen inputs are stored on the _inputs VVGenShared. Multiple inputs in the same parameter position are always summed. Rendering on the Gen is stored in the outputs, a table of one frame for each output. Clients of the generator freely read from the outputs vector. 
class Gen: public std::enable_shared_from_this<Gen> {

    public://--------------------------=---------------------------------------
    // public typedefs
	//! A mapping of index number
    typedef std::unordered_map<ParameterIndexT, ParameterTypePtr> MapIndexToParameterTypePtr;

    //! A vector of GenPtr instances used for slots. Slots do not yet need to define outputs number; generatlly assume that we use the first outputs?
    typedef std::vector<GenPtr> VGenPtr;
		
    //! An OutputConnection is a pair formed of GeneeratorShared and an integer representing the output/outputs number, starting from zero, to be read from in that input.
    typedef std::pair<GenPtr, ParameterIndexT> GenPtrOutPair;
    typedef std::vector<GenPtrOutPair> VGenPtrOutPair;
    typedef std::vector<VGenPtrOutPair> VVGenPtrOutPair;
    
    // enums    
    //! Enumeration of IDs for each type of generator avaialble; used in factory methods to return configure Generators. 
    enum GeneratorID {
        ID_Constant,    
        ID_Add,
        ID_Multiply,   
        ID_Buffer,
        ID_Breakpoints,
        ID_Phasor,
        ID_Sine,
        ID_Map,
        ID_AttackDecay
    };
        
		
    protected://----------------------------------------------------------------
    //! The name of the class. This is set during the class constructor by the derived class, and thus needs to be protected.
    std::string _class_name;

    //! Public generator id, can be used for class matching. 
    GeneratorID _class_id;

    private://------------------------------------------------------------------
    // Rename ParameterIndexT as OutputIndexType
    //! Store the number of output channels, that this Gen is currently set up with. Storage array acount.
    ParameterIndexT _output_count;
    
	//! The _outputs_size is derived from frame _output_count times the _frame_size. OutputsSizeT is expected to store size up to long multichannel audio files. Total storage size.
    OutputsSizeT _outputs_size;
    
    // TODO: use InputIndexType ParameterIndexT 
    //! Number of input parameters used by this Gen. More than one Gen can reside in each slot. 
    ParameterIndexT _input_count;

	//! Number of slots used by this Gen. One Gen can reside in each slot. 
    ParameterIndexT _slot_count;    

	//! Store the Envrionment instance. 
    EnvironmentPtr _environment;
	
    protected://---------------------------------------------------------------

	//! The size of each frame for each _output_count as stored in the outputs vector. This is protected because render() methods must access.
    FrameSizeT _frame_size; // if changed, need to rebuild outputs	
    
    //! The sampling rate, taken from an Environment instance, and is passed through from a GeneraotrConfig. We store this for efficiency; not prepared to handle if this changes. 
	OutputsSizeT _sampling_rate;
	
	//! The nyquist frequency, .5 * SamplingRate; this is stored to optimize calculations that need this value.
	OutputsSizeT _nyquist;
		
    //! Define if this Gen has resizable frame size. Most generators do not have have resizable frame size; only some (like a Buffer) do.
    bool _frame_size_is_resizable; // TODO: make private?	
                            
    //! The number of renderings that have passed since the last reset. Protected because render() and reset() routines need to alter this. RenderCountT must be the largest integer available.
    RenderCountT _render_count;
	
    //! The main storage for ParameterTypePtr instances used as inputs. These are mapped by index value, which is the same index value in the inputs vector. This is only protected and not private so that Constant can override print_inputs.
    std::unordered_map<ParameterIndexT,
                            ParameterTypePtr> _input_parameter_type;	
		
    //! A std::vector of vectors of GeneratorsShared / outputs id pairs that are the inputs to this function. This could be an unordered map too, but vector will have optimal performance when we know the index in advance.
    VVGenPtrOutPair _inputs;
    
	//! For each render call, we sum all inputs up to the common frame size available in the input and store that in a Vector of sample types. This is done to make render() methods cleaner and remove redundancy.
	VVSampleT _summed_inputs;    
    
	//! A std::vector of GeneratorsShared that are used for configuration of this Gen. Unlike inputs, only one Gen can occupy a slot position. Example: a buffer has a slot for duration of that buffer. It is not yet decided if generators in slots shold be rendered. Further, which output of the generator slot is alos not yet assigned.
	VGenPtr _slots;
	
	//! Must define slots as ParameterTypes, meaning the same can be used for both inputs and for slots. This also means slots must be Generators. These are mapped by index value, which is the same index value in the inputs vector. Note that all slots must be filled for the generator to be used, so perhaps defaults should be provided. 
    std::unordered_map<ParameterIndexT,
            ParameterTypePtr> _slot_parameter_type;
	
	
    //! We store a ParameterTypePtr for each defined output, telling us what it is.
    std::unordered_map<ParameterIndexT,
            ParameterTypePtr> _output_parameter_type;		
		

    public://------------------------------------------------------------------
        
    //! A vector of sample vectors. This might be deemed best as private, but for performance this is public: no function call is required to read from it. 
    VVSampleT outputs;	
    

    // ========================================================================
    // methods ================================================================
	
    private://-----------------------------------------------------------------   		
    //! Resize the outputs vector. Always called when registering an output or changing frame size.
    void _resize_outputs();
	

    protected://---------------------------------------------------------------
	
    //! Validate the current contnets of the outputs data. A virtual method for overridding in derived classes (e.g., Breakpoints). Called whenever a outputs are set as a whole (aka, set by array). Will raise an exception.
    Validity _validate_outputs();
    
    //! Called by Generators during init() to configure the input parameters found in this Gen. ParameterTypePtr instances are stored in the Gen, the _input_count is incremented, and _inputs is given a blank vector for appending to. The order of execution matters. 
    void _register_input_parameter_type(ParameterTypePtr pts);

    //! Remove all inputs; used by slots that dynamically chagne inputs and outputs; all existing inputs will remain. 
    void _clear_input_parameter_types();

	//! Called by Generators during init() to configure the slot parameters found in this Gen.
    void _register_slot_parameter_type(ParameterTypePtr pts);

	//! Define an output. This calls _resize_outputs().
    void _register_output_parameter_type(ParameterTypePtr pts);

    //! Remove all outputs; used by slots that dynamically change inputs and outputs; all existing inputs will remain. 
    void _clear_output_parameter_types();
	
	//! Call this every time a new slot has been added or changed. This is necessarily virtual as subclasses need to handle their slots in their own way. 
	virtual void _update_for_new_slot();
    
    //! Call the render method on all stored inputs. This is done once for each render call; this calls each  input Generator's render() method. This means that during render() calls, _render_inputs() does not need to be called. 
    inline void _render_inputs(RenderCountT f);
	
	//! Flatten or sum multiple inputs that reside in the same input type. This is done to optimize dealing with multiple inputs in the same input type ahead of calculations for rendering. Results are stored in _summed_inputs VV. The fs argument is the number of frames to read.  
	inline void _sum_inputs(FrameSizeT fs);
    
	//! Call reset on all inputs. 
	void _reset_inputs();
        	
    //! Public method for resizing based on frame size. Calls _resize_outputs only if necessary. 
    void _set_frame_size(FrameSizeT f);    
    
    
    public://------------------------------------------------------------------
    //! Factory for Generators using a provided EnvironmentPtr. 
    static GenPtr make_with_environment(GeneratorID q, 
            EnvironmentPtr e);

    //! Factory for all Generators with by generator ID alone. This creates a GenPtr, and calls its init() method.     
    static GenPtr make(GeneratorID);


    public://------------------------------------------------------------------

	//! Main constructor that takes a generator config.
    explicit Gen(EnvironmentPtr e);

    //! The default constructor is deleted: we always must pass in an Envrionment. 
    Gen() = delete;
    
    
    //! Initialize the Gen. This method is responsible for creating ParameterTypeValuePtr instances and adding them to the Gen using the _register_input_parameter_type method. This method also does the initial sizing of the Gen, and thus could raise an exception. Additional buffers that might be needed for this Gen can be stored here. As this is virtual the base-classes init is not called, and must be called explicitly in derived classes. This should only be called once in the life of a generator.
    virtual void init();
    
    //! Reset all inputs, and zero out the outputs array and the _render count. Derived classes should manually call the base class.
    virtual void reset();
	

    //! Set a default input and slot configuration. If other inputs or slots are configured, the will be removed. 
    virtual void set_default();

    //! Return the the number of outputs dimensions
    ParameterIndexT get_output_count() const {return _output_count;};

    //! Return the the outputs size, or the total number of samples used for all frames at all outputs.
    FrameSizeT get_outputs_size() const {return _outputs_size;};
    
    //! Get the average value of all outputs values. 
    SampleT get_outputs_average() const;

    //! Get the average of single _output_count of outputs. If d is 0, all dimensions are averaged. If d is greater than the number of dimensions, and error is raised. 
    SampleT get_output_average(ParameterIndexT d) const;

    //! Return a Boolean if this Gen has resizable frame size
    bool frame_size_is_resizable() const {return _frame_size_is_resizable;};
	
    //! Return the the frame size, the number of samples per output channel. The frame size is always at or greater than the common frame size.
    OutputsSizeT get_frame_size() const {return _frame_size;};	

    //! Return a copy of the environment shared pointer.
    EnvironmentPtr get_environment() const {
        return _environment; 
    };
    
	//! Return the common frame size derived from the stored shared Environment. If this Gen has resizable frames, this may not be the same as the current frame size. 
    OutputsSizeT get_common_frame_size() const {
			return _environment->get_common_frame_size();};	

	//! Get the sampling rate. 
    OutputsSizeT get_sampling_rate() const {return _sampling_rate;};	

	//! Get the nyquist frequency. 
    OutputsSizeT get_nyquist() const {return _nyquist;};	

    
	// info strings ...........................................................    

    //! Get the name and address (memory start) for this Gen, as a single string.
    std::string get_name_address() const;
    
    //! Get the name and addres for this Gen, labeling the name with "Gen" and wrapping the address in braces. This is used for a more readable presentation.
    std::string get_label_address() const;
    
    //! Get a complete label for this generator. This includes the get_label_address() presentation, alongwith the input and output count. 
    std::string get_label() const;
    
	//! Output stream friend function: returns the label of the Gen. 
	friend std::ostream& operator<<(std::ostream& ostream, const Gen& g);
    
	//! Output stream friend function: returns the label of the GenPtr.     
	friend std::ostream& operator<<(std::ostream& ostream, const GenPtr g);

    //! Return the name as a string. 
    std::string get_class_name() const {return _class_name;};

    GeneratorID get_class_id() const {return _class_id;};


	// display ...............................................................    
    //! Print the outputs buffer for all dimensions at the current render count. The optional start/end values can specify vaules within the frame range
    void print_outputs(FrameSizeT start=0, FrameSizeT end=0);

    //! Print the the hierarchical list of all input values. This is virtual because Constant must print inputs in as different way. No other generator should need to specialize. 
    virtual void print_inputs(bool recursive=false, UINT8 recurse_level=0);

    //! Render the requested frame if not already rendered. This is virtual because every Gen renders in a different way. 
    virtual void render(RenderCountT f); 


    //! Plot the outputs by piping it to gnuplot using a subprocess. 
	void illustrate_outputs();

    //! Create a graphiz illustration. 
	void illustrate_network();


	// loading/writing to outputs .............................................    
	
    //! Load the outputs into a passed-in vector. The vector is cleared before loading. 
    void write_outputs_to_vector(VSampleT& vst) const;

    //! Write out all outpout to the provided file path. If this is a Buffer, this can be used to write an audio file.
    virtual void write_output_to_fp(const std::string& fp, 
                                    ParameterIndexT d=0) const;
	
        
	//! Set the outputs from an array. This low level routine is the only way outputs are directly written from a collection. The reeturned bool is the result of _validate_outputs. 
	Validity set_outputs_from_array(SampleT* v, OutputsSizeT s,
							ParameterIndexT ch, bool interleaved=true);
								
	//! Set the outputs (resizing if possible) to values passsed in from a vector of SampleT. Note this presently copies values from a vector to an array, and thus requires 2x the memory alloc. Should by a const vector but cannot yet get usage right when deriving an array pointer; but: the passed in vector should not be changed. 
	void set_outputs_from_vector(VSampleT& vst,
								ParameterIndexT ch, bool interleaved=true);


    //! If we are in a Buffer class, this method loads a complete file path to an audio file into the output of this Gen. This is not implemented in the base class Gen. 
    virtual void set_outputs_from_fp(const std::string& fp);


    //! Overridden, overloaded function for setting outputs. Implemented by Buffer, but defined on base so all have access. Need a Injector because otherwise we would take more parameters for channels, etc.
    virtual void set_outputs(const Inj<SampleT>& bi);

    //! Set output swith a file path represented as a string.
    virtual void set_outputs(const std::string& fp);



	// inputs and slots ......................................................    
    //! Return the number of inputs; this is not the same as the number of Generators, as each input may have 1 or more Generators
    ParameterIndexT get_input_count() {return _input_count;};

    //! Return the number of slots. There is only one Gen per slot. 
    ParameterIndexT get_slot_count() {return _slot_count;};

	//! Return the parameter index for a named parameter.
    ParameterIndexT get_input_index_from_parameter_name(const
            std::string& s);

	//! Return the parameter index for the first-encountered parameter type id; raises an exception if not found.
    ParameterIndexT get_input_index_from_class_id(const
            ParameterType::ParameterTypeID ptid);

	//! Return the parameter slot for a named parameter.
    ParameterIndexT get_slot_index_from_parameter_name(const std::string& s);	


    // TODO: must check for duplicated connections and silently skip them; 
    
	// inputs ..............................................................        
    //! Directly set a parameter given an index. This will remove/erase any multiple inputs for this parameter
    virtual void set_input_by_index(
            ParameterIndexT i,
            GenPtr gs,
            ParameterIndexT pos=0);
    
    virtual void set_input_by_index(
            ParameterIndexT i,
            SampleT v,
            ParameterIndexT pos=0);

    ///! Set input by class id of the ParameterType. 
    void set_input_by_class_id(
            ParameterType::ParameterTypeID ptid,
            GenPtr gs,
            ParameterIndexT pos=0);
                                        
    void set_input_by_class_id(
            ParameterType::ParameterTypeID ptid,
            SampleT v, // accept numbers
            ParameterIndexT pos=0);


    //! Add a multiple input at this parameter. 
    virtual void add_input_by_index(ParameterIndexT i, 
            GenPtr gs, ParameterIndexT pos=0);

    virtual void add_input_by_index(ParameterIndexT i, SampleT v, 
            ParameterIndexT pos=0);
  
  
    //! Get a vector of GenPtr for an input, given the input index. This should be a copy of the vector, and is thus slow. This is virtual to provide Constant to override and return an empty vector (even though it might have an input).
    virtual VGenPtrOutPair get_input_gens_by_index(
            ParameterIndexT i);

    //! Remove all GenPtr attacked to all inputs.
    void clear_inputs();
  
	// slot ..............................................................    	
    //! Directly set a parameter to a slot given an index. This will remove/erase any parameter on this slot. The update parameter permits disabling updating a slot, useful during initial configuration. 
    virtual void set_slot_by_index(ParameterIndexT i, GenPtr gs, 
									bool update=true);
    
	//! Overridden to handle a constant. 
    virtual void set_slot_by_index(ParameterIndexT i, SampleT v, 
									bool update=true);

    //! Return the single gen at this slot position (not a vector, like inputs).
    virtual GenPtr get_slot_gen_at_index(ParameterIndexT i);
    
    // Remove all GenPtr attacked to all inputs: does not make sense to do this, because we always need one gen in each slot position
    //void clear_slots();
    

};

//.............................................................................
// functions on GenPtr ...............................................
//! Parsimonious serial connection: connect the min of a and b in straight connections. If count is zero, we set all available connections from start to end.
inline GenPtr connect_serial_to_inputs(GenPtr lhs, GenPtr rhs, 
        ParameterIndexT start=0, ParameterIndexT count=0) {
    // connect from left to right, so from lhs to rhs
    // lhs is above rhs in downard flow
    // get min of lhs out and rhs in, match as many in parallel as possible    
    ParameterIndexT availLen = std::min(
            lhs->get_output_count(), rhs->get_input_count());
    if (start >= availLen) {
        // the last available index is availLen -1
		throw std::domain_error(
            "the start exceedes the range of min in/out");                
    }    
    // count of 0 takes as much as possible; this may be too much given various starts, but in looping we have an alternative exit branch
    if (count == 0) {
        count = availLen;
    }    
    ParameterIndexT i;    
    for (i = start; i != (start+count); ++i) {
        // count may be greater than avialLen and not be an error; just take as much as possible
        if (i >= availLen) break;
        // in, gen, out of that gen
        // we use add_* so we can do this repeatedly on the same position through multiple calls; TODO: this is not always desirable.
        rhs->add_input_by_index(i, lhs, i);
    }
    return rhs; // this might return a reference instaed of same GenPtr
}


//! Assign SampleValue (after being converted to a Constant) directly as an input to one or more inputs on rhs GenPtr.
inline GenPtr connect_serial_to_inputs(SampleT lhs, GenPtr rhs, 
        ParameterIndexT start=0, ParameterIndexT count=0) {        
    // set environment from lhs
	GenPtr g_lhs = Gen::make_with_environment(
            Gen::ID_Constant, rhs->get_environment());
    g_lhs->set_input_by_index(0, lhs); // this will call Constant::reset()
    return aw::connect_serial_to_inputs(g_lhs, rhs); // will return rhs
}


//! Assign an Injector as an input to matching inputs on rhs GenPtr.
inline GenPtr connect_serial_to_inputs(const Inj<SampleT>& lhs, GenPtr rhs) {
    // create vector, fill with values;
    VSampleT inj;
    lhs.fill_interleaved(inj);
    // get lesser of input, injection size
    ParameterIndexT availLen = std::min(rhs->get_input_count(),
            inj.size());
    for(ParameterIndexT i=0; i<availLen; ++i) {
        rhs->add_input_by_index(i, inj[i]);
    }
    return rhs;
}

//! Assign an Injector as an input to matching inputs on rhs GenPtr.
inline GenPtr connect_serial_to_inputs(const Inj<GenPtr>& lhs, GenPtr rhs) {
    Gen::VGenPtr inj; // create vector, fill with values;
    lhs.fill_interleaved(inj);
    ParameterIndexT availLen = std::min(rhs->get_input_count(),
            inj.size());
    for(ParameterIndexT i=0; i<availLen; ++i) {
        rhs->add_input_by_index(i, inj[i]);
    }
    return rhs;
}



// slot connections ..............................................

//! Assign an Inj of sample values as an input to matching slot positions.
inline GenPtr connect_serial_to_slots(const Inj<SampleT>& lhs,
        GenPtr rhs) {
    VSampleT inj; // create vector, fill with values;
    lhs.fill_interleaved(inj);
    ParameterIndexT availLen = std::min(rhs->get_slot_count(),
            inj.size());
    for(ParameterIndexT i=0; i < availLen; ++i) {
        rhs->set_slot_by_index(i, inj[i]); // set, not add
    }
    return rhs;
}

//! Assign an Inj of Gen Ptrs.
inline GenPtr connect_serial_to_slots(const Inj<GenPtr>& lhs,
        GenPtr rhs) {
    Gen::VGenPtr inj; // create vector, fill with values;
    lhs.fill_interleaved(inj);
    ParameterIndexT availLen = std::min(rhs->get_slot_count(),
            inj.size());
    for(ParameterIndexT i=0; i < availLen; ++i) {
        rhs->set_slot_by_index(i, inj[i]); // set, not add
    }
    return rhs;
}




// operator >> ...............................................................

//! Connect all outputs available from lhs to all inputs available from rhs. Chained connectison, e.g., are permitted. a >> b >> c
inline GenPtr operator>>(GenPtr lhs, GenPtr rhs) {
    return connect_serial_to_inputs(lhs, rhs);
}

//! Connect a SampleT, making it into a constant.
inline GenPtr operator>>(SampleT lhs, GenPtr rhs) {
    return connect_serial_to_inputs(lhs, rhs);
}

//! Connect an 1D Injector, where each element is mapped to an argument in order. It is an exception to provide more than 1 dimension. This will always set parameters, not add them.
inline GenPtr operator>>(const Inj<SampleT>& lhs, GenPtr rhs) {
    return connect_serial_to_inputs(lhs, rhs);
}

// Set inputs with an Inj refefence. Will assign as many connections as possible. 
inline GenPtr operator>>(const Inj<GenPtr>& lhs, GenPtr rhs) {
    return connect_serial_to_inputs(lhs, rhs);
}


// operator || ...............................................................

//! Connect a 1D Injector, where each element is mapped to a slot in order. It is an exception to provide more than 1 dimension. This will always set slots.
inline GenPtr operator||(const Inj<SampleT>& lhs, GenPtr rhs) {
    return connect_serial_to_slots(lhs, rhs);
}

inline GenPtr operator||(const Inj<GenPtr>& lhs, GenPtr rhs) {
    return connect_serial_to_slots(lhs, rhs);
}




// operator &&................................................................
// memory (outputs storage) direct settting


inline GenPtr operator&&(const aw::Inj<SampleT>& lhs,
        GenPtr rhs) {
    rhs->set_outputs(lhs); // will throw if rhs is not Buffer
    return rhs;
} 

inline GenPtr operator&&(const std::string lhs, GenPtr rhs) {
    //return aw::connect_parallel(lhs, rhs, Gen::ID_Multiply);
    rhs->set_outputs(lhs); // will throw if rhs is not Buffer
    return rhs;        
} 


// parallel connections ......................................................

//! Connect two generators as inputs into another generator, where that generator is given by the passed in GeneratorID. This is a binary operator.
inline GenPtr connect_parallel(
        GenPtr lhs, 
        GenPtr rhs, 
        Gen::GeneratorID gid) {
    
    // get lesser of outputs between the two, then create an add with that many slots
    ParameterIndexT j = std::min(lhs->get_output_count(), 
            rhs->get_output_count());
    // use the passed in gen id, this is usually ID_Add, ID_Multiply
    GenPtr g = Gen::make_with_environment(gid, 
            lhs->get_environment());
    // the returned Gen needs to support multiple channels; these are set as slow 0
    g->set_slot_by_index(0, j);
    // try to conect as many in to out as possible for each gen
    ParameterIndexT i;
    for (i = 0; i != j; ++i) {
        g->add_input_by_index(i, lhs, i);
        g->add_input_by_index(i, rhs, i);        
    }
    return g;
}

//! Variant with SampleT as left-hand side.
inline GenPtr connect_parallel(
        SampleT lhs, 
        GenPtr rhs, 
        Gen::GeneratorID gid) {
    GenPtr g_lhs = Gen::make_with_environment(
            Gen::ID_Constant, rhs->get_environment());
    g_lhs->set_input_by_index(0, lhs); 
    GenPtr g = aw::connect_parallel(g_lhs, rhs, gid);
    return g;
}

//! Variant with SampleT as right-hand side.
inline GenPtr connect_parallel(
        GenPtr lhs, 
        SampleT rhs, 
        Gen::GeneratorID gid) {
    GenPtr g_rhs = Gen::make_with_environment(
            Gen::ID_Constant, lhs->get_environment());
    g_rhs->set_input_by_index(0, rhs); 
    GenPtr g = aw::connect_parallel(lhs, g_rhs, gid);
    return g;
}



// operator + .................................................................
inline GenPtr operator+(GenPtr lhs, GenPtr rhs) {
    return aw::connect_parallel(lhs, rhs, Gen::ID_Add);
}

inline GenPtr operator+(GenPtr lhs, SampleT rhs) {
    return aw::connect_parallel(lhs, rhs, Gen::ID_Add);
}

inline GenPtr operator+(SampleT lhs, GenPtr rhs) {
    return aw::connect_parallel(lhs, rhs, Gen::ID_Add);
}

// Alternative opperators:
// |, || set slots
// & set output buffer directly


// operator * .................................................................
inline GenPtr operator*(GenPtr lhs, GenPtr rhs) {
    return aw::connect_parallel(lhs, rhs, Gen::ID_Multiply);
}

inline GenPtr operator*(GenPtr lhs, SampleT rhs) {
    return aw::connect_parallel(lhs, rhs, Gen::ID_Multiply);
}

inline GenPtr operator*(SampleT lhs, GenPtr rhs) {
    return aw::connect_parallel(lhs, rhs, Gen::ID_Multiply);
} 





//=============================================================================
//! A Gen that returns a constant value, or fills its outputs vector with the same vale for all frames. 
class Constant;
typedef std::shared_ptr<Constant> ConstantPtr;
class Constant: public Gen {

    private://-----------------------------------------------------------------
	//! Storage for the internal constant values. This is an array because we want to support a similar interface of applying multiple values to a single input parameter. 
    VSampleT _values;


    public://------------------------------------------------------------------

	explicit Constant(EnvironmentPtr);
    
    virtual void init();
    
    virtual void reset();
	
	//! This overridden method needs only increment the _render_count, as the outputs array is set when reset() is called. 
    virtual void render(RenderCountT f); 	
	
	//! This derived function is necessary to handle displaying internal input components.
	virtual void print_inputs(bool recursive=false, UINT8 recurse_level=0);

	//! As Constant does not compose any Generators even though it has an input defined, this overridden method must return an empty vector.     
    virtual VGenPtrOutPair get_input_gens_by_index(ParameterIndexT i);
    
    //! This overridden method throws an exception: you cannot set a Gen to a constant.
    virtual void set_input_by_index(ParameterIndexT i, GenPtr gs, 
            ParameterIndexT pos=0);    
                                        
    //! Set value as a SampleT value.
	virtual void set_input_by_index(ParameterIndexT i, SampleT v, 
            ParameterIndexT pos=0);
    
    //! This overridden method throws an exception: you cannot set a Gen to a constant.    
    virtual void add_input_by_index(ParameterIndexT i, GenPtr gs, 
            ParameterIndexT pos=0);    
                                        
    //! Add value as a SampleT value.                                        
	virtual void add_input_by_index(ParameterIndexT i, SampleT v, 
            ParameterIndexT pos=0);
    
};


//=============================================================================
//! All Generators that can process a single input (parameter slot) with the same operator (and knowing the number of inputs and starting value) (+, *, avg?) can derive from this class.
class _BinaryCombined;
// no shared, as an ABC
class _BinaryCombined: public Gen {

    protected://---------------------------------------------------------------
    SampleT _n_opperands;
    //! Iniitial value in iterative operations.
    SampleT _n_opperands_init;

    // it seems like this should be const somehow; not sure about performanc implications
    char _op_switch;

	//! Overridden to apply slot settings and reset as necessary. 
	void _update_for_new_slot();

    public://------------------------------------------------------------------
    explicit _BinaryCombined(EnvironmentPtr);

    virtual void init();    
		
	//! Render addition. 
    virtual void render(RenderCountT f); 	
};




//=============================================================================
//! A mult sums all Generators across all dimensions at its single operand input. Derives from Add, as all operators will 
class Add;
typedef std::shared_ptr<Add> AddPtr;
class Add: public _BinaryCombined {

    public://------------------------------------------------------------------
    explicit Add(EnvironmentPtr);
    virtual void init();
};


//=============================================================================
//! A mult sums all Generators across all dimensions at its single operand input. Derives from Add, as all operators will 
class Multiply;
typedef std::shared_ptr<Multiply> MultiplyPtr;
class Multiply: public _BinaryCombined {

    public://------------------------------------------------------------------
    explicit Multiply(EnvironmentPtr);
    virtual void init();
};


//=============================================================================
//! A Buffer has the ability to load its outputs array to and from the file system. Further, the buffer generally has a larger frame size, permitting storing extended time periods in outputs. 
class Buffer;
typedef std::shared_ptr<Buffer> BufferPtr;
class Buffer: public Gen {

    protected://---------------------------------------------------------------
	//! Overridden to apply slot settings and reset as necessary. 
	void _update_for_new_slot();

    public://------------------------------------------------------------------
    explicit Buffer(EnvironmentPtr);
	
    virtual void init();
	
	//! Render the buffer: each render cycle must completely fille the buffer, meaning that inputs will be called more often, have a higher render number. Is this a problem? 
    virtual void render(RenderCountT f);	
			
    //! Write to an audio file to given the ouput file path. The optional ParameterIndexT argument can be used to specify a single _output_count of many to write. If ParameterIndexT is 0, all outputs are written.
    virtual void write_output_to_fp(const std::string& fp, 
                                    ParameterIndexT d=0) const;
        
    //! Set the outputs of this Gen to the content of an audio file provided as an audio path. This overridden method makes the usage of libsndfile to read in a file.
    virtual void set_outputs_from_fp(const std::string& fp);

    //! Overridden set outputs from Inj reference.
    virtual void set_outputs(const Inj<SampleT>& bi);

    //! Overridden set outputs a std::string, treated a file path.
    virtual void set_outputs(const std::string& fp);
    
};




//=============================================================================
//! A derived buffer for storing (and validating) break point data. This is not an interpolator, but just a derived storage class. 
class Breakpoints;
typedef std::shared_ptr<Breakpoints> BreakpointsPtr;
class Breakpoints: public Buffer {

    public://------------------------------------------------------------------

    explicit Breakpoints(EnvironmentPtr);

    virtual void init();
    
    //! Breakpoints overrides _validate_outputs to make sure that this is two dimensional and that x values are increasing. This does not raise an exception to allow caller to clean up memory.
    virtual Validity _validate_outputs();
    
};





//=============================================================================
//! The phasor has a ramp from 0 to 1 for each _output_count defined. Only the first _output_count of multiple dimensional inputs is used. 
class Phasor;
typedef std::shared_ptr<Phasor> PhasorPtr;
class Phasor: public Gen {

    private://-----------------------------------------------------------------
    ParameterIndexT _input_index_frequency;    
    ParameterIndexT _input_index_phase;    
	
	// TODO: these should be Vectors of size equal to frame and filled from the input
    SampleT _sum_frequency;
    SampleT _sum_phase;	
	
	// state variables used for wave calc
	//SampleT _period_seconds;
	SampleT _amp;
	SampleT _amp_prev;		
	
	RenderCountT _period_samples;

    public://------------------------------------------------------------------
    explicit Phasor(EnvironmentPtr);
	
    virtual void init();    

    virtual void reset();
		
	//! Render the phasor. 
    virtual void render(RenderCountT f);
};




//=============================================================================
//! A pure (calculated) sine wave. The sine has a ramp from -1 to 1 for each _output_count defined. Range is between -1 and 1. For performance, values are not mapped in this generator; use a Map instance. 
class Sine;
typedef std::shared_ptr<Sine> SinePtr;
class Sine: public Gen {

    private://-----------------------------------------------------------------
    ParameterIndexT _input_index_frequency;    
    ParameterIndexT _input_index_phase;    
	
    //SampleT _sum_frequency;
    // SampleT _sum_phase;
    SampleT _angle_increment;
    SampleT _phase_increment;
    SampleT _cur_phase;
    SampleT _cur_fq;
    
	// SampleT _amp_prev;
	
	RenderCountT _sample_count;		
    OutputsSizeT _i;

    public://------------------------------------------------------------------

    explicit Sine(EnvironmentPtr);
	
    ~Sine();

    virtual void init();
    
    virtual void set_default();    
    
    virtual void reset();
    
	//! Render the pure sine..
    virtual void render(RenderCountT f);
};



//=============================================================================
//! A mapping routine that takes five parameters: source, destination min, destination max, source min, source max. Values of the source greater than min/max are truncaated and held at that value.
class Map;
typedef std::shared_ptr<Map> MapPtr;
class Map: public Gen {

    private://-----------------------------------------------------------------
    ParameterIndexT _input_index_src;
    ParameterIndexT _input_index_src_lower; 	
    ParameterIndexT _input_index_src_upper; 	
    ParameterIndexT _input_index_dst_lower; 	
    ParameterIndexT _input_index_dst_upper; 	

    OutputsSizeT _i;
    
    SampleT _limit_src;
    SampleT _range_src;
    SampleT _range_dst;

    SampleT _min_src;
    SampleT _max_src;

    SampleT _min_dst;
    SampleT _max_dst;
    
    
    public://------------------------------------------------------------------
    explicit Map(EnvironmentPtr);
	
    virtual void init();
    
    virtual void set_default();
		
    virtual void reset();
    
	//! Perform the mapping.
    virtual void render(RenderCountT f);
};



//=============================================================================
//! An AD envelope.
class AttackDecay;
typedef std::shared_ptr<AttackDecay> AttackDecayPtr;
class AttackDecay: public Gen {

    private://-----------------------------------------------------------------
    ParameterIndexT _input_index_trigger;
    ParameterIndexT _input_index_attack;
    ParameterIndexT _input_index_decay;
    ParameterIndexT _input_index_exponent;
    ParameterIndexT _input_index_cycle;

    ParameterIndexT _output_index_eoa;
    ParameterIndexT _output_index_eod;

    OutputsSizeT _i;
    
    // make these sample types because will deivide by
    SampleT _a_samps;
    SampleT _d_samps;
    
    SampleT _last_amp; // last amp    
    SampleT _stage_amp_range; // last unit interval amp, pre exp scaling

    bool _trigger_a; // last unit interval amp, pre exp scaling
    
    //! Progress samps are set to zero at trigger
    OutputsSizeT _progress_samps;
    //! Store envelope stage as 0 (off); 1 (A); 2 (D)
    UINT8 _env_stage;
    SampleT _amp;
    
    public://------------------------------------------------------------------
    explicit AttackDecay(EnvironmentPtr);
	
    virtual void init();
    
    virtual void set_default();
		
    virtual void reset();
    
	//! Perform the envelope
    virtual void render(RenderCountT f);
};


	






} // end namespace aw

#endif // ends _AW_GENERATOR_H_
