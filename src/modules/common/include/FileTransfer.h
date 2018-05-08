//
// Created by gnowacki on 07.05.18.
//

#ifndef FILEEXCHANGER_FILETRANSFER_H
#define FILEEXCHANGER_FILETRANSFER_H

#include "../../../include/inc.h"

#define BUF_SIZE 1024

class FileTransfer
{
private:
    int socketDescriptor;

public:
    FileTransfer(int socketDescriptor);

    int sendOneFile(char* filePath);
    char* receiveOneFile(void);
};

#endif //FILEEXCHANGER_FILETRANSFER_H
