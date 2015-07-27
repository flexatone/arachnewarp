/*! \file aw_Output.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_INPUT_H    
#define AW_INPUT_H

#include <boost/shared_ptr.hpp>
#include <sndfile.hh>
#include <string>

#include "aw_Common.h"




namespace aw {


    class Input; // forward declaration
    typedef boost::shared_ptr<Input> InputPtr;
    

    //! Object model of system and output data stream.
    class Input
    {
    public:
    
        Input(aw::SystemPtr o); 
    
        //! Pass in by reference a dynamic table object
        int read(char* fp, aw::DynamicTable& table);

        int read(std::string& fp, aw::DynamicTable& table);
        
        // could have a stream() method for streaming audio in    

    private:
    
        SystemPtr sys_;
    
    } ; // end class Input


} // end namespace aw


#endif





