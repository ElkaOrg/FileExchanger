//
// FileExchanger
// Created by gnowacki on 07.05.18.
// Implementation by dram
//

#include <DirManagment.h>
#include "../include/FileTransfer.h"

int calcReadBytes(int bufferSizeForFile, int fileSize) {
    if (bufferSizeForFile > fileSize) {
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

    if (!file.good()) {
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

    if (fileNameMaxLength + fileSize < maxTxtBuffer) {
        msg.size = htonl(fileNameMaxLength + fileSize); // we assume that size fit to maxTextBuffer
        msg.type = htonl(7);
    } else {
        msg.type = htonl(6); // size does not fit, divide
        msg.size = htonl(maxTxtBuffer - sizeof(message_header)); //default, we assume that size fit to maxTextBuffer
    }
    char buffer[maxTxtBuffer];
    int bufferSizeForFile = maxTxtBuffer - sizeof(message_header) - fileNameMaxLength;

    memset(buffer, 0, maxTxtBuffer);
    memcpy(buffer, &msg, sizeof(message_header)); // message header
    memcpy(buffer + sizeof(message_header), fileName.c_str(), fileName.size()); // 40 bytes for filename

    int readedBytes = calcReadBytes(bufferSizeForFile, fileSize);

    file.read(buffer + sizeof(msg) + fileNameMaxLength, readedBytes);
    fileSize -= readedBytes;
    write(socketId, buffer, sizeof(buffer));

    while (fileSize > 0) {
        // continue sending file
        readedBytes = calcReadBytes(bufferSizeForFile, fileSize);
        msg.size = htonl(readedBytes + fileNameMaxLength);

        fileSize -= readedBytes;
        write(socketId, buffer, sizeof(buffer));

        //that was last packet
        if (fileSize == 0) {
            msg.type = htonl(7);
        } else {
            msg.type = htonl(6);
        }

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
    memcpy(fileName, buf + sizeof(message_header), fileNameMaxLength); //copy file name

    std::string fileNameString = parseFileName(fileName, sizeof(fileName)); //auto removes 0

    std::fstream file;
    std::string oldName = folderPath + "_tmp_" + fileNameString;
    file.open(oldName, std::ios::out | std::ios::ate | std::ios::binary);
    if(!file.good()){
        std::cout << "Failed when receiving, a file! Could not open file!";
    }
    file.write(buf + sizeof(message_header) + fileNameMaxLength, header->size - fileNameMaxLength);
    file.close();

    std::string newName = folderPath + fileNameString;
    //rename only if file was send to the end
    if (header->type == 7) { // whole file was send
        std::cout << "File received: " + newName << std::endl;
        std::rename(oldName.c_str(), newName.c_str());
    }
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

    size_t size = sizeof(msg) + fileNameMaxLength;
    auto buffer = new char[size];
    memset(buffer, 0x00, size);

    memcpy(buffer, &msg, sizeof(msg));
    memcpy(buffer + sizeof(msg), fileName.c_str(), fileName.size());
    write(socketId, buffer, size);
    delete[] buffer;

    return true;
}
