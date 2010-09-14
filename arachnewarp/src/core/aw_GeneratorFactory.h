/*! \file aw_GeneratorFactory.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/

#ifndef AW_GENERATOR_FACTORY_H    
#define AW_GENERATOR_FACTORY_H

#include <boost/shared_ptr.hpp>

#include "aw_Common.h"
#include "aw_Generator.h"

//! Object to create Generators
class GeneratorFactory
{
public:

    GeneratorFactory(SystemPtr o); 

    GeneratorPtr create(aw::GeneratorName gn);

    GeneratorPtr create(std::string& gnStr);

    GeneratorPtr create(char* const charArray);


private:

    SystemPtr sys_;

} ;



#endif





