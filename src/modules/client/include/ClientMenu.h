//
// FileExchanger
// Created by gnowacki on 07.05.18.
//

#ifndef FILEEXCHANGER_CLIENTMENU_H
#define FILEEXCHANGER_CLIENTMENU_H

#include <memory>
#include <../../../include/DataBase.h>
#include <../../../include/DirManagment.h>
#include "ClientConnection.h"

class ClientMenu
{
private:
    std::shared_ptr<DataBase> clientDb = std::make_shared<DataBase>(clientSettingsFile);
    ClientConnection clientConnection = ClientConnection(clientDb);
    void menuManageConnection();
    void menuSettings();

    void sayUnknownOption();

public:
    ClientMenu();

    void showMainMenu();

    void menuDownloadFile();
};

#endif //FILEEXCHANGER_CLIENTMENU_H
