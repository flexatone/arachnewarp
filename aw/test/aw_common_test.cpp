// g++-4.7 -std=c++11 -I ../src aw_common_test.cpp ../src/aw_common.cpp -DSTAND_ALONE -l boost_filesystem -l boost_system -l boost_unit_test_framework -Wall -o aw_common_test

// clang++ -I ../src aw_common_test.cpp ../src/aw_common.cpp -DSTAND_ALONE -l boost_filesystem -l boost_system -l boost_unit_test_framework -Wall -o aw_common_test

// to add -std=c++11 -stdlib=libc++



#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE main
#endif

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

// stand-alone way of testing with non-compiled boost
//#define BOOST_TEST_MODULE
//#include <boost/test/included/unit_test.hpp>
//#include <boost/test/included/unit_test_framework.hpp> 

#include <iostream>
#include <string>
#include <list>

#include "aw_common.h"

using namespace aw;



BOOST_AUTO_TEST_CASE(aw_common_test1) {
	//std::cout << "running aw_common_test 1" << std::endl;

    boost::filesystem::path home(get_fp_home());
    // std::cout << home << std::endl;
        
}

BOOST_AUTO_TEST_CASE(aw_common_test2) {
	//std::cout << "running aw_common_test 2" << std::endl;
	
	//EnvironmentPtr es = EnvironmentPtr(new Environment);

    EnvironmentPtr es = Environment::get_default_env();
    
	std::string fp = es->get_fp_temp("test.txt");
	std::string match("test.txt");
	// give starting position, in source, length of target, target
	
	BOOST_CHECK_EQUAL(fp.compare(fp.length() - match.length(), 
					match.length(), 
					match), 0);
					
	BOOST_CHECK_EQUAL(es->get_sampling_rate(), 44100);
}


BOOST_AUTO_TEST_CASE(aw_common_test3) {
	//std::cout << "running aw_common_test 1" << std::endl;

    std::string in("<a test {34}>");

    escape(in, "<>{}", "\\");

    BOOST_CHECK_EQUAL(in, "\\<a test \\{34\\}\\>");

        
}


BOOST_AUTO_TEST_CASE(aw_common_test_inline_1) {

    BOOST_CHECK_EQUAL(double_limiter(.5, 0, 1), .5);
    BOOST_CHECK_EQUAL(double_limiter(1.5, 0, 1), 1);
    BOOST_CHECK_EQUAL(double_limiter(-.5, 0, 1), 0);

}

BOOST_AUTO_TEST_CASE(aw_test_true_min_max) {
    SampleT min(-1);
    SampleT max(-1);
    true_min_max(100, 4, &min, &max);

    BOOST_CHECK_EQUAL(min, 4);
    BOOST_CHECK_EQUAL(max, 100);
    
    min = -1;
    max = -1;
    true_min_max(4, 100, &min, &max);
    BOOST_CHECK_EQUAL(min, 4);
    BOOST_CHECK_EQUAL(max, 100);

    min = -1;
    max = -1;
    true_min_max(3000, -12, &min, &max);
    BOOST_CHECK_EQUAL(min, -12);
    BOOST_CHECK_EQUAL(max, 3000);

}



BOOST_AUTO_TEST_CASE(aw_test_mtof) {

    SampleT x;
    x = mtof(69);
    BOOST_CHECK_CLOSE(x, 440.0, .0001);

    x = mtof(69+12);
    BOOST_CHECK_CLOSE(x, 880.0, .0001);

}

BOOST_AUTO_TEST_CASE(aw_test_phase_limiter_a) {
    SampleT a(3.0);
    
    phase_limiter(a);
    BOOST_CHECK_CLOSE(a, 3.0, .0001);
    
    a = 6.8;
    phase_limiter(a);
    BOOST_CHECK_CLOSE(a, 0.516814, .001);

    a = -3.14 * 6;
    phase_limiter(a);
    //std::cout << "here: " << a << std::endl;
    BOOST_CHECK_CLOSE(a, 0.00955592, .001);
}


BOOST_AUTO_TEST_CASE(aw_test_environment_1) {

//    EnvironmentPtr e1 = Environment::make();
//    BOOST_CHECK_EQUAL(e1->get_common_frame_size(), 64);

    EnvironmentPtr e2 = Environment::make_with_frame_size(128);
    BOOST_CHECK_EQUAL(e2->get_common_frame_size(), 128);

    
}	


BOOST_AUTO_TEST_CASE(aw_test_environment_2) {
    // test setting and making the default environment
    EnvironmentPtr e1 = Environment::get_default_env();
    BOOST_CHECK_EQUAL(e1->get_common_frame_size(), 64);

    EnvironmentPtr e2 = Environment::make_with_frame_size(128);
    Environment::set_default_env(e2);
    EnvironmentPtr e3 = Environment::get_default_env();
    BOOST_CHECK_EQUAL(e3->get_common_frame_size(), 128);
    
    // call with no args to clear newly set value
    Environment::set_default_env();

}	


BOOST_AUTO_TEST_CASE(aw_test_rounded) {

    BOOST_CHECK_EQUAL(std::round(1.5), 2);
    BOOST_CHECK_EQUAL(std::round(1.4), 1);
    BOOST_CHECK_EQUAL(std::round(-1.4), -1);
    BOOST_CHECK_EQUAL(std::round(-1.5), -2);
    
}



BOOST_AUTO_TEST_CASE(aw_buffer_injector_a) {

    Inj<SampleT> bi({3, 6, 2, 3, 5});
    BOOST_CHECK_EQUAL(bi.get_frame_size(), 5);
    BOOST_CHECK_EQUAL(bi.get_channels(), 1);
    
}



BOOST_AUTO_TEST_CASE(aw_inj_a) {

    Inj<SampleT> bi({3, 6, 2, 3, 5});
    BOOST_CHECK_EQUAL(bi.get_frame_size(), 5);
    BOOST_CHECK_EQUAL(bi.get_channels(), 1);
    
}



BOOST_AUTO_TEST_CASE(aw_buffer_injector_b) {

    std::vector<SampleT> post;

    Inj<SampleT> bi1({ {1, .5}, {10, .2}, {20, .5}});
    BOOST_CHECK_EQUAL(bi1.get_frame_size(), 3);
    BOOST_CHECK_EQUAL(bi1.get_channels(), 2);


    Inj<SampleT> bi2({ {1}, {10, .2}, {20, .5}});
    BOOST_CHECK_EQUAL(bi2.get_frame_size(), 3);
    BOOST_CHECK_EQUAL(bi2.get_channels(), 2);
    
    bi2.fill_interleaved(post);
    BOOST_CHECK_EQUAL(post[0], 1);
    BOOST_CHECK_EQUAL(post[1], 0); // filled zero!
    BOOST_CHECK_EQUAL(post[2], 10);
    BOOST_CHECK_EQUAL(post[3], .2);
    BOOST_CHECK_EQUAL(post[4], 20);
    BOOST_CHECK_EQUAL(post[5], .5);
    
}


BOOST_AUTO_TEST_CASE(aw_buffer_injector_c) {
    Inj<SampleT> bi({3, 6, 2, 3, 5});
    std::vector<SampleT> post;
    bi.fill_interleaved(post);
    BOOST_CHECK_EQUAL(post[0], 3);
    BOOST_CHECK_EQUAL(post[1], 6);
    BOOST_CHECK_EQUAL(post[2], 2);
}


BOOST_AUTO_TEST_CASE(aw_random_a) {

    std::list<int> col;
    for (int x=0;x<5;++x) {
//        std::cout << "Random::uniform(): " << Random::uniform() << std::endl;
        col.push_back(Random::uniform());
    }
    for (int x=0;x<5;++x) {
        //std::cout << "Random::uniform_bi_polar(): " << Random::uniform_bi_polar() << std::endl;
        col.push_back(Random::uniform_bi_polar());
    }    
    for (int x=0;x<10;++x) {
//        std::cout << "Random::round(11.7): " << Random::round(11.7) << std::endl;
        col.push_back(Random::round(11.7));
        
    }
    for (int x=0;x<10;++x) {
        //std::cout << "Random::round(8.1): " << Random::round(8.1) << std::endl;
        col.push_back(Random::round(8.1));
        
    }

    for (int x=0;x<10;++x) {
        //std::cout << "Random::uniform_switch(): " << Random::uniform_switch() << std::endl;
        col.push_back(Random::uniform_switch());
        
    }
    
}



BOOST_AUTO_TEST_CASE(aw_to_samp) {
    BOOST_CHECK_EQUAL(mtof(69), 440.0);
    BOOST_CHECK_EQUAL(mtof(60), 261.62556530059862);

    // thes return an integer
    BOOST_CHECK_EQUAL(ftosamps(2, 44100, 22050), 22050);
    BOOST_CHECK_EQUAL(ftosamps(441, 44100, 22050), 100);

    // checks for min fq to avoid divide by zero
    BOOST_CHECK_EQUAL(ftosamps(0, 44100, 22050), 4410000000);


    BOOST_CHECK_EQUAL(mtosamps(69, 44100, 22050), 100);
        
}













