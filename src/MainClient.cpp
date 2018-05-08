//
// Created by gnowacki on 03.04.18.
//
#include <iostream>

#include "Client.h"

#include "include/DataBase.h"

int main()
{
    std::cout << "Client main" << std::endl;

    DataBase db("settings.txt");
    db.saveKey("path", "xyzdasdz");
    db.saveKey("path", "xyzdasdz");
    db.saveKey("path", "xyzdasdz3");
    db.saveKey("path", "xyzdasdz3");
    db.saveKey("path3", "xyzdasdz3");
    db.saveKey("path3", "xyzdasdz35");

    std::cout << db.getKey("path3");
    Client client;

    client.connectToBroker();


    return 0;
}
