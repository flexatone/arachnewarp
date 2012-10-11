#ifndef _AW_PLOT_H_
#define _AW_PLOT_H_

#include <tr1/memory>
#include <vector>

#include "aw_common.h"


namespace aw {


//! An interface to GNUplot writing.
class Plotter;
typedef std::tr1::shared_ptr<Plotter> PlotterShared;
class Plotter {
    private://------------------------------------------------------------------

    public://-------------------------------------------------------------------
    explicit Plotter();
    ~Plotter();

    // make private; public now for testing    
    void _write(const std::vector<SampleType>& v, FrameDimensionType d, 
            const std::string& fp, bool interleaved=true); 
    

};




} // end namespace aw

#endif // ends _AW_PLOT_H_
