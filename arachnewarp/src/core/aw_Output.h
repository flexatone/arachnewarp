/*! \file aw_Output.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/

#include <boost/shared_ptr.hpp>

#include <sndfile.hh>

#include "aw_Common.h"
#include "aw_Generator.h"


#ifndef AW_OUTPUT_H    
#define AW_OUTPUT_H

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


//! Object model of system and output data stream.
class Output
{
public:

    Output(SystemPtr o); 

    int write(GeneratorPtr gen, double dur, char* fp, int fileFormat);

    int write(GeneratorPtr gen, double dur, char* fp);

    int write(GeneratorPtr gen, double dur);


private:

    SystemPtr sys_;

} ;



#endif





