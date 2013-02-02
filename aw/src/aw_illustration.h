#ifndef _AW_PLOTTER_H_
#define _AW_PLOTTER_H_

#include <tr1/memory>
#include <vector>
#include <sstream>

#include "aw_common.h"
#include "aw_illustration.h"
#include "aw_generator.h"



namespace aw {

//! Common base class for graphing routines.
class Illustration {

    protected://------------------------------------------------------------------
    //! A stringstream instance; to be used to write a file or to print to stdio. 
    std::stringstream _stream;

    explicit Illustration();
    
    virtual ~Illustration();

    public://-------------------------------------------------------------------
    //! Write the data to the stringstream. 
//    virtual void draw(const std::vector<SampleType>& v);
//    
//    virtual void draw(const std::vector<SampleType>& v, OutputCountType d);

    //! Replace all other overrdies with this
    virtual void draw(const GeneratorShared g);


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
class TimeDomainGraph : public Illustration{
    
    public://-------------------------------------------------------------------

    explicit TimeDomainGraph();
    
    virtual ~TimeDomainGraph();

    //! Low level drawing routine, assumes 1 dimension of output. Useful for calling on things other than Generators.
    virtual void draw_vector(const std::vector<SampleType>& v, 
    OutputCountType d=1);


    //! Write the plot to internal storage, given a vector of data and a _output_count. The data is assumed to be adjacent, not interleaved.     
    virtual void draw(const GeneratorShared g);

    
//    virtual void draw(const std::vector<SampleType>& v, OutputCountType d); 
//
//    virtual void draw(const std::vector<SampleType>& v); 

    //! Pipe the the plot to gnuplot directly.
    virtual void pipe();
    
};



//! An interface to Graphiz writing.
class NetworkGraph;
typedef std::tr1::shared_ptr<NetworkGraph> NetworkGraphShared;
class NetworkGraph : public Illustration{
    
    public://-------------------------------------------------------------------

    explicit NetworkGraph();
    
    virtual ~NetworkGraph();
    
    virtual void draw(const GeneratorShared g);
    

    //! Write the plot to internal storage, given a vector of data and a _output_count. The data is assumed to be adjacent, not interleaved. 
//    virtual void draw(const std::vector<SampleType>& v, OutputCountType d); 
//
//    virtual void draw(const std::vector<SampleType>& v); 

    //! Pipe the the plot to gnuplot directly.
    virtual void pipe();
    
};







} // end namespace aw

#endif // ends _AW_PLOTTER_H_
