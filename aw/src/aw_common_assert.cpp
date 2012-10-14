// g++ aw_common_assert.cpp aw_common.h aw_common.cpp -Wall -l boost_filesystem -l boost_system -o aw_common_assert


#include "aw_common.h"

#include <string>
#include <iostream>
#include <cassert>

bool test_1() {
    // gets a boost path object
    std::string test = aw::get_fp_home().string();
    std::cout << test << std::endl;


    aw::Environment e;
    
    std::cout << e.get_fp_plot() << std::endl;
    
    
    return true;
}



int main() {

    assert(test_1());
    
}