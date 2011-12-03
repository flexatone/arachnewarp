/*! \file aw_Output.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/

#ifndef AW_SYSTEM_H    
#define AW_SYSTEM_H

#include <boost/shared_ptr.hpp>

// #include "aw_Common.h"


namespace aw {

class System; // forward declaration
typedef boost::shared_ptr<System> SystemPtr;

//! Object model of system and output data stream.
class System {
    public:
        //! Constructor with default sampling rate and block size.
        System(int sr=44100, int bs=8); 
    
        //! Return the current sampling rate.
        int getSamplingRate() const;
    
        //! Return the block size, or the number of values written in a block call. Note: not presently in use.
        int getBlockSize() const;
    
    protected:
        // A System can store references buffers as well; these buffers will then
        // be available to all Generators
    
    private:
        int samplingRate_;
        // used for block based calculations; not yet full implemented
        int blockSize_;

} ; // end class System


} // end namespace aw




#endif





