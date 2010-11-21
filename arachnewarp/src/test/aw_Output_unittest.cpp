/*! \file aw_Output_unittest.cpp
    \brief Object model of output stream
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/



#include <gtest/gtest.h>

#include "aw_System.h"
#include "aw_Common.h"
#include "aw_Output.h"
#include "aw_WaveSquare.h"
#include "aw_WaveSine.h"
#include "aw_RandomUniform.h"
#include "aw_Multiply.h"
#include "aw_GeneratorFactory.h"


// provide testCase, name
TEST(BasicTests, Output001) {


    SystemPtr sys(new System);

    Output out(sys); 

    WaveSquarePtr gen1(new WaveSquare(sys));
    RandomUniformPtr gen2(new RandomUniform(sys));

    // to write audio file, pass the gen to the Output

    gen1->setParameter(aw::pNameMinimum, -.9);
    gen1->setParameter(aw::pNameMaximum, .9);


    gen2->setParameter(aw::pNameMinimum, .01);
    gen2->setParameter(aw::pNameMaximum, .1);
    gen2->setParameter(aw::pNameRefresh, .5,
                      aw::pContextNamePeriodSeconds);


    gen1->setParameter(aw::pNameDutyCycle, gen2);

    gen1->setParameter(aw::pNameRate, 80,
                    aw::pContextNameFrequency);

    // out.write(gen1, 3);
    //gen1->printSample(6, 0, 4410);

}



TEST(BasicTests, Output002) {


    SystemPtr sys(new System(22050));

    Output out(sys); 


    // to write audio file, pass the gen to the Output
    WaveSinePtr gen1(new WaveSine(sys));
    gen1->setParameter(aw::pNameMinimum, -.9);
    gen1->setParameter(aw::pNameMaximum, .9);


    WaveSinePtr gen2(new WaveSine(sys));
    gen2->setParameter(aw::pNameMinimum, 250);
    gen2->setParameter(aw::pNameMaximum, 240);
    gen2->setParameter(aw::pNameRate, 1,
                      aw::pContextNamePeriodSeconds);

    gen1->setParameter(aw::pNameRate, gen2,
                    aw::pContextNameFrequency);

//     out.write(gen1, 10);
    //gen1->printSample(6, 0, 4410);

}




TEST(BasicTests, Output003) {


    SystemPtr sys(new System(44100));

    Output out(sys); 

    WaveSinePtr gen4(new WaveSine(sys));
    gen4->setParameter(aw::pNameMinimum, .01);
    gen4->setParameter(aw::pNameMaximum, .5);
    gen4->setParameter(aw::pNameRate, 4,
                      aw::pContextNamePeriodSeconds);


    // to write audio file, pass the gen to the Output
    WaveSinePtr gen1(new WaveSine(sys));
    gen1->setParameter(aw::pNameMinimum, 0);
    gen1->setParameter(aw::pNameMaximum, 1);
    gen1->setParameter(aw::pNameRate, gen4,
                      aw::pContextNamePeriodSeconds);


    WaveSinePtr gen5(new WaveSine(sys));
    gen5->setParameter(aw::pNameMinimum, 2);
    gen5->setParameter(aw::pNameMaximum, 200);
    gen5->setParameter(aw::pNameRate, 3,
                      aw::pContextNamePeriodSeconds);


    RandomUniformPtr gen2(new RandomUniform(sys));
    gen2->setParameter(aw::pNameMinimum, -.9);
    gen2->setParameter(aw::pNameMaximum, .9);
    gen2->setParameter(aw::pNameRefresh, gen5,
                      aw::pContextNamePeriodSamples);

    MultiplyPtr gen3(new Multiply(sys));
    gen3->setParameter(aw::pNameOperand1, gen1);
    gen3->setParameter(aw::pNameOperand2, gen2);

//     out.write(gen3, 8);
    //gen3->printSample(6, 0, 4410);

}






TEST(BasicTests, Output004) {

    SystemPtr sys(new System(44100));

    Output out(sys); 

    WaveSinePtr gen1(new WaveSine(sys));
    gen1->setParameter(aw::pNameMinimum, -.5);
    gen1->setParameter(aw::pNameMaximum, .5);
    gen1->setParameter(aw::pNameRate, 110,
                      aw::pContextNameFrequency);

    WaveSinePtr gen2(new WaveSine(sys));
    gen2->setParameter(aw::pNameMinimum, -.5);
    gen2->setParameter(aw::pNameMaximum, .5);
    gen2->setParameter(aw::pNameRate, 220,
                      aw::pContextNameFrequency);

    WaveSinePtr gen3(new WaveSine(sys));
    gen3->setParameter(aw::pNameMinimum, -.5);
    gen3->setParameter(aw::pNameMaximum, .5);
    gen3->setParameter(aw::pNameRate, 440,
                      aw::pContextNameFrequency);

    WaveSinePtr gen4(new WaveSine(sys));
    gen4->setParameter(aw::pNameMinimum, -.5);
    gen4->setParameter(aw::pNameMaximum, .5);
    gen4->setParameter(aw::pNameRate, 880,
                      aw::pContextNameFrequency);

// 
//     RandomUniformPtr gen2(new RandomUniform(sys));
//     gen2->setParameter(aw::pNameMinimum, -.9);
//     gen2->setParameter(aw::pNameMaximum, .9);
//     gen2->setParameter(aw::pNameRefresh, gen5,
//                       aw::pContextNamePeriodSamples);

    MultiplyPtr gen5(new Multiply(sys));
    gen5->setParameter(aw::pNameOperand1, gen1);
    gen5->setParameter(aw::pNameOperand2, gen2);
    gen5->setParameter(aw::pNameOperand3, gen3);
    gen5->setParameter(aw::pNameOperand4, gen4);


    // out.write(gen5, 6);
    //gen3->printSample(6, 0, 4410);

}








TEST(BasicTests, Output005) {

    SystemPtr sys(new System(22050)); // smart pointer
    GeneratorFactory gf(sys); // one instance
    Output out(sys); 


    GeneratorPtr gen4 = gf.create(aw::gNameWaveSine);
    gen4->setParameter(aw::pNameRate, .2, aw::pContextNameFrequency);
    gen4->setParameter(aw::pNameMinimum, .5);
    gen4->setParameter(aw::pNameMaximum, 8);


    GeneratorPtr gen1 = gf.create(aw::gNameWaveSine);
    gen1->setParameter(aw::pNameRate, gen4, aw::pContextNameFrequency);
    gen1->setParameter(aw::pNameMinimum, 0);
    gen1->setParameter(aw::pNameMaximum, 1);


    GeneratorPtr gen2 = gf.create(aw::gNameRandomUniform);
    gen2->setParameter(aw::pNameMinimum, -.8);
    gen2->setParameter(aw::pNameMaximum, .8);


    GeneratorPtr gen3 = gf.create(aw::gNameMultiply);
    gen3->setParameter(aw::pNameOperand1, gen1);
    gen3->setParameter(aw::pNameOperand2, gen2);


    GeneratorPtr gen6 = gf.create(aw::gNameWaveSine);
    gen6->setParameter(aw::pNameRate, 3.5, aw::pContextNamePeriodSeconds);
    // values in midi pitch 
    gen6->setParameter(aw::pNameMinimum, 40);
    gen6->setParameter(aw::pNameMaximum, 140);

    GeneratorPtr gen5 = gf.create(aw::gNameFilterLowPass);
    gen5->setParameter(aw::pNameRate, gen6, aw::pContextNamePitch);
    gen5->setParameter(aw::pNameOperand1, gen3);


    //out.write(gen5, 8);
    //gen1->printSample(4410, 0, 10);

}


// august 18 2010
TEST(BasicTests, Output006) {

    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance
    Output out(sys); 


    GeneratorPtr gen1 = gf.create("click{rate{constant{120}}{bpm}amp{constant{0.9}}}");

    GeneratorPtr gen2 = gf.create("click");
    gen2->setParameter("rate", 365, "bpm");
    gen2->setParameter("amp", .6);


    GeneratorPtr gen3a = gf.create("wavesine{rate{constant{5}}{sec}min{constant{.1}}max{constant{.6}}}");

    GeneratorPtr gen3 = gf.create("click");
    gen3->setParameter("rate", 1220, "bpm");
    gen3->setParameter("amp", gen3a);

    GeneratorPtr gen4 = gf.create("click{rate{constant{65}}{bpm}amp{constant{-0.8}}}");
//     gen4->setParameter("rate", 65, "bpm");
//     gen4->setParameter("amp", -.8);


    GeneratorPtr gen5 = gf.create("add");
    gen5->setParameter("op1", gen1);
    gen5->setParameter("op2", gen2);
    gen5->setParameter("op3", gen3);
    gen5->setParameter("op4", gen4);

    //out.write(gen5, 10);
//     gen1->printSample(4410, 0, 10);

}


// september 3 2010
TEST(BasicTests, Output007) {

    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance
    Output out(sys); 
    
    GeneratorPtr gen1 = gf.create("click{rate{constant{220}}{bpm}amp{wavesine{rate{constant{5}}{sec}min{constant{.1}}max{constant{.6}}}}}");
    
    EXPECT_EQ(gen1->getParameterString(), "Click{rate{Constant{220}}{bpm}amplitude{WaveSine{rate{Constant{5}}{seconds}minimum{Constant{0.1}}maximum{Constant{0.6}}}}}");
    
    GeneratorPtr gen2 = gf.create("panstereo{panleftright{wavesine{rate{2}{sec}min{0}max{1}}}}");
    
    EXPECT_EQ(gen2->getParameterString(), "PanStereo{operand1{Constant{0}}panLeftRight{WaveSine{rate{Constant{2}}{seconds}minimum{Constant{0}}maximum{Constant{1}}}}}");
    
    gen2->setParameter("op1", gen1);
    
    EXPECT_EQ(gen2->getParameterString(), "PanStereo{operand1{Click{rate{Constant{220}}{bpm}amplitude{WaveSine{rate{Constant{5}}{seconds}minimum{Constant{0.1}}maximum{Constant{0.6}}}}}}panLeftRight{WaveSine{rate{Constant{2}}{seconds}minimum{Constant{0}}maximum{Constant{1}}}}}");
    
    // compact form
    EXPECT_EQ(gen2->getParameterString(true), "PanStereo{operand1{Click{rate{220}{bpm}amplitude{WaveSine{rate{5}{seconds}minimum{0.1}maximum{0.6}}}}}panLeftRight{WaveSine{rate{2}{seconds}minimum{0}maximum{1}}}}");
    
    //out.write(gen2, 10);

}




// september 18 2010
TEST(BasicTests, Output008) {

    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance
    Output out(sys); 
    

//     "click{rate{220}{bpm}amp{wavesine{rate{.5}{sec}min{.01}max{.8}}}}");
        

    GeneratorPtr gen1 = gf.create(
    "filterLowPass { \
        op1{click{ \
            rate{selector{\
                    valueList{60, 120, 240, 480} \
                    selectionMethod{3} \
                    refresh{1}{sec} \
                    }\
                }{bpm}  \
            amp{wavesine{ \
                rate{3}{sec}  \
                min{.5} \
                max{.9} \
                    } \
                } \
            } \
        }\
        rate{wavesine{ \
                rate{4.5}{sec} \
                min{100} \
                max{6000} \
            } \
        }{frequency} \
    }");


    GeneratorPtr gen2 = gf.create(
    "click{ \
        rate{selector{\
                valueList{240, 480, 960} \
                selectionMethod{3} \
                refresh{2}{sec} \
                }\
            }{bpm}  \
        amp{wavesine{ \
            rate{2}{sec}  \
            min{.1} \
            max{.6} \
            } \
        } \
     }");


    GeneratorPtr gen3 = gf.create(
    "multiply{\
        op1{ \
            filterLowPass{ \
                op1{randomUniform{  \
                    refresh{1}{samples} \
                    min{-.1}    \
                    max{.1}    \
                    } \
                }\
                rate{wavesine{ \
                        rate{7.5}{sec} \
                        min{10} \
                        max{80} \
                    } \
                }{pitch} \
            } \
        } \
        op2{ \
            wavesine{ \
                rate{480}{bpm} \
                min{0} \
                max{.25} \
            } \
        } \
    }");

        
    GeneratorPtr gen4 = gf.create("add");
    gen4->setParameter("op1", gen1);
    gen4->setParameter("op2", gen2);
    gen4->setParameter("op3", gen3);

    GeneratorPtr gen5 = gf.create(" \
        panstereo{ \
            panleftright{ \
                wavesine{rate{2 \
                    }{sec} \
                min{0} \
                max{1} \
                } \
            } \
        }");
    gen5->setParameter("op1", gen4);
    
    EXPECT_EQ(gen5->getParameterString(true), "PanStereo{operand1{Add{operand1{FilterLowPass{operand1{Click{rate{Selector{valueList{60,120,240,480}selectionMethod{3}refresh{1}{seconds}}}{bpm}amplitude{WaveSine{rate{3}{seconds}minimum{0.5}maximum{0.9}}}}}rate{WaveSine{rate{4.5}{seconds}minimum{100}maximum{6000}}}{frequency}}}operand2{Click{rate{Selector{valueList{240,480,960}selectionMethod{3}refresh{2}{seconds}}}{bpm}amplitude{WaveSine{rate{2}{seconds}minimum{0.1}maximum{0.6}}}}}operand3{Multiply{operand1{FilterLowPass{operand1{RandomUniform{refresh{1}{samples}minimum{-0.1}maximum{0.1}}}rate{WaveSine{rate{7.5}{seconds}minimum{10}maximum{80}}}{pitch}}}operand2{WaveSine{rate{480}{bpm}minimum{0}maximum{0.25}}}operand3{1}operand4{1}}}operand4{0}}}panLeftRight{WaveSine{rate{2}{seconds}minimum{0}maximum{1}}}}");

    // over 20 here causes an bug
    // out.write(gen5, 20);
    //out.write(gen5, 20, 
    //"/Volumes/xdisc/_sync/_x/src/arachneWarp/out/aw2010-09-18.wav");

}


    // 0 (ordered cyclic forward), 1 (ordered cyclic reverse), 2 (ordered oscillate), 3 (random choice), 4 (random permutate), 5 (random walk)

// november 14 2010
TEST(BasicTests, Output009) {

    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance
    Output out(sys); 


    GeneratorPtr gen1 = gf.create(
    "click{ \
        rate{selector{\
                valueList{160, 80, 320, 640,  80, 640, 640} \
                selectionMethod{3} \
                refresh{12}{sec} \
                }\
            }{bpm}  \
        amp{selector{\
                valueList{.6, .1, .2, .8, .7, .1, .7, .1, .9, .1} \
                selectionMethod{5} \
                refresh{1}{samples} \
                } \
        } \
     }");

    GeneratorPtr gen1a = gf.create(
    "filterLowPass{ \
        rate{wavesine{ \
                rate{7.5}{sec} \
                min{80} \
                max{120} \
            } \
        }{pitch} \
    }");
    gen1a->setParameter("op1", gen1);


    GeneratorPtr gen2 = gf.create(
    "click{ \
        rate{selector{\
                valueList{160, 80, 320, 640, 80, 40, 640} \
                selectionMethod{2} \
                refresh{6}{sec} \
                }\
            }{bpm}  \
        amp{selector{\
                valueList{.6, .1, .2, 0, 0, .6, .3, .1, .5, 0, 0, .2, .3, .5} \
                selectionMethod{5} \
                refresh{1}{samples} \
                } \
        } \
     }");

    GeneratorPtr gen2a = gf.create(
    "filterLowPass{ \
        rate{wavesine{ \
                rate{3.2}{sec} \
                min{120} \
                max{140} \
            } \
        }{pitch} \
    }");
    gen2a->setParameter("op1", gen2);


    GeneratorPtr gen3 = gf.create("add");
    gen3->setParameter("op1", gen1a);
    gen3->setParameter("op2", gen2a);


    //out.write(gen3, 20, 
    //"/Volumes/xdisc/_sync/_x/src/arachneWarp/out/aw2010-11-14.wav");

}



// november 21 2010
TEST(BasicTests, Output010) {

    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance
    Output out(sys); 


    GeneratorPtr gen1 = gf.create(
    "click{ \
        rate{selector{\
                valueList{160, 80} \
                selectionMethod{3} \
                refresh{12}{sec} \
                }\
            }{bpm}  \
        amp{selector{\
                valueList{.6, .1, .2, .8, .7, .1, .7, .1, .9, .1} \
                selectionMethod{5} \
                refresh{1}{samples} \
                } \
        } \
     }");
    GeneratorPtr gen1b = gf.create(
    "filterLowPass{ \
        rate{selector{valueList{40,4000,20,20,19000} \
                refresh{2.0}{sec} \
                selectionMethod{3} \
            } \
        }{fq} \
    }");
    gen1b->setParameter("op1", gen1);
    GeneratorPtr gen1a = gf.create(" \
        panstereo{ \
            panleftright{0} \
        }");
    gen1a->setParameter("op1", gen1b);
    

//     EXPECT_EQ(gen1a->getParameterString(true), "PanStereo{operand1{Click{rate{Selector{valueList{160,80}selectionMethod{3}refresh{12}{seconds}}}{bpm}amplitude{Selector{valueList{0.6,0.1,0.2,0.8,0.7,0.1,0.7,0.1,0.9,0.1}selectionMethod{5}refresh{1}{samples}}}}}panLeftRight{0}}");


    GeneratorPtr gen2 = gf.create(
    "click{ \
        rate{selector{\
                valueList{360, 220, 2300} \
                selectionMethod{3} \
                refresh{12}{sec} \
                }\
            }{bpm}  \
        amp{selector{\
                valueList{.6, .1, .2, .8, .7, .1, .7, .1, .6, .1} \
                selectionMethod{5} \
                refresh{1}{samples} \
                } \
        } \
     }");
    GeneratorPtr gen2b = gf.create(
    "filterLowPass{ \
        rate{selector{valueList{40,400,20,20,12000} \
                refresh{3.5}{sec} \
                selectionMethod{3} \
            } \
        }{fq} \
    }");
    gen2b->setParameter("op1", gen2);
    GeneratorPtr gen2a = gf.create(" \
        panstereo{ \
            panleftright{1} \
        }");
    gen2a->setParameter("op1", gen2b);



    GeneratorPtr gen3 = gf.create(
    "click{ \
        rate{selector{\
                valueList{1200, 4800, 9600, 9600, 16000} \
                selectionMethod{3} \
                refresh{4}{sec} \
                }\
            }{bpm}  \
        amp{selector{\
                valueList{0, .1, .2, .8, .3, .4, .5, .6, .7, .8, .8, .8, .7, .6, .5, .4, .3, .2, .1, 0} \
                selectionMethod{3} \
                refresh{1.5}{sec} \
                } \
        } \
     }");
    GeneratorPtr gen3b = gf.create(
    "filterLowPass{ \
        rate{wavesine{ \
                rate{12}{sec} \
                min{20} \
                max{140} \
            } \
        }{pitch} \
    }");
    gen3b->setParameter("op1", gen3);
    GeneratorPtr gen3a = gf.create(" \
        panstereo{ \
            panleftright{ \
                wavesine{rate{4 \
                    }{sec} \
                min{.3} \
                max{.7} \
                } \
            } \
        }");
    gen3a->setParameter("op1", gen3b);


    GeneratorPtr gen4 = gf.create(
    "multiply{\
        op1{ \
            filterLowPass{ \
                op1{randomUniform{  \
                    refresh{1}{samples} \
                    min{-.8}    \
                    max{.8}    \
                    } \
                }\
                rate{wavesine{ \
                        rate{11}{sec} \
                        min{5} \
                        max{50} \
                    } \
                }{pitch} \
            } \
        } \
        op2{ \
            wavesine{ \
                rate{8}{sec} \
                min{0} \
                max{.3} \
            } \
        } \
    }");

    GeneratorPtr gen4a = gf.create(" \
        panstereo{ \
            panleftright{ \
                wavesine{rate{8 \
                    }{sec} \
                min{.15} \
                max{.85} \
                } \
            } \
        }");
    gen4a->setParameter("op1", gen4);

    GeneratorPtr gen5 = gf.create("polyAdd");
    gen5->setParameter("op1", gen1a);
    gen5->setParameter("op2", gen2a);
    gen5->setParameter("op3", gen3a);
    gen5->setParameter("op4", gen4a);


    //out.write(gen5, 20, "/Volumes/xdisc/_sync/_x/src/arachneWarp/out/aw2010-11-21.wav");

}





// =============================================================================

// scratch bed
TEST(BasicTests, Scratch) {

    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance
    Output out(sys); 

    GeneratorPtr gen3 = gf.create(aw::gNameRandomUniform);
    gen3->setParameter(aw::pNameMinimum, .6);
    gen3->setParameter(aw::pNameMaximum, .95);
// 
    GeneratorPtr gen1 = gf.create(aw::gNameWaveSine);
    gen1->setParameter(aw::pNameRate, 4, aw::pContextNamePeriodSeconds);
    gen1->setParameter(aw::pNameMinimum, .05);
    gen1->setParameter(aw::pNameMaximum, .4);
// 
//     GeneratorPtr gen5 = gf.create(aw::gNameFilterHighPass);
//     gen5->setParameter(aw::pNameOperand1, gen2); // noise
//     gen5->setParameter(aw::pNameRate, 18000, aw::pContextNameFrequency);
// 

    GeneratorPtr gen2 = gf.create(aw::gNameClick);
    gen2->setParameter(aw::pNameRate, gen1, aw::pContextNamePeriodSeconds);
    gen2->setParameter(aw::pNameAmplitude, gen3);


    // out.write(gen2, 8);
    //gen1->printSample(4410, 0, 10);

}








// =============================================================================


TEST(BasicTests, OutputProblemCase01) {

    // this shows how fq modulation continually increases
    // not a result of passing the wrong sample time to the new value GeneratorFactory
    
    // values provided by fq mod generator are correct, and not increasing
    
    // problem might be due to doing fq mod at non zero crossing

    SystemPtr sys(new System(22050)); // smart pointer
    GeneratorFactory gf(sys); // one instance
    Output out(sys); 


    GeneratorPtr gen4 = gf.create(aw::gNameWaveSine);
    gen4->setParameter(aw::pNameRate, 4, aw::pContextNameFrequency);
    gen4->setParameter(aw::pNameMinimum, 200);
    gen4->setParameter(aw::pNameMaximum, 202);

    // control frequency of this oscillator b/n 200 and 210
    GeneratorPtr gen1 = gf.create(aw::gNameWaveSine);
    gen1->setParameter(aw::pNameRate, gen4, aw::pContextNameFrequency);
    gen1->setParameter(aw::pNameMinimum, -.8);
    gen1->setParameter(aw::pNameMaximum, .8);


//     GeneratorPtr gen2 = gf.create(aw::gNameRandomUniform);
//     gen2->setParameter(aw::pNameMinimum, -.8);
//     gen2->setParameter(aw::pNameMaximum, .8);
// 
// 
//     GeneratorPtr gen3 = gf.create(aw::gNameMultiply);
//     gen3->setParameter(aw::pNameOperand1, gen1);
//     gen3->setParameter(aw::pNameOperand2, gen2);
// 

    // out.write(gen1, 8);

    //gen1->printSample(4410, 0, 10);

}
