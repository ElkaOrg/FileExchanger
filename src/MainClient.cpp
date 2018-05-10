//
// Created by gnowacki on 03.04.18.
//
#include <iostream>

#include "Client.h"

#include "include/DataBase.h"
#include "include/DirManagment.h"

int main()
{
    std::cout << "Client main" << std::endl;

    DirManagment dir("upload/");

    std::cout << dir.calculateDirHash() << std::endl;
    Client client;

    client.connectToBroker();


    return 0;
}
