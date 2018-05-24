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

    bool sayDontHaveFile(const std::string &fileName);

    bool sendFile(const std::string &filePath);
};
#endif //FILEEXCHANGER_CLIENTCONNECTION_H
