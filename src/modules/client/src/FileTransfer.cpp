//
// Created by gnowacki on 07.05.18.
//

#include "../include/FileTransfer.h"

int FileTransfer::sendOneFile(char *filePath)
{
    std::fstream fileStream;
    char buffer[BUF_SIZE];

    fileStream.open(filePath, std::ios::in);
    if( fileStream.good() == true )
    {
        fileStream >> buffer;


        fileStream.close();
    }

}

int FileTransfer::receiveOneFile(char *filePath)
{

}