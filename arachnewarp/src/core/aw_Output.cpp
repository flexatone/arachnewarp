/*! \file aw_Output.cpp
    \brief Object model of output stream
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <sndfile.hh>

#include <boost/scoped_array.hpp>


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

    // number of channels is dependent on generator type
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
    // dur is given in floating point seconds:
    // means this is being casted to an integer
	aw::Int64Unsigned sampleCount = sys_->getSamplingRate() * dur;


    std::clog << "about to write samples samples: " << sampleCount << 
    " gen->getGeneratorType(): " << gen->getGeneratorType() << std::endl;

    // must declare sample array within scope of each branch
    if (gen->getGeneratorType() == aw::gTypeMono) {

        // other methods of creating storage array, pre file writing, led 
        // to problems over a certain size; thus, a dynamic array must be used
        // the boost scoped array manages deletion
        //float *sample = new float [sampleCount];
        //float sample[sampleCount]; // this fails over a certain size
        boost::scoped_array<float> sample(new float[sampleCount]);
        // write each sample
        for (int i=0; i<sampleCount; i++) {
            //std::clog<< "writing samples: " << i << std::endl;
            sample[i] = gen->getValueAtSample(i);
        }
        // pass a pointer to the first array index, as well as the size
        outfile.write(&sample[0], sampleCount);
    }
    else {
        // interleaved, twice as large
        //const unsigned int size = sampleCount * 2; 
        //float sample[size];

        boost::scoped_array<float> sample(new float[sampleCount * 2]);

        // get a pointer to double 
        aw::WorkingArrayPtr outPoly;
        for (int i=0; i<sampleCount; i++) {
            // pointer to poly array
            outPoly = gen->getPolyAtSample(i);
            // get first two values in array
            sample[2*i] = outPoly[0]; 
            sample[(2*i)+1] = outPoly[1];
        }
        outfile.write(&sample[0], sampleCount * 2);
    }


    return 0;

}


int Output :: write(GeneratorPtr gen, double dur, char* fp)
{

	int format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    return write(gen, dur, fp, format);

}


int Output :: write(GeneratorPtr gen, double dur)
{

	char* fp="/Volumes/xdisc/_sync/_x/src/arachneWarp/out/foo.wav";
	int format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    return write(gen, dur, fp, format);

}
