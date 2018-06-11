//
// FileExchanger
// Created by gnowacki on 07.05.18
// Implementation by dram
//

#ifndef FILEEXCHANGER_FILETRANSFER_H
#define FILEEXCHANGER_FILETRANSFER_H

#include "../../../include/inc.h"
#include <string>
#include <Message.h>


class FileTransfer
{
private:

public:

    static int sendOneFile(int socketId, const std::string &filePath, const std::string &fileName);
    static int recvOneFile(const std::string & folderPath, char * buf, int bufN);

    static std::string parseFileName(const char * fileName, int n);

    static bool sayDontHaveFile(int socketId, const std::string &fileName);
};

#endif //FILEEXCHANGER_FILETRANSFER_H
