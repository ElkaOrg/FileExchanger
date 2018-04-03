#define BOOST_TEST_MODULE ProgramTests
#include <boost/test/unit_test.hpp>

#define private public      //for test purpose

#include "../src/include/Broker.h"
#include "../src/include/Client.h"

Broker* Broker::instancePtr = NULL;

BOOST_AUTO_TEST_SUITE(AllTests)

    BOOST_AUTO_TEST_CASE(GetBrokerInstanceTwiceResultsInSameInstance) {

        Broker broker1 = Broker::getInstance();
        Broker broker2 = Broker::getInstance();

        BOOST_CHECK_EQUAL(broker1.instancePtr, broker2.instancePtr);    //ignore error because #define private public
    }

BOOST_AUTO_TEST_SUITE_END()