
#include "aw_pa_performer.h"

namespace aw {




int PAPerformer :: Callback :: render_mono(
        const void* inputBuffer, // will not change input
        void* outputBuffer, // will write ti output
        unsigned long framesPerBuffer, 
        const PaStreamCallbackTimeInfo* timeInfo, 
        PaStreamCallbackFlags statusFlags
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
        const void* inputBuffer, // will not change input
        void* outputBuffer, // will write ti output
        unsigned long framesPerBuffer, 
        const PaStreamCallbackTimeInfo* timeInfo, 
        PaStreamCallbackFlags statusFlags
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
    
	try {
		std::cout << "setting up PortAudio..." << std::endl;
		// Set up the System:
		portaudio::AutoSystem autoSys;
		portaudio::System& sys = portaudio::System::instance();

        std::cout << "default output device..." << Pa_GetDeviceCount() <<std::endl;

		// Set up the parameters required to open a (Callback)Stream:
		portaudio::DirectionSpecificStreamParameters outParams(
                sys.defaultOutputDevice(),
                2,
                portaudio::FLOAT32,
                false, // interleaved
                sys.defaultOutputDevice().defaultLowOutputLatency(),
                NULL);

		portaudio::StreamParameters params(
                portaudio::DirectionSpecificStreamParameters::null(),
                outParams,
                _environment->get_sampling_rate(),
                _environment->get_common_frame_size(),
                paClipOff);

        // create outside of scope
        auto f_callback = &PAPerformer::Callback::render_mono;
        // pass function call back // pass function by reference
        if (_callback.channels == 2) {
            f_callback = &PAPerformer::Callback::render_stereo;            
        }
        else {
            std::cout << "channel count not handled" << std::endl;
        }

        PACallbackStream stream(
            params,
            _callback, // pass instance
            f_callback);


        std::cout << "Stream info: " << "stream.outputLatency(): " << 
                stream.outputLatency() << std::endl;


		std::cout << "Starting playback" << std::endl;
		stream.start();
		sys.sleep(dur * 1000); // sleep sustains playback:

		stream.stop();
		stream.close();
		sys.terminate();
	}
	catch (const portaudio::PaException &e) {
		std::cout << "A PortAudio error occured: " << e.paErrorText() << std::endl;
	}
	catch (const portaudio::PaCppException &e) {
		std::cout << "A PortAudioCpp error occured: " << e.what() << std::endl;
	}
	catch (const std::exception &e) {
		std::cout << "A generic exception occured: " << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "An unknown exception occured." << std::endl;
	}
	return 0;
}








} // end namespace aw


