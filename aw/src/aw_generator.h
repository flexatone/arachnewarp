#ifndef _AW_GENERATOR_H_
#define _AW_GENERATOR_H_


#include <vector>
#include <string>
#include <tr1/unordered_map>

//#include <memory> # only with -std=c++0x
// #include <boost/shared_ptr.hpp>
#include <tr1/memory>

#include "aw_common.h"

namespace aw {


//==============================================================================
class ParameterType;
//! Shared ParameterType. 
typedef std::tr1::shared_ptr<ParameterType> ParameterTypeShared;
//! The ParameterType, based on subclass definition, defines the meaning of an input slot that can be filled by a Generator. While subclass defines the meaning of the parameter, parameters can have instance names for the particular usage of a Generator. 
class ParameterType {
    protected://----------------------------------------------------------------
    std::string _class_name;
    std::string _instance_name;
    // for specific instances we can define what the rate or sample value means 
    std::string _description; 

    // can also define, for a particular instance, an expected context; that is, if fq is required, this can be defined here. then, when given another generator at a different context, conversion can happen?
	
    public://-------------------------------------------------------------------
    explicit ParameterType();
    virtual ~ParameterType();

	//! Output stream processor.
	friend std::ostream& operator<<(std::ostream& output, 
						const ParameterType& pt);

    //! Set the name of this ParameterType. Passed by const reference as assignment makes the necessary copy. This name is set by the Generator and may not be the same even if its the same ParameterType subclass. 
    void set_instance_name(const std::string& s) {_instance_name = s;};

    //! Return the name as a string. 
    std::string get_instance_name() const {return _instance_name;};
    
};

class ParameterTypeValue;
//! Shared ParameterTypeValue.
typedef std::tr1::shared_ptr<ParameterTypeValue> ParameterTypeValueShared;
//! A subclass of ParameterType that specifies a value; the value can be one of many sorts of things such as a constant or an opperand. 
class ParameterTypeValue: public ParameterType {

    public://-------------------------------------------------------------------
    explicit ParameterTypeValue();
    virtual ~ParameterTypeValue();
};

// TODO: provide a RateConverter that permits specifiying values in a variety of different ways. 
class ParameterTypeFrequency;
typedef std::tr1::shared_ptr<ParameterTypeFrequency> ParameterTypeFrequencyShared;
//! A subclass of ParameterType that specifies a frequency; this is assumed presently to be in Hertz.
class ParameterTypeFrequency: public ParameterType {
    public://-------------------------------------------------------------------
    explicit ParameterTypeFrequency();
    virtual ~ParameterTypeFrequency();
};

// TODO: phase can be provided in floating point values or degrees; figure out how to handle this
class ParameterTypePhase;
typedef std::tr1::shared_ptr<ParameterTypePhase> ParameterTypePhaseShared;
//! A subclass of ParameterType that specifies a phase; this is assumed presently to be in floating-point values.
class ParameterTypePhase: public ParameterType {
    public://-------------------------------------------------------------------
    explicit ParameterTypePhase();
    virtual ~ParameterTypePhase();
};






//==============================================================================
class GeneratorConfig;
//! A shared GeneratorConfig.
typedef std::tr1::shared_ptr<GeneratorConfig> GeneratorConfigShared;
//! The GeneratorConfig provides configuration parameters for Generators. If not provided, a default GeneratorConfig is used. General configuration parameters include an initial frame dimension and size, as well has a handle to a shared Environment.
class GeneratorConfig {
    public://-------------------------------------------------------------------

	//! Static factory method for default construction.
    static GeneratorConfigShared make_default();

	//! Static factory method for creation with a defined dimension. 
    static GeneratorConfigShared make_with_dimension(FrameDimensionType d);
        
	//! Primary constructor for creation with a shared Enviroinment. 
    explicit GeneratorConfig(EnvironmentShared e);
	
	//! Deconstructor
    ~GeneratorConfig();

    FrameDimensionType get_init_frame_dimension() const {
		return _init_frame_dimension;};

    void set_init_frame_dimension(FrameDimensionType d) {
		_init_frame_dimension = d;};
    
    FrameSizeType get_init_frame_size() const {return _init_frame_size;};
	
	//! Return the Environement shared pointer. Should be const to this class. 
	EnvironmentShared get_environment() const {return _environment;};
    
    private://------------------------------------------------------------------
	
	//! This is the initial frame dimension requested; for some Generators, this may be not be a permitted dimension. 
    FrameDimensionType _init_frame_dimension;
	
	//! This is the initial frame size. 
    FrameSizeType _init_frame_size; 
	
	//! We store an instance of an environment on generator config. It is possible that we might, at some time after creation, change the Environment. However, doing this should explicitly require calling a methods
	EnvironmentShared _environment; 

};





//==============================================================================
// Dimensionality

// After adding an input to a generator, we have to look at all inputs attached to that generator (recursively) and find the max dimensionality. If this dimensionality is greater then the current dimensionality of the Generator, and the Generator get_dimension_dyanmics == true, then resize to the maximum size. Otherwise, keep at current size. The render method will have to take into account having higher dimensionality inputs
// Case 1: Create Add@2 and add operator inputs Constant@4 and Constant@3; Add should automatically resize to Add@4, as addition should not mix dimensionalities.




//==============================================================================   
class Generator;
typedef std::tr1::shared_ptr<Generator> GeneratorShared;
//! Generator class. Base-class of all Generators. A Generaotr has inputs and an output. Inputs are a vector of vectors of Generators. The number of types, and types of inputs, are defined by the mapping _input_parameter_type; the Generator inputs are stored on the _inputs VVGenShared. Multiple inputs in the same parameter position are always summed. 

//! When multiple inputs have a different dimensionality than the Generator a number of options are available. The Generator might scale output to match that of the highest dimensionality of any one input. Alternatively, the Generator's dimensionality might remain fixed and only read the first or some other selection of input dimension. 

//! If the _dimension_dynamics attribute is DD_ResizableFreely, the Generator can be dynamically resized based on inputs. 


class Generator {

    public://-------------------------------------------------------------------    
    // public typedefs
	//! A mapping of index number
    typedef std::tr1::unordered_map<ParameterIndexType, ParameterTypeShared> MapIndexToParameterTypeShared;
    
	typedef std::vector<SampleType> VSampleType;
	
    typedef std::vector<GeneratorShared> VGenShared;
    typedef std::vector< VGenShared > VVGenShared;
	
    typedef std::vector<OutputSizeType> VOutputSize;
    typedef std::vector<VOutputSize> VVOutputSize;

    //! Enumeration of IDs for each type of generator avaialble; used in factory methods to return configure Generators. 
    enum GeneratorID {
        ID_Constant,    
        ID_Add,
        ID_BufferFile,		
        ID_Phasor,				
    };
        
    enum DimensionDynamics {
        DD_FixedMono,    
		//DD_Fixed_N, // size is fixed based on class definition; does not change		
        DD_ResizableAtInit, // size can only be set at init
        DD_ResizableFreely,		
    };
		
    protected://----------------------------------------------------------------
    //! The name of the class. This is set during the class constructor by the derived class, and thus needs to be protected.
    std::string _class_name;

    private://------------------------------------------------------------------
    //! Store the number of dimensions, similar to channels, that this Generator is currently set up with. 
    FrameDimensionType _frame_dimension;
	
	//! The size of each frame for each dimension.
    FrameSizeType _frame_size; // if changed, need to rebuild output
	
	//! The _output_size is derived from frame dimension times the frame size. 
    OutputSizeType _output_size;
    
    //! Number of input parameter slots used by this Generator. More than one Generator can reside in each slot. 
    ParameterIndexType _input_parameter_count;    
    
	//! Store the GeneratorConfig instance. This also stores a handle to shared Environment in stance. 
    GeneratorConfigShared _generator_config;    
	
    protected://----------------------------------------------------------------
	
	//! The sampling rate, taken from an Environment instance, as passed through from a GeneraotrConfig. 
	OutputSizeType _sampling_rate;
	
	//! The nyquist frequency, .5 * SamplingRate; this is stored to optimize calculations that need this value.
	OutputSizeType _nyquist;
	
    //! Define if this Generator has one of three states to describe resizability. Most generators have resizable output; only some (like a mixer) do not.
    DimensionDynamics _dimension_dynamics;
	
    //! Define if this Generator has resizable frames size. Most generators do not have have resizable frame size; only some (like a Buffer or WaveTable) do.
    bool _frame_size_is_resizable;	
                            
    //! The number of frames that have passed since the last reset. Protected because render() and reset() routines need to alter this. 
    RenderCountType _render_count;
	
    //! The main storage for ParameterTypeShared instances. These are mapped by index value, which is the same index value in the inputs vector. This is only protected and not private so that Constant can override print_inputs.
    std::tr1::unordered_map<ParameterIndexType, 
                            ParameterTypeShared> _input_parameter_type;	
	
    //! Store and update the output sizes of all inputs (a vector for each parameter type, and an OutputSizeType for each value therein); this only needs to be updated when resizing has happened in the inputs. This needs to be accessed in subclass render routines, so will be protected for now.
    VVOutputSize _inputs_output_size;	
	
    //! A std::vector of vectors of GeneratorsShared that are the inputs to this function. This could be an unordered map too, but vector will have optimal performance when we know the index in advance.
    VVGenShared _inputs;	
	
	//! A vector of output offsets, to be used when iterating over dimension and incorporating interleaved or non-interleaved presentation of the output. This must be updated after every resizing, as dimension and frame size may have changed. This is protected because subclass methods may want to use this structure (but the should not change it)
	VFrameSizeType _dimension_offsets; 

    public://-------------------------------------------------------------------
        
    //! A linear array of samples, which may include multiple dimensions placed in series. This might be private, but for performance this is presently public; when configured to run  dimensions can be stored via requests and than used as constants w/o function calls. 
    SampleType* output;	
    

    // methods =================================================================
	
    private://------------------------------------------------------------------   	
	
    //! Resize the output vector. Always called during init and also by set_dimension. Will remove an exisiting array. For public resizing use set_dimension(). Will not reset.  
    void _resize_output();    
	
    protected://----------------------------------------------------------------
		
    //! Called by Generators during init() to configure the input parameters found in this Generator. ParameterTypeShared instances are stored in the Generator, the _input_parameter_count is incremented, and both _inputs and _inputs_output_size are givne a blank vector for appending to. 
    void _register_input_parameter_type(ParameterTypeShared pts);

    //! Recursively search for the max input dimension. 
    virtual FrameDimensionType _find_max_input_dimension(FrameDimensionType d=1);

    //! Call this everytime a new input has been added. Does necessary and may resize this generator. This should not be called in render(). Pre-fetches all the output sizes of all inputs. These are stored in _inputs_output_size.
    void _update_for_new_input();
    
    //! Call the render method on all stored inputs. This is done once for each render call; this calls each  input Generator's render() method. This means that during render() calls, _render_inputs() does not need to be called. 
    inline void _render_inputs(RenderCountType f);

    public://-------------------------------------------------------------------
	//! Factory for all Generators that creates a  Generator with a dimension. Calls init() method. 
    static GeneratorShared make_with_dimension(GeneratorID, FrameDimensionType);
	
    //! Factory for all Generators. This creates a Generator, and calls its init() method. 
    static GeneratorShared make(GeneratorID);


    public://-------------------------------------------------------------------
	//! Main constructor that takes a generator config. 
    explicit Generator(GeneratorConfigShared gc);
    
    virtual ~Generator();
    
    //! Initialize the Generator. This method is responsible for creating ParameterTypeValueShared instances and adding them to the Generator using the _register_input_parameter_type method. This method also does the initial sizing of the Generator, and thus could raise an exception. Additional buffers that might be needed for this Generator can be stored here. As this is virtual the base-classes init is not called. 
    virtual void init();    

    //! Return a Boolean if this Generator has resizable output
    DimensionDynamics get_dimension_dyanmics() const {return _dimension_dynamics;};
    
    //! Public method for resizing based on dimesion. Calls _resize_output only if necessary. This method is not called at init(), and thus represets any post-init change to dimension size, such as that based on changes in input dimension size. 
    void set_dimension(FrameDimensionType d);    

    //! Return the the number of output dimensions
    FrameDimensionType get_dimension() const {return _frame_dimension;};

    //! Return the the output size
    FrameSizeType get_output_size() const {return _output_size;};
    
    //! Get the average value of all output values. 
    SampleType get_output_abs_average() const;

    //! Get the average of single dimension of output. If d is 0, all dimensions are averaged. If d is greater than the number of dimensions, and error is raised. 
    SampleType get_output_average(FrameDimensionType d) const;

    //! Return a Boolean if this Generator has resizable frame size
    bool frame_size_is_resizable() const {return _frame_size_is_resizable;};

    //! Public method for resizing based on frame size. Calls _resize_output only if necessary. 
    void set_frame_size(FrameSizeType f);    
	
    //! Return the the frame size.
    OutputSizeType get_frame_size() const {return _frame_size;};	

	//! Get the sampling rate. 
    OutputSizeType get_sampling_rate() const {return _sampling_rate;};	

	//! Get the nyquist frequency. 
    OutputSizeType get_nyquist() const {return _nyquist;};	

    //! Reset all parameters, and zero out the output array.
    virtual void reset();

	//! Output stream friend function: returns the name of the Generator. 
	friend std::ostream& operator<<(std::ostream& output, const Generator& g);
    
    //! Return the name as a string. 
    std::string get_class_name() const {return _class_name;};


	// display ................................................................    
    //! Print the output buffer for all dimensions at the current sample.
    void print_output();

    //! Print the the hierarchical list of all input values. This is virtual because Constant must print inputs in as different way. No other generator should need to specialize. 
    virtual void print_inputs(bool recursive=false, UINT8 recurse_level=0);

    //! Render the requested frame if not already rendered. This is virtual because every Generator renders in a different way. 
    virtual void render(RenderCountType f); 

	//! Create a plot at a temporary file path. Another method should take a string to write this file somewhere. 
	void plot_output_to_temp_fp(bool open=true);


	// loading/writing to output ..............................................    
	
    //! Load the output into a passed-in vector. The vector is cleared before loading. 
    void write_output_to_vector(VSampleType& vst) const;

    //! Write out all outpout to the provided file path. If this is a BufferFile, this can be used to write an audio file.
    virtual void write_output_to_fp(const std::string& fp, 
                                    FrameDimensionType d=0) const;
	
	//! Set the output from an array. 
	void set_output_from_array(SampleType* v, OutputSizeType s, 
							FrameDimensionType ch, bool interleaved=true);
								
	//! Set the output (resizing if possible) to values passsed in from a vector of SampleType. Note this presently copies values from a vector to an array, and thus requires 2x the memory alloc. 
	void set_output_from_vector(const VSampleType& vst, 
								FrameDimensionType ch, bool interleaved=true);


    //! If we are in a BufferFile class, this method loads a complete file path to an audio file into the outpout of this Generator. 
    virtual void set_output_from_fp(const std::string& fp);

	// parameter ..............................................................    
    //! Return the number of parameters; this is not the same as the number of Generators, as each parameter may have 1 or more Generators
    ParameterIndexType get_parameter_count() {
        return _input_parameter_count;};

	//! Return the parameter index for a named parameter.
    ParameterIndexType get_parameter_index_from_name(const std::string& s);

    //! Directly set a parameter given an index. This will remove/erase any multiple inputs for this parameter
    virtual void set_parameter_by_index(ParameterIndexType i, 
                                        GeneratorShared gs);
    virtual void set_parameter_by_index(ParameterIndexType i, SampleType v);

    //! Add a multiple input at this parameter. 
    virtual void add_parameter_by_index(ParameterIndexType i, 
                                        GeneratorShared gs);

    virtual void add_parameter_by_index(ParameterIndexType i, SampleType v);
  
};






//==============================================================================
//! A Generator that returns a constant value, or fills its output vector with the same vale for all frames. 
class Constant;
typedef std::tr1::shared_ptr<Constant> ConstantShared;
class Constant: public Generator {

    private://------------------------------------------------------------------
	//! Storage for the internal constant values. This is an array because we want to support a similar interface of applying multiple values to a single input parameter. 
    VSampleType _values;

    protected://----------------------------------------------------------------

    //! Virtual version here needs to return a terminal value, which is the dimensionallity of this generator. 
    virtual FrameDimensionType _find_max_input_dimension(FrameDimensionType d=1);

    public://-------------------------------------------------------------------
//    explicit Constant();
	explicit Constant(GeneratorConfigShared);
    ~Constant();
    
    virtual void init();	
    virtual void reset();
	
	//! This overridden method needs only increment the _render_count, as the output array is set when reset() is called. 
    virtual void render(RenderCountType f); 	
	
	//! This derived function is necessary to handle displaying internal input components.
	virtual void print_inputs(bool recursive=false, UINT8 recurse_level=0);
    
    //! This overridden method throws an exception: you cannot set a Generator to a constant.
    virtual void set_parameter_by_index(ParameterIndexType i, GeneratorShared gs);    
                                        
    //! Set value as a SampleType value.
	virtual void set_parameter_by_index(ParameterIndexType i, SampleType v);
    
    //! This overridden method throws an exception: you cannot set a Generator to a constant.    
    virtual void add_parameter_by_index(ParameterIndexType i, GeneratorShared gs);    
                                        
    //! Add value as a SampleType value.                                        
	virtual void add_parameter_by_index(ParameterIndexType i, SampleType v);
    
    
};

//==============================================================================
//! An adder that sums all Generators across all dimensions at its single operand input.
class Add;
typedef std::tr1::shared_ptr<Add> AddShared;
class Add: public Generator {

    private://------------------------------------------------------------------
    ParameterIndexType _input_index_opperands;    
    SampleType _sum_opperands;

    public://-------------------------------------------------------------------
    explicit Add(GeneratorConfigShared);
	
    ~Add();

    virtual void init();    
		
	//! Render addition. 
    virtual void render(RenderCountType f); 	
};




//==============================================================================
//! A BufferFile has the ability to load its output array to and from the file system. Further, the buffer generally has a larger frame size, permitting storing extended time periods in output. 
class BufferFile;
typedef std::tr1::shared_ptr<BufferFile> BufferFileShared;
class BufferFile: public Generator {

    public://-------------------------------------------------------------------
    explicit BufferFile(GeneratorConfigShared);
	
    ~BufferFile();

    virtual void init();    
		
    //! Write to an audio file to given the ouput file path. The optional FrameDimensionType argument can be used to specify a single dimension of many to write. If FrameDimensionType is 0, all dimensions are written.
    virtual void write_output_to_fp(const std::string& fp, 
                                    FrameDimensionType d=0) const;
        
    //! Set the output of this Generator to the content of an audio file. This overridden method makes the usage of libsndfile to read in a file. 
    virtual void set_output_from_fp(const std::string& fp);
        
};





//==============================================================================
//! The phasor has a ramp from 0 to 1 for each dimension defined. Only the first dimension of multiple dimensional inputs is used. 

class Phasor;
typedef std::tr1::shared_ptr<Phasor> PhasorShared;
class Phasor: public Generator {

    private://------------------------------------------------------------------
    ParameterIndexType _input_index_frequency;    
    ParameterIndexType _input_index_phase;    
	
	// TODO: these should be Vectors of size equal to frame and filled from the input
    SampleType _sum_frequency;
    SampleType _sum_phase;	
	
	// state variables used for wave calc
	//SampleType _period_seconds;
	SampleType _amp;
	SampleType _amp_prev;		
	
	RenderCountType _period_samples;		

    public://-------------------------------------------------------------------
    explicit Phasor(GeneratorConfigShared);
	
    ~Phasor();

    virtual void init();    
		
	//! Render the phasor. 
    virtual void render(RenderCountType f);
};










} // end namespace aw

#endif // ends _AW_GENERATOR_H_
