#ifndef _AW_PLOTTER_H_
#define _AW_PLOTTER_H_

#include <tr1/memory>
#include <vector>
#include <sstream>

#include "aw_common.h"


namespace aw {

//! Common base class for graphing routines.
class Plotter {

    protected://------------------------------------------------------------------
    //! A stringstream instance; to be used to write a file or to print to stdio. 
    std::stringstream _stream;

    explicit Plotter();
    
    virtual ~Plotter();

    public://-------------------------------------------------------------------
    //! Write the data to the stringstream. 
    virtual void draw(const std::vector<SampleType>& v);
    
    virtual void draw(const std::vector<SampleType>& v, OutputCountType d);

    //! Pipe the the plot to gnuplot directly.
    virtual void pipe();

    // shared base class methods -----------------------------------------------
    //! Print the plot to standard out. 
    void print();
        
    //! Write the plot to a file given by the file path argument. 
    void write(const std::string& fp);

};



//! An interface to GNUplot writing. Rename this TimeDomain
class TimeDomainGraph;
typedef std::tr1::shared_ptr<TimeDomainGraph> TimeDomainGraphShared;
class TimeDomainGraph : public Plotter{
    
    public://-------------------------------------------------------------------

    explicit TimeDomainGraph();
    
    virtual ~TimeDomainGraph();

    //! Write the plot to internal storage, given a vector of data and a _output_count. The data is assumed to be adjacent, not interleaved. 
    virtual void draw(const std::vector<SampleType>& v, OutputCountType d); 

    virtual void draw(const std::vector<SampleType>& v); 

    //! Pipe the the plot to gnuplot directly.
    virtual void pipe();
    
};



//! An interface to Graphiz writing.
class NetworkGraph;
typedef std::tr1::shared_ptr<NetworkGraph> NetworkGraphShared;
class NetworkGraph : public Plotter{
    
    public://-------------------------------------------------------------------

    explicit NetworkGraph();
    
    virtual ~NetworkGraph();

    //! Write the plot to internal storage, given a vector of data and a _output_count. The data is assumed to be adjacent, not interleaved. 
    virtual void draw(const std::vector<SampleType>& v, OutputCountType d); 

    virtual void draw(const std::vector<SampleType>& v); 

    //! Pipe the the plot to gnuplot directly.
    virtual void pipe();
    
};







} // end namespace aw

#endif // ends _AW_PLOTTER_H_
