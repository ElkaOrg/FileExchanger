//
// Created by gnowacki on 03.04.18.
//
#include <iostream>

#include "Client.h"

int main()
{
    std::cout << "Client main" << std::endl;

    Client client;

    client.connectToBroker();

    return 0;
}
