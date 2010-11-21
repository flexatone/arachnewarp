/*! \file aw_WaveSquare.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_PAN_STEREO_H
#define AW_PAN_STEREO_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "aw_Generator.h"
#include "aw_PolyGenerator.h"


class PanStereo; // forward declarator

typedef boost::shared_ptr<PanStereo> PanStereoPtr;

//! PolyGenerator that returns two values, one for each channel. Pan values, as defined by aw::pNamePanLeftRight, are always between 0 and 1. 

class PanStereo: public PolyGenerator 
{
public:

    PanStereo(SystemPtr sys); 

    ~PanStereo();

    void init();

    void reset();

    std::string getName();

    aw::WorkingArrayPtr getPolyAtSample(aw::SampleTimeType);


private:

    //! Store current pan position   
    double panPos_;
    //! Store amp derive from operator
    double amp_;
   
} ;



#endif



