
#include <stdexcept>
#include <iostream>
#include <stdexcept>


// Need this for getting user home directory when not set to HOME
#include <pwd.h>

// Anything that includes common will need to include -l boost_filesystem -l boost_system to run the Environment code here
#include <boost/filesystem.hpp>


#include "aw_common.h"



namespace aw {


void escape(std::string& str, const std::string& replace_targets, 
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
    // TODO: figure out how to do this idiom    
    //while (out < out+size) {
        //std::cout << *out++ << ' ';
    //}    
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

// TODO: add a static variable to store a default environment instance, and a static method to get and set the default

Environment :: Environment(FrameSizeT fs) 
	: _sampling_rate{44100},
    _common_frame_size{fs} { // default is 64
	// post initializers
    _load_defaults(); // file paths, not frame size
}

EnvironmentPtr Environment :: make() {
    EnvironmentPtr e = EnvironmentPtr(new Environment);
    return e;
}

EnvironmentPtr Environment :: make_with_frame_size(FrameSizeT fs) {
    EnvironmentPtr e = EnvironmentPtr(new Environment(fs));
    return e;
}


// Environment :: ~Environment() {}

void Environment :: _load_defaults() {
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

std::string Environment :: get_fp_temp(std::string name) const {
    // this might read from a file or do other configurations
    return (_temp_directory / name).string();
}



//==============================================================================

// TODO: Need two types of injectors: one for values, another for simple collections of generators; could alternatively simply have >> take VGenPtr({})















Injector :: Injector(ILSampleT src) {
    // this always have 1 dimension
    _channels = 1;
    _parsed.reserve(src.size());
    for (auto x : src) {
        _parsed.push_back(x);
    }
}

Injector :: Injector(ILILSampleT src) {
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

InjectorPtr Injector :: make(ILSampleT src) {
    return InjectorPtr(new Injector(src));
}

InjectorPtr Injector :: make(ILILSampleT src) {
    return InjectorPtr(new Injector(src));
}


ParameterIndexT Injector :: get_channels() const {
    return _channels;
}

OutputsSizeT Injector :: get_frame_size() const {
    return _parsed.size() / _channels;
}

void Injector :: fill_interleaved(VSampleT& post) const {
// Pass in a reference to a vector and have it cleared, sized, and filled.
    //std::cout << "fill_interleaved: " << get_frame_size() << std::endl;
    post.clear();
    post.reserve(_parsed.size());
    for (auto x : _parsed) {
        post.push_back(x);
    }
}








} // end namespace aw



