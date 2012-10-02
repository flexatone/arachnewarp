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
//! parameter types define what various input means. examples include: SampleValue (can be constant, operand), or Rate, Phase, Amplitude
class ParameterType;
typedef std::tr1::shared_ptr<ParameterType> ParameterTypeShared;
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
    void set_name(const std::string& s) {_instance_name = s;};

    //! Return the name as a string. 
    std::string get_instance_name() const {return _instance_name;};
    
};

class ParameterTypeValue;
typedef std::tr1::shared_ptr<ParameterTypeValue> ParameterTypeValueShared;
class ParameterTypeValue: public ParameterType {

    public://-------------------------------------------------------------------
    explicit ParameterTypeValue();
    virtual ~ParameterTypeValue();

};


//==============================================================================
// Dimensionality

// When adding an input to a generator, we have to look at all inputs recursively and find the amx dimensionality. If this dimensionality is greater then the current dimensionality, and 


//==============================================================================
//! The GeneratorConfig provides input parameters for Generators. If not provide, a default GeneratorConfig is used. 
class GeneratorConfig;
typedef std::tr1::shared_ptr<GeneratorConfig> GeneratorConfigShared;
class GeneratorConfig {
    public://-------------------------------------------------------------------

    static GeneratorConfigShared make_default();
    static GeneratorConfigShared make_with_dimension(FRAME_DIM_T d);
        
    GeneratorConfig();
    ~GeneratorConfig();

    FRAME_DIM_T get_init_frame_dimension() const {return _init_frame_dimension;};
    void set_init_frame_dimension(FRAME_DIM_T d) {_init_frame_dimension = d;};
    
    FRAME_SIZE_T get_frame_size() const {return _frame_size;};
    
    private://------------------------------------------------------------------
    FRAME_DIM_T _init_frame_dimension;
    FRAME_SIZE_T _frame_size; 

};



//==============================================================================
// todo: add mechanism to tag generators
class Generator;
typedef std::tr1::shared_ptr<Generator> GeneratorShared;
class Generator {
	
    public://-------------------------------------------------------------------    
    // public typedefs
    typedef std::tr1::unordered_map<PARAMETER_INDEX_T, ParameterTypeShared> MapIndexToParameterTypeShared;
    
	typedef std::vector<SAMPLE_T> VV_SAMPLE_T;
    
    typedef std::vector<GeneratorShared> V_GEN_SHARED_T;
    typedef std::vector< V_GEN_SHARED_T > VV_GEN_SHARED_T;

    typedef std::vector<FRAME_DIM_T> V_FRAME_DIM_T;
    typedef std::vector<V_FRAME_DIM_T> VV_FRAME_DIM_T;
    
    protected://----------------------------------------------------------------
    //! The name of the class. This is set during the class constructor. 
    std::string _class_name;

    //! Store the number of dimensions, similar to channels, that this Generator is currently set up with. 
    FRAME_DIM_T _output_frame_dimension;
	
	//! The size of each frame for each dimension.
    FRAME_SIZE_T _frame_size; // if changed, need to rebuild output
	
	//! The _output_size is derived from frame dimension times the frame size. 
    FRAME_SIZE_T _output_size;
    
    //! Define if this Generator has resiable outpout. Most generators have resizable output; only some (like a mono or stereo mixer) do not.
    bool _dimension_is_resizable;
        
    //! The number of frames that have passed since the last reset.
    FRAME_COUNT_T _frame_count;
    
    //! The main storage for ParameterTypeShared instances. These are mapped by index value, which is the same index value in the inputs vector. 
    std::tr1::unordered_map<PARAMETER_INDEX_T, 
                            ParameterTypeShared> _input_parameter_type;
                            
    //! Numner of inptu parameter slots used by this Generator. More than one Generator can reside in each slot. 
    PARAMETER_INDEX_T _input_parameter_count;

    //! A std::vector of vectors of GeneratorsShared that are the inputs to this function. This could be an unordered map too, but vector will have optimal performance when we know the index in advance.
    VV_GEN_SHARED_T _inputs;	

    //! Store and update the output sizes of all inputs; this only needs to be updated when resizing has happened in the inputs. 
    VV_FRAME_DIM_T _inputs_output_size;	


    public://-------------------------------------------------------------------

    //! This defines the types of generators avaialble from the factory. 
    enum GeneratorID {
        ID_Constant,    
        ID_Add,
    };
    
        
    //! A linear array of samples, which may include multiple dimensions in series. This might be private, but for performance this is presently public; when configured to run  dimensions can be stored via requests and than used as constants w/o function calls. 
    SAMPLE_T* output;


    // methods =================================================================
    protected://----------------------------------------------------------------
		
    //! Resize the output vector. Always called during init and also by set_dimension. Will remove an exisiting array. For public resizing use set_dimension().  
    void _resize_output();    

    //! Called by Generators during init() to configure the input parameters found in this Generator. ParameterTypeShared instances are stored in the Generator, the _input_parameter_count is incremented, and both _inputs and _inputs_output_size are givne a blank vector for appending to. 
    void _register_input_parameter_type(ParameterTypeShared pts);

    //! Recursively search for the max input dimension. 
    virtual FRAME_DIM_T _find_max_input_dimension(FRAME_DIM_T d=1);

    //! Call this everytime a new input has been added. Does necessary and may resize this generator. This should not be called in render(). Pre-fetches all the output sizes of all inputs. These are stored in _inputs_output_size.
    void _update_for_new_input();
    
    //! Call the render method on all stored inputs. This is done once for each render call; this calls each  input Generator's render() method. This means that during render() calls, _render_inputs() does not need to be called. 
    inline void _render_inputs(FRAME_COUNT_T f);


    public://-------------------------------------------------------------------
    //! Factory for all Generators. This creates a Generator, and calls its init() method. 
    static GeneratorShared make(GeneratorID);

    explicit Generator();
    
    virtual ~Generator();
    
    //! Initialize the Generator. This method is responsible for creating ParameterTypeValueShared instances and adding them to the Generator using the _register_input_parameter_type method. This method also does the initial sizing of the Generator, and thus could raise an exception. Additional buffers that might be needed for this Generator can be stored here. As this is virtual the base-classes init is not called. 
    virtual void init();    

    //! Return the the number of output dimensions
    bool is_resizable() const {return _dimension_is_resizable;};
    
    //! Public method for resizing. Calls _resize_output only if necessary. 
    void set_dimension(FRAME_DIM_T d);    

    //! Return the the number of output dimensions
    FRAME_DIM_T get_dimension() const {return _output_frame_dimension;};

    //! Return the the output size
    FRAME_DIM_T get_output_size() const {return _output_size;};

    //! Reset all parameters, and zero out the output array.
    virtual void reset();

	//! Output stream friend function: returns the name of the Generator. 
	friend std::ostream& operator<<(std::ostream& output, const Generator& g);


    //! Return the name as a string. 
    std::string get_class_name() const {return _class_name;};


    //! Print the output buffer for all dimensions at the current sample.
    void print_output();

    //! Print the the hierarchical list of all input values. This is virtual because Constant must print inputs in as different way. No other generator should need to specialize. 
    virtual void print_inputs(bool recursive=false, UINT8_T recurse_level=0);

    //! Render the requested frame if not already rendered. This is virtual because every Generator renders in a different way. 
    virtual void render(FRAME_COUNT_T f); 

    //! Return the number of parameters; this is not the same as the number of Generators, as each parameter may have 1 or more Generators
    PARAMETER_INDEX_T get_parameter_count() {
        return _input_parameter_count;};

	//! Return the parameter index for a named parameter.
    PARAMETER_INDEX_T get_parameter_index_from_name(const std::string& s);

    //! Directly set a parameter given an index. This will remove/erase any multiple inputs for this parameter
    virtual void set_parameter_by_index(PARAMETER_INDEX_T i, 
                                        GeneratorShared gs);
    virtual void set_parameter_by_index(PARAMETER_INDEX_T i, SAMPLE_T v);

    //! Add a multiple input at this parameter. 
    virtual void add_parameter_by_index(PARAMETER_INDEX_T i, 
                                        GeneratorShared gs);

    virtual void add_parameter_by_index(PARAMETER_INDEX_T i, SAMPLE_T v);
  
};






//==============================================================================
class Constant;
typedef std::tr1::shared_ptr<Constant> ConstantShared;
class Constant: public Generator {

    private://------------------------------------------------------------------
	//! Storage for the internal constant values. This is an array because we want to support a similar interface of applying multiple values to a single input parameter. 
    VV_SAMPLE_T _values;

    protected://----------------------------------------------------------------

    //! Virtual version here needs to return a terminal value, which is the dimensionallity of this generator. 
    virtual FRAME_DIM_T _find_max_input_dimension(FRAME_DIM_T d=1);

    public://-------------------------------------------------------------------
    explicit Constant();
    ~Constant();
    
    virtual void init();	
    virtual void reset();
	
	//! This derived function is necessary to handle displaying internal input components.
	virtual void print_inputs(bool recursive=false, UINT8_T recurse_level=0);
	
	//! This overridden method needs only increment the _frame_count, as the output array is set when reset() is called. 
    virtual void render(FRAME_COUNT_T f); 	
    
    //! This overridden method throws an exception: you cannot set a Generator to a constant.
    virtual void set_parameter_by_index(PARAMETER_INDEX_T i, 
                                        GeneratorShared gs);    
                                        
    //! Set value as a SAMPLE_T value.
	virtual void set_parameter_by_index(PARAMETER_INDEX_T i, SAMPLE_T v);
    
    //! This overridden method throws an exception: you cannot set a Generator to a constant.    
    virtual void add_parameter_by_index(PARAMETER_INDEX_T i, 
                                        GeneratorShared gs);    
                                        
    //! Add value as a SAMPLE_T value.                                        
	virtual void add_parameter_by_index(PARAMETER_INDEX_T i, SAMPLE_T v);
    
    
};

//==============================================================================
class Add;
typedef std::tr1::shared_ptr<Add> AddShared;
class Add: public Generator {

    private://------------------------------------------------------------------
    PARAMETER_INDEX_T _input_index_opperands;    
    SAMPLE_T _sum_opperands;

    protected://----------------------------------------------------------------

    public://-------------------------------------------------------------------
    explicit Add();
    ~Add();

    virtual void init();    
		
	//! Render addition. 
    virtual void render(FRAME_COUNT_T f); 	
};







} // end namespace aw

#endif // ends _AW_GENERATOR_H_
