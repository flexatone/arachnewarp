#ifndef _AW_PLOTTER_H_
#define _AW_PLOTTER_H_

#include <tr1/memory>
#include <vector>
#include <sstream>

#include "aw_common.h"


namespace aw {


//! An interface to GNUplot writing.
class Plotter;
typedef std::tr1::shared_ptr<Plotter> PlotterShared;
class Plotter {
    private://------------------------------------------------------------------
    
    //! A stringstream instance; to be used to write a file or to print to stdio. 
    std::stringstream _stream;

    public://-------------------------------------------------------------------

    explicit Plotter();
    ~Plotter();

    //! Write the plot to internal storage, given a vector of data and a _output_count. The data is assumed to be adjacent, not interleaved. 
    void plot(const std::vector<SampleType>& v, OutputCountType d, 
                bool interleaved=true); 

    //! Print the plot to standard out. 
    void print();

    //! Pipe the the plot to gnuplot directly.
    void pipe();
    
    //! Write the plot to a file given by the file path argument. 
    void write(const std::string& fp);

};




} // end namespace aw

#endif // ends _AW_PLOTTER_H_
