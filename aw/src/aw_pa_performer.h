#ifndef _AW_PA_PERFORMER_H_
#define _AW_PA_PERFORMER_H_

#include <iostream>
#include "portaudiocpp/PortAudioCpp.hxx"
#include "aw_generator.h"


namespace aw {


//! The port-audio performer.
class PAPerformer {

    public://-------------------------------------------------------------------

    struct Callback {
        //! Storage for callback
        GenPtr root_gen;
        RenderCountT render_count;
        RenderCountT channels;

        //! Callback routine
        int render_mono(
                const void* inputBuffer,
                void* outputBuffer, 
                unsigned long framesPerBuffer, 
                const PaStreamCallbackTimeInfo* timeInfo,
                PaStreamCallbackFlags statusFlags
                );
        
        int render_stereo(
                const void* inputBuffer,
                void* outputBuffer, 
                unsigned long framesPerBuffer, 
                const PaStreamCallbackTimeInfo* timeInfo,
                PaStreamCallbackFlags statusFlags
                );
    };

    typedef portaudio::MemFunCallbackStream<Callback>
        PACallbackStream;
    
    private://-----------------------------------------------------------------

    //! The prepared callback instances is stored.
    Callback _callback;

    //! We extract an environemnt from the generator apssed in at creation
    EnvironmentPtr _environment;
    
    
    public://------------------------------------------------------------------

    PAPerformer() = delete;

    //! Pass in a GenPtr on creation
    PAPerformer(GenPtr g);
    
    //! Call with the duration of the performance in seconds.
    int operator()(int dur);

};

















} // end namespace
#endif // ends _AW_PERFORMER_H_





