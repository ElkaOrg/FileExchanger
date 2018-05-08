#define BOOST_TEST_MODULE ProgramTests
#include <boost/test/unit_test.hpp>
#include <modules/common/include/FileTransfer.h>

#define private public      //for test purpose

#include "../src/include/Broker.h"
#include "../src/include/Client.h"


BOOST_AUTO_TEST_SUITE(AllTests)

    BOOST_AUTO_TEST_CASE(GetBrokerInstanceTwiceResultsInSameInstance) {
        Broker* broker1 = Broker::getInstance();
        Broker* broker2 = Broker::getInstance();

        BOOST_CHECK_EQUAL(broker1, broker2);
    }

    BOOST_AUTO_TEST_CASE(checkIfFileIsPlacedIntoBufferInFileTransferSendOneFile) {
        FileTransfer fileTransfer(1);   //doesn't matter what socket descriptor we pass in this test

        fileTransfer.sendOneFile("files_for_tests/file1.txt");

        BOOST_CHECK_EQUAL(1, 1);
    }

BOOST_AUTO_TEST_SUITE_END()