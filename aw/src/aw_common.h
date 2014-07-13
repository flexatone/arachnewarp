#ifndef _AW_COMMON_H_
#define _AW_COMMON_H_

#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <list>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <initializer_list>
#include <random>

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
// 32 bit int is 2,147,483,647, which gives over 13.5 hours at 44.1 sr
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


SampleT const PI {3.14159265358979323846264338};
SampleT const PI2 {PI * 2.0};
SampleT const LOGTWO {0.69314718055994528623};
SampleT const LOGTEN {2.302585092994};
// used for panning calcs
SampleT const PIOVER4 = {PI * 0.25};
SampleT const SQRT2OVER2 = {sqrt(2.0) * 0.5};

//! We store a minimum frequency value, necessary for handling case where the frequency goes through zero and we need to shift to a non-zero value. This value (.00001) is approx 28 hours, or more than 1 day. 
SampleT const MIN_FQ {.00001};

// -120 dB, or pow(10, -120/20), or 1e-06
SampleT const MIN_AMP {.000001};

//! The trigger threshold, or the value above which we determine that we have a trigger.
SampleT const TRIG_THRESH {.99999};


//! Defined for all text-based hierarchical displays. 
std::uint8_t const INDENT_SIZE {3};

const std::string OUT_SYMBOL {"*"};
const std::string IN_SYMBOL {"+"};
const std::string SLOT_SYMBOL {"~"};

// place in a colors names space
namespace ansi_color {
    const std::string RESET {"\033[0m"};
    const std::string BLACK {"\033[30m"};
    const std::string RED {"\033[31m"};
    const std::string GREEN {"\033[32m"};
    const std::string YELLOW {"\033[33m"};
    const std::string BLUE {"\033[34m"};
    const std::string MAGENTA {"\033[35m"};
    const std::string CYAN {"\033[36m"};
    const std::string WHITE {"\033[37m"};
    const std::string BOLDBLACK {"\033[1m\033[30m"};
    const std::string BOLDRED {"\033[1m\033[31m"};
    const std::string BOLDGREEN {"\033[1m\033[32m"};
    const std::string BOLDYELLOW {"\033[1m\033[33m"};
    const std::string BOLDBLUE {"\033[1m\033[34m"};
    const std::string BOLDMAGENTA {"\033[1m\033[35m"};
    const std::string BOLDCYAN {"\033[1m\033[36m"};
    const std::string BOLDWHITE {"\033[1m\033[37m"};
}


const std::string COLOR_H1 {ansi_color::BOLDWHITE};
const std::string COLOR_SLOT {ansi_color::WHITE};
const std::string COLOR_INPUT {ansi_color::GREEN};
const std::string COLOR_OUTPUT {ansi_color::YELLOW};


inline std::string color_embrace(std::string body, std::string color,
            char left='{', char right='}') {
    std::stringstream msg;
    msg << color << left << ansi_color::RESET << body << color << right << ansi_color::RESET;
    return msg.str();
}

inline std::string color_symbol_embrace(std::string symbol, PIndexT n,
        std::string color) {
    std::stringstream n_str;
    n_str << n;
    std::stringstream msg;
    msg << color << symbol << ansi_color::RESET << color_embrace(n_str.str(), color);
    return msg.str();
}

   
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
    return fq == 0 ? MIN_FQ : 
        fq > nyquist ? nyquist : 
        fq < -nyquist ? -nyquist : fq;
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
    return src <= min ? min :
        src >= max ? max : src;
}


//! Floiting point mod that properly handles negative values (like Python).
inline SampleT bipolar_fmod(SampleT n, SampleT m) {
    // ((n % M) + M) % M
    return fmod(fmod(n, m) + m, m);
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
    
    


template <typename T>
void print_iterable(const T& iterable) {
    std::cout << "< ";
    for (auto x : iterable) {
        std::cout << x << ' ';
    }
    std::cout << '>' << std::endl;
}   

template <typename T>
SampleT mean_iterable(const T& iterable) {
    SampleT sum{0};
    PIndexT count{0};
    for (auto x : iterable) {
        sum += x;
        count += 1;
    }
    return sum / count;
}   



// taken from pd/chuck; need to update types, and integerate as necessar
//-----------------------------------------------------------------------------
//
//double trunc( double a )
//{
//    return (double)(long)a;
//}
//
//double remainder( long a, long b )
//{
//    long div = a/b;
//    return a - b*div;
//}
//
//double ftom( double f )
//{   
//    // return (f > 0 ? 17.3123405046 * log(.12231220585 * f) : -1500);
//    // TODO: optimize
//    return (f > 0 ? (log(f/440.0) / LOGTWO) * 12.0 + 69 : -1500);
//}
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



// utility classes =============================================================


//! Utility class for sourcing random values using static functions. 
class Random {
    private://-----------------------------------------------------------------
    public://-------------------------------------------------------------------

    //! Struct storage of core engines and distributions, shared across by a single static instance.
    struct Core {
        // basic and high-precision engeinges
        std::minstd_rand re_lin_congruential {std::random_device{}()};
        std::mt19937 re_marsenne {std::random_device{}()};
        
        // useful distributions
        std::uniform_real_distribution<SampleT> uniform_dist {0, 1};

        std::uniform_real_distribution<SampleT> uniform_dist_bi_polar {-1, 1};

        std::uniform_int_distribution<UINT8> uniform_switch {0, 1};

    };

    //! Core random engines and distributions, shared by a single static attribute. Init in implementation file.
    static Core core;

    //! A random unform distribution between 0 and 1. 
    static inline SampleT uniform() {
        return core.uniform_dist(core.re_lin_congruential);
    }

    static inline SampleT uniform_switch() {
        return core.uniform_switch(core.re_lin_congruential);
    }

    static inline SampleT uniform_bi_polar() {
        return core.uniform_dist_bi_polar(core.re_lin_congruential);

    }
    //! Probabilistic rounding for a number to nearest integer based on random weighting.
    static inline SampleT round(SampleT v) {
        // need just the floating point component
        SampleT v_tail = v - std::floor(v);
        // e.g.: if les then .7, then ceil, else foloor
        if (core.uniform_dist(core.re_lin_congruential) <= v_tail) {
            return std::ceil(v);
        }
        else {
            return std::floor(v);
        }
    }


};







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



//! A class (Injector) used to pass list and nested list values with initializer lists.
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

    //! Pass in vector of SampleTypes and fill up a linear representation of the provided values.
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
















