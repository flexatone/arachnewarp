/*! \file aw_Output.cpp
    \brief Object model of output stream
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <sndfile.hh>


#include "aw_Output.h"
#include "aw_Generator.h"


// =============================================================================
Output :: Output(SystemPtr o)
    : sys_(o) // initialize
{
}



// =============================================================================
int Output :: write(GeneratorPtr gen, double dur, char* fp, int fileFormat)
{

	// char* outfilename="/Volumes/xdisc/_sync/_x/src/arachneWarp/out/foo.wav";
    // should determine file format by file extension

    // sample format is indepedent of System settings

	int unsigned format = fileFormat;
	int unsigned channels;

    if (gen->getGeneratorType() == aw::gTypeMono) {
        channels = 1;
    }
    else { // a gTypePoly
        channels = 2;
    }    

	SndfileHandle outfile(fp, SFM_WRITE, format, channels, 
                          sys_->getSamplingRate());
	if (not outfile) return -1;
	// create an array of the necessary size based on sampling rate and dur
	const unsigned int sampleCount = sys_->getSamplingRate() * dur;


    // must declare sample array within scope of each branch
    if (gen->getGeneratorType() == aw::gTypeMono) {

        const unsigned int size = sampleCount;
        float sample[size];

        for (int i=0; i<sampleCount; i++) {
            sample[i] = gen->getValueAtSample(i);
        }
        outfile.write(&sample[0], size);
    }
    else {
        // interleaved, twice as large
        const unsigned int size = sampleCount * 2; 
        float sample[size];

        double* outPoly;
        for (int i=0; i<sampleCount; i++) {
            outPoly = gen->getPolyAtSample(i);
            sample[2*i] = outPoly[0];
            sample[(2*i)+1] = outPoly[1];
        }
        outfile.write(&sample[0], size);
    }


    return 0;

}


int Output :: write(GeneratorPtr gen, double dur)
{

	char* fp="/Volumes/xdisc/_sync/_x/src/arachneWarp/out/foo.wav";
	int format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    return write(gen, dur, fp, format);

}
