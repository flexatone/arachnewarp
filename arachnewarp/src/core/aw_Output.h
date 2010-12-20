/*! \file aw_Output.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_OUTPUT_H    
#define AW_OUTPUT_H

#include <boost/shared_ptr.hpp>
#include <sndfile.hh>

#include "aw_Common.h"
#include "aw_Generator.h"


// sndfile.h for details
// combine a major and minor format with | operator
// enum AudioFileFormatMajor {
// 	sfFormatWave			= 0x010000,
// 	sfFormatAiff			= 0x020000,		
//     };
// 
// enum AudioFileFormatMinor {
// 	sfFormatPcm16		= 0x0002,	
// 	sfFormatPcm24		= 0x0003,	
//     };




namespace aw {


    //! Object model of system and output data stream.
    class Output
    {
    public:
    
        Output(aw::SystemPtr o); 
    
        int write(aw::GeneratorPtr gen, double dur, char* fp, int fileFormat);
    
        int write(aw::GeneratorPtr gen, double dur, char* fp);
    
        int write(aw::GeneratorPtr gen, double dur);
    
    
    private:
    
        SystemPtr sys_;
    
    } ; // end class Output


} // end namespace aw


#endif





