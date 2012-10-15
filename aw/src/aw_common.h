#ifndef _AW_COMMON_H_
#define _AW_COMMON_H_


#include <string>
#include <vector>

#include <tr1/cstdint> // has uint32_t
#include <tr1/memory>

#include <boost/filesystem.hpp>



namespace aw {

//enumerations =================================================================




//typedefs =====================================================================

// define the sample format
typedef double SampleType; // sample value type

// frame size type: probadbly under 10,000, 0 to 65535
// if we store complete audio files in a frame, this will need to be bigger
typedef std::tr1::uint16_t FrameSizeType;

//! Output size. May need to be larger if we accept file sizes. 
typedef std::tr1::uint16_t OutputSizeType;

// dimension probably never more than 200!
typedef std::tr1::uint8_t FrameDimensionType; 

//! A small unsigned interger forf specialized cases. 
typedef std::tr1::uint8_t UINT8; 

//! Each Generator counts the number of frames that have passed; this number needs to be very large and overflow gracefully. 
typedef std::tr1::uint64_t FrameCountType; 

// never have more thean 200 parameter types
typedef std::tr1::uint8_t ParameterIndexType; 

SampleType  const PI(3.14159265358979323846264338);
SampleType const PI2(3.14159265358979323846264338*2.0);

//! Defined for all hierarchical displays. 
std::tr1::uint8_t const INDENT_SIZE(2);


// functions ===================================================================
//! Print an arry of SampleType of size type FrameSizeType.
void print(SampleType* out, FrameSizeType size);


boost::filesystem::path get_fp_home();

//std::string gen_id_to_name(GeneratorID q);
//GeneratorID gen_name_to_id(const std::string& q);




// utility classes =============================================================

class Environment;
typedef std::tr1::shared_ptr<Environment> EnvironmentShared;
//! A representation of the users enviroinment
class Environment {
    private://-----------------------------------------------------------------
    
    //! Store the temp directory
    //std::string _temp_directory;
    boost::filesystem::path _temp_directory ;   

    void _load_defaults();
	
    public://-------------------------------------------------------------------

    explicit Environment();
    ~Environment();

    boost::filesystem::path get_fp_plot(std::string name="plot.plt") const;


};









} // end namespace
#endif // ends _AW_COMMON_H_
















