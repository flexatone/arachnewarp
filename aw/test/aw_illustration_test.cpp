// g++ -I ../src  aw_illustration_test.cpp ../src/aw_illustration.cpp ../src/aw_generator.cpp ../src/aw_common.cpp -DSTAND_ALONE -l boost_filesystem -l boost_system -l boost_unit_test_framework -l boost_unit_test_framework -l sndfile -Wall -o aw_illustration_test


#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE main
#endif  
#include <boost/test/unit_test.hpp>


#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>


#include "aw_common.h"
#include "aw_illustration.h"
#include "aw_generator.h"


// provide testCase, name
BOOST_AUTO_TEST_CASE(aw_illustration_test_1) {
	// std::cout << "running aw_illustration_test1 1" << std::endl;

    // as enum it has an interger value
    //BOOST_CHECK_EQUAL(aw::Gen_Name_Constant, 0);
	
	aw::TimeDomainGraph p;
	std::vector<aw::SampleType> v;
	for (int i=-100; i<100; ++i) {
		v.push_back(i);
	}
	aw::OutputCountType d(2);

    std::string fp("plot.plt");
    
    BOOST_REQUIRE_THROW(p.draw_vector(v, 0), std::invalid_argument);
    
	p.draw_vector(v, d);
    //p.print();
    //p.write(fp); 
}

BOOST_AUTO_TEST_CASE(aw_illustration_test_2) {
	std::cout << "running aw_illustration_test_2" << std::endl;
    
	aw::GeneratorShared g1 = aw::Generator::make(aw::Generator::ID_Buffer);
	// create two channel buffer
	g1->set_slot_by_index(0, 2);
	// for five second
	g1->set_slot_by_index(1, 5.0);
		
	// create 
	aw::GeneratorShared g2 = aw::Generator::make(aw::Generator::ID_Phasor);    
	g2->add_input_by_index(0, 4); // a constant frequency
	
	aw::GeneratorShared g3 = aw::Generator::make(aw::Generator::ID_Phasor);    
	g3->add_input_by_index(0, 12); // a constant frequency
	g3->add_input_by_index(0, -2); // a constant frequency

	aw::GeneratorShared g4 = aw::Generator::make(aw::Generator::ID_Phasor);    
	g4->add_input_by_index(0, 3); // a constant frequency

	// add phasor to buffer input; might scale buffer if necessary; could mix multiple too
	g1->add_input_by_index(0, g2);
	g1->add_input_by_index(1, g3);
	g1->add_input_by_index(1, g4);
    
    
    //g1->illustrate_network();
	//g1->illustrate_outputs();

	
    
}














