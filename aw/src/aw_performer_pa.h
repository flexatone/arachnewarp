#ifndef _AW_PAC_PERFORMER_H_
#define _AW_PAC_PERFORMER_H_

#include <iostream>
#include "portaudio.h"

#include "aw_generator.h"


namespace aw {


    namespace c_pa { // c tools for c port audio
        
    //! Storage for callback
    struct PACBData {
        GenPtr root_gen;
        // times the buffer size
        RenderCountT pre_roll_render_max;
        RenderCountT pre_roll_render_count;
        RenderCountT render_count;
        RenderCountT channels;
    };
        
    //! Callback routines
    int pacb_render_mono(
                       const void *inputBuffer,
                       void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void* userData
                       );

    int pacb_render_stereo(
                         const void *inputBuffer,
                         void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void* userData
                         );

    //! Completion function
    void pacb_stream_finished(void* userData);
        
    } // end namespace c_pa
    
    
//! The port-audio C-based performer.
class PAPerformer {

    public://-------------------------------------------------------------------
    
    private://-----------------------------------------------------------------

    //! The callback data instance
    c_pa::PACBData _cb_data;

    //! We extract an environemnt from the generator apssed in at creation
    EnvPtr _environment;

    //! This is the number of seconds of pre-roll silence before the root generator is called.
    unsigned int _pre_roll_seconds {2};
    
    public://------------------------------------------------------------------

    PAPerformer() = delete;

    //! Pass in a GenPtr on creation
    PAPerformer(GenPtr g);
    
    //! Call with the duration of the performance in seconds.
    int operator()(int dur);

};

















} // end namespace
#endif // ends _AW_PERFORMER_H_





