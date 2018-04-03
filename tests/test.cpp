#define BOOST_TEST_MODULE ProgramTests
#include <boost/test/unit_test.hpp>

#include "../src/include/Broker.h"
#include "../src/include/Client.h"

BOOST_AUTO_TEST_SUITE(DemoTest)

    BOOST_AUTO_TEST_CASE(FailTest) {
        BOOST_CHECK_EQUAL(5, fun(4));
    }

    BOOST_AUTO_TEST_CASE(PassTest) {
        BOOST_CHECK_EQUAL(5, fun(5));
    }

BOOST_AUTO_TEST_SUITE_END()