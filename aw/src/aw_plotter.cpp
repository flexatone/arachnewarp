

#include <cstdio> // for popen and related 

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>

#include "aw_plotter.h"

namespace aw {


//------------------------------------------------------------------------------
Plotter :: Plotter() {}

Plotter :: ~Plotter() {}

void Plotter :: draw(const std::vector<SampleType>& v, OutputCountType d) {
    throw std::invalid_argument("not implemented");
}

void Plotter :: draw(const std::vector<SampleType>& v) {
    throw std::invalid_argument("not implemented");
}

void Plotter :: pipe() {
    throw std::invalid_argument("not implemented");
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






//------------------------------------------------------------------------------
TimeDomainGraph :: TimeDomainGraph() {}

TimeDomainGraph :: ~TimeDomainGraph() {}

void TimeDomainGraph :: draw(const std::vector<SampleType>& v, 
    OutputCountType d) {
    // d is the output count in the vector

    // get frame size, or units per _output_count    
    FrameSizeType frameSize = static_cast<FrameSizeType>(v.size());
    // assume that frame size is even division of size
    if (d >= 1) {
        frameSize = static_cast<FrameSizeType>(v.size() / d);
    }
    else {
    	throw std::invalid_argument("_output_count must be greater than zero");    
    }
    
    // if we get a crazy large _output_count, or have a small vector
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
        
    for (OutputCountType dStep=1; dStep<d+1; ++dStep) {
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


void TimeDomainGraph :: draw(const std::vector<SampleType>& v) {
    draw(v, 0);
}


void TimeDomainGraph :: pipe() {
    // this uses c-style file pointers to as these are what we get out of popen; must convert std::string to c string for usage by fprint
    FILE* gp;
    gp = popen("gnuplot -persist", "w");
    if (gp == NULL) throw std::domain_error("popen failed");
    fprintf(gp, "%s", _stream.str().c_str()); // must use format to avoid error
    // pclose will wait for termination
    if (pclose(gp) == -1) throw std::domain_error("pclose failed");
}




NetworkGraph :: NetworkGraph() {}

NetworkGraph :: ~NetworkGraph() {}

void NetworkGraph :: draw(const std::vector<SampleType>& v, 
    OutputCountType d) {

}

void NetworkGraph :: draw(const std::vector<SampleType>& v) {
    draw(v, 0);
}


void NetworkGraph :: pipe() {
    // this uses c-style file pointers to as these are what we get out of popen; must convert std::string to c string for usage by fprint
    FILE* gp;
    gp = popen("gnuplot -persist", "w");
    if (gp == NULL) throw std::domain_error("popen failed");
    fprintf(gp, "%s", _stream.str().c_str()); // must use format to avoid error
    // pclose will wait for termination
    if (pclose(gp) == -1) throw std::domain_error("pclose failed");
}







} // end namespace aw











// dot -Tpdf test.dot -o test.pdf

// digraph G {
// //splines=curved;
// splines=curved;
// // shape = box
// node [shape=record, fontname=Arial, fontsize=12];
// edge [color=grey];
// ranksep="1.5 equally";
// size="8,8";

// genA [label =   "<doc> \<genA@3@2\{x,y,z\}\> \
//                | <in0> in:0  \
//                | <in1> in:1  \
//                | <in2> in:2  \
//                | <out0> out:0  \
//                | <out1> out:1  \
//                | <out2> out:2  \
//             "];
// 
// genB [label =   "<doc> \<genB@2@2\{x,y,z\}\> \
//                | <in0> in:0  \
//                | <in1> in:1  \
//                | <out0> out:0  \
//                | <out1> out:1  \
//                | <slot0> slot:1  \
//             "];
// 
// genC [label =   "<doc> \<genC@2@1\{x,y,z\}\> \
//                | <in0> in:0  \
//                | <in1> in:1  \
//                | <out0> out:0  \
//             "];
// 
// 
// 
// "genA":out0:s -> "genB":in0:n;
// "genA":out1:s -> "genB":in1:n;
// "genA":out2:s -> "genB":in1:n;
// "genA":out2:s -> "genB":in0:n;
// 
// "genB":out0:s -> "genC":in0:n;
// "genB":out1:s -> "genC":in1:n;
// 
// "genC":out0:s -> "genA":in0:n;
// 
// }
// 






