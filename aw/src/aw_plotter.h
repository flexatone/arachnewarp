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
    std::stringstream _stream;

    public://-------------------------------------------------------------------

    explicit Plotter();
    ~Plotter();

    void plot(const std::vector<SampleType>& v, FrameDimensionType d, 
                bool interleaved=true); 

    void print();
    
    void write(const std::string& fp);

};




} // end namespace aw

#endif // ends _AW_PLOTTER_H_
