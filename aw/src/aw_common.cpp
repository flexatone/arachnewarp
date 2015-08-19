
#include <stdexcept>
#include <iostream>
#include <stdexcept>


// Need this for getting user home directory when not set to HOME
#include <pwd.h>

// Anything that includes common will need to include -l boost_filesystem -l boost_system to run the Env code here
#include <boost/filesystem.hpp>


#include "aw_common.h"



namespace aw {


void escape(std::string& str, 
        const std::string& replace_targets, 
        const std::string& prefix) {
    // replace a string by character; all passed in by reference; this is used in writing dot files for graphiz
    std::string::const_iterator i;
    std::stringstream s; 
    std::string replace;
    size_t pos;
    for (i=replace_targets.begin(); i!=replace_targets.end(); ++i) {
        //std::cout << "here: " << *i << std::endl;
        // do replacement
        pos = 0;
        // get a replacemetn string for *i
        s.str(""); 
        s << prefix << *i;
        replace = s.str();

        while((pos=str.find(*i, pos)) != std::string::npos) {
            // target is always of length 1
            str.replace(pos, 1, replace);
            pos += replace.length(); // replacement is of various length
        }
    }
}


//==============================================================================
// display

void print(SampleT* out, FrameSizeT size) {
    std::cout << "<array ";
    for (FrameSizeT i=0; i<size; ++i) {
        std::cout << out[i] << ' ';
    }
    std::cout << '>' << std::endl;
}

const char* get_fp_home() {
    // do not need anyimport to use getnev
    const char* homeDir = getenv("HOME");
    //const char* homeDir(0);
    // getenv may not always be correct; 
    if (!homeDir) {
        // this is a struct from pwd header in C; does not need struct 
        // this leaks a small but constant amount ubuntu
        passwd* pwd = getpwuid(getuid());
        if (pwd) // if not null
           homeDir = pwd->pw_dir;
    }
    if (!homeDir) { // if failed
		throw std::domain_error("no home directory can be found");
    }
    return homeDir; // will get converted
}




//==============================================================================
// utility classes



// must initialize private static member attribute in impl file
Random::Core Random::core = Random::Core();

// must initialize private static member attribute in impl file
EnvPtr Env::_default_env = nullptr;

Env :: Env(FrameSizeT fs) 
	: _sampling_rate{44100},
    _common_frame_size{fs} { // default is 64
	// post initializers
    _load_defaults(); // file paths, not frame size
}

EnvPtr Env :: make_with_frame_size(FrameSizeT fs) {
    // static method
    return EnvPtr(new Env(fs));
}

//EnvPtr Env :: make() {
//    // static method
//    return make_with_frame_size();
//}

void Env :: set_default_env(EnvPtr env) {
    // static method
    _default_env = env;
}

EnvPtr Env :: get_default_env() {
    // static method
    if (_default_env == nullptr) {
        // get default configuration
        set_default_env(make_with_frame_size());
    }
    return _default_env;
}


void Env :: _load_defaults() {
    // this method is called on init
    _temp_directory = boost::filesystem::path(get_fp_home()) / ".arachne_warp";
    //std::cout << _temp_directory << std::endl;
    if (not boost::filesystem::exists(_temp_directory)) {
        //std::cout << "creating dir: " << _temp_directory << std::endl;
        bool ok = boost::filesystem::create_directory(_temp_directory);
        if (!ok) {
            throw std::invalid_argument("cannot create directory: " + 
                    _temp_directory.string());
        }
    }
}

std::string Env :: get_fp_temp(std::string name) const {
    // this might read from a file or do other configurations
    return (_temp_directory / name).string();
}







} // end namespace aw



