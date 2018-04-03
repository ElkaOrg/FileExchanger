//
// Created by gnowacki on 03.04.18.
//
#include <iostream>

#include "Broker.h"

Broker* Broker::instancePtr = NULL;     //if not present causes undefined reference

int main() {
    std::cout << "Broker main" << std::endl;

    Broker broker = Broker::getInstance();
    broker.waitForClients();

    return 0;
}

