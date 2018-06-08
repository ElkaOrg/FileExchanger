//
// Created by gnowacki on 03.04.18.
//

#ifndef FILEEXCHANGER_BROKER_H
#define FILEEXCHANGER_BROKER_H

#include "inc.h"
#include "FileWait.h"
#include <Message.h>
#include <map>
#include <unordered_map>

class Broker {
private:
    static Broker* instancePtr;
    std::vector<pthread_t> threads;
    static std::unordered_map<int,std::vector<std::string>> clients;
    static std::vector <FileWait> fileWaits;
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
    static void sendRequestForFile(int socket, std::string filename, char* buff);

    void terminate();

    /* Private constructor to prevent instancing. */
    Broker();

    static message_header receiveMessage(char *buff, int bufSize, int socket, int * readedBytes);

    void static checkFiles(std::vector<FileWait> &fileWaits);
};


#endif //FILEEXCHANGER_BROKER_H
