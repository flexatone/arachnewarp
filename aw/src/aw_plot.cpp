
#include <iostream>
#include <iomanip>

#include "aw_plot.h"

namespace aw {


Plotter :: Plotter() {}

Plotter :: ~Plotter() {}

void Plotter :: _write(const std::vector<SampleType>& v, FrameDimensionType d, 
            bool interleaved) {
                
    // TODO: need to write to file
    
    //std::cerr << "Plot._write()" << std::endl;
    
    //std::ofstream f; // for writing, need out file stream
    //f.open(fp->c_str(), std::ios::app);
    
    // get frame size, or size per dimension
    FrameSizeType frameSize(v.size());
    // TODO: d cannot be zero or less; raise exception if it is 
    if (d > 1) {
        frameSize = v.size() / d;
    }
        
    std::cout << "\
unset key \n\
set style line 11 lc rgb '#ffffff' lt 1 \n\
set tics out nomirror scale 0,0.001 \n\
set format '' \n\
set grid \n\
set lmargin screen 0.02 \n\
set rmargin screen 0.98 " << std::endl;    
    
    std::cout << "set multiplot layout " << static_cast<int>(d) 
        <<",1" << std::endl;

    for (int i=1; i<d+1; ++i) {
        // TODO: need to increment color 
        std::cout << "set style line " << i 
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
                
        std::cout << "set tmargin screen " << top << std::endl;
        std::cout << "set bmargin screen " << bottom << std::endl;
        std::cout << "plot '-' using ($1) with impulse linestyle " 
            << static_cast<int>(dStep) << std::endl;    

        // provide data here; assuming non-interleaved
        for (FrameSizeType i=frameSize*(dStep-1); i<frameSize*dStep; ++i) {
            std::cout << std::setprecision(8) << v[i] << std::endl;
        }
        std::cout << "e" << std::endl; // show end of data        
    }
    std::cout << "unset multiplot\n" << "pause -1\n" << std::endl;
        
}

} // end namespace aw











