//
// Created by gnowacki on 07.05.18.
//

#ifndef FILEEXCHANGER_FILETRANSFER_H
#define FILEEXCHANGER_FILETRANSFER_H

#include "../../../include/inc.h"

#define BUF_SIZE 1024

class FileTransfer
{
public:
    int sendOneFile(char* filePath);
    int receiveOneFile(char *filePath);
};

#endif //FILEEXCHANGER_FILETRANSFER_H
