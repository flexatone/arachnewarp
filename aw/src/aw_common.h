#ifndef _AW_COMMON_H_
#define _AW_COMMON_H_


#include <string>
#include <vector>

#include <tr1/cstdint> // has uint32_t
#include <tr1/memory>

#include <boost/filesystem.hpp>
#include <boost/exception/all.hpp>



namespace aw {

//enumerations =================================================================




//typedefs =====================================================================

// define the sample format
//! The sample type is used for sample values, e.g., amplitude measurements. 
typedef double SampleType; // sample value type

// frame size type: probadbly under 10,000, 0 to 65535
// if we store complete audio files in a frame, this will need to be bigger
//! The size of a single frame (or vector), or the number of samples processed per computation cycle. This is a very large integers as we might need to accomodate loading in large audio files. 
typedef std::tr1::uint32_t FrameSizeType;

//! Output size. Was uint16_t, but for handling files was increased to uint32_t. In general, the output is the frame size times the number of dimensions, so output is always greater than or equal to frame size. 
typedef std::tr1::uint32_t OutputSizeType;

// dimension probably never more than 200!
//! The number of dimensions for a Generator. 
typedef std::tr1::uint8_t FrameDimensionType; 

//! A vector of frame dimension types. This is used for offsets into the output.
typedef std::vector<FrameSizeType> VFrameSizeType;

//! A small unsigned interger for specialized cases. 
typedef std::tr1::uint8_t UINT8; 

//! Each Generator counts the number of frames that have passed; this number needs to be very large and overflow gracefully. 
typedef std::tr1::uint64_t RenderCountType; 

// never have more thean 200 parameter types
typedef std::tr1::uint8_t ParameterIndexType; 

SampleType  const PI(3.14159265358979323846264338);
SampleType const PI2(3.14159265358979323846264338*2.0);

//! Defined for all hierarchical displays. 
std::tr1::uint8_t const INDENT_SIZE(2);


// functions ===================================================================
//! Print an arry of SampleType of size type FrameSizeType.
void print(SampleType* out, FrameSizeType size);

//! Return the users home directory as a const char pointer. This is what is returned by low-level calls, and is thus returned here to reduce creating temporary objects.
const char* get_fp_home();

//std::string gen_id_to_name(GeneratorID q);
//GeneratorID gen_name_to_id(const std::string& q);




// utility classes =============================================================

class Environment;

//! The shared Environment is always const: it cannot be changed from the outside.
typedef std::tr1::shared_ptr<const Environment> EnvironmentShared;
//! A representation of the users enviroinment. Presently this uses boost
class Environment {
    private://-----------------------------------------------------------------
    
    //! Store the temp directory
    //std::string _temp_directory;
    boost::filesystem::path _temp_directory ;   

    void _load_defaults();
	
    public://-------------------------------------------------------------------

    explicit Environment();
    
    ~Environment();

    //! Return a file path for a plot in the user's temporary directory. 
    std::string get_fp_plot(std::string name="plot.plt") const;

    //also need methods to return audio files from a default audio dir

};









} // end namespace
#endif // ends _AW_COMMON_H_
















