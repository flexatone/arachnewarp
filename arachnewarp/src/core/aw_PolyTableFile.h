/*! \file aw_WaveSquare.h
    \brief header
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#ifndef AW_POLY_TABLE_FILE_H
#define AW_POLY_TABLE_FILE_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "aw_Generator.h"
#include "aw_PolyGenerator.h"
#include "aw_Common.h"
#include "aw_Input.h"




namespace aw {

    class PolyTableFile; // forward declarator
    typedef boost::shared_ptr<PolyTableFile> PolyTableFilePtr;

    //! A PolyGenerator of stored of values derived from an audio file, where data points are bundled into groups per frame and output as a poly. This can handle n-channel audio files.
    class PolyTableFile: public aw::PolyGenerator 
    {
    public:
    
        PolyTableFile(aw::SystemPtr sys); 
    
        ~PolyTableFile();
    
        std::string getName();
    
        std::string getParameterString(bool compact=false);
    
        void init();
    
        void reset();
    
    
        //! override form on Generator
        void setParameter(const aw::ParameterName p, 
                          std::string& fpString, 
                          aw::ParameterContext pc=aw::pContextNameNone);
    
        //! override form on Generator
        void setParameter(const aw::ParameterName p, 
                          char* const fpCharArray, 
                          aw::ParameterContext pc=aw::pContextNameNone);
    
        //! overload Generator form
        void setParameter(std::string& pString, 
                          std::string& vString, 
                          std::string& pcString);
    
        // overload Generator form
        void readParameterString(std::string& ps);
    
        aw::WorkingArrayPtr getPolyAtSample(aw::SampleTimeType);
    
    private:
    
        // store file path provided as an argument
        std::string fp_;
        // store data from file loaded on set parameter
        aw::DynamicTable table_;
    
        // keep an input instance with each class
        InputPtr input_;  //

    
    } ; // end class PolyTableFile
    


} // end namespace aw



#endif



