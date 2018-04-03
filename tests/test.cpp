#define BOOST_TEST_MODULE ProgramTests
#include <boost/test/unit_test.hpp>

#define private public      //for test purpose

#include "../src/include/Broker.h"
#include "../src/include/Client.h"


BOOST_AUTO_TEST_SUITE(AllTests)

    BOOST_AUTO_TEST_CASE(GetBrokerInstanceTwiceResultsInSameInstance) {


        BOOST_CHECK_EQUAL(1, 2);
    }

BOOST_AUTO_TEST_SUITE_END()