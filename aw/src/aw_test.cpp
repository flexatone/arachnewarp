
// g++ aw_Common_test.cpp aw_Common.cpp -Wall -o aw_Common_test

#define BOOST_TEST_MODULE "test runner"

#include <boost/test/included/unit_test.hpp>
#include <boost/test/included/unit_test_framework.hpp> 


BOOST_FIXTURE_TEST_SUITE(MyFooTest, CMyFooTestFixture);


