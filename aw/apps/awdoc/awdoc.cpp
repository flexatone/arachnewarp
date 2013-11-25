// g++-4.7 -std=c++11 awdoc.cpp -I ../../src ../../src/aw_generator.cpp ../../src/aw_common.cpp ../../src/aw_timer.cpp ../../src/aw_illustration.cpp -l boost_filesystem -l boost_system -l sndfile -L /usr/local/lib -Wall -O3 -o awdoc

#include "aw_generator.h"

using namespace aw;


int main() {

    GenPtr g = Gen::make(GenID::Constant);
    g->doc();

}