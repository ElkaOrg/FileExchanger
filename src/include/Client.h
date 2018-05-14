//
// Created by gnowacki on 03.04.18.
//

#ifndef FILEEXCHANGER_CLIENT_H
#define FILEEXCHANGER_CLIENT_H

#include "inc.h"
#include "../modules/client/include/ClientMenu.h"


class Client {
private:
    int socketId = 0;
public:
    /**
     *
     * @return socketDescriptor
     */
    int connectToBroker();

    void disconnectFromBroker();

    bool isConnected();
};


#endif //FILEEXCHANGER_CLIENT_H
