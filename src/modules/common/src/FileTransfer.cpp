//
// Created by gnowacki on 07.05.18.
//

#include <DirManagment.h>
#include "../include/FileTransfer.h"

int calcReadBytes(int bufferSizeForFile, int fileSize){
    if(bufferSizeForFile > fileSize){
        return bufferSizeForFile;
    }
    return fileSize;
}
int FileTransfer::sendOneFile(int socketId, const std::string &filePath, const std::string &fileName) {
    const int maxTxtBuffer = 1024; // how many data can be readed from file

    if (!DirManagment::isValidFile(filePath)) {
        sayDontHaveFile(socketId, fileName);
    }

    std::fstream file;
    file.open(filePath.c_str(), std::ios::in | std::ios::binary);

    if(!file.good()){
        std::cout << "Opening error";
        return 1;
    }
    //get size of file
    std::streampos fileSize = 0;
    fileSize = file.tellg();
    file.seekg(0, std::ios::end);
    fileSize = file.tellg() - fileSize;
    file.seekg(0, std::ios::beg); // go back to the beggining


    message_header msg;
    msg.type = htonl(6);

    if(fileNameMaxLength + fileSize < maxTxtBuffer){
        msg.size = htonl(fileNameMaxLength + fileSize); // we assume that size fit to maxTextBuffer
    } else {
        msg.size = htonl(maxTxtBuffer - sizeof(message_header)); //default, we assume that size fit to maxTextBuffer
    }
    char buffer[maxTxtBuffer];
    int bufferSizeForFile = maxTxtBuffer - sizeof(message_header) - fileNameMaxLength;

    memset(buffer, 0, maxTxtBuffer);
    memcpy(buffer, &msg, sizeof(message_header)); // message header
    memcpy(buffer+sizeof(message_header), fileName.c_str(), fileName.size()); // 40 bytes for filename

    int readedBytes = calcReadBytes(bufferSizeForFile, fileSize);

    file.read(buffer+sizeof(msg)+fileNameMaxLength, readedBytes);
    fileSize -= readedBytes;
    write(socketId, buffer, sizeof(buffer));

    while(fileSize > 0){
        // continue sending file
        msg.type = htonl(7);
        readedBytes = calcReadBytes(bufferSizeForFile, fileSize);
        msg.size = htonl(readedBytes + fileNameMaxLength);

        fileSize-= readedBytes;
        write(socketId, buffer, sizeof(buffer));
        sleep(1);
    }
    file.close();

    return true;
}

int FileTransfer::recvOneFile(const std::string &folderPath, char *buf, int length) {
    char typeAndSize[8] = {0};
    memcpy(typeAndSize, buf, sizeof(typeAndSize));
    auto *header = (struct message_header *) typeAndSize;
    header->type = ntohl(header->type);
    header->size = ntohl(header->size);

    char fileName[40] = {0};
    memcpy(fileName, buf + 8, 40); //copy file name

    std::string fileNameString = std::string(fileName); //auto removes 0

    std::fstream file;
    std::string oldName = folderPath + "/" + "_tmp_" + fileNameString;
    file.open(oldName, std::ios::out | std::ios::ate | std::ios::binary);
    file.write(buf + 8 + 40, header->size - 40);
    file.close();

    std::string newName = folderPath + "/" + fileNameString;

    std::rename(oldName.c_str(), newName.c_str());

    return 0;
}

std::string FileTransfer::parseFileName(const char *fileName, int n) {
    std::string result;
    for (int i = 0; i < n && fileName[i] != 0; i++) {
        result += fileName[i];
    }
    return result;
}

bool FileTransfer::sayDontHaveFile(int socketId, const std::string &fileName) {
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
