/*! \file aw_Convert_unittest.cpp
    \brief Testing for aw_Convert.cpp
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include "aw_Common.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>


using namespace aw;



// provide testCase, name
TEST(BasicTests, DenormBasic) {

    // args are value, min, max
    EXPECT_EQ(aw::denormalize(0.0, 0.0, 1.0), 0);

    EXPECT_EQ(aw::denormalize(0.5, 0.0, 1.0), 0.5);
    EXPECT_EQ(aw::denormalize(0.5, 1.0, 0.0), 0.5);

    EXPECT_EQ(aw::denormalize(1.0, 0.0, 1.0), 1);

    EXPECT_EQ(aw::denormalize(0.5, -100.0, 100.0), 0);

    EXPECT_EQ(aw::denormalize(0.8, -100.0, 100.0), 60);

    // note that this handles ints just fine
    EXPECT_EQ(aw::denormalize(0.95, 0, 5), 4.75);

    EXPECT_EQ(aw::denormalize(0.5, 0, 5), 2.5);


    // if given value is outside of 0-1, throw an out of range exception
    ASSERT_THROW(aw::denormalize(-5), std::out_of_range);

}


// TEST(BasicTests, RandomRangeBasic) {
// 
//     // args are min, max
//     // always within range of 1 less than max
//     EXPECT_EQ(aw::randomRange(5, 6), 5);
//     EXPECT_EQ(aw::randomRange(0, 1), 0);
//     EXPECT_EQ(aw::randomRange(20000, 20001), 20000);
// 
//     std::vector<aw::Int32Signed> a;
//     for (int i=0; i<10; i++) {
//         a.push_back(aw::randomRange(0,10)); // values 0 to 9 generated
//     }
//     aw::printVector(a, "aw::randomRange between 0 and 9");
// 
// 
//     std::vector<int> b;
//     for (int i=0; i<10; i++) {
//         b.push_back(aw::randomRange(100,104)); // values 0 to 9 generated
//     }
//     aw::printVector(b, "aw::randomRange between 100 and 104");
// 
//     std::vector<int> c;
//     for (int i=0; i<10; i++) {
//         c.push_back(aw::randomRange(-20,-10)); // values 0 to 9 generated
//     }
//     aw::printVector(c, "aw::randomRange between -20 and -10");
// 
// 
// }


TEST(BasicTests, RandomIntegerRangeBasic) {

    // args are min, max
    // always within range of 1 less than max

    EXPECT_EQ(aw::randomIntegerRange(0, 0), 0);
    EXPECT_EQ(aw::randomIntegerRange(10, 10), 10);

    EXPECT_EQ(aw::randomIntegerRange(10, 20) < 21, true);
    EXPECT_EQ(aw::randomIntegerRange(10, 20) < 21, true);
    EXPECT_EQ(aw::randomIntegerRange(10, 20) < 21, true);
    EXPECT_EQ(aw::randomIntegerRange(10, 20) < 21, true);
    EXPECT_EQ(aw::randomIntegerRange(10, 20) < 21, true);
    EXPECT_EQ(aw::randomIntegerRange(10, 20) < 21, true);

    EXPECT_EQ(aw::randomIntegerRange(10, 20) > 9, true);
    EXPECT_EQ(aw::randomIntegerRange(10, 20) > 9, true);
    EXPECT_EQ(aw::randomIntegerRange(10, 20) > 9, true);
    EXPECT_EQ(aw::randomIntegerRange(10, 20) > 9, true);
    EXPECT_EQ(aw::randomIntegerRange(10, 20) > 9, true);
    EXPECT_EQ(aw::randomIntegerRange(10, 20) > 9, true);

    // test that sequential values are not the same
    aw::Int32Signed q1 = aw::randomIntegerRange(0, 20000);
    aw::Int32Signed q2 = aw::randomIntegerRange(0, 20000);
    aw::Int32Signed q3 = aw::randomIntegerRange(0, 20000);
    aw::Int32Signed q4 = aw::randomIntegerRange(0, 20000);

    EXPECT_EQ(q1 != q2, true);
    EXPECT_EQ(q2 != q3, true);
    EXPECT_EQ(q3 != q4, true);
    EXPECT_EQ(q1 != q4, true);

    std::vector<int> a;
    for (int i=0; i<10; i++) {
        a.push_back(aw::randomIntegerRange(0,20)); 
    }
    aw::printVector(a, "aw::randomIntegerRange between 0 and 20");

    std::vector<int> b;
    for (int i=0; i<10; i++) {
        b.push_back(aw::randomIntegerRange(-1000,1000)); 
    }
    aw::printVector(b, "aw::randomIntegerRange between -1000 and 1000");



//     std::vector<int> b;
//     for (int i=0; i<10; i++) {
//         b.push_back(aw::randomRange(100,104)); // values 0 to 9 generated
//     }
//     aw::printVector(b, "aw::randomRange between 100 and 104");
// 
//     std::vector<int> c;
//     for (int i=0; i<10; i++) {
//         c.push_back(aw::randomRange(-20,-10)); // values 0 to 9 generated
//     }
//     aw::printVector(c, "aw::randomRange between -20 and -10");


}



TEST(BasicTests, RandomUnit) {

    // args are min, max
    // always within range of 1 less than max
    EXPECT_EQ(aw::randomUnit() > 0, true);
    EXPECT_EQ(aw::randomUnit() < 1, true);

//     std::cout << "aw::randomUnit()" << aw::randomUnit() << std::endl; 
//     std::cout << "aw::randomUnit()" << aw::randomUnit() << std::endl; 
//     std::cout << "aw::randomUnit()" << aw::randomUnit() << std::endl; 



}



TEST(BasicTests, TimeConversions) {

    // args are min, max
    // always within range of 1 less than max
    EXPECT_EQ(aw::sampleTimeToSecondTime(44100, 44100), 1);
    EXPECT_EQ(aw::sampleTimeToSecondTime(66150, 44100), 1.5);

    // secont tim to sample time
    EXPECT_EQ(aw::secondTimeToSampleTime(2, 44100), 88200);


    // bpm conversions
    EXPECT_EQ(aw::bpmToSecondTime(60), 1.0);
    EXPECT_EQ(aw::bpmToSecondTime(120), 0.5);
    EXPECT_EQ(aw::bpmToSecondTime(30), 2.0);

}



TEST(BasicTests, EnumeratedToString) {
    EXPECT_EQ(aw::parameterContextToString(aw::pContextNameNone), 
        "none");

    EXPECT_EQ(aw::parameterContextToString(aw::pContextNameFrequency), 
        "frequency");

    // parameterNameToString

    EXPECT_EQ(aw::parameterNameToString(aw::pNameMinimum), "minimum");



    EXPECT_EQ(aw::parameterNameToString(aw::pNameMaximum), 
        "maximum");

    EXPECT_EQ(aw::parameterNameToString(aw::pNameOperand2), "operand2");
    EXPECT_EQ(aw::parameterNameToString(aw::pNameMinimum), "minimum");
    EXPECT_EQ(aw::parameterNameToString(aw::pNameMaximum), "maximum");



}




TEST(BasicTests, StringToEnumerated) {

    EXPECT_EQ(aw::stringToGeneratorName("add"), aw::gNameAdd);
    EXPECT_EQ(aw::stringToGeneratorName("ADD"), aw::gNameAdd);
    EXPECT_EQ(aw::stringToGeneratorName(" adD "), aw::gNameAdd);

    EXPECT_EQ(aw::stringToGeneratorName(" multiply "), aw::gNameMultiply);

    EXPECT_EQ(aw::stringToGeneratorName(" constant "), aw::gNameConstant);

    EXPECT_EQ(aw::stringToGeneratorName(" ranDomuniform "), aw::gNameRandomUniform);

    EXPECT_EQ(aw::stringToGeneratorName(" wavesine "), aw::gNameWaveSine);
    EXPECT_EQ(aw::stringToGeneratorName(" CLICK "), aw::gNameClick);

    EXPECT_EQ(aw::stringToGeneratorName(" polyADD "), aw::gNamePolyAdd);


    EXPECT_EQ(aw::stringToParameterContext(" frequency "), aw::pContextNameFrequency);
    EXPECT_EQ(aw::stringToParameterContext("periodseconds"), aw::pContextNamePeriodSeconds);
    EXPECT_EQ(aw::stringToParameterContext("seconds"), aw::pContextNamePeriodSeconds);
    EXPECT_EQ(aw::stringToParameterContext("sec"), aw::pContextNamePeriodSeconds);

    EXPECT_EQ(aw::stringToParameterContext("periodsamples"), aw::pContextNamePeriodSamples);
    EXPECT_EQ(aw::stringToParameterContext("samples"), aw::pContextNamePeriodSamples);
    EXPECT_EQ(aw::stringToParameterContext("samp"), aw::pContextNamePeriodSamples);

    EXPECT_EQ(aw::stringToParameterContext("BPM"), aw::pContextNameBeatsPerMinute);

    EXPECT_EQ(aw::stringToParameterContext(" pitch "), aw::pContextNamePitch);
    EXPECT_EQ(aw::stringToParameterContext("BPM"), aw::pContextNameBeatsPerMinute);




    EXPECT_EQ(aw::stringToParameterName("minimum"), aw::pNameMinimum);
    EXPECT_EQ(aw::stringToParameterName("max"), aw::pNameMaximum);
    EXPECT_EQ(aw::stringToParameterName("refresh"), aw::pNameRefresh);
    EXPECT_EQ(aw::stringToParameterName("rate"), aw::pNameRate);

    EXPECT_EQ(aw::stringToParameterName("value"), aw::pNameValue);
    EXPECT_EQ(aw::stringToParameterName("dutycycle"), aw::pNameDutyCycle);

    EXPECT_EQ(aw::stringToParameterName("operand1"), aw::pNameOperand1);
    EXPECT_EQ(aw::stringToParameterName("operand2"), aw::pNameOperand2);
    EXPECT_EQ(aw::stringToParameterName("operand3"), aw::pNameOperand3);
    EXPECT_EQ(aw::stringToParameterName("operand4"), aw::pNameOperand4);

    EXPECT_EQ(aw::stringToParameterName("amp"), aw::pNameAmplitude);
    EXPECT_EQ(aw::stringToParameterName("amplitude"), aw::pNameAmplitude);

}




TEST(BasicTests, StringConversions) {

    EXPECT_EQ(aw::numberToString(1), "1");

    EXPECT_EQ(aw::numberToString(1.0001), "1.0001");


    std::string str1 = "TEST";
    // process in place
    aw::scrubString(str1);
    EXPECT_EQ(str1, "test");


    std::string str2 = "     tEsT    ";
    // process in place
    aw::scrubString(str2);
    EXPECT_EQ(str2, "test");

    std::string str3 = "   t  E    s    T    ";
    // process in place
    aw::scrubString(str3);
    EXPECT_EQ(str3, "test");


    std::string str4 = "   t \nE    s\n   T    ";
    // process in place
    aw::scrubString(str4);
    EXPECT_EQ(str4, "test");

    std::string str5 = "   t \nE\t\t    s\n   T    ";
    // process in place
    aw::scrubString(str5);
    EXPECT_EQ(str5, "test");



    // test counting substrings
    EXPECT_EQ(aw::countSubStrings("tomato tomato", "to"), 4);
    EXPECT_EQ(aw::countSubStrings("tomato tomato", "tomato"), 2);
    EXPECT_EQ(aw::countSubStrings("tomato tomato", "a"), 2);
    EXPECT_EQ(aw::countSubStrings("tomato tomato", "om"), 2);
    EXPECT_EQ(aw::countSubStrings("tomato tomato", "4"), 0);

    EXPECT_EQ(aw::countSubStrings("FilterLowPass{operand1{Constant{0}} rate{Constant{220}}{frequency}}", "{"), 6);

    EXPECT_EQ(aw::countSubStrings("FilterLowPass{operand1{Constant{0}} rate{Constant{220}}{frequency}}", "}"), 6);



    EXPECT_EQ(aw::stringWellFormed("FilterLowPass{operand1{Constant{0}} rate{Constant{220}}{frequency}}", "{", "}"), true);

    EXPECT_EQ(aw::stringWellFormed("FilterLowPass{operand1{Constant{0}} rate{Constant{220}}{frequency}", "{", "}"), false);


    EXPECT_EQ(aw::stringWellFormed("FilterLowPass{operand1{Constant{0}} rate{Constant{220}}{frequency}", "{", "}"), false);



}



TEST(BasicTests, StringListConversions) {

    // default is to remove extra space by scrubbing
    std::vector<std::string> match1;
    match1.push_back("this");
    match1.push_back("is");
    match1.push_back("a");
    match1.push_back("test");
    std::vector<std::string> test1 = aw::splitString("this, IS, A, test", ",");

    EXPECT_EQ(test1[0], match1[0]);
    EXPECT_EQ(test1[1], match1[1]);
    EXPECT_EQ(test1[2], match1[2]);
    EXPECT_EQ(test1[3], match1[3]);

    // turning off space scrubbing
    std::vector<std::string> match2;
    match2.push_back("this");
    match2.push_back(" IS");
    match2.push_back(" A");
    std::vector<std::string> test2 = aw::splitString("this, IS, A, test", 
                                                    ",", false);

    EXPECT_EQ(test2[0], match2[0]);
    EXPECT_EQ(test2[1], match2[1]);
    EXPECT_EQ(test2[2], match2[2]);

    // string list to numbers

    std::string src3("1.3, 3.5, 6.1");
    EXPECT_EQ(aw::canConvertFromStringToVectorDouble(src3), true);

    std::vector<double> test3 = aw::stringToVectorDouble(src3);

    EXPECT_EQ(test3[0], 1.3);
    EXPECT_EQ(test3[1], 3.5);
    EXPECT_EQ(test3[2], 6.1);


    // looks awt least for commas
    std::string src4("1.3 3.5 6.1");
    EXPECT_EQ(aw::canConvertFromStringToVectorDouble(src4), false);



}



TEST(BasicTests, ParameterStringExtractionBasic) {


    // we get just the arguments of the first-specified parameter
    std::string src1("FilterLowPass{operand1{Constant{0}}rate{Constant{220}}{frequency}}{frequency}");

    std::string working1; 
    std::string dst1; 
    aw::getFirstParameterArguments(src1, dst1, working1);
    EXPECT_EQ(dst1, "operand1{Constant{0}}rate{Constant{220}}{frequency}");
    // working gets just what is left
    EXPECT_EQ(working1, "{frequency}");

    // get name
    std::string working1b; 
    std::string dst1b; 
    aw::getFirstParameterName(src1, dst1b, working1b);
    EXPECT_EQ(dst1b, "FilterLowPass");
    // working gets just what is left
    EXPECT_EQ(working1b, "{operand1{Constant{0}}rate{Constant{220}}{frequency}}{frequency}");

    // get context from
    // "FilterLowPass{operand1{Constant{0}}rate{Constant{220}}{frequency}}{frequency}"
    std::string working1c;
    std::string dst1c;
    aw::getFirstParameterContext(src1, dst1c, working1c);
    EXPECT_EQ(dst1c, "frequency");


    std::string working2; 
    std::string dst2; 
    aw::getFirstParameterArguments(dst1, dst2, working2);
    EXPECT_EQ(dst2, "Constant{0}");
    EXPECT_EQ(working2, "rate{Constant{220}}{frequency}");


    // get name from 
    // "operand1{Constant{0}}rate{Constant{220}}{frequency}"

    std::string working2b; 
    std::string dst2b; 
    aw::getFirstParameterName(dst1, dst2b, working2b);
    EXPECT_EQ(dst2b, "operand1");
    // working gets just what is left
    EXPECT_EQ(working2b, "{Constant{0}}rate{Constant{220}}{frequency}");


    std::string working3; 
    std::string dst3; 
    aw::getFirstParameterArguments(working2, dst3, working3);
    EXPECT_EQ(dst3, "Constant{220}");
    EXPECT_EQ(working3, "{frequency}");

    // get name from 
    // Constant{0}

    std::string working3b; 
    std::string dst3b; 
    aw::getFirstParameterName(dst2, dst3b, working3b);
    EXPECT_EQ(dst3b, "Constant");
    // working gets just what is left
    EXPECT_EQ(working3b, "{0}");


    // alternate source strings
    std::string src2("RandomUniform{refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}}");

    std::string working4c;
    std::string dst4c;
    aw::getFirstParameterContext(src2, dst4c, working4c);
    EXPECT_EQ(dst4c, "");


    std::string src3("refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");

    std::string working5c;
    std::string dst5c;
    aw::getFirstParameterContext(src3, dst5c, working5c);
    EXPECT_EQ(dst5c, "trigger");
    EXPECT_EQ(working5c, "minimum{Constant{0}}maximum{Constant{1}}");


    std::string src4("minimum{Constant{0}}maximum{Constant{1}}");

    std::string working6c;
    std::string dst6c;
    aw::getFirstParameterContext(src4, dst6c, working6c);
    EXPECT_EQ(dst6c, "");
    EXPECT_EQ(working6c, "minimum{Constant{0}}maximum{Constant{1}}");



}



TEST(BasicTests, TestGetFirstParameterName) {

    std::string src1("RandomUniform");
    std::string dst1;
    std::string working1;
    aw::getFirstParameterName(src1, dst1, working1);
    EXPECT_EQ(dst1, "RandomUniform");
    EXPECT_EQ(working1, "");

    std::string src2("Constant");
    std::string dst2;
    std::string working2;
    aw::getFirstParameterName(src2, dst2, working2);
    EXPECT_EQ(dst2, "Constant");
    EXPECT_EQ(working2, "");


    std::string src3("refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");
    std::string dst3;
    std::string working3;
    aw::getFirstParameterName(src3, dst3, working3);
    EXPECT_EQ(dst3, "refresh");
    EXPECT_EQ(working3, "{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");


    std::string src4("");
    std::string dst4;
    std::string working4;
    aw::getFirstParameterName(src4, dst4, working4);
    EXPECT_EQ(dst4, "");
    EXPECT_EQ(working4, "");




//     EXPECT_EQ(arguments, "");
//     EXPECT_EQ(context, ""); // no context for randomuniform
//     EXPECT_EQ(working, "");// no more chars to process

}



TEST(BasicTests, TestGetFirstParameterArguments) {

    std::string src1("RandomUniform");
    std::string dst1;
    std::string working1;
    aw::getFirstParameterArguments(src1, dst1, working1);
    EXPECT_EQ(dst1, "");
    EXPECT_EQ(working1, "RandomUniform");

    std::string src2("Constant");
    std::string dst2;
    std::string working2;
    aw::getFirstParameterArguments(src2, dst2, working2);
    EXPECT_EQ(dst2, "");
    EXPECT_EQ(working2, "Constant");

    std::string src3("refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");
    std::string dst3;
    std::string working3;
    aw::getFirstParameterArguments(src3, dst3, working3);
    EXPECT_EQ(dst3, "Constant{1}");
    EXPECT_EQ(working3, "{trigger}minimum{Constant{0}}maximum{Constant{1}}");


    std::string src4("RandomUniform{refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}}");
    std::string dst4;
    std::string working4;
    aw::getFirstParameterArguments(src4, dst4, working4);
    EXPECT_EQ(dst4, "refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");
    EXPECT_EQ(working4, "");


    std::string src5("");
    std::string dst5;
    std::string working5;
    aw::getFirstParameterArguments(src5, dst5, working5);
    EXPECT_EQ(dst5, "");
    EXPECT_EQ(working5, "");

}



TEST(BasicTests, TestGetFirstParameterContext) {

    std::string src1("RandomUniform");
    std::string dst1;
    std::string working1;
    aw::getFirstParameterContext(src1, dst1, working1);
    EXPECT_EQ(dst1, "");
    EXPECT_EQ(working1, "RandomUniform");

    std::string src2("");
    std::string dst2;
    std::string working2;
    aw::getFirstParameterContext(src2, dst2, working2);
    EXPECT_EQ(dst2, "");
    EXPECT_EQ(working2, "");

    std::string src3("refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");
    std::string dst3;
    std::string working3;
    aw::getFirstParameterContext(src3, dst3, working3);
    EXPECT_EQ(dst3, "trigger");
    EXPECT_EQ(working3, "minimum{Constant{0}}maximum{Constant{1}}");

    std::string src4("RandomUniform{refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}}");
    std::string dst4;
    std::string working4;
    aw::getFirstParameterContext(src4, dst4, working4);
    EXPECT_EQ(dst4, "");
    EXPECT_EQ(working4, "RandomUniform{refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}}");


    std::string src5("min{constant{-10}}max{constant{10}}rate{constant{30}}{frequency}");
    std::string dst5;
    std::string working5;
    aw::getFirstParameterContext(src5, dst5, working5);
    EXPECT_EQ(dst5, ""); // should be empty
    EXPECT_EQ(working5, "min{constant{-10}}max{constant{10}}rate{constant{30}}{frequency}");

    std::string src6("min{-10}max{10}rate{30}{frequency}");
    std::string dst6;
    std::string working6;
    aw::getFirstParameterContext(src6, dst6, working6);
    EXPECT_EQ(dst6, ""); // should be empty
    EXPECT_EQ(working6, "min{-10}max{10}rate{30}{frequency}");


    std::string src7("rate{30}{frequency}min{-10}max{10}");
    std::string dst7;
    std::string working7;
    aw::getFirstParameterContext(src7, dst7, working7);
    EXPECT_EQ(dst7, "frequency"); 
    EXPECT_EQ(working7, "min{-10}max{10}");



}







TEST(BasicTests, ParameterStringExtractionB) {

    std::string src("RandomUniform{refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}}");
    
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;
    
    aw::getFirstParameterTrio(src, name, arguments, context, working);
    
    EXPECT_EQ(name, "RandomUniform");
    EXPECT_EQ(arguments, "refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");
    EXPECT_EQ(context, ""); // no context for randomuniform
    EXPECT_EQ(working, "");// no more chars to process

}


TEST(BasicTests, ParameterStringExtractionC) {

    std::string src("refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");
    
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;
    
    aw::getFirstParameterTrio(src, name, arguments, context, working);
    
    EXPECT_EQ(name, "refresh");
    EXPECT_EQ(arguments, "Constant{1}");
    EXPECT_EQ(context, "trigger"); // context for refresh
    EXPECT_EQ(working, "minimum{Constant{0}}maximum{Constant{1}}");// remaining 
}


TEST(BasicTests, ParameterStringExtractionD) {

    std::string src("Constant{1}");
    
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;
    
    aw::getFirstParameterTrio(src, name, arguments, context, working);
    
    EXPECT_EQ(name, "Constant");
    EXPECT_EQ(arguments, "1");
    EXPECT_EQ(context, "");
    EXPECT_EQ(working, "");

}


TEST(BasicTests, ParameterStringExtractionE) {

    std::string src("minimum{Constant{0}}maximum{Constant{1}}");
    
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;
    
    aw::getFirstParameterTrio(src, name, arguments, context, working);
    
    EXPECT_EQ(name, "minimum");
    EXPECT_EQ(arguments, "Constant{0}");
    EXPECT_EQ(context, ""); // context for refresh
    EXPECT_EQ(working, "maximum{Constant{1}}");// remaining parameters

}


TEST(BasicTests, ParameterStringExtractionF) {

    std::string src("maximum{Constant{1}}");
    
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;
    
    aw::getFirstParameterTrio(src, name, arguments, context, working);
    
    EXPECT_EQ(name, "maximum");
    EXPECT_EQ(arguments, "Constant{1}");
    EXPECT_EQ(context, ""); // context for refresh
    EXPECT_EQ(working, "");// remaining parameters

}


TEST(BasicTests, ParameterStringExtractionG) {

    std::string src("Constant");
    
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;
    
    aw::getFirstParameterTrio(src, name, arguments, context, working);
    
    EXPECT_EQ(name, "Constant");
    EXPECT_EQ(arguments, "");
    EXPECT_EQ(context, ""); // context for refresh
    EXPECT_EQ(working, "");// remaining parameters

}


TEST(BasicTests, ParameterStringExtractionH) {

    std::string src("randomuniform");
    
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;
    
    aw::getFirstParameterTrio(src, name, arguments, context, working);
    
    EXPECT_EQ(name, "randomuniform");
    EXPECT_EQ(arguments, "");
    EXPECT_EQ(context, ""); // context for refresh
    EXPECT_EQ(working, "");// remaining parameters

}



TEST(BasicTests, TestStringToDouble) {

    std::string test1("2.4000");     
    ASSERT_FLOAT_EQ(aw::stringToDouble(test1), 2.4);
    ASSERT_FLOAT_EQ(aw::canConvertFromStringToDouble(test1), true);

    std::string test2("3.8000");     
    ASSERT_FLOAT_EQ(aw::stringToDouble(test2), 3.8);
    ASSERT_FLOAT_EQ(aw::canConvertFromStringToDouble(test2), true);

    std::string test3("valeu{3.8000}");     
    ASSERT_FLOAT_EQ(aw::canConvertFromStringToDouble(test3), false);

    std::string test5("3, 56,3");     
    ASSERT_FLOAT_EQ(aw::canConvertFromStringToDouble(test5), false);


    // TODO
    // this should be false
    std::string test4("230}/sdf");     
    ASSERT_FLOAT_EQ(aw::canConvertFromStringToDouble(test4), true);


}


TEST(BasicTests, TestArrayDoubleToString) {

    double a1[] = {2.3, 5.4, 8.2};
    std::string out1 = aw::arrayDoubleToString(a1, 3);
    EXPECT_EQ(out1, "2.3,5.4,8.2");
    std::vector<double> v1 = aw::stringToVectorDouble(out1);
    EXPECT_EQ(v1[0], 2.3);
    EXPECT_EQ(v1[2], 8.2);


    double a2[] = {200, 54, 123, 7, 3, 7, 2,  90, .00001};
    std::string out2 = aw::arrayDoubleToString(a2, 9);
    EXPECT_EQ(out2, "200,54,123,7,3,7,2,90,1e-05");

    std::vector<double> v2 = aw::stringToVectorDouble(out2);
    EXPECT_EQ(v2[0], 200);
    EXPECT_EQ(v2[8], 1e-05);

}


TEST(BasicTests, TestMod) {
    // works for both positive and negative; cannot be
    // be sure in other cases

    EXPECT_EQ(aw::mod(0, 3), 0);
    EXPECT_EQ(aw::mod(1, 3), 1);
    EXPECT_EQ(aw::mod(2, 3), 2);
    EXPECT_EQ(aw::mod(3, 3), 0);
    EXPECT_EQ(aw::mod(4, 3), 1);
    EXPECT_EQ(aw::mod(5, 3), 2);


    EXPECT_EQ(aw::mod(0, 3), 0);
    EXPECT_EQ(aw::mod(-1, 3), 2);
    EXPECT_EQ(aw::mod(-2, 3), 1);
    EXPECT_EQ(aw::mod(-3, 3), 0);
    EXPECT_EQ(aw::mod(-4, 3), 2);
    EXPECT_EQ(aw::mod(-5, 3), 1);


}




TEST(BasicTests, DoubleToIntProbabilistic) {

    EXPECT_EQ(aw::doubleToIntProabilistic(1) == 1, true);
    EXPECT_EQ(aw::doubleToIntProabilistic(2.0) == 2, true);

    EXPECT_EQ(aw::doubleToIntProabilistic(30.999) == 31, true);

    // create 100 solutions for different ranges, and average
    // averages should be w/n probabilistic expectation
    double sum(0);
    double x(0);
    int count(100);
    double avg(0);

    //std::vector<aw::Int32Signed> a;
    for (int i=0; i<count; i++) {
        x = aw::doubleToIntProabilistic(20.5);
        sum += x;
        //a.push_back(x); 
    }
    //aw::printVector(a, "aw::doubleToIntProabilistic 20.5");
    avg = sum / count;
    EXPECT_EQ(avg > 20.25 && avg < 20.75, true);
    sum = 0;

    //std::vector<aw::Int32Signed> b;
    for (int i=0; i<count; i++) {
        x = aw::doubleToIntProabilistic(40.75);
        sum += x;
        //b.push_back(x); 
    }
    avg = sum / count;
    //aw::printVector(b, "aw::doubleToIntProabilistic 40.85");
    EXPECT_EQ(avg > 40.5 && avg < 41, true);
    sum = 0;

    //std::vector<aw::Int32Signed> c;
    for (int i=0; i<count; i++) {
        x = aw::doubleToIntProabilistic(60.25);
        sum += x;
        //c.push_back(x); 
    }
    //aw::printVector(c, "aw::doubleToIntProabilistic 60.15");
    avg = sum / count;
    EXPECT_EQ(avg > 60 && avg < 60.5, true);
    sum = 0;


}




TEST(BasicTests, RandomInteger) {

    for (int i=0; i<30; i++) {
        EXPECT_EQ(aw::randomInteger(3) < 3, true);
        //std::cout << "aw::randomInteger(3): " << aw::randomInteger(3) << std::endl; 
    }

    for (int i=0; i<30; i++) {
        EXPECT_EQ(aw::randomInteger(11) < 11, true);
        //std::cout << "aw::randomInteger(11): " << aw::randomInteger(11) << std::endl; 
    }


}





TEST(BasicTests, ShuffleVector) {

    double sum(0);
    int count(0);
    double avg(0);

    std::vector<int> v;

    for (int i=0; i<30; i++) {
        v.push_back(i);
        // std::cout << "aw::randomInteger(3): " << aw::randomInteger(3) << std::endl; 
    }

    for (int i=0; i<30; i++) {
        // try to shuffle in place
        aw::shuffleIntegerVector(v);
        sum = 0;
        count = 0;
        for (int i=0; i<v.size(); i++) {
            sum += v[i];
            count += 1;
        }
        avg = sum / count;
        //aw::printVector(b, "aw::doubleToIntProabilistic 40.85");
        EXPECT_EQ(avg, 14.5);
        EXPECT_EQ(sum, 435);

        //aw::printVector(v, "shuffle (post)");
    }


}

