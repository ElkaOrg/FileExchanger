//
// Created by gnowacki on 07.05.18.
//

#include "../include/FileTransfer.h"

int FileTransfer::sendOneFile(int socketId, const std::string &filePath, const std::string &fileName)
{
    const int maxTxtBuffer = 1024; // how many data can be readed from file

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
    msg.size = htonl(fileNameMaxLength + fileSize); //default, we assume that size fit to maxTextBuffer
    msg.type = htonl(6);

    //set max msg buffer


    size_t size = sizeof(msg) + msg.size;
    char *buffer;
    size_t bufferSize = 0;

    if (size > fileNameMaxLength + sizeof(msg) + maxTxtBuffer) { //divide files
        msg.size = htonl(fileNameMaxLength + maxTxtBuffer);
        bufferSize = maxTxtBuffer + sizeof(msg) + fileNameMaxLength;
        buffer = new char[bufferSize];
    } else {
        buffer = new char[size]; // all fits
        bufferSize = size;
    }
    memset(buffer, 0x00, bufferSize);

    memcpy(buffer, &msg, sizeof(msg));//first read
    memcpy(buffer + sizeof(msg), fileName.c_str(), fileName.size());

    size_t nread = fread(buffer + sizeof(msg) + fileNameMaxLength, 1, maxTxtBuffer, fp);
    fileSize -= nread;
    write(socketId, buffer, bufferSize);
    while (fileSize > 0) {
        memset(buffer, 0x00, bufferSize); // clean buffer
        if (fileSize + fileNameMaxLength + sizeof(msg) < bufferSize) {
            // if fileSize fits to buffer
            msg.size = htonl(fileNameMaxLength + fileSize); //default, we assume that size fit to maxTextBuffer
        } else {
            msg.size = htonl(fileNameMaxLength + maxTxtBuffer); //default, we assume that size fit to maxTextBuffer
        }
        msg.type = htonl(7); // 9 - continue sending file

        memcpy(buffer, &msg, sizeof(msg));
        memcpy(buffer + sizeof(msg), fileName.c_str(), fileName.size());
        nread = fread(buffer + sizeof(msg) + fileNameMaxLength, 1, maxTxtBuffer, fp); // read again
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

int FileTransfer::recvOneFile(const std::string& folderPath, char *buf, int bufN) {
    char typeAndSize[8] = {0};
    memcpy(typeAndSize, buf, sizeof(typeAndSize));
    auto *header = (struct message_header *) typeAndSize;
    header->type = ntohl(header->type);
    header->size = ntohl(header->size);

    char fileName[40] = {0};
    memcpy(fileName, buf+8, 40); //copy file name

    std::string fileNameString = std::string(fileName); //auto removes 0

    std::fstream file;
    std::string oldName = folderPath+"/"+"_tmp_"+fileNameString;
    file.open(oldName, std::ios::out);
    file.write(buf+8+40, header->size-40);
    file.close();

    std::string newName = folderPath+"/"+fileNameString;

    std::rename(oldName.c_str(), newName.c_str());
    
    return 0;
}

std::string FileTransfer::parseFileName(const char *fileName, int n) {
    std::string result;
    for(int i = 0; i < n && fileName[i] != 0; i++){
        result += fileName[i];
    }
    return result;
}
