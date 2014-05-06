

#include <cstdio> // for popen and related 

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <vector>

#include "aw_illustration.h"
#include "aw_generator.h"

namespace aw {

//------------------------------------------------------------------------------
Illustration :: Illustration() {}

Illustration :: ~Illustration() {}

//void Illustration :: draw(const std::vector<SampleT>& v, PIndexT d) {
//    throw std::invalid_argument("not implemented");
//}
//
//void Illustration :: draw(const std::vector<SampleT>& v) {
//    throw std::invalid_argument("not implemented");
//}

void Illustration :: draw(GenPtr g) {
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

void TimeDomainGraph :: draw_vector(const std::vector<SampleT>& v, 
    PIndexT d) {
    // d is the output count in the vector

    // get frame size, or units per _output_count    
    FrameSizeT frameSize = static_cast<FrameSizeT>(v.size());
    // assume that frame size is even division of size
    if (d >= 1) {
        frameSize = static_cast<FrameSizeT>(v.size() / d);
    }
    else {
    	throw std::invalid_argument("_output_count must be greater than zero");    
    }
    
    // if we get a crazy large _output_count, or have a small vector
    if (frameSize < 1) {
    	throw std::invalid_argument("frame size is less than 1");
    }
    
    // set line style based on frame size
    std::string line_style;
    if (frameSize > 40) {
        line_style = "impulse";
    }
    else {
        line_style = "linespoints";
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

    for (std::size_t i=1; i < d+1; ++i) {
        // TODO: need to increment/map color 
        _stream << "set style line " << i 
            << " lt 1 lw 1 pt 3 lc rgb '#332255'" << std::endl;
    }
    
    // margins need to be written from to bottom
    double height(1.0 / static_cast<double>(d));
    double pos(1.0);
    double margin(.04);
    double top;
    double bottom;
        
    for (PIndexT dStep=1; dStep < d + 1; ++dStep) {
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
        // impulse: good for audio
        // good for brakpoints: linespoints
        // TODO: if frameSize tn 20 points or so, use linespoints, else impulse
        _stream << "plot '-' using ($1) with " << line_style << " linestyle "
            << static_cast<int>(dStep) << std::endl;    

        // provide data here; assuming non-interleaved
        for (FrameSizeT i=frameSize*(dStep-1); i<frameSize*dStep; ++i) {
            _stream << std::setprecision(8) << v[i] << std::endl;
        }
        _stream << "e" << std::endl; // show end of data
    }
    _stream << "unset multiplot\n" << "pause -1\n" << std::endl;
}


//void TimeDomainGraph :: draw(const std::vector<SampleT>& v) {
//    draw(v, 0);
//}

void TimeDomainGraph :: draw(GenPtr g) {    
	VSampleT v;
	g->write_outputs_to_vector(v); // load outputs into this vecotr
    draw_vector(v, g->get_output_count());
    
}

void TimeDomainGraph :: pipe() {
    // this uses c-style file pointers to as these are what we get out of popen; must convert std::string to c string for usage by fprint
    // note: for this to work on ubuntu, need to: sudo apt-get install gnuplot-x11
    std::cout << "here" << std::endl;
    FILE* gp;
    gp = popen("gnuplot -persist", "w");
    if (gp == NULL) throw std::domain_error("popen failed");
    fprintf(gp, "%s", _stream.str().c_str()); // must use format to avoid error
    // pclose will wait for termination
    if (pclose(gp) == -1) throw std::domain_error("pclose failed");
}






// Static dictionary returns
std::string ColorDictionary::get(GenID gid){
    if (gid == GenID::Constant) {
        return "lightsteelblue3";
    }
    else {
        return "slategray4";
    }
}




//------------------------------------------------------------------------------
NetworkGraph :: NetworkGraph() {}

NetworkGraph :: ~NetworkGraph() {}

// grpahiz colors can be found here:
// http://www.graphviz.org/doc/info/colors.html

void NetworkGraph :: _draw_generator(GenPtr g, 
        SharedMapStringBool memo) {
	if (!g) { // if we get an empty pointer
		// could aternatively just return as a base case of recursion
	    throw std::invalid_argument("the GeneratoreShared is empty");
	}	
    // std::cout << "HERE: " << g << std::endl;
    
    // get_name_address is the id tag for this gen: no spaces or bad chars
    _stream << g->get_name_address() << " [";
    
    // the label is verbos, and describes i/o
    // create a doc segment of the box
    std::string label = g->get_label_address();
    escape(label, "{}<>", "\\");
    _stream << "label = \"<doc>  " << label << " ";

    PIndexT pos(0);    
    // iterate over slots
    for (pos=0; pos < g->get_slot_count(); ++pos) {
        _stream << " | <x" << static_cast<int>(pos) << 
                    "> s::" << static_cast<int>(pos) << " "; // << std::endl;
    }
    // iterate over inputs    
    for (pos=0; pos < g->get_input_count(); ++pos) {
        _stream << " | <in" << static_cast<int>(pos) << 
                    "> in::" << static_cast<int>(pos) << " "; // << std::endl;
    }        
    // iterate over outputs
    for (pos=0; pos < g->get_output_count(); ++pos) {
        _stream << " | <out" << static_cast<int>(pos) << 
                    "> out::" << static_cast<int>(pos) << " "; // << std::endl;
    }
	// end the definition of the label/node
    _stream << "\" fillcolor=\"" << ColorDictionary::get(g->get_class_id()) << "\"];" << std::endl;
    
    // next, define connections
    Gen::VGenPtrOutPair g_ins;
    GenPtr g_slot;
    Gen::VGenPtrOutPair::const_iterator k;

    for (pos=0; pos < g->get_slot_count(); ++pos) {    
        // a single gen shared for this position
        g_slot = g->get_slot_gen_at_index(pos);  
        // show from name to this gen at this slot
        _stream << "\"" << g_slot->get_name_address() << "\":";
        // out is not defined!
        _stream << "doc" << ":s"; // south
        _stream << " -> ";
        _stream << "\"" << g->get_name_address() << "\":";
        _stream << "x" << static_cast<int>(pos) << ":n"; // to north
        _stream << " [color=honeydew3];" << std::endl; // close line        
        // recurse on each gen
        // TODO: do we need to look at memo here before writing?
        _draw_generator(g_slot, memo);
    }    

    PIndexT g_ins_out_pos(0);
    GenPtr g_in;
    Gen::VGenPtrOutPair::const_iterator j; 
    // show connections by describing inputs
    // iter over each input position
    for (pos=0; pos < g->get_input_count(); ++pos) {    
        // a vector of gen shared/ out number pairs for this position
        g_ins = g->get_input_gens_by_index(pos);
        
        //std::cout << g << " g_ins:" << g_ins.size() << std::endl;
        // for each input position, iter over all connections to gens at outputs
        for (j = g_ins.begin(); j != g_ins.end(); ++j) {
            g_ins_out_pos = (*j).second;
            g_in = (*j).first;  

            // "genA":out0:s -> "genB":in0:n;
            // show from input to this gen at this input
            _stream << "\"" << g_in->get_name_address() << "\":";
            _stream << "out" << static_cast<int>(g_ins_out_pos) << ":s"; // south
            _stream << " -> ";
            _stream << "\"" << g->get_name_address() << "\":";
            _stream << "in" << static_cast<int>(pos) << ":n"; // to north
            _stream << " [color=darkslategray];" << std::endl; // close line        
			
			// recurse on each gen; this only advances if g_ins has length (not the cast when dealing with a constant)
            if (memo->count(g_in->get_name_address()) == 0) {
                (*memo)[g_in->get_name_address()] = false;
                _draw_generator(g_in, memo);
                (*memo)[g_in->get_name_address()] = true;
            }
        }                
    }    
    // connect all inputs to this as an output, using  g->get_name_address()
}




void NetworkGraph :: draw(GenPtr g) {    
    // color is the the color of the outline of the box-shape
    _stream.str("");     
    // header
    _stream << "\
digraph G { \n\
dpi = 300; \n\
node [shape=record, style=filled, color=grey16, fillcolor=lemonchiffon4, fontcolor=lemonchiffon1, fontsize=10, fontname=Courier, fontsize=10]; \n\
edge [color=grey]; \n\
ranksep=\"1.5 equally\"; \n\
size=\"8,8\"; " << std::endl;

    // add each generator recursively
    // need to store diction of encountered 
    
    SharedMapStringBool memo = SharedMapStringBool(new MapStringBool);
    //SharedMapStringBool memo = SharedMapStringBool();
    _draw_generator(g, memo);
    // close the header
    _stream << "}" << std::endl;
    
}


void NetworkGraph :: pipe() {
    // this uses c-style file pointers to as these are what we get out of popen; must convert std::string to c string for usage by fprint
    FILE* gp;
	// dot -Tpdf test.dot -o test.pdf	
    //gp = popen("dot -Tpng | display", "w");
    gp = popen("dot -Tpdf -o dot.pdf", "w");
	
    if (gp == NULL) throw std::domain_error("popen failed");
    fprintf(gp, "%s", _stream.str().c_str()); // must use format to avoid error
    // pclose will wait for termination
    if (pclose(gp) == -1) throw std::domain_error("pclose failed");
}




} // end namespace aw


