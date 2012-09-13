
// g++ aw_test.cpp -Wall -o aw_test

#define BOOST_TEST_MODULE "test runner"

#include <boost/test/included/unit_test.hpp>
#include <boost/test/included/unit_test_framework.hpp> 


// should just be able to combine other test files

#include "aw_common_test.cpp"

BOOST_FIXTURE_TEST_SUITE(MyFooTest, CMyFooTestFixture);


