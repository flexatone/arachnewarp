
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>

#include "aw_plotter.h"

namespace aw {


Plotter :: Plotter() {}

Plotter :: ~Plotter() {}


void Plotter :: plot(const std::vector<SampleType>& v, 
    FrameDimensionType d, bool interleaved) {

    if (d <= 0) {
    	throw std::invalid_argument("dimension must be greater than zero");
    }                
    // get frame size, or units per dimension    
    FrameSizeType frameSize(v.size());
    if (d > 1) {
        frameSize = v.size() / d;
    }
    // if we get a crazy large dimension, or have a small vector
    if (frameSize < 1) {
    	throw std::invalid_argument("frame size is less than 1");
    }                

    // clear the string
    _stream.str("");     
    _stream << "\
unset key \n\
set style line 11 lc rgb '#ffffff' lt 1 \n\
set tics out nomirror scale 0,0.001 \n\
set format '' \n\
set grid \n\
set lmargin screen 0.02 \n\
set rmargin screen 0.98 " << std::endl;    
    
    _stream << "set multiplot layout " << static_cast<int>(d) 
        <<",1" << std::endl;

    for (int i=1; i<d+1; ++i) {
        // TODO: need to increment color 
        _stream << "set style line " << i 
            << " lt 1 lw 1 pt 3 lc rgb '#332255'" << std::endl;
    }
    
    // margins need to be written from to bottom
    double height(1.0 / static_cast<double>(d));
    double pos(1.0);
    double margin(.04);
    double top;
    double bottom;
        
    for (FrameDimensionType dStep=1; dStep<d+1; ++dStep) {
        // use whole margin at top on first
        if (dStep == 1) {
            top = pos - margin; 
        }
        else  {
            top = pos - (margin * 0.5);
        }
        // use whole bottom margin on last
        if (dStep == d) {
            bottom = pos - height + margin;
        }
        else  {
            bottom = pos - height + (margin * 0.5);
        }        
        pos -= height;
                
        _stream << "set tmargin screen " << top << std::endl;
        _stream << "set bmargin screen " << bottom << std::endl;
        _stream << "plot '-' using ($1) with impulse linestyle " 
            << static_cast<int>(dStep) << std::endl;    

        // provide data here; assuming non-interleaved
        for (FrameSizeType i=frameSize*(dStep-1); i<frameSize*dStep; ++i) {
            _stream << std::setprecision(8) << v[i] << std::endl;
        }
        _stream << "e" << std::endl; // show end of data        
    }
    _stream << "unset multiplot\n" << "pause -1\n" << std::endl;
}



void Plotter :: print() {
    std::cout << _stream.str() << std::endl;
}


void Plotter :: write(const std::string& fp) {
    std::ofstream f; // for writing, need out file stream
    f.open(fp.c_str()); //  std::ios::in
    f << _stream.str() << std::endl;
    f.close();
}




} // end namespace aw











