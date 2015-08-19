// note: need ot include src dir for finding headers, and provide relative path to cpp srec files. 

// g++ -I ../src aw_common_assert.cpp ../src/aw_common.cpp -Wall -l boost_filesystem -l boost_system -o aw_common_assert


#include "aw_common.h"

#include <string>
#include <iostream>
#include <cassert>


bool test_1();
bool test_1() {
    // gets a boost path object
    std::string test = std::string(aw::get_fp_home());
    std::cout << test << std::endl;

    aw::Env e;
    
    std::cout << e.get_fp_temp("name.txt") << std::endl;
    
    return true;
}



int main() {

    assert(test_1());
    
}