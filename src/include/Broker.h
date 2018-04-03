//
// Created by gnowacki on 03.04.18.
//

#ifndef FILEEXCHANGER_BROKER_H
#define FILEEXCHANGER_BROKER_H

#include "inc.h"

//TODO delete it
//int fun(int arg)
//{
//    return arg;
//}


class Broker {
private:
    static Broker* instancePtr;
    std::vector<pthread_t> threads;

public:
    static Broker& getInstance();
    void waitForClients(void);
private:
    static void* handleClient(void* msgsock);
    static void terminateWrapper(int arg);
    void terminate(int arg);
};


#endif //FILEEXCHANGER_BROKER_H
