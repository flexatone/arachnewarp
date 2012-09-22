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
    protected:
    std::string _name;
    // for specific instances we can define what the rate or sample value means 
    std::string _description; 

    // can also define, for a particular instance, an expected context; that is, if fq is required, this can be defined here. then, when given another generator at a different context, conversion can happen?
    public:    

    ParameterType();
    ~ParameterType();

    // not passing by const ref because making a copy?
    void set_name(std::string s) {_name = s;};
    // should this return a const by reference?
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
    public:
    typedef std::tr1::unordered_map<PARAMETER_INDEX_T, ParameterTypeShared> MapIndexToParameterTypeShared;

    protected:    
    FRAME_DIM_T _output_frame_dimension;
    FRAME_SIZE_T _output_size; // can use frame size, as is 16 bit
    FRAME_SIZE_T _frame_size; // if changed, need to rebuild output
    
    //! The number of rames that have passed since the last reset
    FRAME_COUNT_T _frame_count;
    
    // to be called in init routine to setup Generator
    void _register_input_parameter_type(ParameterTypeShared pts);

    //! Can store dictionary of paramter type classes: these can store all sorts of specialized metadata data about the inputs needed for this Generator
    std::tr1::unordered_map<PARAMETER_INDEX_T, 
                            ParameterTypeShared> _input_parameter_type;
    PARAMETER_INDEX_T _input_parameter_count;


    //! Initialize the Generator, allocating the output array.
    virtual void _init();

    //! Resize the output vector. Always called at creation; can be called when inputs are added
    void _resize_output();

    public:
    
    //! A std::vector if GeneratorsShared that are the inputs to this function. This remains public so that a client of this Generator can access the input directly form the vector at constant time without a function call. This could be an unordered map too, but vector will have optimal performance.
    std::vector<GeneratorShared> inputs;

    //! TODO: may need inputs_required, inputs_free
    // some gens, like operators, might have any number of inputs
    // or: each required input is actually itself a vector of inputs
    // meaning that every required input (of a certain parameter type) can have multiple sub-inputs
    // like this: std::vector<std::vector<GeneratorShared>> inputs;

    
    //! A linear array of samples, which may include multiple dimensions in series. This probably should be private, but for performance this is presently public; when configured to run  dimensions can be stored via requests and than used as constants w/o function calls. 
    SAMPLE_T* output;


    Generator();
    virtual ~Generator();

    //! Reset all parameters, and zero out the output array.
    void reset();
    
    //! Print the output buffer for all dimensions at the current sample.
    void print_output();

    //! Render the requested frame if not already rendered
    // should this be passed by const ref?
    void render(FRAME_COUNT_T f); 

    PARAMETER_INDEX_T get_parameter_count() {
        return _input_parameter_count;};

    PARAMETER_INDEX_T get_parameter_index_from_name(const std::string& s);

    //! directly set a parameter given an index
    void set_parameter_by_index(PARAMETER_INDEX_T i, GeneratorShared gs);
  
};


//==============================================================================
class Constant;
typedef std::tr1::shared_ptr<Constant> ConstantShared;
class Constant: public Generator {
    protected:
    virtual void _init();

    public:
    Constant();
    ~Constant();


};





} // end namespace aw

#endif // ends _AW_GENERATOR_H_
