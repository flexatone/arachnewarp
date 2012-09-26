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
// parameter types define what various input means
// examples include: SampleValue (can be constant, operand)
// Rate, Phase, Amplitude
// 
class ParameterType;
typedef std::tr1::shared_ptr<ParameterType> ParameterTypeShared;
class ParameterType {
    protected://----------------------------------------------------------------
    std::string _name;
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
    void set_name(const std::string& s) {_name = s;};

    //! Return the name as a string. 
    std::string get_name() {return _name;};
    
};

class ParameterTypeValue;
typedef std::tr1::shared_ptr<ParameterTypeValue> ParameterTypeValueShared;
class ParameterTypeValue: public ParameterType {
};



//==============================================================================
// todo: add mechanism to tag generators
class Generator;
typedef std::tr1::shared_ptr<Generator> GeneratorShared;
class Generator {
	
    protected://----------------------------------------------------------------
    std::string _name;

    FRAME_DIM_T _output_frame_dimension;
    FRAME_SIZE_T _output_size; // can use frame size, as is 16 bit
    FRAME_SIZE_T _frame_size; // if changed, need to rebuild output
    
    //! The number of frames that have passed since the last reset.
    FRAME_COUNT_T _frame_count;
    
    //! The main storage for ParameterTypeShared instances. These are mapped by index value, which is the same index value in the inputs vector. 
    std::tr1::unordered_map<PARAMETER_INDEX_T, 
                            ParameterTypeShared> _input_parameter_type;
    PARAMETER_INDEX_T _input_parameter_count;


    public://-------------------------------------------------------------------
    typedef std::tr1::unordered_map<PARAMETER_INDEX_T, ParameterTypeShared> MapIndexToParameterTypeShared;

	typedef std::vector<SAMPLE_T> VSample;
    typedef std::vector<GeneratorShared> VGeneratorShared;
    typedef std::vector< VGeneratorShared > VVGeneratorShared;



    // this means that inputs are not the same as parameters; each parameter can have multiple inputs
    
    //! A linear array of samples, which may include multiple dimensions in series. This probably should be private, but for performance this is presently public; when configured to run  dimensions can be stored via requests and than used as constants w/o function calls. 
    SAMPLE_T* output;


    // methods ================================================================
    protected://----------------------------------------------------------------
	
    //! A std::vector of vectors of GeneratorsShared that are the inputs to this function. This could be an unordered map too, but vector will have optimal performance when we know the index in advance.
    VVGeneratorShared _inputs;	
	
    //! Initialize the Generator. This method is responsible for creating ParameterTypeValueShared instances and adding them to the Generator using the _register_input_parameter_type method. 
    virtual void _init();

    //! Resize the output vector. Always called at creation; can be called when inputs are added that have a different output size and folddown is set to a fixed value. 
    void _resize_output();

    // Called by Generators during _init() to configure the input parameters found in this Generator. ParameterTypeShared instances are stored in the Generator and the _input_parameter_count is incremented. 
    void _register_input_parameter_type(ParameterTypeShared pts);


    public://-------------------------------------------------------------------
    explicit Generator();
    virtual ~Generator();

    //! Reset all parameters, and zero out the output array.
    virtual void reset();

	friend std::ostream& operator<<(std::ostream& output, const Generator& g);

    //! Print the output buffer for all dimensions at the current sample.
    void print_output();

    //! Print the the hierarchical list of all input values. This is virtual because Constant must print inputs in as different way. No other generator should need to specialize. 
    virtual void print_inputs();

    //! Render the requested frame if not already rendered. This is virtual because every Generator renders in a different way. 
    virtual void render(FRAME_COUNT_T f); 

    //! Return the number of parameters; this is not the same as the number of Generators, as each parameter may have 1 or more Generators
    PARAMETER_INDEX_T get_parameter_count() {
        return _input_parameter_count;};

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
    VSample _values;

    protected://----------------------------------------------------------------
    virtual void _init();

    public://-------------------------------------------------------------------
    explicit Constant();
    ~Constant();
	
    virtual void reset();
	
	//! This derived function is necessary to handle displaying internal input components.
	virtual void print_inputs();
	
	//! This overridden method needs only increment the _frame_count, as the output array is set when reset() is called. 
    virtual void render(FRAME_COUNT_T f); 	
	virtual void set_parameter_by_index(PARAMETER_INDEX_T i, SAMPLE_T v);
	virtual void add_parameter_by_index(PARAMETER_INDEX_T i, SAMPLE_T v);
};

//==============================================================================
class Add;
typedef std::tr1::shared_ptr<Add> AddShared;
class Add: public Generator {

    private://------------------------------------------------------------------
    PARAMETER_INDEX_T _input_index_num_value;    
    SAMPLE_T _sum;


    protected://----------------------------------------------------------------
    virtual void _init();

    public://-------------------------------------------------------------------
    explicit Add();
    ~Add();
		
	//! Render addition. 
    virtual void render(FRAME_COUNT_T f); 	
};





} // end namespace aw

#endif // ends _AW_GENERATOR_H_
