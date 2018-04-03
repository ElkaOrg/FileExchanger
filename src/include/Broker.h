//
// Created by gnowacki on 03.04.18.
//

#ifndef FILEEXCHANGER_BROKER_H
#define FILEEXCHANGER_BROKER_H

#include "inc.h"

class Broker {
private:
    std::vector<pthread_t> threads;

public:
    void waitForClients(void);
private:
    static void* handleClient(void* msgsock);
    void terminate(int arg);
};


#endif //FILEEXCHANGER_BROKER_H
