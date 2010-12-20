/*! \file aw_Output.cpp
    \brief Object model of output stream
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <sndfile.hh>
#include "aw_System.h"




namespace aw {


// =============================================================================
System :: System(int sr, int bs)
    : samplingRate_(sr),
      blockSize_(bs)
{
}


// =============================================================================
int System :: getSamplingRate() const
{
    return samplingRate_;
}

// =============================================================================
int System :: getBlockSize() const
{
    return blockSize_;
}



} // end namespace aw
