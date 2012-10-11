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

// After adding an input to a generator, we have to look at all inputs attached to that generator (recursively) and find the max dimensionality. If this dimensionality is greater then the current dimensionality of the Generator, and the Generator is_resizable == true, then resize to the maximum size. Otherwise, keep at current size. The render method will have to take into account having higher dimensionality inputs
// Case 1: Create Add@2 and add operator inputs Constant@4 and Constant@3; Add should automatically resize to Add@4, as addition should not mix dimensionalities.
// Case 2: Create MixMono



//==============================================================================
//! The GeneratorConfig provides input parameters for Generators. If not provide, a default GeneratorConfig is used. 
class GeneratorConfig;
typedef std::tr1::shared_ptr<GeneratorConfig> GeneratorConfigShared;
class GeneratorConfig {
    public://-------------------------------------------------------------------

	// Static factory methods
    static GeneratorConfigShared make_default();
    static GeneratorConfigShared make_with_dimension(FrameDimensionType d);
        
	//! Constructor
    explicit GeneratorConfig();
	
	//! Deconstructor
    ~GeneratorConfig();

    FrameDimensionType get_init_frame_dimension() const {
		return _init_frame_dimension;};

    void set_init_frame_dimension(FrameDimensionType d) {
		_init_frame_dimension = d;};
    
    FrameSizeType get_frame_size() const {return _frame_size;};
    
    private://------------------------------------------------------------------
    FrameDimensionType _init_frame_dimension;
    FrameSizeType _frame_size; 

};


//==============================================================================
// forward declarations
class Generator;
typedef std::tr1::shared_ptr<Generator> GeneratorShared;


//==============================================================================
// GeneratorBase

////! This base class is used to initialize common attributes without duplicating code in different constructors. 
//class GeneratorBase {
//	
//    public://-------------------------------------------------------------------    
//    // public typedefs
//    typedef std::tr1::unordered_map<ParameterIndexType, ParameterTypeShared> MapIndexToParameterTypeShared;
//    
//	typedef std::vector<SampleType> VSampleType;
//	
//    typedef std::vector<GeneratorShared> VGenShared;
//    typedef std::vector< VGenShared > VVGenShared;
//	
//    typedef std::vector<FrameDimensionType> VFrameDimension;
//    typedef std::vector<VFrameDimension> VVFrameDimension;
//	
//    protected://----------------------------------------------------------------
//    //! The name of the class. This is set during the class constructor. 
//    std::string _class_name;
//
//    //! Store the number of dimensions, similar to channels, that this Generator is currently set up with. 
//    FrameDimensionType _output_frame_dimension;
//	
//	//! The size of each frame for each dimension.
//    FrameSizeType _frame_size; // if changed, need to rebuild output
//	
//	//! The _output_size is derived from frame dimension times the frame size. 
//    FrameSizeType _output_size;
//    
//    //! Define if this Generator has resiable outpout. Most generators have resizable output; only some (like a mono or stereo mixer) do not.
//    bool _dimension_is_resizable;
//        
//    //! The number of frames that have passed since the last reset.
//    FrameCountType _frame_count;
//    
//    //! Numner of inptu parameter slots used by this Generator. More than one Generator can reside in each slot. 
//    ParameterIndexType _input_parameter_count;
//    
//    //! Store and update the output sizes of all inputs; this only needs to be updated when resizing has happened in the inputs. 
//    VVFrameDimension _inputs_output_size;	
//    
//    //! The main storage for ParameterTypeShared instances. These are mapped by index value, which is the same index value in the inputs vector. 
//    std::tr1::unordered_map<ParameterIndexType, 
//                            ParameterTypeShared> _input_parameter_type;
//                            
//    //! A std::vector of vectors of GeneratorsShared that are the inputs to this function. This could be an unordered map too, but vector will have optimal performance when we know the index in advance.
//    VVGenShared _inputs;	
//
//    public://-------------------------------------------------------------------
//
//    //! This defines the types of generators avaialble from the factory. 
//    enum GeneratorID {
//        ID_Constant,    
//        ID_Add,
//    };
//        
//    //! A linear array of samples, which may include multiple dimensions in series. This might be private, but for performance this is presently public; when configured to run  dimensions can be stored via requests and than used as constants w/o function calls. 
//    SampleType* output;	
//	
//
//    public://-------------------------------------------------------------------    
//	//! Constructor. Only initializes variables.
//	GeneratorBase();
//	
//	//! Deconstructor. Virtual. 
//	virtual ~GeneratorBase();
//};
//


//==============================================================================
// Generator

class Generator {

    public://-------------------------------------------------------------------    
    // public typedefs
    typedef std::tr1::unordered_map<ParameterIndexType, ParameterTypeShared> MapIndexToParameterTypeShared;
    
	typedef std::vector<SampleType> VSampleType;
	
    typedef std::vector<GeneratorShared> VGenShared;
    typedef std::vector< VGenShared > VVGenShared;
	
    typedef std::vector<FrameDimensionType> VFrameDimension;
    typedef std::vector<VFrameDimension> VVFrameDimension;
	
    protected://----------------------------------------------------------------
    //! The name of the class. This is set during the class constructor. 
    std::string _class_name;

    //! Store the number of dimensions, similar to channels, that this Generator is currently set up with. 
    FrameDimensionType _output_frame_dimension;
	
	//! The size of each frame for each dimension.
    FrameSizeType _frame_size; // if changed, need to rebuild output
	
	//! The _output_size is derived from frame dimension times the frame size. 
    FrameSizeType _output_size;
    
    //! Define if this Generator has resiable outpout. Most generators have resizable output; only some (like a mono or stereo mixer) do not.
    bool _dimension_is_resizable;
        
    //! The number of frames that have passed since the last reset.
    FrameCountType _frame_count;
    
    //! Numner of inptu parameter slots used by this Generator. More than one Generator can reside in each slot. 
    ParameterIndexType _input_parameter_count;
    
    //! Store and update the output sizes of all inputs; this only needs to be updated when resizing has happened in the inputs. 
    VVFrameDimension _inputs_output_size;	
    
    //! The main storage for ParameterTypeShared instances. These are mapped by index value, which is the same index value in the inputs vector. 
    std::tr1::unordered_map<ParameterIndexType, 
                            ParameterTypeShared> _input_parameter_type;
                            
    //! A std::vector of vectors of GeneratorsShared that are the inputs to this function. This could be an unordered map too, but vector will have optimal performance when we know the index in advance.
    VVGenShared _inputs;	

    public://-------------------------------------------------------------------

    //! This defines the types of generators avaialble from the factory. 
    enum GeneratorID {
        ID_Constant,    
        ID_Add,
    };
        
    //! A linear array of samples, which may include multiple dimensions in series. This might be private, but for performance this is presently public; when configured to run  dimensions can be stored via requests and than used as constants w/o function calls. 
    SampleType* output;	
    

    private://------------------------------------------------------------------   
	//! Store the GeneratorConfig instance here; this is the only data attributes stored here and not in GeneratorBase. 
    GeneratorConfigShared _generator_config;

    // methods =================================================================
    protected://----------------------------------------------------------------
		
    //! Resize the output vector. Always called during init and also by set_dimension. Will remove an exisiting array. For public resizing use set_dimension(). Will not reset.  
    void _resize_output();    

    //! Called by Generators during init() to configure the input parameters found in this Generator. ParameterTypeShared instances are stored in the Generator, the _input_parameter_count is incremented, and both _inputs and _inputs_output_size are givne a blank vector for appending to. 
    void _register_input_parameter_type(ParameterTypeShared pts);

    //! Recursively search for the max input dimension. 
    virtual FrameDimensionType _find_max_input_dimension(FrameDimensionType d=1);

    //! Call this everytime a new input has been added. Does necessary and may resize this generator. This should not be called in render(). Pre-fetches all the output sizes of all inputs. These are stored in _inputs_output_size.
    void _update_for_new_input();
    
    //! Call the render method on all stored inputs. This is done once for each render call; this calls each  input Generator's render() method. This means that during render() calls, _render_inputs() does not need to be called. 
    inline void _render_inputs(FrameCountType f);


    public://-------------------------------------------------------------------
	//! Factory for all Generators that creates a  Generator with a dimension. Calls init() method. 
    static GeneratorShared make_with_dimension(GeneratorID, FrameDimensionType);
	
    //! Factory for all Generators. This creates a Generator, and calls its init() method. 
    static GeneratorShared make(GeneratorID);

	//! Default constructor
//    explicit Generator();

	//! Main constructor that takes a generator config. 
    explicit Generator(GeneratorConfigShared gc);
    
    virtual ~Generator();
    
    //! Initialize the Generator. This method is responsible for creating ParameterTypeValueShared instances and adding them to the Generator using the _register_input_parameter_type method. This method also does the initial sizing of the Generator, and thus could raise an exception. Additional buffers that might be needed for this Generator can be stored here. As this is virtual the base-classes init is not called. 
    virtual void init();    

    //! Return the the number of output dimensions
    bool is_resizable() const {return _dimension_is_resizable;};
    
    //! Public method for resizing. Calls _resize_output only if necessary. 
    void set_dimension(FrameDimensionType d);    

    //! Return the the number of output dimensions
    FrameDimensionType get_dimension() const {return _output_frame_dimension;};

    //! Return the the output size
    FrameSizeType get_output_size() const {return _output_size;};

    //! Reset all parameters, and zero out the output array.
    virtual void reset();

	//! Output stream friend function: returns the name of the Generator. 
	friend std::ostream& operator<<(std::ostream& output, const Generator& g);


    //! Return the name as a string. 
    std::string get_class_name() const {return _class_name;};


    //! Print the output buffer for all dimensions at the current sample.
    void print_output();

    //! Print the the hierarchical list of all input values. This is virtual because Constant must print inputs in as different way. No other generator should need to specialize. 
    virtual void print_inputs(bool recursive=false, UINT8 recurse_level=0);

    //! Render the requested frame if not already rendered. This is virtual because every Generator renders in a different way. 
    virtual void render(FrameCountType f); 

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
	
	//! This derived function is necessary to handle displaying internal input components.
	virtual void print_inputs(bool recursive=false, UINT8 recurse_level=0);
	
	//! This overridden method needs only increment the _frame_count, as the output array is set when reset() is called. 
    virtual void render(FrameCountType f); 	
    
    //! This overridden method throws an exception: you cannot set a Generator to a constant.
    virtual void set_parameter_by_index(ParameterIndexType i, 
                                        GeneratorShared gs);    
                                        
    //! Set value as a SampleType value.
	virtual void set_parameter_by_index(ParameterIndexType i, SampleType v);
    
    //! This overridden method throws an exception: you cannot set a Generator to a constant.    
    virtual void add_parameter_by_index(ParameterIndexType i, 
                                        GeneratorShared gs);    
                                        
    //! Add value as a SampleType value.                                        
	virtual void add_parameter_by_index(ParameterIndexType i, SampleType v);
    
    
};

//==============================================================================
class Add;
typedef std::tr1::shared_ptr<Add> AddShared;
class Add: public Generator {

    private://------------------------------------------------------------------
    ParameterIndexType _input_index_opperands;    
    SampleType _sum_opperands;

    protected://----------------------------------------------------------------

    public://-------------------------------------------------------------------
//    explicit Add();
    explicit Add(GeneratorConfigShared);
	
    ~Add();

    virtual void init();    
		
	//! Render addition. 
    virtual void render(FrameCountType f); 	
};







} // end namespace aw

#endif // ends _AW_GENERATOR_H_
