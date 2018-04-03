//
// Created by gnowacki on 03.04.18.
//
#include <iostream>

#include "Broker.h"

int main() {
    std::cout << "Broker main" << std::endl;

    Broker broker;
    broker.waitForClients();

    return 0;
}

