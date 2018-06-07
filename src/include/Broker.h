//
// Created by gnowacki on 03.04.18.
//

#ifndef FILEEXCHANGER_BROKER_H
#define FILEEXCHANGER_BROKER_H

#include "inc.h"
#include <Message.h>
#include <map>

class Broker {
private:
    static Broker* instancePtr;
    std::vector<pthread_t> threads;
    static std::map<int,std::vector<std::string>> clients;

public:
    void waitForClients();

    /* Static access method. */
    static Broker* getInstance();

    ~Broker();

    static void terminateWrapper(int arg);

private:
    static void* handleClient(void* msgsock);
    static void sendRequest(int socketId, uint32_t requestType);
    static int checkFilename(std::string filename);
    static void sendErrorToClient(int socket, std::string message);
    static bool checkFile(const std::string& name);

    void terminate();

    /* Private constructor to prevent instancing. */
    Broker();

    static message_header receiveMessage(char *buff, int bufSize, int socket);
};


#endif //FILEEXCHANGER_BROKER_H
