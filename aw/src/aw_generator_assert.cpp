// g++ aw_generator_assert.cpp aw_generator.cpp aw_common.cpp -Wall -o aw_generator_assert

#include <cassert>


#include "aw_generator.h"
#include "aw_common.h"


int main() {

	// can creat the table subclass
	aw::Constant g3;
    assert(g3.get_parameter_count() == 1);


    aw::ConstantShared g4 = aw::ConstantShared(new aw::Constant);
    assert(g4->get_parameter_count() == 1);
	
}