
#include "aw_alsa_performer.h"

// C headers; must inclide -l asound when compiling
#include <alsa/asoundlib.h>
#include <stdio.h>

#include <iostream>
#define PCM_DEVICE "default"


// tutorials here:
// http://alsa.opensrc.org/HowTo_Asynchronous_Playback

namespace aw {


ALSAPerformer :: ALSAPerformer(GenPtr g) {
    _root_gen = g;
    // _callback.channels = g->get_output_count();
    _environment = g->get_environment();
}

// note: these do not yet work

int ALSAPerformer :: operator()(int dur) {
    // http://www.alsa-project.org/main/index.php/FramesPeriods
    int dir;
    unsigned int rate = _environment->get_sampling_rate();
    unsigned int channels = 2;

    // the number of 
    RenderCountT period = _environment->get_common_frame_size();
    std::cout << "period: " << period << std::endl;

    // period is number of frames in between each hardware interrupt in bytes
    snd_pcm_uframes_t periodsize = period * 2;

    snd_pcm_t *pcm_handle;
    snd_pcm_uframes_t frames; // derived below 
 
    // open the PCM device in playback mode
    if (snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0) < 0) 
        printf("ERROR: Can't open \"%s\" PCM device.", PCM_DEVICE);
 
    // allocate parameters object and fill it with default values
    snd_pcm_hw_params_t *hw_params;    
    snd_pcm_hw_params_malloc(&hw_params); 
    snd_pcm_hw_params_any(pcm_handle, hw_params);
 
    // set parameters
    if (snd_pcm_hw_params_set_access(
            pcm_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) 
            fprintf(stderr, "ERROR: Can't set interleaved mode");
    if (snd_pcm_hw_params_set_format(
            pcm_handle, hw_params, SND_PCM_FORMAT_S16_LE) < 0) 
            fprintf(stderr, "ERROR: Can't set format.");
    if (snd_pcm_hw_params_set_rate_near(pcm_handle, hw_params, &rate, 0) < 0) 
            fprintf(stderr, "ERROR: Can't set rate.");
    if (snd_pcm_hw_params_set_channels(pcm_handle, hw_params, channels) < 0) 
            fprintf(stderr, "ERROR: Can't set channels number.");   

    // period size is in bytes
    snd_pcm_uframes_t buffer_size = periodsize * 4;
    snd_pcm_hw_params_set_buffer_size_near (pcm_handle, hw_params, &buffer_size);
    if (snd_pcm_hw_params_set_period_size_near(pcm_handle, hw_params, 
            &periodsize, &dir) < 0) 
            fprintf(stderr, "ERROR: Can't set channels number.");   

    // Set buffer size (in frames). The resulting latency is given by latency = periodsize * periods / (rate * bytes_per_frame)
    // if (snd_pcm_hw_params_set_buffer_size(
    //         pcm_handle, params, periodsize * periods) < 0) {
    //   fprintf(stderr, "Error setting buffersize.\n");
    //   return(-1);
    // }
    // write parameters
    if (snd_pcm_hw_params(pcm_handle, hw_params) < 0)
            printf("ERROR: Can't set harware parameters.\n");
    snd_pcm_hw_params_free (hw_params);

    // Allocate buffer to hold single period
    snd_pcm_hw_params_get_period_size(hw_params, &frames, 0);
 
    std::cout << "frames: " << frames << std::endl;

    // periodsize must include multiplier for channels; muliple by 2 for byte size
    char* data;    
    data = (char *)malloc(2 * periodsize * channels);

    RenderCountT render_count = 0;
    RenderCountT render_max = (dur * rate) / period;

    unsigned int period_pos = 0;

    snd_pcm_prepare(pcm_handle);

    while (render_count < render_max) {
        _root_gen->render(render_count);

        // transfer data
        for (unsigned int i=0; i < period; ++i) {
            // need to handle reading multiple channels if defined
            period_pos = i * 2;
            data[period_pos] = _root_gen->outputs[0][i] * 127;
            data[period_pos+1] = _root_gen->outputs[1][i] * 127;
            }
        // frames is siz ein bytes
        while (snd_pcm_writei(pcm_handle, data,  2 * periodsize * channels) < 0) {
            fprintf(stderr, "<<<< Buffer Underrun >>>>\n");
        }
        ++render_count;
        }


    // unsigned char* data;
    // unsigned int l1, l2;
    // short s1, s2;

    // data = (unsigned char *)malloc(periodsize);
    // for(l1 = 0; l1 < 200; l1++) {
    //     for(l2 = 0; l2 < frames; l2++) {
    //         s1 = (l2 % 128) * 100 - 5000;  
    //         s2 = (l2 % 256) * 100 - 5000;  
    //         data[4*l2] = (unsigned char)s1;
    //         data[4*l2+1] = s1 >> 8;
    //         data[4*l2+2] = (unsigned char)s2;
    //         data[4*l2+3] = s2 >> 8;
    //         }
    //     while ((pcm = snd_pcm_writei(pcm_handle, data, frames)) < 0) {
    //         snd_pcm_prepare(pcm_handle);
    //         fprintf(stderr, "<<<<<<<<<<<<<<< Buffer Underrun >>>>>>>>>>>>>>>\n");
    //     }
    // }

 
    snd_pcm_drain(pcm_handle);
    snd_pcm_close(pcm_handle);
    free(data);
 
    return 0;
}








} // end namespace aw


