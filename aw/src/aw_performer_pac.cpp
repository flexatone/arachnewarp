
#include "aw_performer_pac.h"

namespace aw {

	
void pacb_stream_finished(void* userData) {
    //PACBData* data = (PACBData*)userData;
    //std::cout << "stream_finished" << std::endl;
}
    
    
int pacb_render_mono(
       const void *inputBuffer,
       void* outputBuffer,
       unsigned long framesPerBuffer,
       const PaStreamCallbackTimeInfo* timeInfo,
       PaStreamCallbackFlags statusFlags,
       void* userData
        ) {
    
    PACBData* data = static_cast<PACBData*>(userData);
    float* out = static_cast<float *>(outputBuffer);
    
    if (data->pre_roll_render_count < data->pre_roll_render_max) {
        data->pre_roll_render_count += framesPerBuffer;
        for (unsigned int i=0; i < framesPerBuffer; ++i) {
            *out++ = 0;
            *out++ = 0;
        }
        return paContinue;
    }
    
    (data->root_gen)->render(data->render_count);
    ++(data->render_count);
    for (unsigned int i=0; i < framesPerBuffer; ++i) {
        *out++ = (data->root_gen)->outputs[0][i];
        *out++ = (data->root_gen)->outputs[0][i];

    }
    return paContinue;
}


int pacb_render_stereo(
        const void *inputBuffer,
        void* outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* userData
        ) {

    PACBData* data = static_cast<PACBData*>(userData);
    float* out = static_cast<float *>(outputBuffer);
    
    if (data->pre_roll_render_count < data->pre_roll_render_max) {
        data->pre_roll_render_count += framesPerBuffer;
        for (unsigned int i=0; i < framesPerBuffer; ++i) {
            *out++ = 0;
            *out++ = 0;
        }
        return paContinue;
    }
    
    (data->root_gen)->render(data->render_count);
    ++(data->render_count);
    for (unsigned int i=0; i < framesPerBuffer; ++i) {
        *out++ = (data->root_gen)->outputs[0][i];
        *out++ = (data->root_gen)->outputs[1][i];
        
    }
    return paContinue;
}


    
    
    
    
PAPerformer :: PAPerformer(GenPtr g) {
    // pass in a generator at creation
    _cb_data.root_gen = g;
    _cb_data.channels = g->get_output_count();
    _environment = g->get_environment();
}

int PAPerformer :: operator()(int dur) {
    // reset for each performance
    _cb_data.render_count = 0;
    _cb_data.pre_roll_render_count = 0;
    _cb_data.pre_roll_render_max = (_pre_roll_seconds *
            _environment->get_sampling_rate());
    
    // create outside of scope
    auto f_callback = &pacb_render_mono;
    // pass function call back // pass function by reference
    if (_cb_data.channels == 2) {
        f_callback = &pacb_render_stereo;
    }
    else {
        std::cerr << "channel count not handled" << std::endl;
    }
    
    PaStreamParameters outputParameters;
    PaStream* stream;
    PaError err;
    err = Pa_Initialize();
    if( err != paNoError ) goto error;
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        //fprintf(stderr,"Error: No default output device.\n");
        std::cerr << "Error: No default output device." << std::endl;
        goto error;
    }
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(
            outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    err = Pa_OpenStream(
            &stream,
            NULL, // no input
            &outputParameters,
            _environment->get_sampling_rate(),
            _environment->get_common_frame_size(), // FRAMES_PER_BUFFER
            paClipOff, // don't bother clipping them
            f_callback,
            &_cb_data);
    
    if( err != paNoError ) goto error;
    
    err = Pa_SetStreamFinishedCallback(stream, &pacb_stream_finished);
    if( err != paNoError ) goto error;
    
    err = Pa_StartStream(stream);
    if( err != paNoError ) goto error;
    
    Pa_Sleep(dur * 1000); // sleep sustains playback:
    
    err = Pa_StopStream(stream);
    if( err != paNoError ) goto error;
    err = Pa_CloseStream(stream);
    if( err != paNoError ) goto error;
    
    Pa_Terminate();
    return err;
    
error:
    Pa_Terminate();
    std::cerr << "An error occured while using the portaudio stream." << std::endl;
    std::cerr << "Error number: " << err << std::endl;
    std::cerr << "Error message: " << Pa_GetErrorText(err) << std::endl;
    return err;
}
    
    




} // end namespace aw


