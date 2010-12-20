/*! \file aw_GeneratorFactory.cpp
    \brief Object model of output stream
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include "aw_GeneratorFactory.h"
#include "aw_Generator.h"
#include "aw_Common.h"

#include "aw_Add.h"
#include "aw_Constant.h"
#include "aw_WaveSquare.h"
#include "aw_WaveSine.h"
#include "aw_RandomUniform.h"
#include "aw_Multiply.h"
#include "aw_FilterLowPass.h"
#include "aw_FilterHighPass.h"
#include "aw_Click.h"
#include "aw_PolyConstant.h"
#include "aw_PolyAdd.h"
#include "aw_Selector.h"
#include "aw_PanStereo.h"
#include "aw_PolyAdd.h"




namespace aw {


// =============================================================================
GeneratorFactory :: GeneratorFactory(aw::SystemPtr o)
    : sys_(o) // initialize
{
}

// see here for more ideas
//http://stackoverflow.com/questions/333400/how-to-design-a-simple-c-object-factory


// =============================================================================
// note: this are in the order presented in the GeneratorName enumerator
GeneratorPtr GeneratorFactory :: create(aw::GeneratorName gn)
{
    switch (gn) {     
        case aw::gNameAdd: {
            aw::AddPtr g(new aw::Add(sys_));
            return g;
            }
        case aw::gNameMultiply: {
            aw::MultiplyPtr g(new aw::Multiply(sys_));
            return g;
            }
        case aw::gNameConstant: {
            aw::ConstantPtr g(new aw::Constant(sys_));
            return g;
            }
        case aw::gNameRandomUniform: {
            aw::RandomUniformPtr g(new aw::RandomUniform(sys_));
            return g;
            }
        case aw::gNameWaveSine: {
            aw::WaveSinePtr g(new aw::WaveSine(sys_));
            return g;
            }
        case aw::gNameWaveSquare: {
            aw::WaveSquarePtr g(new aw::WaveSquare(sys_));
            return g;
            }
        case aw::gNameFilterLowPass: {
            aw::FilterLowPassPtr g(new aw::FilterLowPass(sys_));
            return g;
            }
        case aw::gNameFilterHighPass: {
            aw::FilterHighPassPtr g(new aw::FilterHighPass(sys_));
            return g;
            }
        case aw::gNameClick: {
            aw::ClickPtr g(new aw::Click(sys_));
            return g;
            }
        case aw::gNamePolyConstant: {
            aw::PolyConstantPtr g(new aw::PolyConstant(sys_));
            return g;
            }
        case aw::gNameSelector: {
            aw::SelectorPtr g(new aw::Selector(sys_));
            return g;
            }
        case aw::gNamePanStereo: {
            aw::PanStereoPtr g(new aw::PanStereo(sys_));
            return g;
            }
        case aw::gNamePolyAdd: {
            aw::PolyAddPtr g(new aw::PolyAdd(sys_));
            return g;
            }

    };
    // compiler warning here for not having an output
}



// -----------------------------------------------------------------------------
//! String version of create(). All other string-like entries use this
GeneratorPtr GeneratorFactory :: create(std::string& gn)
{
    // remove all spaces and returns
    aw::scrubString(gn);

    if (aw::stringWellFormed(gn, &aw::charPmtrOpen, &aw::charPmtrClose) == 
        false) {
        throw std::invalid_argument("malformed parameter string cannot be used to create a generator: " + gn);
    }
    
    //std::cout << "GeneratorFactory::create(std::string& gn): " << gn << std::endl;

    // convert string to a GeneratorName, pass to create
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;

    aw::getFirstParameterTrio(gn, name, arguments, context, working);

    //std::cout << "GeneratorFactory::create(std::string& gn)::arguments: " << arguments << std::endl;


    // converter string to enum name
    GeneratorPtr g = create(aw::stringToGeneratorName(name));

    // next, read and set arguments and context
    if (arguments != "")
        g->readParameterString(arguments);

    return g;
}


// -----------------------------------------------------------------------------
//! Create a Generator give a character array. Calls string version of create().
GeneratorPtr GeneratorFactory :: create(char* const charArray)
{
    // convert string to a GeneratorName, pass to create
    // no direct conversion is necessary here
    std::string gn(charArray);
    GeneratorPtr g = create(gn); // pass to string version
    return g;
}


} // end namespace aw
