
#include "aw_performer_pac.h"

namespace aw {

	
void PAPerformer :: Callback :: stream_finished(void* userData ) {
    //    paTestData *data = (paTestData *) userData;
    std::cout << "stream_finished" << std::endl;
}
    
    
int PAPerformer :: Callback :: render_mono(
       const void *inputBuffer,
       void *outputBuffer,
       unsigned long framesPerBuffer,
       const PaStreamCallbackTimeInfo* timeInfo,
       PaStreamCallbackFlags statusFlags,
       void *userData
        ) {
    
    // cast to a multi-dimensionalal array
    float** out = static_cast<float **>(outputBuffer);
    
    if (pre_roll_render_count < pre_roll_render_max) {
        pre_roll_render_count += framesPerBuffer;
        for (unsigned int i=0; i < framesPerBuffer; ++i) {
            out[0][i] = 0;
            out[1][i] = 0;
        }
        return paContinue;
    }
    
    root_gen->render(render_count);
    ++render_count;
    for (unsigned int i=0; i < framesPerBuffer; ++i) {
        // need to handle reading multiple channels if defined
        out[0][i] = root_gen->outputs[0][i];
        out[1][i] = root_gen->outputs[0][i];

    }
    return paContinue;
}


int PAPerformer :: Callback :: render_stereo(
        const void *inputBuffer,
        void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData
        ) {

    // cast to a multi-dimensionalal array
    float** out = static_cast<float **>(outputBuffer);
    
    if (pre_roll_render_count < pre_roll_render_max) {
        pre_roll_render_count += framesPerBuffer;
        for (unsigned int i=0; i < framesPerBuffer; ++i) {
            out[0][i] = 0;
            out[1][i] = 0;
        }
        return paContinue;
    }
    
    root_gen->render(render_count);
    ++render_count;
    for (unsigned int i=0; i < framesPerBuffer; ++i) {
        // need to handle reading multiple channels if defined
        out[0][i] = root_gen->outputs[0][i];
        out[1][i] = root_gen->outputs[1][i];
    }
    return paContinue;
}


    
    
    
    
PAPerformer :: PAPerformer(GenPtr g) {
    // pass in a generator at creation
    _callback.root_gen = g;
    _callback.channels = g->get_output_count();
    _environment = g->get_environment();
}

int PAPerformer :: operator()(int dur) {
    // reset for each performance
    _callback.render_count = 0;
    _callback.pre_roll_render_count = 0;
    _callback.pre_roll_render_max = (_pre_roll_seconds *
            _environment->get_sampling_rate());
    
    
    PaStreamParameters outputParameters;
    PaStream* stream;
    PaError err;
    err = Pa_Initialize();
    if( err != paNoError ) goto error;
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        goto error;
    }
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(
            outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    
    // create outside of scope
    auto f_callback = &PAPerformer::Callback::render_mono;
    // pass function call back // pass function by reference
    if (_callback.channels == 2) {
        f_callback = &PAPerformer::Callback::render_stereo;
    }
    else {
        std::cout << "channel count not handled" << std::endl;
    }
    
    err = Pa_OpenStream(
            &stream,
            NULL, // no input
            &outputParameters,
            _environment->get_sampling_rate(),
            _environment->get_common_frame_size(), // FRAMES_PER_BUFFER
            paClipOff, // don't bother clipping them
            f_callback,
            NULL);
    
    if( err != paNoError ) goto error;
    //sprintf( data.message, "No Message" );
    
    err = Pa_SetStreamFinishedCallback(stream,
            &PAPerformer::Callback::stream_finished);
    
    if( err != paNoError ) goto error;
    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;
    
    Pa_Sleep(dur * 1000); // sleep sustains playback:
    
    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;
    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;
    
    Pa_Terminate();
    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}
    
    
    
    
//	try {
//		std::cout << "setting up PortAudio..." << std::endl;
//		// Set up the System:
//		portaudio::AutoSystem autoSys;
//		portaudio::System& sys = portaudio::System::instance();
//
//        std::cout << "default output device..." << Pa_GetDeviceCount() <<std::endl;
//
//		// Set up the parameters required to open a (Callback)Stream:
//		portaudio::DirectionSpecificStreamParameters outParams(
//                sys.defaultOutputDevice(),
//                2,
//                portaudio::FLOAT32,
//                false, // interleaved
//                sys.defaultOutputDevice().defaultLowOutputLatency(),
//                NULL);
//
//		portaudio::StreamParameters params(
//                portaudio::DirectionSpecificStreamParameters::null(),
//                outParams,
//                _environment->get_sampling_rate(),
//                _environment->get_common_frame_size(),
//                paClipOff);
//
//        // create outside of scope
//        auto f_callback = &PAPerformer::Callback::render_mono;
//        // pass function call back // pass function by reference
//        if (_callback.channels == 2) {
//            f_callback = &PAPerformer::Callback::render_stereo;            
//        }
//        else {
//            std::cout << "channel count not handled" << std::endl;
//        }
//
//        PACallbackStream stream(
//            params,
//            _callback, // pass instance
//            f_callback);
//
//
//        std::cout << "Stream info: " << "stream.outputLatency(): " << 
//                stream.outputLatency() << std::endl;
//
//
//		std::cout << "Starting playback" << std::endl;
//		stream.start();
//		sys.sleep(dur * 1000); // sleep sustains playback:
//
//		stream.stop();
//		stream.close();
//		sys.terminate();
//	}
//	catch (const portaudio::PaException &e) {
//		std::cout << "A PortAudio error occured: " << e.paErrorText() << std::endl;
//	}
//	catch (const portaudio::PaCppException &e) {
//		std::cout << "A PortAudioCpp error occured: " << e.what() << std::endl;
//	}
//	catch (const std::exception &e) {
//		std::cout << "A generic exception occured: " << e.what() << std::endl;
//	}
//	catch (...) {
//		std::cout << "An unknown exception occured." << std::endl;
//	}
//	return 0;
// }








} // end namespace aw


