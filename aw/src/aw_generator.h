#ifndef _AW_GENERATOR_H_
#define _AW_GENERATOR_H_


#include <vector>
#include <string>
#include <utility> // has pair
#include <tr1/unordered_map>

//#include <memory> # only with -std=c++0x
// #include <boost/shared_ptr.hpp>
// <boost/enable_shared_from_this.hpp>
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
	friend std::ostream& operator<<(std::ostream& matrix, const ParameterType& pt);

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
};

// TODO: provide a RateConverter that permits specifiying values in a variety of different ways. Parameter type frequency should by usable for all duration types
class ParameterTypeFrequency;
typedef std::tr1::shared_ptr<ParameterTypeFrequency> ParameterTypeFrequencyShared;
//! A subclass of ParameterType that specifies a frequency; this is assumed presently to be in Hertz.
class ParameterTypeFrequency: public ParameterType {
    public://-------------------------------------------------------------------
    explicit ParameterTypeFrequency();
};

class ParameterTypeDuration;
typedef std::tr1::shared_ptr<ParameterTypeDuration> ParameterTypeDurationShared;
//! A subclass of ParameterType that specifies a duration in seconds.
class ParameterTypeDuration: public ParameterType {
    public://-------------------------------------------------------------------
    explicit ParameterTypeDuration();
};


// TODO: phase can be provided in floating point values or degrees; figure out how to handle this
class ParameterTypePhase;
typedef std::tr1::shared_ptr<ParameterTypePhase> ParameterTypePhaseShared;
//! A subclass of ParameterType that specifies a phase; this is assumed presently to be in floating-point values.
class ParameterTypePhase: public ParameterType {
    public://-------------------------------------------------------------------
    explicit ParameterTypePhase();
};


//! Used by some Generator to configure slots for both input and output channels, or dimensionality (frame/output count) of the internal marix. 
class ParameterTypeChannels;
typedef std::tr1::shared_ptr<ParameterTypeChannels> ParameterTypeChannelsShared;
//! A subclass of ParameterType that specifies a phase; this is assumed presently to be in floating-point values.
class ParameterTypeChannels: public ParameterType {
    public://-------------------------------------------------------------------
    explicit ParameterTypeChannels();
};









//==============================================================================   
class Generator;
typedef std::tr1::shared_ptr<Generator> GeneratorShared;
//! Generator class. Base-class of all Generators. A Generator has inputs and outputs. Inputs are a vector of vectors of Generators/ out number pairs. The number of types, and types of inputs, are defined by the mapping _input_parameter_type; the Generator inputs are stored on the _inputs VVGenShared. Multiple inputs in the same parameter position are always summed. Rendering on the Generator is stored in the matrix, a table of one frame for each output. Clients of the generator freely read from the matrix array, given the matrix itself and precalculated out_to_matrix_offset values. 
class Generator: public std::tr1::enable_shared_from_this<Generator> {

    public://-------------------------------------------------------------------    
    // public typedefs
	//! A mapping of index number
    typedef std::tr1::unordered_map<ParameterIndexType, ParameterTypeShared> MapIndexToParameterTypeShared;
    
	typedef std::vector<SampleType> VSampleType;
	typedef std::vector<VSampleType> VVSampleType;
		
    typedef std::vector<FrameSizeType> VFrameSize;
    typedef std::vector<VFrameSize> VVFrameSize;

    //! An OutputConnection is a pair formed of GeneeratorShared and an integer representing the matrix number, starting from zero, to be read. 
    typedef std::pair<GeneratorShared, MatrixSizeType> GenSharedOutPair;
    typedef std::vector<GenSharedOutPair> VGenSharedOutPair;
    typedef std::vector<VGenSharedOutPair> VVGenSharedOutPair;

    //! A vector of GeneratorShared instances used for slots. Slots do not yet need to define matrix number; generatlly assume that we use the first matrix?
    typedef std::vector<GeneratorShared> VGenShared;
    
    // enums    
    //! Enumeration of IDs for each type of generator avaialble; used in factory methods to return configure Generators. 
    enum GeneratorID {
        ID_Constant,    
        ID_Add,
        ID_BufferFile,		
        ID_Phasor,				
    };
        
		
    protected://----------------------------------------------------------------
    //! The name of the class. This is set during the class constructor by the derived class, and thus needs to be protected.
    std::string _class_name;

    private://------------------------------------------------------------------
    //! Store the number of dimensions, similar to channels, that this Generator is currently set up with. 
    OutputCountType _output_count;
    
	//! The size of each frame for each _output_count as stored in the matrix. 
    FrameSizeType _frame_size; // if changed, need to rebuild matrix	
    
	//! The _matrix_size is derived from frame _output_count times the _frame_size. 
    MatrixSizeType _matrix_size;
    
    //! Number of input parameters used by this Generator. More than one Generator can reside in each slot. 
    ParameterIndexType _input_parameter_count;    
	//! Number of slots used by this Generator. One Generator can reside in each slot. 
    ParameterIndexType _slot_parameter_count;    

	//! Store the Envrionment instance. 
    EnvironmentShared _environment;
	
    protected://----------------------------------------------------------------
	
	//! The sampling rate, taken from an Environment instance, and is passed through from a GeneraotrConfig. We store this for efficiency; not prepared to handle if this changes. 
	MatrixSizeType _sampling_rate;
	
	//! The nyquist frequency, .5 * SamplingRate; this is stored to optimize calculations that need this value.
	MatrixSizeType _nyquist;
		
    //! Define if this Generator has resizable frame size. Most generators do not have have resizable frame size; only some (like a Buffer) do.
    bool _frame_size_is_resizable; // TODO: make private?	
                            
    //! The number of renderings that have passed since the last reset. Protected because render() and reset() routines need to alter this. 
    RenderCountType _render_count;
	
    //! The main storage for ParameterTypeShared instances used as inputs. These are mapped by index value, which is the same index value in the inputs vector. This is only protected and not private so that Constant can override print_inputs.
    std::tr1::unordered_map<ParameterIndexType, 
                            ParameterTypeShared> _input_parameter_type;	
		
    //! A std::vector of vectors of GeneratorsShared / matrix id pairs that are the inputs to this function. This could be an unordered map too, but vector will have optimal performance when we know the index in advance.
    VVGenSharedOutPair _inputs;
	
    // Store and update the matrix sizes of all inputs (a vector for each parameter type, and an MatrixSizeType for each value therein); this only needs to be updated when resizing has happened in the inputs. This needs to be accessed in subclass render routines, so will be protected for now.
//    VVFrameSize _inputs_frame_size;	
    
	//! A std::vector of GeneratorsShared that are used for configuration of this Generator. 
	VGenShared _slots;
	
	//! Must define slots as ParameterTypes, meaning the same can be used for both inputs and for slots. This also means slots must be Generators. These are mapped by index value, which is the same index value in the inputs vector. Note that all slots must be filled for the generator to be used, so perhaps defaults should be provided. 
    std::tr1::unordered_map<ParameterIndexType, 
                            ParameterTypeShared> _slot_parameter_type;		
	
	//! For render call, we sum all inputs up to the common frame size available in the input and store that in a Vector of sample types.
    // TODO: possibly replace this with a raw array; problem with this is that when registering inputs we will resize for each opperation; would need to register inputs all at once to avoid destroying/creating a matrix like structure. 
	VVSampleType _summed_inputs;
	

    public://-------------------------------------------------------------------
        
    //! A linear array of samples, which may include multiple dimensions (e.g. channels) placed in series. This might be private, but for performance this is presently public: no function call is required to read from it. To read from 
    SampleType* matrix;	
    
	//! A vector of frame offsets, to be used when iterating over _output_count and incorporating non-interleaved presentation of the matrix. This must be updated after every resizing, as _output_count and frame size may have changed. This is public so that Generator clients can quickly read from matrix without a function call. 
	VFrameSizeType out_to_matrix_offset;     

    // =========================================================================
    // methods =================================================================
	
    private://------------------------------------------------------------------   	
	
    //! Resize the matrix vector. Always called during init and also by _set_output_count. Will remove an exisiting array.
    void _resize_matrix();    
	
    protected://----------------------------------------------------------------
		
    //! Called by Generators during init() to configure the input parameters found in this Generator. ParameterTypeShared instances are stored in the Generator, the _input_parameter_count is incremented, and _inputs is given a blank vector for appending to. 
    void _register_input_parameter_type(ParameterTypeShared pts);

    //! Remove all inputs; used by slots that dynamically chagne inputs and outputs; all existing inputs will remain. 
    void _clear_input_parameter_types();

	//! Called by Generators during init() to configure the slot parameters found in this Generator.
    void _register_slot_parameter_type(ParameterTypeShared pts);

    //! Call this everytime a new input has been added. Does necessary and may resize this generator. This should not be called in render(). Pre-fetches all the matrix sizes of all inputs. These are stored in _inputs_frame_size.
    void _update_for_new_input();

	//! Call this every time a new slot has been added or changed. This is necessarily virtual as subclasses need to handle their slots in their own way. 
	virtual void _update_for_new_slot();
    
    //! Call the render method on all stored inputs. This is done once for each render call; this calls each  input Generator's render() method. This means that during render() calls, _render_inputs() does not need to be called. 
    inline void _render_inputs(RenderCountType f);
	
	//! Flatten or sum multiple inputs that reside in the same input type. This is done to optimize dealing with multiple inputs in the same input type ahead of calculations for rendering. Results are stored in _summed_inputs VV. 
	inline void _sum_inputs();
    
	//! Call reset on all inputs. 
	void _reset_inputs();
    
    //! Method for resizing based on dimesion. Calls _resize_matrix only if necessary. This method is not called at init(), and thus represets any post-init change to _output_count size, such as that based on changes in input _output_count size. 
    void _set_output_count(OutputCountType d);    
    	
    //! Public method for resizing based on frame size. Calls _resize_matrix only if necessary. 
    void _set_frame_size(FrameSizeType f);    
    
    
    public://-------------------------------------------------------------------
    //! Factory for all Generators. This creates a Generator, and calls its init() method. 
    static GeneratorShared make(GeneratorID);


    public://-------------------------------------------------------------------
	//! Main constructor that takes a generator config. 
    explicit Generator(EnvironmentShared e);
    
    virtual ~Generator();
    
    //! Initialize the Generator. This method is responsible for creating ParameterTypeValueShared instances and adding them to the Generator using the _register_input_parameter_type method. This method also does the initial sizing of the Generator, and thus could raise an exception. Additional buffers that might be needed for this Generator can be stored here. As this is virtual the base-classes init is not called. 
    virtual void init();    

    //! Return a Boolean if this Generator has resizable matrix
//    DimensionDynamics get_dimension_dyanmics() const {return _dimension_dynamics;};
    

    //! Return the the number of matrix dimensions
    OutputCountType get_output_count() const {return _output_count;};

    //! Return the the matrix size, or the total number of samples used for all frames at all outputs.
    FrameSizeType get_matrix_size() const {return _matrix_size;};
    
    //! Get the average value of all matrix values. 
    SampleType get_matrix_average() const;

    //! Get the average of single _output_count of matrix. If d is 0, all dimensions are averaged. If d is greater than the number of dimensions, and error is raised. 
    SampleType get_output_average(OutputCountType d) const;

    //! Return a Boolean if this Generator has resizable frame size
    bool frame_size_is_resizable() const {return _frame_size_is_resizable;};
	
    //! Return the the frame size, the number of samples per output matrix. The frame size is always at or greater than the common frame size. 
    MatrixSizeType get_frame_size() const {return _frame_size;};	

	//! Return the common frame size derived from the stored shared Environment. If this Generator has resizable frames, this may not be the same as the current frame size. 
    MatrixSizeType get_common_frame_size() const {
			return _environment->get_common_frame_size();};	

	//! Get the sampling rate. 
    MatrixSizeType get_sampling_rate() const {return _sampling_rate;};	

	//! Get the nyquist frequency. 
    MatrixSizeType get_nyquist() const {return _nyquist;};	

    //! Reset all parameters, and zero out the matrix array.
    virtual void reset();
	
    
	// info strings .............................................................    

    //! Get the name and address for this Generator, as a single string. 
    std::string get_name_address() const;
    
    //! Get the name and addres for this Generator, labeling th ename with Gen and wrapping the address in braces. 
    std::string get_label_address() const;
    
    //! Get a complete label for this generator.
    std::string get_label() const;
    
	//! Output stream friend function: returns the name of the Generator. 
	friend std::ostream& operator<<(std::ostream& matrix, const Generator& g);
    
    //! Return the name as a string. 
    std::string get_class_name() const {return _class_name;};


	// display ................................................................    
    //! Print the matrix buffer for all dimensions at the current sample.
    void print_matrix();

    //! Print the the hierarchical list of all input values. This is virtual because Constant must print inputs in as different way. No other generator should need to specialize. 
    virtual void print_inputs(bool recursive=false, UINT8 recurse_level=0);

    //! Render the requested frame if not already rendered. This is virtual because every Generator renders in a different way. 
    virtual void render(RenderCountType f); 


    //! Plot the matrix by piping it to gnuplot using a subprocess. 
	void plot_matrix();

    //! Create a graphiz illustration. 
	void illustrate_network();


	// loading/writing to matrix ..............................................    
	
    //! Load the matrix into a passed-in vector. The vector is cleared before loading. 
    void write_matrix_to_vector(VSampleType& vst) const;

    //! Write out all outpout to the provided file path. If this is a BufferFile, this can be used to write an audio file.
    virtual void write_output_to_fp(const std::string& fp, 
                                    OutputCountType d=0) const;
	
	//! Set the matrix from an array. 
	void set_matrix_from_array(SampleType* v, MatrixSizeType s, 
							OutputCountType ch, bool interleaved=true);
								
	//! Set the matrix (resizing if possible) to values passsed in from a vector of SampleType. Note this presently copies values from a vector to an array, and thus requires 2x the memory alloc. 
	void set_matrx_from_vector(const VSampleType& vst, 
								OutputCountType ch, bool interleaved=true);


    //! If we are in a BufferFile class, this method loads a complete file path to an audio file into the outpout of this Generator. 
    virtual void set_matrix_from_fp(const std::string& fp);


	// inputs and slots ........................................................    
    //! Return the number of inputs; this is not the same as the number of Generators, as each input may have 1 or more Generators
    ParameterIndexType get_input_count() {return _input_parameter_count;};

    //! Return the number of slots. There is only one Generator per slot. 
    ParameterIndexType get_slot_count() {return _slot_parameter_count;};

	//! Return the parameter index for a named parameter.
    ParameterIndexType get_input_index_from_parameter_name(const std::string& s);

	//! Return the parameter slot for a named parameter.
    ParameterIndexType get_slot_index_from_parameter_name(const std::string& s);	


	// input ..............................................................    
    //! Directly set a parameter given an index. This will remove/erase any multiple inputs for this parameter
    virtual void set_input_by_index(ParameterIndexType i, 
                                        GeneratorShared gs);
    virtual void set_input_by_index(ParameterIndexType i, SampleType v);

    //! Add a multiple input at this parameter. 
    virtual void add_input_by_index(ParameterIndexType i, 
                                        GeneratorShared gs);

    virtual void add_input_by_index(ParameterIndexType i, SampleType v);
  
	// slot ..............................................................    	
    //! Directly set a parameter given an index. This will remove/erase any parameter on this slot. The update parameter permits disabling updating a slot, useful during initial configuration. 
    virtual void set_slot_by_index(ParameterIndexType i, GeneratorShared gs, 
									bool update=true);
    
	//! Overridden to handle a constant. 
    virtual void set_slot_by_index(ParameterIndexType i, SampleType v, 
									bool update=true);

};






//==============================================================================
//! A Generator that returns a constant value, or fills its matrix vector with the same vale for all frames. 
class Constant;
typedef std::tr1::shared_ptr<Constant> ConstantShared;
class Constant: public Generator {

    private://------------------------------------------------------------------
	//! Storage for the internal constant values. This is an array because we want to support a similar interface of applying multiple values to a single input parameter. 
    VSampleType _values;


    public://-------------------------------------------------------------------
//    explicit Constant();
	explicit Constant(EnvironmentShared);
    ~Constant();
    
    virtual void init();	
    virtual void reset();
	
	//! This overridden method needs only increment the _render_count, as the matrix array is set when reset() is called. 
    virtual void render(RenderCountType f); 	
	
	//! This derived function is necessary to handle displaying internal input components.
	virtual void print_inputs(bool recursive=false, UINT8 recurse_level=0);
    
    //! This overridden method throws an exception: you cannot set a Generator to a constant.
    virtual void set_input_by_index(ParameterIndexType i, GeneratorShared gs);    
                                        
    //! Set value as a SampleType value.
	virtual void set_input_by_index(ParameterIndexType i, SampleType v);
    
    //! This overridden method throws an exception: you cannot set a Generator to a constant.    
    virtual void add_input_by_index(ParameterIndexType i, GeneratorShared gs);    
                                        
    //! Add value as a SampleType value.                                        
	virtual void add_input_by_index(ParameterIndexType i, SampleType v);
    
    
};

//==============================================================================
//! An adder sums all Generators across all dimensions at its single operand input.
class Add;
typedef std::tr1::shared_ptr<Add> AddShared;
class Add: public Generator {

    private://------------------------------------------------------------------
    SampleType _sum_opperands;

//    protected://-----------------------------------------------------------------
	//! Overridden to apply slot settings and reset as necessary. 
	void _update_for_new_slot();

    public://-------------------------------------------------------------------
    explicit Add(EnvironmentShared);
	
    ~Add();

    virtual void init();    
		
	//! Render addition. 
    virtual void render(RenderCountType f); 	
};




//==============================================================================
//! A BufferFile has the ability to load its matrix array to and from the file system. Further, the buffer generally has a larger frame size, permitting storing extended time periods in matrix. 
class BufferFile;
typedef std::tr1::shared_ptr<BufferFile> BufferFileShared;
class BufferFile: public Generator {

    protected://-----------------------------------------------------------------
	//! Overridden to apply slot settings and reset as necessary. 
	void _update_for_new_slot();

    public://-------------------------------------------------------------------
    explicit BufferFile(EnvironmentShared);
	
    ~BufferFile();

    virtual void init();    
	
	//! Render the buffer: each render cycle must completely fille the buffer, meaning that inputs will be called more often, have a higher render number. Is this a problem? 
    virtual void render(RenderCountType f);	
			
    //! Write to an audio file to given the ouput file path. The optional OutputCountType argument can be used to specify a single _output_count of many to write. If OutputCountType is 0, all outputs are written.
    virtual void write_output_to_fp(const std::string& fp, 
                                    OutputCountType d=0) const;
        
    //! Set the matrix of this Generator to the content of an audio file. This overridden method makes the usage of libsndfile to read in a file. 
    virtual void set_matrix_from_fp(const std::string& fp);
        
};



//==============================================================================
//! A Recorder writes its inputs to a Buffer. 
/*class Recorder;*/
/*typedef std::tr1::shared_ptr<Recorder> RecorderShared;*/
/*class Recorder: public Generator {*/

    /*public://-------------------------------------------------------------------*/
    /*explicit Recorder(GeneratorConfigShared);*/
	
    /*~Recorder();*/

    /*virtual void init();    */
		
    /*//! Write the contents recoreded to an audio file. This works by calling the same method on the Buffer stored in the slot. */
    /*virtual void write_output_to_fp(const std::string& fp, */
                                    /*OutputCountType d=0) const;*/
        
	/*//! Rendering does not write to matrix, but instead writes to the slot-stored Buffer. */
    /*virtual void render(RenderCountType f);        */
/*};*/







//==============================================================================
//! The phasor has a ramp from 0 to 1 for each _output_count defined. Only the first _output_count of multiple dimensional inputs is used. 

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
    explicit Phasor(EnvironmentShared);
	
    ~Phasor();

    virtual void init();    
		
	//! Render the phasor. 
    virtual void render(RenderCountType f);
};










} // end namespace aw

#endif // ends _AW_GENERATOR_H_
