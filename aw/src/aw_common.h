#ifndef _AW_COMMON_H_
#define _AW_COMMON_H_

#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cstdint> // has uint32_t
#include <memory>
#include <unordered_map>
#include <initializer_list>

#include <boost/filesystem.hpp>
#include <boost/exception/all.hpp> // needed for filesystem?

//! @file
//! The ArachneWarp namespace, including all components of the system.
namespace aw {

//enumerations =================================================================

//typedefs =====================================================================

// define the sample format
//! The sample type is used for sample values, e.g., amplitude measurements in the output vector. 
typedef double SampleType; // sample value type

//! One dimensional initializer list of SampleTypes.
typedef std::initializer_list<SampleType> ILSampleType;

//! Two dimensional initializer list of SampleTypes.
typedef std::initializer_list<ILSampleType> ILILSampleType;

//! One dimensional vector of samples.
typedef std::vector<SampleType> VSampleType;

//! Two dimensinoal vector of samples. 
typedef std::vector<VSampleType> VVSampleType;

    
//! The size of a single frame (or vector), or the number of samples processed per computation cycle or stored in a single channel of output data. This is a very large integer as we might need to accomodate loading in large audio files as a single frame.
typedef std::uint32_t FrameSizeType;

//! Output size. Was uint16_t, but for handling audio files was increased to uint32_t. In general, the outputs is the frame size times the number of outputs, so outputs is always greater than or equal to frame size. 
typedef std::uint32_t OutputsSizeT;


//! An unsigned integer to represent the position of a parameter type, i.e., an input or output position. Assumed to never have more thean 200 parameter inputs for a Generator.
typedef std::size_t ParameterIndexType; 


typedef std::vector<ParameterIndexType> VParameterIndexT;

//! A vector of frame size types. This is used for offsets into the outputs.
typedef std::vector<FrameSizeType> VFrameSizeType;

//! A small unsigned interger for specialized cases. 
typedef std::uint8_t UINT8; 

//! An unsigned integer for each Generator that counts the number of frames that have passed; this number needs to be very large and overflow gracefully. 
typedef std::uint64_t RenderCountType; 



// TODO: replace with a SharedGenerator: but, we need a SharedGenerator to be hashable, which requires us to extend std::hash or similar
//! A mapping of a String to bool. 
typedef std::unordered_map<std::string, bool> MapStringBool;

//! The shared pointer version of MapStringParameterIndexPairToBool.
typedef std::shared_ptr<MapStringBool> SharedMapStringBool;


SampleType const PI(3.14159265358979323846264338);
SampleType const PI2(3.14159265358979323846264338 * 2.0);
SampleType const LOGTWO(0.69314718055994528623);
SampleType const LOGTEN(2.302585092994);

//! We store a minimum frequency value, necessary for handling case where the frequency goes through zero and we need to shift to a non-zero value. This value (.00001) is approx 28 hours, or more than 1 day. 
SampleType const MIN_FQ(.00001);

// -120 dB, or pow(10, -120/20), or 1e-06
SampleType const MIN_AMP(.000001);


//! The trigger threshold, or the value aboive which we determine that we have a trigger.
SampleType const TRIG_THRESH(.99999);


//! Defined for all text-based hierarchical displays. 
std::uint8_t const INDENT_SIZE(2);


// functions ===================================================================

//! Escape one or more characters privided by a string and a single prefix. Changes are made in place to the passed in string. 
void escape(std::string& str, const std::string& replace_targets, 
        const std::string& prefix);


//! Print an arry of SampleType of size type FrameSizeType.
void print(SampleType* out, FrameSizeType size);

//! Return the users home directory as a const char pointer. This is what is returned by low-level calls, and is thus returned here to reduce creating temporary objects.
const char* get_fp_home();


//! Frequency values might swing through zero, or exceed Nyquist. This inline function solves this problem by returning a minimum number of zero is hit.
// note that inline functions need to be defined in the header
inline SampleType frequency_limiter(SampleType fq, SampleType nyquist) {
	// this is inlined
	fq = fq == 0 ? MIN_FQ : fq;
	fq = fq > nyquist ? nyquist : fq;
	return fq < -nyquist ? -nyquist : fq;
}

//! Limit a phase value between 0 and PI2 by wrapping: done in place. 
inline void phase_limiter(SampleType& phase) {
    while (phase >= PI2) {
        phase -= PI2;
    }
    while (phase < 0.0) {
        phase += PI2;
    }
}


inline SampleType double_limiter(SampleType src, SampleType min,
        SampleType max) {
    src = src < min ? min : src;
    return src > max ? max : src;
}



//! Detect and assign true min / max, based on comparison. Min and max are set by passing in pointers to to the value that are set in place.
inline void true_min_max(
        SampleType lower,
        SampleType upper,
        SampleType* min,
        SampleType* max) {

    if (upper < lower){
        *min = upper;
        *max = lower;
    }
    else { // if equal nothing happens
        *min = lower;
        *max = upper;
    }
}



//! Convert midi to frequency. 	
inline SampleType mtof(SampleType f) {
    // midi to frequency
    if (f <= -1500) return 0;
    else if (f > 1499) return mtof(1499);
    else return pow(2, (f-69) / 12.0) * 440.0;
}



// string procs ===============================================================


//! Convert a string into lower case in place. 
inline void to_lower(std::string& src) {
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
}

//! Remove characters from a string passed in place.
inline void remove(std::string& src, const char target) {
    src.erase(std::remove(src.begin(), src.end(), target), src.end());
}


//! Split a string by a delimiter. 
inline void split(
        const std::string& src,
        const char delim,
        std::vector<std::string>& post) {
    std::stringstream ss(src);
    std::string item;
    while (std::getline(ss, item, delim)) {
        post.push_back(item);
    }

}

//! A templated function that will take a string and convert to a vector of type T, using istreingstream conversion.
template <typename T>
void string_to_vector(
        const std::string& src, // input
        std::vector<T>& post, // return collector
        const char delim=',',
        const std::string& remove="") {

    std::vector<std::string> col; // need a temp col of strings
    std::vector<std::string>::const_iterator i;
    
    T var;
    std::istringstream iss;
    std::string working(src); // copy to permit changing in place

    for (std::string::const_iterator j=remove.begin(); j != remove.end(); ++j) {
        aw::remove(working, *j);
    }
    // split in place by delim into col
    aw::split(working, delim, col);
    // for each part of col, make into type T
    for(i=col.begin(); i!=col.end(); ++i) {
        iss.clear(); // clear it on each pass
        iss.str(*i);
        iss >> var;
        post.push_back(var);
    }
}








    

// taken from pd/chuck; need to update types, and integerate as necessar
//-----------------------------------------------------------------------------
//double round( double a )
//{
//    if( a >= 0 ) return (double)(long)( a + .5 );
//    else return (double)(long)( a - .5 );
//}
//
//
//double trunc( double a )
//{
//    return (double)(long)a;
//}
//
//
//double remainder( long a, long b )
//{
//    long div = a/b;
//    return a - b*div;
//}
//
//
//double ftom( double f )
//{   
//    // return (f > 0 ? 17.3123405046 * log(.12231220585 * f) : -1500);
//    // TODO: optimize
//    return (f > 0 ? (log(f/440.0) / LOGTWO) * 12.0 + 69 : -1500);
//}
//
//
//double powtodb( double f )
//{
//    if( f <= 0 ) return (0);
//    else
//    {
//        double val = 100 + 10./LOGTEN * log(f);
//        return (val < 0 ? 0 : val);
//    }
//}
//
//
//double rmstodb( double f )
//{
//    if( f <= 0 ) return (0);
//    else
//    {
//        double val = 100 + 20./LOGTEN * log(f);
//        return (val < 0 ? 0 : val);
//    }
//}
//
//
//double dbtopow( double f )
//{
//    if( f <= 0 )
//        return (0);
//    else
//    {
//        if( f > 870 ) f = 870;
//        return (exp((LOGTEN * 0.1) * (f-100.)));
//    }
//}
//
//
//double dbtorms( double f )
//{
//    if( f <= 0 )
//        return (0);
//    else
//    {
//        if( f > 485 ) f = 485;
//        return (exp((LOGTEN * 0.05) * (f-100.)));
//    }
//}
//
//unsigned long nextpow2( unsigned long n )
//{
//    unsigned long nn = n;
//    for( ; n &= n-1; nn = n );
//    return nn * 2;
//}
//
//unsigned long ensurepow2( unsigned long n )
//{
//    return nextpow2( n-1 );
//}
//
//



// utility classes =============================================================

class Environment;
//! The shared Environment is always const: it cannot be changed from the outside.
typedef std::shared_ptr<const Environment> EnvironmentShared;
//! A representation of the user's enviroinment. The Environment provides file-system-related access (file paths, etc) as well as audio I/O and related hardware related configuration options. 
class Environment {

    private://-----------------------------------------------------------------
    //! We internally store the temp directory as a Boost filepath object. The temp directory is stored in the user's home directory. This can be overriden in the future with a different location.
    boost::filesystem::path _temp_directory;
	
	//! Private sampling rate storage. Defaults to 44100. 
	OutputsSizeT _sampling_rate;
    
    //! Common (but not all) frame size. Defaults to 64.  
	FrameSizeType _common_frame_size;
	
    //! Load default directories.
    void _load_defaults();
	
    public://-------------------------------------------------------------------

    // need to =delete the default constructor
    
    explicit Environment(FrameSizeType fs=64);
    
    //~Environment();
	
    //! Factory method that sets defaults.
    static EnvironmentShared make();    

    //! Factory method that provides frame size defaults. Could by 'make from'. 
    static EnvironmentShared make_with_frame_size(FrameSizeType fs=64);
    
    //! Return the sampling rate
	OutputsSizeT get_sampling_rate() const {return _sampling_rate;};

    //! Return the common (or shared) frame size. 
    FrameSizeType get_common_frame_size() const {return _common_frame_size;};

	//! This returns a file path in the environment-specified temporary directory. By default this is in the user directory .arachnewaro. This returns a string for easier compatibility with clients, rather than a Boost file path
    std::string get_fp_temp(std::string name) const;


    //also need methods to return audio files from a default/or set audio dir

};



//	int x = f<double>({2, 4, 5});
//template<class T> int f(std::initializer_list<T> args) {
//	return args.size();
//}
//	int x = f<double>({2, 4, 5});
//	x = f<std::initializer_list<double>>({{2,3}, {4,6}});
    

class BufferInjector;

typedef std::shared_ptr<BufferInjector> BufferInjectorShared;

class BufferInjector {
    private:
        std::vector<SampleType> _parsed;
        ParameterIndexType _channels;
        bool _equal_width;
    
    public:
    
        BufferInjector() = delete;

        //! A single flat list
        BufferInjector(ILSampleType);
    
        //! A nested list.
        BufferInjector(ILILSampleType);
    
        ParameterIndexType get_channels();
    
        //! The frame size is the total number of samples per channel. 
        OutputsSizeT get_frame_size();

        //! Pass in vector of SampleTypes and fill it up a linear representation of the provided values.
        void fill_interleaved(std::vector<SampleType>&);
    
};





} // end namespace
#endif // ends _AW_COMMON_H_
















