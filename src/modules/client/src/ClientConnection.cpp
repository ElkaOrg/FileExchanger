//
// Created by dram on 23.05.18.
//

#include <Message.h>
#include <boost/shared_ptr.hpp>
#include <DirManagment.h>
#include "../include/ClientConnection.h"

int ClientConnection::connectToBroker(const std::string &brokerIp, const uint16_t brokerPort) {
    if (socketId > 0) {
        throw std::runtime_error("Already connected?!");
    }

    struct sockaddr_in server{};

    socketId = socket(AF_INET, SOCK_STREAM, 0);
    if (socketId < 0) {
        throw std::runtime_error("Socket opening error!");
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(brokerIp.c_str());
    server.sin_port = htons(brokerPort);

    if (connect(socketId, (struct sockaddr *) &server, sizeof server) == -1) {
        throw std::runtime_error("Cant connect!");;
    }

    //send ehlo
    startRecv();
    sendEhlo();
    return socketId;
}

void ClientConnection::startRecv() {
    pthread_create(&recvThread, NULL, &ClientConnection::recvThreadFunction, this);
}


void *ClientConnection::recvThreadFunction(void *object) {
    auto *connection = (ClientConnection *) object;
    char buf[512] = {0};
    char typeAndSize[8] = {0};
    ssize_t bytesRead = 0;
    do {
        bytesRead = read(connection->socketId, buf, sizeof(buf));
        memcpy(typeAndSize, buf, sizeof(typeAndSize));
        auto *header = (struct message_header *) typeAndSize;
        header->type = ntohl(header->type);
        header->size = ntohl(header->size);

        std::string folderPath = "";
        char fileName[40];

        std::cout << "Got header!" << header->type << std::endl;

        switch (header->type) {
            case 0: {
                break;
            }
            case 1: {
                folderPath = connection->clientDb.get()->getKey(clientSharedFolderKey);
                if (folderPath.empty()) {
                    std::cout << "Brak wpisanej sciezki udostepnianego folderu!" << std::endl;
                } else {
                    DirManagment dirMgmt = DirManagment(folderPath);
                    connection->sendFileNames(dirMgmt.getAllFileNames());
                }
                break;
            }
            case 2: {
                folderPath = connection->clientDb.get()->getKey(clientSharedFolderKey);
                if (folderPath.empty()) {
                    std::cout << "Brak wpisanej sciezki udostepnianego folderu!" << std::endl;
                } else {
                    DirManagment dirMgmt = DirManagment(folderPath);
                    connection->sendFilesHashCode(dirMgmt.calculateDirHash());
                }
                break;
            }
            case 3: {

                int nrOfFiles = header->size / 40;
                for (int i = 0; i <= nrOfFiles; i++) {
                    char filename[40] = {0};
                    memcpy(filename, buf + 8 + 40 * i, sizeof(filename));
                    std::cout << fileName << std::endl;
                }

                break;
            }
            case 4: {
                if (header->size != 40) {
                    throw std::runtime_error("Invalid file size");
                }
                memset(fileName, 0x00, sizeof(fileName));
                memcpy(fileName, buf + 8, sizeof(fileName));

                std::string folderPath = connection->clientDb.get()->getKey(clientSharedFolderKey);
                if (folderPath == "") {
                    std::cout << "Brak wpisanej sciezki udostepnianego folderu!" << std::endl;
                } else {
                    DirManagment dirMgmt = DirManagment(folderPath);
                    connection->sendFile(folderPath + std::string(fileName));
                }
                break;
            }
            case 5: {
                if (header->size != 40) {
                    throw std::runtime_error("Invalid file size");
                }
                memset(fileName, 0x00, sizeof(fileName));
                memcpy(fileName, buf + 8, sizeof(fileName));
                std::cout << "Nie MA TAKIEGO PLIKU!" << std::string(fileName) << std::endl;
                break;
            }
            case 6: {
                if (header->size > 500) {
                    throw std::runtime_error("Tresc bledu jest za dluga!");
                }
                char errorMsg[header->size] = {0};
                memcpy(errorMsg, buf + 8, sizeof(header->size));
                std::cout << "Broker Blad!: " << errorMsg << std::endl;

                break;
            }
            default:
                throw std::runtime_error("Unknown header");
        }

    } while (bytesRead > 0);
    //start reading again

    recvThreadFunction(object);
}

void ClientConnection::disconnectFromBroker() {
    close(socketId);
    socketId = 0;
}

bool ClientConnection::isConnected() {
    return socketId > 0;
}
bool ClientConnection::getAllFiles() {
    message_header msg;
    msg.type = htonl(3);
    msg.size = 0;

    size_t size = sizeof(msg);
    auto buffer = new char[size];
    memset(buffer, 0x00, size);

    memcpy(buffer, &msg, size);

    write(socketId, buffer, size);
    delete[] buffer;
    return true;
}

bool ClientConnection::sendEhlo() {
    message_header msg;
    msg.type = 0;
    msg.size = 0;

    size_t size = sizeof(msg);
    auto buffer = new char[size];
    memset(buffer, 0x00, size);

    memcpy(buffer, &msg, size);

    write(socketId, buffer, size);
    delete[] buffer;

    /*
     * 1- klient
    std::string folderPath = clientDb.get()->getKey(clientSharedFolderKey);
     if(folderPath == ""){
        std::cout << "Brak wpisanej sciezki udostepnianego folderu!" << std::endl;
     }
     else {
     DirManagment dirMgmt = DirManagment(folderPath);
    sendFileNames(dirMgmt.getAllFileNames());
     }
     */

    /*
    2 - klient hashcode
     */
    /*std::string folderPath = clientDb.get()->getKey(clientSharedFolderKey);
    if(folderPath == ""){
        std::cout << "Brak wpisanej sciezki udostepnianego folderu!" << std::endl;
    }
    else {
        DirManagment dirMgmt = DirManagment(folderPath);
        sendFilesHashCode(dirMgmt.calculateDirHash());
    }
    */

    /*
     * 3 - klient prosi o plik brokera


    std::string requestFile = "abc.xyz";
    requestForFile(requestFile);
     */

    /*
    /**
     * send file

    std::string folderPath = clientDb.get()->getKey(clientSharedFolderKey);
    if (folderPath == "") {
        std::cout << "Brak wpisanej sciezki udostepnianego folderu!" << std::endl;
    } else {
        DirManagment dirMgmt = DirManagment(folderPath);
        sendFile(folderPath + "f1");
    }
*/
    return true;
}

bool ClientConnection::sendFileNames(const std::vector<std::string> &fileNames) {
    message_header msg;
    msg.type = htonl(1);
    msg.size = htonl(fileNameMaxLength * fileNames.size());

    size_t size = sizeof(msg) + fileNameMaxLength * fileNames.size();
    auto buffer = new char[size];
    memset(buffer, 0x00, size);

    memcpy(buffer, &msg, sizeof(msg));

    int i = 0;
    for (auto &name : fileNames) {
        memcpy(buffer + sizeof(msg) + fileNameMaxLength * i, name.c_str(), name.length());
        i++;
    }
    write(socketId, buffer, size);
    delete[] buffer;

    return true;
}

bool ClientConnection::sendFilesHashCode(std::size_t hashCode) {
    message_header msg;
    msg.type = htonl(2);
    msg.size = htonl(sizeof(size_t));

    size_t size = sizeof(msg) + msg.size;
    auto buffer = new char[size];
    memset(buffer, 0x00, size);

    memcpy(buffer, &msg, sizeof(msg));
    memcpy(buffer + sizeof(msg), &hashCode, msg.size);
    write(socketId, buffer, size);
    delete[] buffer;

    return true;
}

bool ClientConnection::requestForFile(const std::string &fileName) {
    message_header msg;
    msg.type = htonl(3);
    msg.size = htonl(fileNameMaxLength);

    size_t size = sizeof(msg) + msg.size;
    auto buffer = new char[size];
    memset(buffer, 0x00, size);

    memcpy(buffer, &msg, sizeof(msg));
    memcpy(buffer + sizeof(msg), fileName.c_str(), fileName.size());
    write(socketId, buffer, size);
    delete[] buffer;

    return true;
}

bool ClientConnection::sayDontHaveFile(const std::string &fileName) {
    message_header msg;
    msg.type = htonl(5);
    msg.size = htonl(fileNameMaxLength);

    size_t size = sizeof(msg) + msg.size;
    auto buffer = new char[size];
    memset(buffer, 0x00, size);

    memcpy(buffer, &msg, sizeof(msg));
    memcpy(buffer + sizeof(msg), fileName.c_str(), fileName.size());
    write(socketId, buffer, size);
    delete[] buffer;

    return true;
}

bool ClientConnection::sendFile(const std::string &filePath) {

    const int maxTxtBuffer = 1024;

    FILE *fp = fopen(filePath.c_str(), "rb");
    if (fp == NULL) {
        printf("File open error");
        return 1;
    }
    //get size of file
    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    fseek(fp, 0, 0);

    message_header msg;
    msg.type = htonl(4);
    msg.size = htonl(fileNameMaxLength + fileSize);

    //set max msg buffer


    size_t size = sizeof(msg) + msg.size;
    char *buffer;
    size_t bufferSize = 0;

    if (size > sizeof(msg) + maxTxtBuffer) {
        buffer = new char[maxTxtBuffer + sizeof(msg)];
        bufferSize = maxTxtBuffer + sizeof(msg);
    } else {
        buffer = new char[size];
        bufferSize = size;
    }
    memset(buffer, 0x00, bufferSize);

    memcpy(buffer, &msg, sizeof(msg));//first read
    size_t nread = fread(buffer + sizeof(msg), 1, maxTxtBuffer, fp);
    fileSize -= nread;
    write(socketId, buffer, bufferSize);
    while (fileSize > 0) {
        memset(buffer, 0x00, bufferSize);
        nread = fread(buffer, 1, bufferSize, fp);
        fileSize -= nread;
        /* If read was success, send data. */
        if (nread > 0) {
            //printf("Sending \n");
            write(socketId, buffer, bufferSize);
        }
    }
    fclose(fp);//TODO error check
    delete[] buffer;

    return true;
}

ClientConnection::ClientConnection(std::shared_ptr<DataBase> &clientDb) : clientDb(clientDb) {

}
