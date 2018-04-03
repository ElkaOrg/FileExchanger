//
// Created by gnowacki on 03.04.18.
//

#ifndef FILEEXCHANGER_BROKER_H
#define FILEEXCHANGER_BROKER_H

#include "inc.h"

class Broker {
private:
    static Broker* instancePtr;
    std::vector<pthread_t> threads;

public:
    void waitForClients(void);

    /* Static access method. */
    static Broker* getInstance();

    ~Broker();

    static void terminateWrapper(int arg);

private:
    static void* handleClient(void* msgsock);
    void terminate(int arg);

    /* Private constructor to prevent instancing. */
    Broker();
};


#endif //FILEEXCHANGER_BROKER_H
