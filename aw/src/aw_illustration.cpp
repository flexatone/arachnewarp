#ifndef _AW_ILLUSTRATION_H_
#define _AW_ILLUSTRATION_H_

#include <cstdio> // for popen and related 

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>

#include "aw_illustration.h"
#include "aw_generator.h"

namespace aw {


//------------------------------------------------------------------------------
Illustration :: Illustration() {}

Illustration :: ~Illustration() {}

//void Illustration :: draw(const std::vector<SampleType>& v, OutputCountType d) {
//    throw std::invalid_argument("not implemented");
//}
//
//void Illustration :: draw(const std::vector<SampleType>& v) {
//    throw std::invalid_argument("not implemented");
//}

void Illustration :: draw(GeneratorShared g) {
    throw std::invalid_argument("not implemented");
}

void Illustration :: pipe() {
    throw std::invalid_argument("not implemented");
}

void Illustration :: print() {
    std::cout << _stream.str() << std::endl;
}

void Illustration :: write(const std::string& fp) {
    std::ofstream f; // for writing, need out file stream
    f.open(fp.c_str()); //  std::ios::in
    f << _stream.str() << std::endl;
    f.close();
}






//------------------------------------------------------------------------------
TimeDomainGraph :: TimeDomainGraph() {}

TimeDomainGraph :: ~TimeDomainGraph() {}

void TimeDomainGraph :: draw_vector(const std::vector<SampleType>& v, 
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


//void TimeDomainGraph :: draw(const std::vector<SampleType>& v) {
//    draw(v, 0);
//}

void TimeDomainGraph :: draw(GeneratorShared g) {    
	Generator::VSampleType v;
	g->write_matrix_to_vector(v); // load matrix into this vecotr
    draw_vector(v, g->get_output_count());
    
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








//------------------------------------------------------------------------------
NetworkGraph :: NetworkGraph() {}

NetworkGraph :: ~NetworkGraph() {}


void NetworkGraph :: _draw_generator(GeneratorShared g) {

    // TODO: need to escape braches in label
    
    // _stream.str("");         
    _stream << g->get_label_address() << " [";
    
    _stream << "label = \"<doc>  " << g->get_label() << std::endl;
    
    // iterate over inputs
    _stream << " | <in0> in:0 " << std::endl;
        
    // iterate over outputs
    _stream << " | <out0> out:0 " << std::endl;
    
    _stream << "\"];" << std::endl;
    
    // connect all inputs to this as an output
}


void NetworkGraph :: draw(GeneratorShared g) {    
    _stream.str("");     
    _stream << "\
 digraph G {                    \n\
 splines=curved;                \n\
 node [shape=record, fontname=Arial, fontsize=12];  \n\
 edge [color=grey];             \n\
 ranksep=\"1.5 equally\";       \n\
 size=\"8,8\";                  " << std::endl;    
 
    _draw_generator(g);
    _stream << "}" << std::endl;
    
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
#endif // ends _AW_ILLUSTRATION_H_











// dot -Tpdf test.dot -o test.pdf

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






