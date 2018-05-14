//
// Created by gnowacki on 03.04.18.
//
#include <iostream>

#include "include/Client.h"

#include "include/DataBase.h"
#include "include/DirManagment.h"

int main()
{
    Client client;
    client.connectToBroker();


    ClientMenu clientMenu(client);
    clientMenu.showMainMenu();
    return 0;
}
