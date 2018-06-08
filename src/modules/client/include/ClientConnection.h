//
// Created by dram on 23.05.18.
//

#ifndef FILEEXCHANGER_CLIENTCONNECTION_H
#define FILEEXCHANGER_CLIENTCONNECTION_H

#include <stdexcept>
#include <netinet/in.h>
#include "../include/inc.h"
#include <DataBase.h>
#include "../include/ConstVariables.h"
#include <memory>

class ClientConnection {
private:
    int socketId = 0;
    pthread_t recvThread;

    bool sendEhlo();
    std::shared_ptr<DataBase> clientDb;
public:
    ClientConnection(std::shared_ptr<DataBase>& clientDb);
    int connectToBroker(const std::string& brokerIp, uint16_t brokerPort);

    void disconnectFromBroker();
    bool isConnected();

    bool sendFileNames(const std::vector<std::string> &fileNames);

    bool sendFilesHashCode(size_t hashCode);

    bool requestForFile(const std::string &fileName);

    bool sendFile(const std::string &filePath, const std::string &fileNam);

    void startRecv();
    static void * recvThreadFunction(void *clientConnection);

    bool getAllFiles();
};
#endif //FILEEXCHANGER_CLIENTCONNECTION_H
