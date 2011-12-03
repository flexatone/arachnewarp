// g++ aw_Common_atest.cpp ../core/aw_Common.cpp -Wall -I "/Volumes/xdisc/_sync/_x/src/cpp/boost_1_43_0" -I "/Volumes/xdisc/_sync/_x/src/arachnewarp/arachnewarp/src/core" -o aw_Common_atest

#include <iostream>
#include <cassert>
#include <list>
#include <vector>
#include <string>

#include "aw_Common.h"


//------------------------------------------------------------------------------
void testTimeContext() {
    assert(aw::timeContextNameSample == 0);
}


// test a basic math opperations
void testMath() {
    assert(aw::mod(4, 3) == 1);
    assert(aw::mod(3, 3) == 0);
    aw::print(aw::bpmToSecondTime(30));
    assert(aw::bpmToSecondTime(30) == 2);
}


void testPrinting() {
    aw::print("testing!");

    int a1[] = {3, 20, 3000};
    aw::print(a1, 3);

    double a2[] = {3.2, 20.54, 30.000014};
    aw::print(a2, 3);

    std::vector<int> v1;
    v1.push_back(30);
    v1.push_back(300000);
    aw::print(v1);

}


void testParameterStringExtraction() {
    std::string src("RandomUniform{refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}}");
    
    std::string name; 
    std::string arguments;
    std::string context;
    std::string working;
    
    aw::getFirstParameterTrio(src, name, arguments, context, working);
    
    aw::print(name);
    assert(name == std::string("RandomUniform"));

    aw::print(arguments);
    assert(arguments ==             
        std::string(
    "refresh{Constant{1}}{trigger}minimum{Constant{0}}maximum{Constant{1}}")
        );
    assert(context == std::string(""));     
    assert(working == std::string(""));     
}

int main() {
    std::cout << "aw_Common_atest.cpp" << std::endl;

    testTimeContext(); 
    testMath();
    testPrinting();

    testParameterStringExtraction();

}