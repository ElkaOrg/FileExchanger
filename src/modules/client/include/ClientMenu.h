//
// Created by gnowacki on 07.05.18.
//

#ifndef FILEEXCHANGER_CLIENTMENU_H
#define FILEEXCHANGER_CLIENTMENU_H

#include "../../../include/inc.h"

class ClientMenu
{
private:
    int socketDescriptor;

public:
    ClientMenu(int socketDescriptor);

    void showMainMenu(void);
};

#endif //FILEEXCHANGER_CLIENTMENU_H
