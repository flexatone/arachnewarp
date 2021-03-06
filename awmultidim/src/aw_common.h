#ifndef _AW_COMMON_H_
#define _AW_COMMON_H_


#include <string>
#include <vector>

#include <tr1/cstdint> // has uint32_t
#include <tr1/memory>

#include <boost/filesystem.hpp>
#include <boost/exception/all.hpp>


//! @file
//! The ArachneWarp namespace, including all components of the system.
namespace aw {

//enumerations =================================================================


//typedefs =====================================================================

// define the sample format
//! The sample type is used for sample values, e.g., amplitude measurements in the output array. 
typedef double SampleType; // sample value type

// frame size type: probadbly under 10,000, 0 to 65535
// if we store complete audio files in a frame, this will need to be bigger
//! The size of a single frame (or vector), or the number of samples processed per computation cycle. This is a very large integer as we might need to accomodate loading in large audio files as a single frame. 
typedef std::tr1::uint32_t FrameSizeType;

//! Output size. Was uint16_t, but for handling files was increased to uint32_t. In general, the output is the frame size times the number of dimensions, so output is always greater than or equal to frame size. 
typedef std::tr1::uint32_t OutputSizeType;

// dimension probably never more than 200!
//! An unisigned integer describing the number of dimensions for a Generator. 
typedef std::tr1::uint8_t FrameDimensionType; 

//! A vector of frame size types. This is used for offsets into the output.
typedef std::vector<FrameSizeType> VFrameSizeType;

//! A small unsigned interger for specialized cases. 
typedef std::tr1::uint8_t UINT8; 

//! An unsigned integer for each Generator that counts the number of frames that have passed; this number needs to be very large and overflow gracefully. 
typedef std::tr1::uint64_t RenderCountType; 

//! An unisnged integer to represent the position of an input parameter. Assumed ot never have more thean 200 parameter inputs for a Generator. 
typedef std::tr1::uint8_t ParameterIndexType; 

SampleType  const PI(3.14159265358979323846264338);
SampleType const PI2(3.14159265358979323846264338*2.0);

//! We store a minimum frequency value, necessary for handling case where the frequency goes through zero and we need to shift to a non-zero value. This value (.00001) is approx 28 hours, or more than 1 day. 
SampleType  const MIN_FQ(.00001);

//! Defined for all hierarchical displays. 
std::tr1::uint8_t const INDENT_SIZE(2);


// functions ===================================================================
//! Print an arry of SampleType of size type FrameSizeType.
void print(SampleType* out, FrameSizeType size);

//! Return the users home directory as a const char pointer. This is what is returned by low-level calls, and is thus returned here to reduce creating temporary objects.
const char* get_fp_home();

//std::string gen_id_to_name(GeneratorID q);
//GeneratorID gen_name_to_id(const std::string& q);


//! Frequency values might swing through zero, or exceed Nyquist. This inline function solves this problem by returning a minimum number of zero is hit.
// note that inline functions need to be defined in the header
inline SampleType frequency_limiter(SampleType fq, SampleType nyquist) {
	// this is inlined
	fq = fq == 0 ? MIN_FQ : fq;
	fq = fq > nyquist ? nyquist : fq;
	return fq < -nyquist ? -nyquist : fq;	
}


// utility classes =============================================================

class Environment;
//! The shared Environment is always const: it cannot be changed from the outside.
typedef std::tr1::shared_ptr<const Environment> EnvironmentShared;
//! A representation of the user's enviroinment. The Environment provides file-system-related access (file paths, etc) as well as audio I/O and related hardware related configuration options. 
class Environment {
    private://-----------------------------------------------------------------
    
    //! We internally store the temp directory as a Boost filepath object. The temp directory is stored in the user's home directory. This can be overriden in the future with different. location. 
    boost::filesystem::path _temp_directory;
	
	//! Private sampling rate storage. Defaults to 44100. 
	OutputSizeType _sampling_rate;
	

    void _load_defaults();
	
    public://-------------------------------------------------------------------

    explicit Environment();
    
    ~Environment();
	
    //! Return the sampleing rate
	OutputSizeType get_sampling_rate() const {return _sampling_rate;};

	//! This returns a file path in the environment-specified temporary directory. By default this is in the user directory .arachnewaro. This returns a string for easier compatibility with clients, rather than a Boost file path
    std::string get_fp_temp(std::string name) const;

    //also need methods to return audio files from a default/or set audio dir

};









} // end namespace
#endif // ends _AW_COMMON_H_
















