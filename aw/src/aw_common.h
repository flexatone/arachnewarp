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
typedef double SampleT; // sample value type

//! One dimensional initializer list of SampleTypes.
typedef std::initializer_list<SampleT> ILSampleT;

//! Two dimensional initializer list of SampleTypes.
typedef std::initializer_list<ILSampleT> ILILSampleT;

//! One dimensional vector of samples.
typedef std::vector<SampleT> VSampleT;

//! Two dimensinoal vector of samples. 
typedef std::vector<VSampleT> VVSampleT;

    
//! The size of a single frame (or vector), or the number of samples processed per computation cycle or stored in a single channel of output data. This is a very large integer as we might need to accomodate loading in large audio files as a single frame.
typedef std::uint32_t FrameSizeT;

//! Output size. Was uint16_t, but for handling audio files was increased to uint32_t. In general, the outputs is the frame size times the number of outputs, so outputs is always greater than or equal to frame size. 
typedef std::uint32_t OutputsSizeT;


//! An unsigned integer to represent the position of a parameter type, i.e., an input or output position. Assumed to never have more thean 200 parameter inputs for a Gen.
typedef std::size_t PIndexT; 


typedef std::vector<PIndexT> VPIndexT;

//! A vector of frame size types. This is used for offsets into the outputs.
typedef std::vector<FrameSizeT> VFrameSizeType;

//! A small unsigned interger for specialized cases. 
typedef std::uint8_t UINT8; 

//! An unsigned integer for each Gen that counts the number of frames that have passed; this number needs to be very large and overflow gracefully. 
typedef std::uint64_t RenderCountT; 



// TODO: replace with a SharedGenerator: but, we need a SharedGenerator to be hashable, which requires us to extend std::hash or similar
//! A mapping of a String to bool. 
typedef std::unordered_map<std::string, bool> MapStringBool;

//! The shared pointer version of MapStringParameterIndexPairToBool.
typedef std::shared_ptr<MapStringBool> SharedMapStringBool;


SampleT const PI(3.14159265358979323846264338);
SampleT const PI2(3.14159265358979323846264338 * 2.0);
SampleT const LOGTWO(0.69314718055994528623);
SampleT const LOGTEN(2.302585092994);

//! We store a minimum frequency value, necessary for handling case where the frequency goes through zero and we need to shift to a non-zero value. This value (.00001) is approx 28 hours, or more than 1 day. 
SampleT const MIN_FQ(.00001);

// -120 dB, or pow(10, -120/20), or 1e-06
SampleT const MIN_AMP(.000001);


//! The trigger threshold, or the value above which we determine that we have a trigger.
SampleT const TRIG_THRESH(.99999);


//! Defined for all text-based hierarchical displays. 
std::uint8_t const INDENT_SIZE(2);


// functions ===================================================================

//! Escape one or more characters privided by a string and a single prefix. Changes are made in place to the passed in string. 
void escape(std::string& str, const std::string& replace_targets, 
        const std::string& prefix);


//! Print an arry of SampleT of size type FrameSizeT.
void print(SampleT* out, FrameSizeT size);

//! Return the users home directory as a const char pointer. This is what is returned by low-level calls, and is thus returned here to reduce creating temporary objects.
const char* get_fp_home();


//! Utility function to format an exception string.
inline std::string str_file_line(const char* file, int line) {
    std::stringstream msg;
    msg << " {file: " << file << " line: " << line << "}";
    return msg.str();
}



//! Frequency values might swing through zero, or exceed Nyquist. This inline function solves this problem by returning a minimum number of zero is hit.
// note that inline functions need to be defined in the header
inline SampleT frequency_limiter(SampleT fq, SampleT nyquist) {
	// this is inlined
	fq = fq == 0 ? MIN_FQ : fq;
	fq = fq > nyquist ? nyquist : fq;
	return fq < -nyquist ? -nyquist : fq;
}

//! Limit a phase value between 0 and PI2 by wrapping: done in place. 
inline void phase_limiter(SampleT& phase) {
    while (phase >= PI2) {
        phase -= PI2;
    }
    while (phase < 0.0) {
        phase += PI2;
    }
}


inline SampleT double_limiter(SampleT src, SampleT min,
        SampleT max) {
    src = src < min ? min : src;
    return src > max ? max : src;
}



//! Detect and assign true min / max, based on comparison. Min and max are set by passing in pointers to to the value that are set in place.
inline void true_min_max(
        SampleT lower,
        SampleT upper,
        SampleT* min,
        SampleT* max) {

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
inline SampleT mtof(SampleT f) {
    // midi to frequency
    if (f <= -1500) return 0;
    else if (f > 1499) return mtof(1499);
    else return pow(2, (f-69) / 12.0) * 440.0;
}


//! Round a SampleT to the nearest int-like Sample T
inline SampleT rounded(SampleT x){
    return x > 0.0 ? std::floor(x + 0.5) : std::ceil(x - 0.5);
}





    

// taken from pd/chuck; need to update types, and integerate as necessar
//-----------------------------------------------------------------------------

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
typedef std::shared_ptr<const Environment> EnvironmentPtr;
//! A representation of the user's enviroinment. The Environment provides file-system-related access (file paths, etc) as well as audio I/O and related hardware related configuration options. 
class Environment {

    private://-----------------------------------------------------------------
    //! We internally store the temp directory as a Boost filepath object. The temp directory is stored in the user's home directory. This can be overriden in the future with a different location.
    boost::filesystem::path _temp_directory;
	
	//! Private sampling rate storage. Defaults to 44100. 
	OutputsSizeT _sampling_rate;
    
    //! Common (but not all) frame size. Defaults to 64.  
	FrameSizeT _common_frame_size;
	
    //! Load default directories.
    void _load_defaults();
    
    //! We store a static (class wid) default environment that is used and set. 
    static EnvironmentPtr _default_env;

    
    public://-------------------------------------------------------------------
    
    explicit Environment(FrameSizeT fs);

    Environment() = delete;
	
    // REDO to require setting frame size and sampling rate. 
//    static EnvironmentPtr make();    

    //! Factory method that provides frame size defaults. Could by 'make from'. 
    static EnvironmentPtr make_with_frame_size(FrameSizeT fs=64);
    
    //! Set a default environment. If called with no args, will reset the default to nullptr, forcing new creation of a default on text run. 
    static void set_default_env(EnvironmentPtr env=nullptr);
    
    //! Get the last set default environment, or a default created on demand.
    static EnvironmentPtr get_default_env();
    
    
    //! Return the sampling rate
	OutputsSizeT get_sampling_rate() const {return _sampling_rate;};

    //! Return the common (or shared) frame size. 
    FrameSizeT get_common_frame_size() const {return _common_frame_size;};

	//! This returns a file path in the environment-specified temporary directory. By default this is in the user directory .arachnewaro. This returns a string for easier compatibility with clients, rather than a Boost file path
    std::string get_fp_temp(std::string name) const;


    //also need methods to return audio files from a default/or set audio dir

};

    	


//! A class used to pass list and nested list values with initializer lists.
template <typename T>
class Inj {
    private: //-----------------------------------------------------
    
    std::vector<T> _parsed;
    PIndexT _channels;
    bool _equal_width;
    
    public: //--------------------------------------------------------
    
    Inj() = delete;
    
//    typedef std::shared_ptr<T> Ptr;
    
    //! A single flat list
    Inj(std::initializer_list<T> src) {
        // this always have 1 dimension
        _channels = 1;
        _parsed.reserve(src.size());
        for (auto x : src) {
            _parsed.push_back(x);
        }
    }
        
    //! A nested list.
    Inj(std::initializer_list< std::initializer_list<T> > src){
        // number of sub groups is channels;
        // find max on first iteration; must go through all
        _channels = 0;
        for (auto group : src) {
            _channels = std::max(_channels, group.size());
        }
        std::size_t group_count;
        for (auto group : src) {
            group_count = 0;
            for (auto x : group) {
                _parsed.push_back(x);
                ++group_count;
            }
            // pad zerof for anything missing
            while (group_count < _channels) {
                _parsed.push_back(0);
                ++group_count;
            }
        }
    }

    PIndexT get_channels() const {
        return _channels;
    }

    //! The frame size is the total number of samples per channel. 
    OutputsSizeT get_frame_size() const {
        return _parsed.size() / _channels;
    }

    //! Pass in vector of SampleTypes and fill it up a linear representation of the provided values.
    void fill_interleaved(std::vector<T>& post) const {
    // Pass in a reference to a vector and have it cleared, sized, and filled.
        //std::cout << "fill_interleaved: " << get_frame_size() << std::endl;
        post.clear();
        post.reserve(_parsed.size());
        for (auto x : _parsed) {
            post.push_back(x);
        }
    }
    
};




//! A simple struct to support returning simple error messages from functions that cannot raise exception
struct Validity {

    //! Bool to check validity.
    bool ok;
    
    //! String reporting what happened
    std::string msg;
};



} // end namespace
#endif // ends _AW_COMMON_H_
















