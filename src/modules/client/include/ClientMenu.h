//
// Created by gnowacki on 07.05.18.
//

#ifndef FILEEXCHANGER_CLIENTMENU_H
#define FILEEXCHANGER_CLIENTMENU_H

#include <../../../../src/include/Client.h>
#include <memory>
#include <../../../include/DataBase.h>
#include <../../../include/DirManagment.h>

class Client;

class ClientMenu
{
private:
    Client & client;
    std::shared_ptr<DataBase> clientDb;

    void menuManageConnection();
    void menuSettings();

    void sayUnknownOption();

public:
    ClientMenu(Client & client);

    void showMainMenu();
};

#endif //FILEEXCHANGER_CLIENTMENU_H
