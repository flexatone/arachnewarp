#ifndef _AW_GENERATOR_H_
#define _AW_GENERATOR_H_

#include <vector>
#include <string>
#include <utility> // has pair
#include <unordered_map>
#include <memory>
#include <set>

#include "aw_common.h"

namespace aw {





//! Class of Gen names for static constructors. Enumeration of IDs for each type of generator avaialble; used in factory methods to return configure Generators.
enum class GenID {
    Constant,
    Add,
    Multiply,
    Buffer,
    BreakPoints,
    BPIntegrator,
    Phasor,
    Sine,
    Map,
    AttackDecay,
    White,
};

//! Old-styl inum for iteration. 
enum ConnID {
    Slot,
    Input,
    Output,
};

static const std::vector<ConnID> ConnIDs {Slot, Input, Output};
static const std::vector<std::string> ConnColor {
        COLOR_SLOT, COLOR_INPUT, COLOR_OUTPUT};


//! Identifiers for Parameter types. 
enum class PTypeID {
    Switch,
    Value,
    Frequency,
    Duration,
    Phase,
    Channels,
    // what about inputs that take trigger or gate in same input?
    Trigger,
    Cycle, // boolean for cycle control
    LowerBoundary,
    UpperBoundary,
    BreakPoints, // for slot in breakpoint, wavetable
    Interpolation,
    TimeContext,
};





//=============================================================================
class PType;

//! Shared PType. 
typedef std::shared_ptr<PType> PTypePtr;

//! The PType, based on subclass definition, defines the meaning of an parameter, or Generator slot that can be filled by a Gen. While subclass defines the meaning of the parameter, parameters can have instance names for the particular usage of a Gen. 
class PType {
    public://------------------------------------------------------------------


    //! Primary constructor static method for creating share Parameter types. 
    static PTypePtr make(PTypeID);

    protected://---------------------------------------------------------------
    //! Class name set on creation. 
    std::string _class_name;
    
    //! Public parameter id, can be used for class matching.
    PTypeID _class_id;
    
    //static const char _class_name_alt[];

    //! The name of the parameter in the context of a specific generator. This should explain selection for idscrete selectors.
    std::string _instance_name;

    // can also define, for a particular instance, an expected context; that is, if fq is required, this can be defined here. then, when given another generator at a different context, conversion can happen?
	
    //! A set of GenIDs that can be used as in input when this PType is declared for an input. 
    std::set<GenID> _compatible_gen;
    
    
    public://------------------------------------------------------------------
    explicit PType();

	//! Output stream processor.
	friend std::ostream& operator<<(std::ostream& outputs, const PType& pt);

    //! Set the name of this PType. Passed by const reference as assignment makes the necessary copy. This name is set by the Gen and may not be the same even if its the same PType subclass. 
    void set_instance_name(const std::string& s) {_instance_name = s;};

    std::string get_class_name() const {return _class_name;};

    //! Return the name as a string. 
    std::string get_instance_name() const {return _instance_name;};
    
    // Return the Parameter type id. 
    PTypeID get_class_id() const {return _class_id;};
    
    //! Validate a GenID using _compatible_gen and return, or raise an exception.
    void validate_gen(GenID);
    
};



class PTypeValue;
//! Shared PTypeValue.
typedef std::shared_ptr<PTypeValue> ParameterTypeValuePtr;
//! A subclass of PType that specifies a value; the value can be one of many sorts of things such as a constant or an opperand. 
class PTypeValue: public PType {
    public://------------------------------------------------------------------
    explicit PTypeValue();
};

class PTypeFrequency;
typedef std::shared_ptr<PTypeFrequency> ParameterTypeFrequencyPtr;
//! A subclass of PType that specifies a frequency; this is assumed presently to be in Hertz.
class PTypeFrequency: public PType {
    public://------------------------------------------------------------------
    explicit PTypeFrequency();
};

class PTypeDuration;
typedef std::shared_ptr<PTypeDuration> ParameterTypeDurationPtr;
//! A subclass of PType that specifies a duration in seconds.
class PTypeDuration: public PType {
    public://------------------------------------------------------------------
    explicit PTypeDuration();
};

class PTypePhase;
typedef std::shared_ptr<PTypePhase> ParameterTypePhaseShared;
//! A subclass of PType that specifies a phase; this is assumed presently to be in floating-point values.
class PTypePhase: public PType {
    public://------------------------------------------------------------------
    explicit PTypePhase();
};

//! Used by some Gen to configure slots for both input and output channels, or dimensionality (frame/output count) of the internal marix. 
class PTypeChannels;
typedef std::shared_ptr<PTypeChannels> ParameterTypeChannelsShared;
//! A subclass of PType that specifies a phase; this is assumed presently to be in floating-point values.
class PTypeChannels: public PType {
    public://------------------------------------------------------------------
    explicit PTypeChannels();
};

class PTypeTrigger;
typedef std::shared_ptr<PTypeTrigger> PTypeTriggerPtr;
//! An impulse or trigger, or a single sample at 1.
class PTypeTrigger: public PType {
    public://------------------------------------------------------------------
    explicit PTypeTrigger();
};

class PTypeCycle;
typedef std::shared_ptr<PTypeCycle> PTypeCyclePtr;
//! An impulse or trigger, or a single sample at 1.
class PTypeCycle: public PType {    
    public://------------------------------------------------------------------
    explicit PTypeCycle();
};


class PTypeLowerBoundary;
typedef std::shared_ptr<PTypeLowerBoundary>
        ParameterTypeLowerBoundaryShared;
//! A lower boundary, likely a minimum, for dynamic sizing of a generator. 
class PTypeLowerBoundary: public PType {
    public://------------------------------------------------------------------
    explicit PTypeLowerBoundary();
};

class PTypeUpperBoundary;
typedef std::shared_ptr<PTypeUpperBoundary>
        ParameterTypeUpperBoundaryShared;
//! A lower boundary, likely a maximum, for dynamic sizing of a generator. 
class PTypeUpperBoundary: public PType {
    public://------------------------------------------------------------------
    explicit PTypeUpperBoundary();
};


class PTypeBreakPoints;
typedef std::shared_ptr<PTypeBreakPoints> PTypeBreakPointsPtr;
//! A table, or a Buffer with 2 channels of data. 
class PTypeBreakPoints: public PType {
    public://------------------------------------------------------------------
    explicit PTypeBreakPoints();
};


//! A parameter (used as a slot) to select various interpoaltion types.
class PTypeInterpolate;
typedef std::shared_ptr<PTypeInterpolate> PTypeInterpolationPtr;
class PTypeInterpolate: public PType {
    public://------------------------------------------------------------------
    explicit PTypeInterpolate();
    
    enum Opt {
        Flat,
        Linear,
        Exponential,
        HalfCosine,
        Cubic
    };
    inline static Opt resolve(SampleT x) {
        return x < 0.5 ? Flat :
                x < 1.5 ? Linear :
                x < 2.5 ? Exponential :
                x < 3.5 ? HalfCosine :
                Cubic; // if out of upper range
    };
    
};

//! A parameter (used as a slot) to select time context forn break-point segments.
class PTypeTimeContext;
typedef std::shared_ptr<PTypeTimeContext> PTypeTimeContextPtr;
class PTypeTimeContext: public PType {
    public://------------------------------------------------------------------
    explicit PTypeTimeContext();

    enum Opt { 
        Samples,
        Seconds
    };
    inline static Opt resolve(SampleT x) {
        return x >= -0.5 && x < 0.5 ? Opt::Samples : Opt::Seconds;
    };
    
};


class PTypeDirection;
typedef std::shared_ptr<PTypeDirection> PTypeDirectionPtr;
class PTypeDirection: public PType {
    public://------------------------------------------------------------------
    explicit PTypeDirection();
    
    enum Opt {
        Forward,
        Reverse,
        Cycle,
        RandomSelect,
        RandomWalk,
        RandomPermutate,
    };
    inline static Opt resolve(SampleT x) {
        return x < 0.5 ? Forward :
                x < 1.5 ? Reverse :
                x < 2.5 ? Cycle :
                x < 3.5 ? RandomSelect :
                x < 4.5 ? RandomWalk :
                RandomPermutate; // if out of upper range
    };
    
};




//=============================================================================
//! Utility class for managing directions of indices. Used in Counter and Sequncer generators. Size is fixed over the life of the object. If size changes, then we must create a new object. 
class DirectedIndex {
    //std::random_shuffle(cards_.begin(), cards_.end())
    // create with a given length size
    // take an a parameter to control Direction
    private:

    //! If we permit random permutation, we have to store all indices
    std::vector<std::size_t> _indices;

    //! If size is zero, we assume max size for this type. We set size at creation and do not change it.
    FrameSizeT _size;

    //! Store the last value returned; in random permutation, we store the index used.
    FrameSizeT _last_value;

    //! The max size is the largest int available. 
    const static FrameSizeT _max {std::numeric_limits<FrameSizeT>::max()};

    //! If the size is greater than this value, requests for random permutation will degrade to random choice. Experiment to find optimum value. 
    const static std::size_t _max_size_for_random_permutation {1000}; 

    bool _can_use_random_permutation;
    bool _forward;

    //! Direction is an old-style enum. 
    PTypeDirection::Opt _direction {PTypeDirection::Opt::Forward};


    public:
    DirectedIndex() = delete;

    explicit DirectedIndex(FrameSizeT size);

    // if we pass in last, we can do random walk, forward, backward, random 
    FrameSizeT next();

    //! The reset returns the index to first postion, which is always zero.  
    void reset();

    //! Must be able to set direction at any time; this does not reset.
    void set_direction(PTypeDirection::Opt d) {_direction = d;};

};












//=============================================================================
class Gen;
typedef std::shared_ptr<Gen> GenPtr;
//! Gen class. Base-class of all Generators. A Gen has inputs and outputs. Inputs are a vector of vectors of Generators/ out number pairs. The number of types, and types of inputs, are defined by the mapping _input_parameter_type; the Gen inputs are stored on the _inputs VVGenShared. Multiple inputs in the same parameter position are always summed. Rendering on the Gen is stored in the outputs, a table of one frame for each output. Clients of the generator freely read from the outputs vector. 
class Gen: public std::enable_shared_from_this<Gen> {

    public://--------------------------=---------------------------------------
    // public typedefs
	//! A mapping of index number
    typedef std::unordered_map<PIndexT, PTypePtr> MapIndexToParameterTypePtr;

    //! A vector of GenPtr instances used for slots. Slots do not yet need to define outputs number; generatlly assume that we use the first outputs?
    typedef std::vector<GenPtr> VGenPtr;
		
    //! An OutputConnection is a pair formed of GeneeratorShared and an integer representing the output/outputs number, starting from zero, to be read from in that input.
    typedef std::pair<GenPtr, PIndexT> GenPtrOutPair;
    typedef std::vector<GenPtrOutPair> VGenPtrOutPair;
    typedef std::vector<VGenPtrOutPair> VVGenPtrOutPair;
    		
    protected://----------------------------------------------------------------
    //! The name of the class. This is set during the class constructor by the derived class, and thus needs to be protected.
    std::string _class_name;

    //! Public generator id, can be used for class matching. 
    GenID _class_id;

    private://------------------------------------------------------------------
    // Rename PIndexT as OutputIndexType
    //! Store the number of output channels, that this Gen is currently set up with. Storage array acount.
    PIndexT _output_count;
    
	//! The _outputs_size is derived from frame _output_count times the _frame_size. OutputsSizeT is expected to store size up to long multichannel audio files. Total storage size.
    OutputsSizeT _outputs_size;
    
    // TODO: use InputIndexType PIndexT 
    //! Number of input parameters used by this Gen. More than one Gen can reside in each slot. 
    PIndexT _input_count;

	//! Number of slots used by this Gen. One Gen can reside in each slot. 
    PIndexT _slot_count;    

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
	
    //! The main storage for PTypePtr instances used as inputs. These are mapped by index value, which is the same index value in the inputs vector. This is only protected and not private so that Constant can override print_inputs.
    std::unordered_map<PIndexT, PTypePtr> _input_parameter_type;	
		
    //! A std::vector of vectors of GeneratorsShared / outputs id pairs that are the inputs to this function. This could be an unordered map too, but vector will have optimal performance when we know the index in advance.
    VVGenPtrOutPair _inputs;
    
	//! For each render call, we sum all inputs up to the common frame size available in the input and store that in a Vector of sample types. This is done to make render() methods cleaner and remove redundancy.
	VVSampleT _summed_inputs;    
    
	//! A std::vector of GeneratorsShared that are used internall for configuration of this Gen. Unlike inputs, only one Gen can occupy a slot position, and these are protected, not public (a suggestion that these are for internal use only). Example: a buffer has a slot for duration of that buffer. It is not yet decided if generators in slots shold be rendered, but at least only once per render step. Further, which output channels of the generator slot to use is decided internally or with another parameter.
	VGenPtr _slots;
	
	//! Must define slots as ParameterTypes, meaning the same can be used for both inputs and for slots. This also means slots must be Generators. These are mapped by index value, which is the same index value in the inputs vector. Note that all slots must be filled for the generator to be used, so perhaps defaults should be provided. 
    std::unordered_map<PIndexT, PTypePtr> _slot_parameter_type;
	
	
    //! We store a PTypePtr for each defined output, telling us what it is.
    std::unordered_map<PIndexT, PTypePtr> _output_parameter_type;
		

    public://------------------------------------------------------------------
        
    //! A vector of sample vectors. This might be deemed best as private, but for performance this is public: no function call is required to read from it. 
    VVSampleT outputs;	
    

    // ========================================================================
    // methods ================================================================
	
    private://-----------------------------------------------------------------   		
    //! Resize the outputs vector. Always called when registering an output or changing frame size.
    void _resize_outputs();
	

    protected://---------------------------------------------------------------
	
    //! Validate the current contnets of the outputs data. A virtual method for overridding in derived classes (e.g., BreakPoints). Called whenever a outputs are set as a whole (aka, set by array). Will raise an exception.
    virtual Validity _validate_outputs();
    
    //! Called by Generators during init() to configure the input parameters found in this Gen. PTypePtr instances are stored in the Gen, the _input_count is incremented, and _inputs is given a blank vector for appending to. The order of execution matters. 
    void _register_input_parameter_type(PTypePtr pts);

    //! Remove all inputs; used by slots that dynamically chagne inputs and outputs; all existing inputs will remain. 
    void _clear_input_parameter_types();

	//! Called by Generators during init() to configure the slot parameters found in this Gen.
    void _register_slot_parameter_type(PTypePtr pts);

	//! Define an output. This calls _resize_outputs().
    void _register_output_parameter_type(PTypePtr pts);

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
    static GenPtr make_with_environment(GenID q, 
            EnvironmentPtr e);

    //! Factory for all Generators with by generator ID alone. This creates a GenPtr, and calls its init() method.     
    static GenPtr make(GenID);

    //! Factory for creating constants given just a numeric type.     
    static GenPtr make(SampleT);


    //! Produce documentation for all generators. 
    static void doc();
    
    
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
    PIndexT get_output_count() const {return _output_count;};

    //! Return the the outputs size, or the total number of samples used for all frames at all outputs.
    FrameSizeT get_outputs_size() const {return _outputs_size;};
    
    //! Get the average value of all outputs values. 
    SampleT get_outputs_average() const;

    //! Get the average of single _output_count of outputs. If d is 0, all dimensions are averaged. If d is greater than the number of dimensions, and error is raised. 
    SampleT get_output_average(PIndexT d) const;

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

//    std::string get_label_slot(PIndexT) const;
//
//    std::string get_label_input(PIndexT) const;
//
//    std::string get_label_output(PIndexT) const;

    std::string get_conn_label(PIndexT n, ConnID conn) const;

    //! Get the name and address (memory start) for this Gen, as a single string.
    std::string get_name_address() const;
    
    //! Get the name and addres for this Gen, labeling the name with "Gen" and wrapping the address in braces. This is used for a more readable presentation.
    std::string get_label_address() const;

    //! Get the specification for this Gen, or the number of slots, ins, and outs.
    std::string get_label_specification() const;
    
    //! Get a complete label for this generator. This includes the get_label_address() presentation, alongwith the input and output count. 
    std::string get_label() const;
    
	//! Output stream friend function: returns the label of the Gen. 
	friend std::ostream& operator<<(std::ostream& ostream, const Gen& g);
    
	//! Output stream friend function: returns the label of the GenPtr.     
	friend std::ostream& operator<<(std::ostream& ostream, const GenPtr g);

    //! Return the name as a string. 
    std::string get_class_name() const {return _class_name;};

    GenID get_class_id() const {return _class_id;};


    //! Get a parameter type given an index and a connection type
    PTypePtr get_parameter_type(PIndexT i, ConnID conn);

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
                                    PIndexT d=0) const;
	
        
	//! Set the outputs from an array. This low level routine is the only way outputs are directly written from a collection. The reeturned bool is the result of _validate_outputs. 
	Validity set_outputs_from_array(SampleT* v, OutputsSizeT s,
							PIndexT ch, bool interleaved=true);
								
	//! Set the outputs (resizing if possible) to values passsed in from a vector of SampleT. Note this presently copies values from a vector to an array, and thus requires 2x the memory alloc. Should by a const vector but cannot yet get usage right when deriving an array pointer; but: the passed in vector should not be changed. 
	void set_outputs_from_vector(VSampleT& vst,
								PIndexT ch, bool interleaved=true);


    //! If we are in a Buffer class, this method loads a complete file path to an audio file into the output of this Gen. This is not implemented in the base class Gen. 
    virtual void set_outputs_from_fp(const std::string& fp);


    //! Overridden, overloaded function for setting outputs. Implemented by Buffer, but defined on base so all have access. Need a Injector because otherwise we would take more parameters for channels, etc.
    virtual void set_outputs(const Inj<SampleT>& bi);

    //! Set output swith a file path represented as a string.
    virtual void set_outputs(const std::string& fp);



	// inputs and slots ......................................................    
    //! Return the number of inputs; this is not the same as the number of Generators, as each input may have 1 or more Generators
    PIndexT get_input_count() {return _input_count;};

    //! Return the number of slots. There is only one Gen per slot. 
    PIndexT get_slot_count() {return _slot_count;};

	//! Return the parameter index for a named parameter.
    PIndexT get_input_index_from_parameter_name(const
            std::string& s);

	//! Return the parameter index for the first-encountered parameter type id; raises an exception if not found.
    PIndexT get_input_index_from_class_id(const
            PTypeID ptid);

	//! Return the parameter slot for a named parameter.
    PIndexT get_slot_index_from_parameter_name(const std::string& s);	


    // TODO: must check for duplicated connections and silently skip them; 
    
	// inputs ..............................................................        
    //! Directly set a parameter given an index. This will remove/erase any multiple inputs for this parameter
    virtual void set_input_by_index(
            PIndexT i,
            GenPtr gs,
            PIndexT pos=0);
    
    virtual void set_input_by_index(
            PIndexT i,
            SampleT v,
            PIndexT pos=0);

    ///! Set input by class id of the PType. 
    void set_input_by_class_id(
            PTypeID ptid,
            GenPtr gs,
            PIndexT pos=0);
                                        
    void set_input_by_class_id(
            PTypeID ptid,
            SampleT v, // accept numbers
            PIndexT pos=0);


    //! Add a multiple input at this parameter. 
    virtual void add_input_by_index(PIndexT i, 
            GenPtr gs, PIndexT pos=0);

    virtual void add_input_by_index(PIndexT i, SampleT v, 
            PIndexT pos=0);
  
  
    //! Get a vector of GenPtr for an input, given the input index. This should be a copy of the vector, and is thus slow. This is virtual to provide Constant to override and return an empty vector (even though it might have an input).
    virtual VGenPtrOutPair get_input_gens_by_index(PIndexT i);

    //! Remove all GenPtr attacked to all inputs.
    void clear_inputs();
  
	// slot ..............................................................    	
    //! Directly set a parameter to a slot given an index. This will remove/erase any parameter on this slot. The update parameter permits disabling updating a slot, useful during initial configuration. 
    virtual void set_slot_by_index(PIndexT i, GenPtr gs, 
									bool update=true);
    
	//! Overridden to handle a constant. 
    virtual void set_slot_by_index(PIndexT i, SampleT v, 
									bool update=true);

    //! Return the single gen at this slot position (not a vector, like inputs).
    virtual GenPtr get_slot_gen_at_index(PIndexT i);
    
    // Remove all GenPtr attacked to all inputs: does not make sense to do this, because we always need one gen in each slot position
    //void clear_slots();
    

};

//.............................................................................
// functions on GenPtr ...............................................
//! Parsimonious serial connection: connect the min of a and b in straight connections. If count is zero, we set all available connections from start to end.
inline GenPtr connect_serial_to_inputs(GenPtr lhs, GenPtr rhs, 
        PIndexT start=0, PIndexT count=0) {
    // connect from left to right, so from lhs to rhs
    // lhs is above rhs in downard flow
    // get min of lhs out and rhs in, match as many in parallel as possible    
    PIndexT availLen = std::min(
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
    PIndexT i;    
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
        PIndexT start=0, PIndexT count=0) {        
    // set environment from lhs
	GenPtr g_lhs = Gen::make_with_environment(
            GenID::Constant, rhs->get_environment());
    g_lhs->set_input_by_index(0, lhs); // this will call Constant::reset()
    return aw::connect_serial_to_inputs(g_lhs, rhs); // will return rhs
}


//! Assign an Injector as an input to matching inputs on rhs GenPtr.
inline GenPtr connect_serial_to_inputs(const Inj<SampleT>& lhs, GenPtr rhs) {
    // create vector, fill with values;
    VSampleT inj;
    lhs.fill_interleaved(inj);
    // get lesser of input, injection size
    PIndexT availLen = std::min(rhs->get_input_count(),
            inj.size());
    for(PIndexT i=0; i<availLen; ++i) {
        rhs->add_input_by_index(i, inj[i]);
    }
    return rhs;
}

//! Assign an Injector as an input to matching inputs on rhs GenPtr.
inline GenPtr connect_serial_to_inputs(const Inj<GenPtr>& lhs, GenPtr rhs) {
    Gen::VGenPtr inj; // create vector, fill with values;
    lhs.fill_interleaved(inj);
    PIndexT availLen = std::min(rhs->get_input_count(),
            inj.size());
    for(PIndexT i=0; i<availLen; ++i) {
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
    PIndexT availLen = std::min(rhs->get_slot_count(),
            inj.size());
    for(PIndexT i=0; i < availLen; ++i) {
        rhs->set_slot_by_index(i, inj[i]); // set, not add
    }
    return rhs;
}

//! Assign an Inj of Gen Ptrs.
inline GenPtr connect_serial_to_slots(const Inj<GenPtr>& lhs,
        GenPtr rhs) {
    Gen::VGenPtr inj; // create vector, fill with values;
    lhs.fill_interleaved(inj);
    PIndexT availLen = std::min(rhs->get_slot_count(),
            inj.size());
    for(PIndexT i=0; i < availLen; ++i) {
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
    //return aw::connect_parallel(lhs, rhs, GenID::Multiply);
    rhs->set_outputs(lhs); // will throw if rhs is not Buffer
    return rhs;        
} 


// parallel connections ......................................................

//! Connect two generators as inputs into another generator, where that generator is given by the passed in GenID. This is a binary operator.
inline GenPtr connect_parallel(
        GenPtr lhs, 
        GenPtr rhs, 
        GenID gid) {
    
    // get lesser of outputs between the two, then create an add with that many slots
    PIndexT j = std::min(lhs->get_output_count(), 
            rhs->get_output_count());
    // use the passed in gen id, this is usually GenID::Add, GenID::Multiply
    GenPtr g = Gen::make_with_environment(gid, 
            lhs->get_environment());
    // the returned Gen needs to support multiple channels; these are set as slow 0
    g->set_slot_by_index(0, j);
    // try to conect as many in to out as possible for each gen
    PIndexT i;
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
        GenID gid) {
    GenPtr g_lhs = Gen::make_with_environment(
            GenID::Constant, rhs->get_environment());
    g_lhs->set_input_by_index(0, lhs); 
    GenPtr g = aw::connect_parallel(g_lhs, rhs, gid);
    return g;
}

//! Variant with SampleT as right-hand side.
inline GenPtr connect_parallel(
        GenPtr lhs, 
        SampleT rhs, 
        GenID gid) {
    GenPtr g_rhs = Gen::make_with_environment(
            GenID::Constant, lhs->get_environment());
    g_rhs->set_input_by_index(0, rhs); 
    GenPtr g = aw::connect_parallel(lhs, g_rhs, gid);
    return g;
}



// operator + .................................................................
inline GenPtr operator+(GenPtr lhs, GenPtr rhs) {
    return aw::connect_parallel(lhs, rhs, GenID::Add);
}

inline GenPtr operator+(GenPtr lhs, SampleT rhs) {
    return aw::connect_parallel(lhs, rhs, GenID::Add);
}

inline GenPtr operator+(SampleT lhs, GenPtr rhs) {
    return aw::connect_parallel(lhs, rhs, GenID::Add);
}

// Alternative opperators:
// |, || set slots
// & set output buffer directly


// operator * .................................................................
inline GenPtr operator*(GenPtr lhs, GenPtr rhs) {
    return aw::connect_parallel(lhs, rhs, GenID::Multiply);
}

inline GenPtr operator*(GenPtr lhs, SampleT rhs) {
    return aw::connect_parallel(lhs, rhs, GenID::Multiply);
}

inline GenPtr operator*(SampleT lhs, GenPtr rhs) {
    return aw::connect_parallel(lhs, rhs, GenID::Multiply);
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
    virtual VGenPtrOutPair get_input_gens_by_index(PIndexT i);
    
    //! This overridden method throws an exception: you cannot set a Gen to a constant.
    virtual void set_input_by_index(PIndexT i, GenPtr gs, 
            PIndexT pos=0);    
                                        
    //! Set value as a SampleT value.
	virtual void set_input_by_index(PIndexT i, SampleT v, 
            PIndexT pos=0);
    
    //! This overridden method throws an exception: you cannot set a Gen to a constant.    
    virtual void add_input_by_index(PIndexT i, GenPtr gs, 
            PIndexT pos=0);    
                                        
    //! Add value as a SampleT value.                                        
	virtual void add_input_by_index(PIndexT i, SampleT v, 
            PIndexT pos=0);
    
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
			
    //! Write to an audio file to given the ouput file path. The optional PIndexT argument can be used to specify a single _output_count of many to write. If PIndexT is 0, all outputs are written.
    virtual void write_output_to_fp(const std::string& fp, 
                                    PIndexT d=0) const;
        
    //! Set the outputs of this Gen to the content of an audio file provided as an audio path. This overridden method makes the usage of libsndfile to read in a file.
    virtual void set_outputs_from_fp(const std::string& fp);

    //! Overridden set outputs from Inj reference.
    virtual void set_outputs(const Inj<SampleT>& bi);

    //! Overridden set outputs a std::string, treated a file path.
    virtual void set_outputs(const std::string& fp);
    
};




//=============================================================================
//! A derived buffer for storing (and validating) break point data. This is not an interpolator, but just a derived storage class. 
class BreakPoints;
typedef std::shared_ptr<BreakPoints> BreakPointsPtr;
class BreakPoints: public Buffer {

    public://------------------------------------------------------------------

    explicit BreakPoints(EnvironmentPtr);

    virtual void init();
    
    //! BreakPoints overrides _validate_outputs to make sure that this is two dimensional and that x values are increasing. This does not raise an exception to allow caller to clean up memory.
    virtual Validity _validate_outputs();
    
};


//=============================================================================
//! A break point integrator, interpolating between two or more time points, value pairs as defined with a BreakPoints Gen. Note that the last time point y values is never explicitly reached in transition from last-1 to last; however, when triggered the sustained value is this last value; when looped, a smoth interpolation to start can be made by last value being the same as the first value. 
class BPIntegrator;
typedef std::shared_ptr<BPIntegrator> BPIntegratorPtr;
class BPIntegrator: public Gen {

    private://-----------------------------------------------------------------

    PIndexT _input_index_trigger;
    PIndexT _input_index_cycle;
    PIndexT _input_index_exponent;

    PIndexT _slot_index_bps;
    PIndexT _slot_index_interp;
    PIndexT _slot_index_t_context;

    FrameSizeT _points_len;
    FrameSizeT _point_count;
    bool _running;
    bool _start;
    
    OutputsSizeT _samps_in_bp;
    OutputsSizeT _samps_last_point;
    OutputsSizeT _samps_next_point;
    OutputsSizeT _samps_width;
    
    PTypeTimeContext::Opt _t_context;
    PTypeInterpolate::Opt _interp;

    SampleT _x_src;
    SampleT _x_dst;
    SampleT _y_src;
    SampleT _y_dst;
    SampleT _y_span;
    
	SampleT _amp;    
    
    protected://---------------------------------------------------------------

    virtual void _update_for_new_slot();

    public://------------------------------------------------------------------

    explicit BPIntegrator(EnvironmentPtr);

    virtual void init();

    virtual void reset();

    virtual void render(RenderCountT f);

    
};


//=============================================================================
//! The phasor has a ramp from 0 to 1 for each _output_count defined. Only the first _output_count of multiple dimensional inputs is used. 
class Phasor;
typedef std::shared_ptr<Phasor> PhasorPtr;
class Phasor: public Gen {

    private://-----------------------------------------------------------------
    PIndexT _input_index_frequency;
    PIndexT _input_index_phase;    
	
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
    PIndexT _input_index_frequency;    
    PIndexT _input_index_phase;    
	
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
    PIndexT _input_index_src;
    PIndexT _input_index_src_lower; 	
    PIndexT _input_index_src_upper; 	
    PIndexT _input_index_dst_lower; 	
    PIndexT _input_index_dst_upper; 	

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
    PIndexT _input_index_trigger;
    PIndexT _input_index_attack;
    PIndexT _input_index_decay;
    PIndexT _input_index_exponent;
    PIndexT _input_index_cycle;

    PIndexT _output_index_eoa;
    PIndexT _output_index_eod;

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


	
//=============================================================================
//! A white-noise generator.
class White;
typedef std::shared_ptr<White> WhitePtr;
class White: public Gen {

    private://-----------------------------------------------------------------
    OutputsSizeT _i;
        
    public://------------------------------------------------------------------
    explicit White(EnvironmentPtr);
    
    virtual void init();
            
    virtual void reset();
    
    //! Perform the noise
    virtual void render(RenderCountT f);
};










} // end namespace aw

#endif // ends _AW_GENERATOR_H_
