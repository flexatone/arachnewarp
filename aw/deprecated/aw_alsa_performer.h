#ifndef _AW_ALSA_PERFORMER_H_
#define _AW_ALSA_PERFORMER_H_

#include <alsa/asoundlib.h>

#include <iostream>
#include "aw_generator.h"



namespace aw {

//! The ALSA performer.
class ALSAPerformer {

    private://-----------------------------------------------------------------
    //! We extract an environemnt from the generator passed in at creation
    EnvironmentPtr _environment;
    GenPtr _root_gen;
    
    public://------------------------------------------------------------------

    ALSAPerformer() = delete;

    //! Pass in a GenPtr on creation
    ALSAPerformer(GenPtr g);
    
    //! Call with the duration of the performance in seconds.
    int operator()(int dur);

};

















} // end namespace
#endif // ends _AW_PERFORMER_H_





