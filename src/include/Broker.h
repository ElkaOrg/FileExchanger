//
// Created by gnowacki on 03.04.18.
//

#ifndef FILEEXCHANGER_BROKER_H
#define FILEEXCHANGER_BROKER_H

#define BROKER_PORT 8080
#define BROKER_ADDR "127.0.0.1"

#include "SocketWrapper.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <pthread.h>
#include "zconf.h"  //good include???

//TODO delete it
int fun(int arg)
{
    return arg;
}


class Broker {
public:
    void waitForClients(void);
private:
    static void* handleClient(void* msgsock);
};


#endif //FILEEXCHANGER_BROKER_H
