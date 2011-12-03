// g++ aw_Common_btest.cpp ../core/aw_Common.cpp -Wall -I "/Volumes/xdisc/_sync/_x/src/cpp/boost_1_43_0" -I "/Volumes/xdisc/_sync/_x/src/arachnewarp/arachnewarp/src/core" -o aw_Common_btest


#define BOOST_TEST_MODULE

#include <stdexcept>
#include <vector>
#include <list>

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "aw_Common.h"



using namespace aw;



// provide testCase, name
BOOST_AUTO_TEST_CASE(DenormBasic) {

    // args are value, min, max
    BOOST_CHECK_EQUAL(aw::denormalize(0.0, 0.0, 1.0), 0);

    BOOST_CHECK_EQUAL(aw::denormalize(0.5, 0.0, 1.0), 0.5);
    BOOST_CHECK_EQUAL(aw::denormalize(0.5, 1.0, 0.0), 0.5);

    BOOST_CHECK_EQUAL(aw::denormalize(1.0, 0.0, 1.0), 1);

    BOOST_CHECK_EQUAL(aw::denormalize(0.5, -100.0, 100.0), 0);

    BOOST_CHECK_EQUAL(aw::denormalize(0.8, -100.0, 100.0), 60);

    // note that this handles ints just fine
    BOOST_CHECK_EQUAL(aw::denormalize(0.95, 0, 5), 4.75);

    BOOST_CHECK_EQUAL(aw::denormalize(0.5, 0, 5), 2.5);


    // if given value is outside of 0-1, throw an out of range exception
    BOOST_CHECK_THROW(aw::denormalize(-5), std::out_of_range);

}

// this function is not active now
// BOOST_AUTO_TEST_CASE(RandomRangeBasic) {
// 
//     // args are min, max
//     // always within range of 1 less than max
//     BOOST_CHECK_EQUAL(aw::randomRange(5, 6), 5);
//     BOOST_CHECK_EQUAL(aw::randomRange(0, 1), 0);
//     BOOST_CHECK_EQUAL(aw::randomRange(20000, 20001), 20000);
// 
//     std::vector<aw::Int32Signed> a;
//     for (int i=0; i<10; i++) {
//         a.push_back(aw::randomRange(0,10)); // values 0 to 9 generated
//     }
//     aw::print(a, "aw::randomRange between 0 and 9");
// 
// 
//     std::vector<int> b;
//     for (int i=0; i<10; i++) {
//         b.push_back(aw::randomRange(100,104)); // values 0 to 9 generated
//     }
//     aw::print(b, "aw::randomRange between 100 and 104");
// 
//     std::vector<int> c;
//     for (int i=0; i<10; i++) {
//         c.push_back(aw::randomRange(-20,-10)); // values 0 to 9 generated
//     }
//     aw::print(c, "aw::randomRange between -20 and -10");
// 
// 
// }


BOOST_AUTO_TEST_CASE(RandomIntegerRangeBasic) {

    // args are min, max
    // always within range of 1 less than max

    BOOST_CHECK_EQUAL(aw::randomIntRange(0, 0), 0);
    BOOST_CHECK_EQUAL(aw::randomIntRange(10, 10), 10);

    BOOST_CHECK_EQUAL(aw::randomIntRange(10, 20) < 21, true);
    BOOST_CHECK_EQUAL(aw::randomIntRange(10, 20) < 21, true);
    BOOST_CHECK_EQUAL(aw::randomIntRange(10, 20) < 21, true);
    BOOST_CHECK_EQUAL(aw::randomIntRange(10, 20) < 21, true);
    BOOST_CHECK_EQUAL(aw::randomIntRange(10, 20) < 21, true);
    BOOST_CHECK_EQUAL(aw::randomIntRange(10, 20) < 21, true);

    BOOST_CHECK_EQUAL(aw::randomIntRange(10, 20) > 9, true);
    BOOST_CHECK_EQUAL(aw::randomIntRange(10, 20) > 9, true);
    BOOST_CHECK_EQUAL(aw::randomIntRange(10, 20) > 9, true);
    BOOST_CHECK_EQUAL(aw::randomIntRange(10, 20) > 9, true);
    BOOST_CHECK_EQUAL(aw::randomIntRange(10, 20) > 9, true);
    BOOST_CHECK_EQUAL(aw::randomIntRange(10, 20) > 9, true);

    // test that sequential values are not the same
    aw::Int32Signed q1 = aw::randomIntRange(0, 20000);
    aw::Int32Signed q2 = aw::randomIntRange(0, 20000);
    aw::Int32Signed q3 = aw::randomIntRange(0, 20000);
    aw::Int32Signed q4 = aw::randomIntRange(0, 20000);

    BOOST_CHECK_EQUAL(q1 != q2, true);
    BOOST_CHECK_EQUAL(q2 != q3, true);
    BOOST_CHECK_EQUAL(q3 != q4, true);
    BOOST_CHECK_EQUAL(q1 != q4, true);

    std::vector<int> a;
    for (int i=0; i<10; i++) {
        a.push_back(aw::randomIntRange(0,20)); 
    }
    aw::print(a, "aw::randomIntRange between 0 and 20");

    std::vector<int> b;
    for (int i=0; i<10; i++) {
        b.push_back(aw::randomIntRange(-1000,1000)); 
    }
    aw::print(b, "aw::randomIntRange between -1000 and 1000");


//     std::vector<int> b;
//     for (int i=0; i<10; i++) {
//         b.push_back(aw::randomRange(100,104)); // values 0 to 9 generated
//     }
//     aw::print(b, "aw::randomRange between 100 and 104");
// 
//     std::vector<int> c;
//     for (int i=0; i<10; i++) {
//         c.push_back(aw::randomRange(-20,-10)); // values 0 to 9 generated
//     }
//     aw::print(c, "aw::randomRange between -20 and -10");


}



BOOST_AUTO_TEST_CASE(RandomUnit) {
    // args are min, max
    // always within range of 1 less than max
    BOOST_CHECK_EQUAL(aw::randomUnit() > 0, true);
    BOOST_CHECK_EQUAL(aw::randomUnit() < 1, true);

    std::cout << "aw::randomUnit(): " << aw::randomUnit() << std::endl; 
    std::cout << "aw::randomUnit(): " << aw::randomUnit() << std::endl; 
    std::cout << "aw::randomUnit(): " << aw::randomUnit() << std::endl; 
}


BOOST_AUTO_TEST_CASE(TimeConversions) {
    // args are min, max
    // always within range of 1 less than max
    BOOST_CHECK_EQUAL(aw::sampleTimeToSecondTime(44100, 44100), 1);
    BOOST_CHECK_EQUAL(aw::sampleTimeToSecondTime(66150, 44100), 1.5);

    // secont tim to sample time
    BOOST_CHECK_EQUAL(aw::secondTimeToSampleTime(2, 44100), 88200);

    // bpm conversions
    BOOST_CHECK_EQUAL(aw::bpmToSecondTime(60), 1.0);
    BOOST_CHECK_EQUAL(aw::bpmToSecondTime(120), 0.5);
    BOOST_CHECK_EQUAL(aw::bpmToSecondTime(30), 2.0);
}



BOOST_AUTO_TEST_CASE(EnumeratedToString) {
    BOOST_CHECK_EQUAL(aw::parameterContextToString(aw::pContextNameNone), 
        "none");
    BOOST_CHECK_EQUAL(aw::parameterContextToString(aw::pContextNameFrequency), 
        "frequency");
    // parameterNameToString
    BOOST_CHECK_EQUAL(aw::parameterNameToString(aw::pNameMinimum), "minimum");
    BOOST_CHECK_EQUAL(aw::parameterNameToString(aw::pNameMaximum), 
        "maximum");

    BOOST_CHECK_EQUAL(aw::parameterNameToString(aw::pNameOperand2), "operand2");
    BOOST_CHECK_EQUAL(aw::parameterNameToString(aw::pNameMinimum), "minimum");
    BOOST_CHECK_EQUAL(aw::parameterNameToString(aw::pNameMaximum), "maximum");
}


BOOST_AUTO_TEST_CASE(StringToEnumerated) {

    BOOST_CHECK_EQUAL(aw::stringToGeneratorName("add"), aw::gNameAdd);
    BOOST_CHECK_EQUAL(aw::stringToGeneratorName("ADD"), aw::gNameAdd);
    BOOST_CHECK_EQUAL(aw::stringToGeneratorName(" adD "), aw::gNameAdd);

    BOOST_CHECK_EQUAL(aw::stringToGeneratorName(" multiply "), aw::gNameMultiply);

    BOOST_CHECK_EQUAL(aw::stringToGeneratorName(" constant "), aw::gNameConstant);

    BOOST_CHECK_EQUAL(aw::stringToGeneratorName(" ranDomuniform "), aw::gNameRandomUniform);

    BOOST_CHECK_EQUAL(aw::stringToGeneratorName(" wavesine "), aw::gNameWaveSine);
    BOOST_CHECK_EQUAL(aw::stringToGeneratorName(" CLICK "), aw::gNameClick);

    BOOST_CHECK_EQUAL(aw::stringToGeneratorName(" polyADD "), aw::gNamePolyAdd);

    BOOST_CHECK_EQUAL(aw::stringToGeneratorName(" polytablefile "),
             aw::gNamePolyTableFile);

    BOOST_CHECK_EQUAL(aw::stringToParameterContext(" frequency "), aw::pContextNameFrequency);
    BOOST_CHECK_EQUAL(aw::stringToParameterContext("periodseconds"), aw::pContextNamePeriodSeconds);
    BOOST_CHECK_EQUAL(aw::stringToParameterContext("seconds"), aw::pContextNamePeriodSeconds);
    BOOST_CHECK_EQUAL(aw::stringToParameterContext("sec"), aw::pContextNamePeriodSeconds);

    BOOST_CHECK_EQUAL(aw::stringToParameterContext("periodsamples"), aw::pContextNamePeriodSamples);
    BOOST_CHECK_EQUAL(aw::stringToParameterContext("samples"), aw::pContextNamePeriodSamples);
    BOOST_CHECK_EQUAL(aw::stringToParameterContext("samp"), aw::pContextNamePeriodSamples);

    BOOST_CHECK_EQUAL(aw::stringToParameterContext("BPM"), aw::pContextNameBeatsPerMinute);

    BOOST_CHECK_EQUAL(aw::stringToParameterContext(" pitch "), aw::pContextNamePitch);
    BOOST_CHECK_EQUAL(aw::stringToParameterContext("BPM"), aw::pContextNameBeatsPerMinute);

    BOOST_CHECK_EQUAL(aw::stringToParameterName("minimum"), aw::pNameMinimum);
    BOOST_CHECK_EQUAL(aw::stringToParameterName("max"), aw::pNameMaximum);
    BOOST_CHECK_EQUAL(aw::stringToParameterName("refresh"), aw::pNameRefresh);
    BOOST_CHECK_EQUAL(aw::stringToParameterName("rate"), aw::pNameRate);

    BOOST_CHECK_EQUAL(aw::stringToParameterName("value"), aw::pNameValue);
    BOOST_CHECK_EQUAL(aw::stringToParameterName("dutycycle"), aw::pNameDutyCycle);

    BOOST_CHECK_EQUAL(aw::stringToParameterName("operand1"), aw::pNameOperand1);
    BOOST_CHECK_EQUAL(aw::stringToParameterName("operand2"), aw::pNameOperand2);
    BOOST_CHECK_EQUAL(aw::stringToParameterName("operand3"), aw::pNameOperand3);
    BOOST_CHECK_EQUAL(aw::stringToParameterName("operand4"), aw::pNameOperand4);

    BOOST_CHECK_EQUAL(aw::stringToParameterName("amp"), aw::pNameAmplitude);
    BOOST_CHECK_EQUAL(aw::stringToParameterName("amplitude"), aw::pNameAmplitude);

    BOOST_CHECK_EQUAL(aw::stringToParameterName("valuelist"), aw::pNameValueList);
    BOOST_CHECK_EQUAL(aw::stringToParameterName("selectionmethod"),
               aw::pNameSelectionMethod);
    BOOST_CHECK_EQUAL(aw::stringToParameterName("pan"), aw::pNamePanLeftRight);
    BOOST_CHECK_EQUAL(aw::stringToParameterName("stride"), aw::pNameStride);    BOOST_CHECK_EQUAL(aw::stringToParameterName("filepath"), aw::pNameFilePath);

}




BOOST_AUTO_TEST_CASE(StringConversions) {

    BOOST_CHECK_EQUAL(aw::numberToString(1), "1");
    BOOST_CHECK_EQUAL(aw::numberToString(1.0001), "1.0001");

    std::string str1 = "TEST";
    // process in place
    aw::scrubStringAndLowerCase(str1);
    BOOST_CHECK_EQUAL(str1, "test");

    std::string str2 = "     tEsT    ";
    // process in place
    aw::scrubStringAndLowerCase(str2);
    BOOST_CHECK_EQUAL(str2, "test");

    std::string str3 = "   t  E    s    T    ";
    // process in place
    aw::scrubStringAndLowerCase(str3);
    BOOST_CHECK_EQUAL(str3, "test");


    std::string str4 = "   t \nE    s\n   T    ";
    // process in place
    aw::scrubStringAndLowerCase(str4);
    BOOST_CHECK_EQUAL(str4, "test");

    std::string str5 = "   t \nE\t\t    s\n   T    ";
    // process in place
    aw::scrubStringAndLowerCase(str5);
    BOOST_CHECK_EQUAL(str5, "test");

    // test counting substrings
    BOOST_CHECK_EQUAL(aw::countSubStrings("tomato tomato", "to"), 4);
    BOOST_CHECK_EQUAL(aw::countSubStrings("tomato tomato", "tomato"), 2);
    BOOST_CHECK_EQUAL(aw::countSubStrings("tomato tomato", "a"), 2);
    BOOST_CHECK_EQUAL(aw::countSubStrings("tomato tomato", "om"), 2);
    BOOST_CHECK_EQUAL(aw::countSubStrings("tomato tomato", "4"), 0);

    BOOST_CHECK_EQUAL(aw::countSubStrings("FilterLowPass{operand1{Constant{0}} rate{Constant{220}}{frequency}}", "{"), 6);

    BOOST_CHECK_EQUAL(aw::countSubStrings("FilterLowPass{operand1{Constant{0}} rate{Constant{220}}{frequency}}", "}"), 6);

    BOOST_CHECK_EQUAL(aw::stringWellFormed("FilterLowPass{operand1{Constant{0}} rate{Constant{220}}{frequency}}", "{", "}"), true);

    BOOST_CHECK_EQUAL(aw::stringWellFormed("FilterLowPass{operand1{Constant{0}} rate{Constant{220}}{frequency}", "{", "}"), false);


    BOOST_CHECK_EQUAL(aw::stringWellFormed("FilterLowPass{operand1{Constant{0}} rate{Constant{220}}{frequency}", "{", "}"), false);
}



BOOST_AUTO_TEST_CASE(StringListConversions) {

    // default is to remove extra space by scrubbing
    std::vector<std::string> match1;
    match1.push_back("this");
    match1.push_back("is");
    match1.push_back("a");
    match1.push_back("test");
    std::vector<std::string> test1 = aw::splitString("this, IS, A, test", ",");

    BOOST_CHECK_EQUAL(test1[0], match1[0]);
    BOOST_CHECK_EQUAL(test1[1], match1[1]);
    BOOST_CHECK_EQUAL(test1[2], match1[2]);
    BOOST_CHECK_EQUAL(test1[3], match1[3]);

    // turning off space scrubbing
    std::vector<std::string> match2;
    match2.push_back("this");
    match2.push_back(" IS");
    match2.push_back(" A");
    std::vector<std::string> test2 = aw::splitString("this, IS, A, test", 
                                                    ",", false);

    BOOST_CHECK_EQUAL(test2[0], match2[0]);
    BOOST_CHECK_EQUAL(test2[1], match2[1]);
    BOOST_CHECK_EQUAL(test2[2], match2[2]);

    // string list to numbers
    std::string src3("1.3, 3.5, 6.1");
    BOOST_CHECK_EQUAL(aw::canConvertFromStringToVectorDouble(src3), true);

    std::vector<double> test3 = aw::stringToVectorDouble(src3);

    BOOST_CHECK_EQUAL(test3[0], 1.3);
    BOOST_CHECK_EQUAL(test3[1], 3.5);
    BOOST_CHECK_EQUAL(test3[2], 6.1);


    // looks awt least for commas
    std::string src4("1.3 3.5 6.1");
    BOOST_CHECK_EQUAL(aw::canConvertFromStringToVectorDouble(src4), false);
}




BOOST_AUTO_TEST_CASE(ParameterStringExtractionBasic) {
    // we get just the arguments of the first-specified parameter
    std::string src1("FilterLowPass{operand1{Constant{0}}rate{Constant{220}}{frequency}}{frequency}");

    std::string working1; 
    std::string dst1; 
    aw::getFirstParameterArguments(src1, dst1, working1);
    BOOST_CHECK_EQUAL(dst1, "operand1{Constant{0}}rate{Constant{220}}{frequency}");
    // working gets just what is left
    BOOST_CHECK_EQUAL(working1, "{frequency}");

    // get name
    std::string working1b; 
    std::string dst1b; 
    aw::getFirstParameterName(src1, dst1b, working1b);
    BOOST_CHECK_EQUAL(dst1b, "FilterLowPass");
    // working gets just what is left
    BOOST_CHECK_EQUAL(working1b, "{operand1{Constant{0}}rate{Constant{220}}{frequency}}{frequency}");

    // get context from
    // "FilterLowPass{operand1{Constant{0}}rate{Constant{220}}{frequency}}{frequency}"
    std::string working1c;
    std::string dst1c;
    aw::getFirstParameterContext(src1, dst1c, working1c);
    BOOST_CHECK_EQUAL(dst1c, "frequency");

    std::string working2; 
    std::string dst2; 
    aw::getFirstParameterArguments(dst1, dst2, working2);
    BOOST_CHECK_EQUAL(dst2, "Constant{0}");
    BOOST_CHECK_EQUAL(working2, "rate{Constant{220}}{frequency}");

    // get name from 
    // "operand1{Constant{0}}rate{Constant{220}}{frequency}"

    std::string working2b; 
    std::string dst2b; 
    aw::getFirstParameterName(dst1, dst2b, working2b);
    BOOST_CHECK_EQUAL(dst2b, "operand1");
    // working gets just what is left
    BOOST_CHECK_EQUAL(working2b, "{Constant{0}}rate{Constant{220}}{frequency}");


    std::string working3; 
    std::string dst3; 
    aw::getFirstParameterArguments(working2, dst3, working3);
    BOOST_CHECK_EQUAL(dst3, "Constant{220}");
    BOOST_CHECK_EQUAL(working3, "{frequency}");

    // get name from 
    // Constant{0}

    std::string working3b; 
    std::string dst3b; 
    aw::getFirstParameterName(dst2, dst3b, working3b);
    BOOST_CHECK_EQUAL(dst3b, "Constant");
    // working gets just what is left
    BOOST_CHECK_EQUAL(working3b, "{0}");


    // alternate source strings
    std::string src2("RandomUniform{refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}}");

    std::string working4c;
    std::string dst4c;
    aw::getFirstParameterContext(src2, dst4c, working4c);
    BOOST_CHECK_EQUAL(dst4c, "");

    std::string src3("refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");

    std::string working5c;
    std::string dst5c;
    aw::getFirstParameterContext(src3, dst5c, working5c);
    BOOST_CHECK_EQUAL(dst5c, "trigger");
    BOOST_CHECK_EQUAL(working5c, "minimum{Constant{0}}maximum{Constant{1}}");

    std::string src4("minimum{Constant{0}}maximum{Constant{1}}");

    std::string working6c;
    std::string dst6c;
    aw::getFirstParameterContext(src4, dst6c, working6c);
    BOOST_CHECK_EQUAL(dst6c, "");
    BOOST_CHECK_EQUAL(working6c, "minimum{Constant{0}}maximum{Constant{1}}");

}



BOOST_AUTO_TEST_CASE(TestGetFirstParameterName) {

    std::string src1("RandomUniform");
    std::string dst1;
    std::string working1;
    aw::getFirstParameterName(src1, dst1, working1);
    BOOST_CHECK_EQUAL(dst1, "RandomUniform");
    BOOST_CHECK_EQUAL(working1, "");

    std::string src2("Constant");
    std::string dst2;
    std::string working2;
    aw::getFirstParameterName(src2, dst2, working2);
    BOOST_CHECK_EQUAL(dst2, "Constant");
    BOOST_CHECK_EQUAL(working2, "");

    std::string src3("refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");
    std::string dst3;
    std::string working3;
    aw::getFirstParameterName(src3, dst3, working3);
    BOOST_CHECK_EQUAL(dst3, "refresh");
    BOOST_CHECK_EQUAL(working3, "{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");

    std::string src4("");
    std::string dst4;
    std::string working4;
    aw::getFirstParameterName(src4, dst4, working4);
    BOOST_CHECK_EQUAL(dst4, "");
    BOOST_CHECK_EQUAL(working4, "");

//     BOOST_CHECK_EQUAL(arguments, "");
//     BOOST_CHECK_EQUAL(context, ""); // no context for randomuniform
//     BOOST_CHECK_EQUAL(working, "");// no more chars to process

}



BOOST_AUTO_TEST_CASE(TestGetFirstParameterArguments) {

    std::string src1("RandomUniform");
    std::string dst1;
    std::string working1;
    aw::getFirstParameterArguments(src1, dst1, working1);
    BOOST_CHECK_EQUAL(dst1, "");
    BOOST_CHECK_EQUAL(working1, "RandomUniform");

    std::string src2("Constant");
    std::string dst2;
    std::string working2;
    aw::getFirstParameterArguments(src2, dst2, working2);
    BOOST_CHECK_EQUAL(dst2, "");
    BOOST_CHECK_EQUAL(working2, "Constant");

    std::string src3("refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");
    std::string dst3;
    std::string working3;
    aw::getFirstParameterArguments(src3, dst3, working3);
    BOOST_CHECK_EQUAL(dst3, "Constant{1}");
    BOOST_CHECK_EQUAL(working3, "{trigger}minimum{Constant{0}}maximum{Constant{1}}");


    std::string src4("RandomUniform{refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}}");
    std::string dst4;
    std::string working4;
    aw::getFirstParameterArguments(src4, dst4, working4);
    BOOST_CHECK_EQUAL(dst4, "refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");
    BOOST_CHECK_EQUAL(working4, "");


    std::string src5("");
    std::string dst5;
    std::string working5;
    aw::getFirstParameterArguments(src5, dst5, working5);
    BOOST_CHECK_EQUAL(dst5, "");
    BOOST_CHECK_EQUAL(working5, "");

}



BOOST_AUTO_TEST_CASE(TestGetFirstParameterContext) {

    std::string src1("RandomUniform");
    std::string dst1;
    std::string working1;
    aw::getFirstParameterContext(src1, dst1, working1);
    BOOST_CHECK_EQUAL(dst1, "");
    BOOST_CHECK_EQUAL(working1, "RandomUniform");

    std::string src2("");
    std::string dst2;
    std::string working2;
    aw::getFirstParameterContext(src2, dst2, working2);
    BOOST_CHECK_EQUAL(dst2, "");
    BOOST_CHECK_EQUAL(working2, "");

    std::string src3("refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");
    std::string dst3;
    std::string working3;
    aw::getFirstParameterContext(src3, dst3, working3);
    BOOST_CHECK_EQUAL(dst3, "trigger");
    BOOST_CHECK_EQUAL(working3, "minimum{Constant{0}}maximum{Constant{1}}");

    std::string src4("RandomUniform{refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}}");
    std::string dst4;
    std::string working4;
    aw::getFirstParameterContext(src4, dst4, working4);
    BOOST_CHECK_EQUAL(dst4, "");
    BOOST_CHECK_EQUAL(working4, "RandomUniform{refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}}");


    std::string src5("min{constant{-10}}max{constant{10}}rate{constant{30}}{frequency}");
    std::string dst5;
    std::string working5;
    aw::getFirstParameterContext(src5, dst5, working5);
    BOOST_CHECK_EQUAL(dst5, ""); // should be empty
    BOOST_CHECK_EQUAL(working5, "min{constant{-10}}max{constant{10}}rate{constant{30}}{frequency}");

    std::string src6("min{-10}max{10}rate{30}{frequency}");
    std::string dst6;
    std::string working6;
    aw::getFirstParameterContext(src6, dst6, working6);
    BOOST_CHECK_EQUAL(dst6, ""); // should be empty
    BOOST_CHECK_EQUAL(working6, "min{-10}max{10}rate{30}{frequency}");


    std::string src7("rate{30}{frequency}min{-10}max{10}");
    std::string dst7;
    std::string working7;
    aw::getFirstParameterContext(src7, dst7, working7);
    BOOST_CHECK_EQUAL(dst7, "frequency"); 
    BOOST_CHECK_EQUAL(working7, "min{-10}max{10}");

}







BOOST_AUTO_TEST_CASE(ParameterStringExtractionB) {

    std::string src("RandomUniform{refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}}");
    
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;
    
    aw::getFirstParameterTrio(src, name, arguments, context, working);
    
    BOOST_CHECK_EQUAL(name, "RandomUniform");
    BOOST_CHECK_EQUAL(arguments, "refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");
    BOOST_CHECK_EQUAL(context, ""); // no context for randomuniform
    BOOST_CHECK_EQUAL(working, "");// no more chars to process

}



BOOST_AUTO_TEST_CASE(ParameterStringExtractionC) {

    std::string src("refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}");
    
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;
    
    aw::getFirstParameterTrio(src, name, arguments, context, working);
    
    BOOST_CHECK_EQUAL(name, "refresh");
    BOOST_CHECK_EQUAL(arguments, "Constant{1}");
    BOOST_CHECK_EQUAL(context, "trigger"); // context for refresh
    BOOST_CHECK_EQUAL(working, "minimum{Constant{0}}maximum{Constant{1}}");// remaining 
}


BOOST_AUTO_TEST_CASE(ParameterStringExtractionD) {

    std::string src("Constant{1}");
    
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;
    
    aw::getFirstParameterTrio(src, name, arguments, context, working);
    
    BOOST_CHECK_EQUAL(name, "Constant");
    BOOST_CHECK_EQUAL(arguments, "1");
    BOOST_CHECK_EQUAL(context, "");
    BOOST_CHECK_EQUAL(working, "");

}


BOOST_AUTO_TEST_CASE(ParameterStringExtractionE) {

    std::string src("minimum{Constant{0}}maximum{Constant{1}}");
    
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;
    
    aw::getFirstParameterTrio(src, name, arguments, context, working);
    
    BOOST_CHECK_EQUAL(name, "minimum");
    BOOST_CHECK_EQUAL(arguments, "Constant{0}");
    BOOST_CHECK_EQUAL(context, ""); // context for refresh
    BOOST_CHECK_EQUAL(working, "maximum{Constant{1}}");// remaining parameters

}


BOOST_AUTO_TEST_CASE(ParameterStringExtractionF) {

    std::string src("maximum{Constant{1}}");
    
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;
    
    aw::getFirstParameterTrio(src, name, arguments, context, working);
    
    BOOST_CHECK_EQUAL(name, "maximum");
    BOOST_CHECK_EQUAL(arguments, "Constant{1}");
    BOOST_CHECK_EQUAL(context, ""); // context for refresh
    BOOST_CHECK_EQUAL(working, "");// remaining parameters

}


BOOST_AUTO_TEST_CASE(ParameterStringExtractionG) {

    std::string src("Constant");
    
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;
    
    aw::getFirstParameterTrio(src, name, arguments, context, working);
    
    BOOST_CHECK_EQUAL(name, "Constant");
    BOOST_CHECK_EQUAL(arguments, "");
    BOOST_CHECK_EQUAL(context, ""); // context for refresh
    BOOST_CHECK_EQUAL(working, "");// remaining parameters

}


BOOST_AUTO_TEST_CASE(ParameterStringExtractionH) {

    std::string src("randomuniform");
    
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;
    
    aw::getFirstParameterTrio(src, name, arguments, context, working);
    
    BOOST_CHECK_EQUAL(name, "randomuniform");
    BOOST_CHECK_EQUAL(arguments, "");
    BOOST_CHECK_EQUAL(context, ""); // context for refresh
    BOOST_CHECK_EQUAL(working, "");// remaining parameters

}



BOOST_AUTO_TEST_CASE(TestStringToDouble) {

    std::string test1("2.4000");     
    BOOST_CHECK_CLOSE(aw::stringToDouble(test1), 2.4, .00001);
    BOOST_CHECK_EQUAL(aw::canConvertFromStringToDouble(test1), true);

    std::string test2("3.8000");     
    BOOST_CHECK_CLOSE(aw::stringToDouble(test2), 3.8, .00001);
    BOOST_CHECK_EQUAL(aw::canConvertFromStringToDouble(test2), true);

    std::string test3("valeu{3.8000}");     
    BOOST_CHECK_EQUAL(aw::canConvertFromStringToDouble(test3), false);

    std::string test5("3, 56,3");     
    BOOST_CHECK_EQUAL(aw::canConvertFromStringToDouble(test5), false);

    // TODO
    // this should be false
    std::string test4("230}/sdf");     
    BOOST_CHECK_EQUAL(aw::canConvertFromStringToDouble(test4), true);


}


BOOST_AUTO_TEST_CASE(TestArrayDoubleToString) {

    double a1[] = {2.3, 5.4, 8.2};
    std::string out1 = aw::arrayDoubleToString(a1, 3);
    BOOST_CHECK_EQUAL(out1, "2.3,5.4,8.2");
    std::vector<double> v1 = aw::stringToVectorDouble(out1);
    BOOST_CHECK_EQUAL(v1[0], 2.3);
    BOOST_CHECK_EQUAL(v1[2], 8.2);


    double a2[] = {200, 54, 123, 7, 3, 7, 2,  90, .00001};
    std::string out2 = aw::arrayDoubleToString(a2, 9);
    BOOST_CHECK_EQUAL(out2, "200,54,123,7,3,7,2,90,1e-05");

    std::vector<double> v2 = aw::stringToVectorDouble(out2);
    BOOST_CHECK_EQUAL(v2[0], 200);
    BOOST_CHECK_EQUAL(v2[8], 1e-05);

}



BOOST_AUTO_TEST_CASE(TestMod) {
    // works for both positive and negative; cannot be
    // be sure in other cases

    BOOST_CHECK_EQUAL(aw::mod(0, 3), 0);
    BOOST_CHECK_EQUAL(aw::mod(1, 3), 1);
    BOOST_CHECK_EQUAL(aw::mod(2, 3), 2);
    BOOST_CHECK_EQUAL(aw::mod(3, 3), 0);
    BOOST_CHECK_EQUAL(aw::mod(4, 3), 1);
    BOOST_CHECK_EQUAL(aw::mod(5, 3), 2);


    BOOST_CHECK_EQUAL(aw::mod(0, 3), 0);
    BOOST_CHECK_EQUAL(aw::mod(-1, 3), 2);
    BOOST_CHECK_EQUAL(aw::mod(-2, 3), 1);
    BOOST_CHECK_EQUAL(aw::mod(-3, 3), 0);
    BOOST_CHECK_EQUAL(aw::mod(-4, 3), 2);
    BOOST_CHECK_EQUAL(aw::mod(-5, 3), 1);
}



BOOST_AUTO_TEST_CASE(DoubleToIntProbabilistic) {

    BOOST_CHECK_EQUAL(aw::doubleToIntProabilistic(1) == 1, true);
    BOOST_CHECK_EQUAL(aw::doubleToIntProabilistic(2.0) == 2, true);

    BOOST_CHECK_EQUAL(aw::doubleToIntProabilistic(30.999) == 31, true);

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
    //aw::print(a, "aw::doubleToIntProabilistic 20.5");
    avg = sum / count;
    BOOST_CHECK_EQUAL(avg > 20.25 && avg < 20.75, true);
    sum = 0;

    //std::vector<aw::Int32Signed> b;
    for (int i=0; i<count; i++) {
        x = aw::doubleToIntProabilistic(40.75);
        sum += x;
        //b.push_back(x); 
    }
    avg = sum / count;
    //aw::print(b, "aw::doubleToIntProabilistic 40.85");
    BOOST_CHECK_EQUAL(avg > 40.5 && avg < 41, true);
    sum = 0;

    //std::vector<aw::Int32Signed> c;
    for (int i=0; i<count; i++) {
        x = aw::doubleToIntProabilistic(60.25);
        sum += x;
        //c.push_back(x); 
    }
    //aw::print(c, "aw::doubleToIntProabilistic 60.15");
    avg = sum / count;
    BOOST_CHECK_EQUAL(avg > 60 && avg < 60.5, true);
    sum = 0;
}



BOOST_AUTO_TEST_CASE(RandomInteger) {
    for (int i=0; i<30; i++) {
        BOOST_CHECK_EQUAL(aw::randomInt(3) < 3, true);
        //std::cout << "aw::randomInt(3): " << aw::randomInt(3) << std::endl; 
    }
    for (int i=0; i<30; i++) {
        BOOST_CHECK_EQUAL(aw::randomInt(11) < 11, true);
        //std::cout << "aw::randomInt(11): " << aw::randomInt(11) << std::endl; 
    }
}



BOOST_AUTO_TEST_CASE(ShuffleVector) {

    double sum(0);
    int count(0);
    double avg(0);

    std::vector<int> v;

    for (int i=0; i<30; i++) {
        v.push_back(i);
        // std::cout << "aw::randomInt(3): " << aw::randomInt(3) << std::endl; 
    }

    for (int i=0; i<30; i++) {
        // try to shuffle in place
        aw::shuffleVectorInt(v);
        sum = 0;
        count = 0;
        for (unsigned int i=0; i<v.size(); i++) {
            sum += v[i];
            count += 1;
        }
        avg = sum / count;
        //aw::print(b, "aw::doubleToIntProabilistic 40.85");
        BOOST_CHECK_EQUAL(avg, 14.5);
        BOOST_CHECK_EQUAL(sum, 435);

        //aw::print(v, "shuffle (post)");
    }


}

