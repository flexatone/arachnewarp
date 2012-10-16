
#include <stdexcept>
#include <iostream>
#include <stdexcept>


// Need this for getting user home directory when not set to HOME
#include <pwd.h>



#include <boost/filesystem.hpp>


#include "aw_common.h"



namespace aw {


//==============================================================================
// display

void print(SampleType* out, FrameSizeType size) {
    std::cout << "<array ";
    for (FrameSizeType i=0; i<size; ++i) {
        std::cout << out[i] << ' ';
    }

    // TODO: figure out how to do this idiom    
    //while (out < out+size) {
        //std::cout << *out++ << ' ';
    //}    
    std::cout << '>' << std::endl;
}



boost::filesystem::path get_fp_home() {
    // do not need anyimport to use getnev
    const char* homeDir = getenv("HOME");
    //const char* homeDir(0);
    // getenv may not always be correct; 
    if (1) {
        // this is a struct from pwd header in C; does not need struct 
        passwd* pwd(NULL);        
        pwd = getpwuid(getuid());
        if (pwd)
           homeDir = pwd->pw_dir;
    }
    return homeDir; // will get converted to a string
}


//==============================================================================
// utility classes


Environment :: Environment() {
    _load_defaults();
}

Environment :: ~Environment() {}



void Environment :: _load_defaults() {
    // this method is called on init
    _temp_directory = get_fp_home() / ".arachne_warp";
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

//! This returns a file name used for temporary plots. 
boost::filesystem::path Environment :: get_fp_plot(std::string name) const {
    // this might read from a file or do other configurations
    return _temp_directory / name;
}


} // end namespace aw



