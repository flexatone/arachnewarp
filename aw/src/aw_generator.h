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

    protected:    
    std::string _name;

    FRAME_DIM_T _output_frame_dimension;
    FRAME_SIZE_T _output_size; // can use frame size, as is 16 bit
    FRAME_SIZE_T _frame_size; // if changed, need to rebuild output
    
    //! The number of rames that have passed since the last reset
    FRAME_COUNT_T _frame_count;
    
    //! Can store dictionary of paramter type classes: these can store all sorts of specialized metadata data about the inputs needed for this Generator
    std::tr1::unordered_map<PARAMETER_INDEX_T, 
                            ParameterTypeShared> _input_parameter_type;
    PARAMETER_INDEX_T _input_parameter_count;


    public:
    typedef std::tr1::unordered_map<PARAMETER_INDEX_T, ParameterTypeShared> MapIndexToParameterTypeShared;

    typedef std::vector<GeneratorShared> VGeneratorShared;
    typedef std::vector< VGeneratorShared > VVGeneratorShared;

    //! A std::vector if GeneratorsShared that are the inputs to this function. This remains public so that a client of this Generator can access the input directly form the vector at constant time without a function call. This could be an unordered map too, but vector will have optimal performance when we know the index in advance.
    // std::vector<GeneratorShared> inputs;

    // or: each required input is actually itself a vector of inputs, meaning that every required input (of a certain parameter type) can have multiple sub-inputs. The inner iterator may be better as a list.
    VVGeneratorShared inputs;

    // this means that inputs are not the same as parameters; each parameter can have multiple inputs
    
    //! A linear array of samples, which may include multiple dimensions in series. This probably should be private, but for performance this is presently public; when configured to run  dimensions can be stored via requests and than used as constants w/o function calls. 
    // This could be a vector, but optimal performance, no need for sorting, suggests using an dyn alloc array for now.
    SAMPLE_T* output;


    // methods ================================================================
    protected:
    //! Initialize the Generator, allocating the output array.
    virtual void _init();

    //! Resize the output vector. Always called at creation; can be called when inputs are added
    void _resize_output();

    // to be called in init routine to setup Generator
    void _register_input_parameter_type(ParameterTypeShared pts);


    public:
    Generator();
    virtual ~Generator();

    //! Reset all parameters, and zero out the output array.
    virtual void reset();

	friend std::ostream &operator<<(std::ostream& output, const Generator& g);

    //! Print the output buffer for all dimensions at the current sample.
    void print_output();

    //! Print the the hierarchical list of all input values.
    void print_inputs();

    //! Render the requested frame if not already rendered
    // should this be passed by const ref?
    void render(FRAME_COUNT_T f); 

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

    private:
    SAMPLE_T _value;

    protected:
    virtual void _init();

    public:
    Constant();
    ~Constant();


};





} // end namespace aw

#endif // ends _AW_GENERATOR_H_
