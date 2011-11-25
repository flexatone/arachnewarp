/*! \file aw_Output.cpp
    \brief Object model of output stream
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <sndfile.hh>
#include <boost/scoped_array.hpp>
#include <vector>
#include <string>

#include "aw_Input.h"
#include "aw_Generator.h"




namespace aw {


// =============================================================================
Input :: Input(aw::SystemPtr o)
    : sys_(o) // initialize
{
}



// =============================================================================
int Input :: read(char* fp, aw::DynamicTable& table)
{

	// char* outfilename="/Volumes/xdisc/_sync/_x/src/arachneWarp/out/foo.wav";
    // should determine file format by file extension

    // sample format is indepedent of System settings

	int unsigned format;
	int unsigned channels;
	int unsigned samplerate;
	int unsigned frames;

    int unsigned totalSamples;

	SndfileHandle file;
	file = SndfileHandle(fp);
	if (not file) return -1;

    samplerate = file.samplerate();
    channels = file.channels();
    frames = file.frames();
    //format = file.format();

    std::clog << "opened file: " << fp << " channels: " << channels << " samplerate: " << samplerate << " frames: " << frames << std::endl;

    // for stereo files, data is given interleaved, one data point for each channel for each frame
    // total number of data points is channels * frame
    totalSamples = frames * channels;
    // note: this will always clear and resize the vector
    table.clear();
    table.reserve(totalSamples);


    // use a constant buffer to read data from file
    const int BUFFER_LEN = 1024; // set to 1024 or similar
    static double buffer[BUFFER_LEN];

    int i = 0;
    while (i < totalSamples) {
        //std::clog << "i pos: " << i << std::endl;

        // get a buffer's worth of data into the array
        file.read(buffer, BUFFER_LEN);    
        for (int j=0; j<BUFFER_LEN; j++) {
            //std::clog << buffer[j] << std::endl;
            // get values from buffer and push onto vector
            table.push_back(buffer[j]);
            i += 1;
            if (i >= totalSamples) break;
        }
    }
    // returns number of channels
    return channels;
}



// =============================================================================
int Input :: read(std::string& fp, aw::DynamicTable& table)
{
    int ch;
    char* fpChar = new char[fp.size()+1];
    strcpy(fpChar, fp.c_str());
    ch = read(fpChar, table);
    delete[] fpChar; // must delete newly created char
    return ch;

    // cannot use this alone b/c c_str is a const char*
    //return read(fp.c_str(), table);
}


} // end namespace aw
