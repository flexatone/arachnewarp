



// g++-4.7 -std=c++11 aw_generator_rt.cpp -I ../src ../src/aw_generator.cpp ../src/aw_common.cpp ../src/aw_illustration.cpp ../src/aw_timer.cpp -l boost_filesystem -l boost_system -l sndfile -l portaudio -l portaudiocpp -Wall -O3 -o aw_generator_rt




#include <iostream>
#include "portaudiocpp/PortAudioCpp.hxx"

#include "aw_generator.h"


// ----------------------------------------------------------------------------


// Some constants:
const int NUM_SECONDS = 5;
const double SAMPLE_RATE = 44100.0;
const int FRAMES_PER_BUFFER = 64;


// ----------------------------------------------------------------------------

// a wrapper for calling arachnewarp generators
class AWPerformer {
public:
    aw::GenPtr root_gen;
    aw::RenderCountT render_count;
    
	explicit AWPerformer() :
            render_count(0) {

        // setup objects
        aw::EnvironmentPtr e = aw::Environment::make_with_frame_size(
                FRAMES_PER_BUFFER);

        aw::GenPtr g1 = aw::Gen::make_with_environment(
                aw::GenID::Sine, e);
        220 >> g1;
        
        // rate of fm
        aw::GenPtr gmod = aw::Gen::make_with_environment(
                aw::GenID::Sine, e);
        8 >> gmod;

        aw::GenPtr gmap = aw::Gen::make_with_environment(
                aw::GenID::Map, e);
        // source
        gmap->set_input_by_index(0, gmod);
        gmap->set_input_by_index(1, -1);
        gmap->set_input_by_index(2, 1);
        // dst
        gmap->set_input_by_index(3, 880+440);
        gmap->set_input_by_index(4, 880-440);
        
        
        aw::GenPtr g2 = aw::Gen::make_with_environment(
                aw::GenID::Sine, e);
        gmap >> g2;

        aw::GenPtr genv = aw::Gen::make(
                aw::GenID::AttackDecay);
        genv->set_input_by_index(1, .25); // atack/decay in sec
        genv->set_input_by_index(2, .25);
        genv->set_input_by_index(4, 1); // self cycle mode
        
        root_gen = (g1 + g2) * genv;
        
        std::cout << "initialized root_gen" << std::endl;
        root_gen->print_inputs();
        
	}
    
    // generate the wave form
	int generate(
            const void* inputBuffer, // will not change input
            void* outputBuffer, // will write ti output
            unsigned long framesPerBuffer, 
            const PaStreamCallbackTimeInfo* timeInfo, 
            PaStreamCallbackFlags statusFlags) {
        
        root_gen->render(render_count);
        ++render_count;
        
        // cast to a multi-dimensionalal array
		float** out = static_cast<float **>(outputBuffer);
		for (unsigned int i=0; i < framesPerBuffer; ++i) { 
			out[0][i] = root_gen->outputs[0][i];
			out[1][i] = root_gen->outputs[0][i];

		}
		return paContinue;
	}

};





// ---------------------------------------------------------------------------
// main:
int main(int, char *[]);
int main(int, char *[])
{
	try
	{
		// Create a SineGenerator object:
		AWPerformer awp;

		std::cout << "Setting up PortAudio..." << std::endl;

		// Set up the System:
		portaudio::AutoSystem autoSys;
		portaudio::System& sys = portaudio::System::instance();

		// Set up the parameters required to open a (Callback)Stream:
		portaudio::DirectionSpecificStreamParameters outParams(
                sys.defaultOutputDevice(),
                2,
                portaudio::FLOAT32,
                false,
                sys.defaultOutputDevice().defaultLowOutputLatency(),
                NULL);

		portaudio::StreamParameters params(
                portaudio::DirectionSpecificStreamParameters::null(),
                outParams,
                SAMPLE_RATE,
                FRAMES_PER_BUFFER,
                paClipOff);

		// pass function call back
		portaudio::MemFunCallbackStream<AWPerformer> stream(
                params,
                awp, // pass instance
                &AWPerformer::generate); // pass function by reference


		std::cout << "Starting playback" << std::endl;
		stream.start();
		sys.sleep(NUM_SECONDS * 1000); // sleep sustains playback:

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


