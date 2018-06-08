//
// Created by dram on 08.06.18.
//

#ifndef FILEEXCHANGER_FILEWAIT_H
#define FILEEXCHANGER_FILEWAIT_H

#include <string>

typedef struct FileWait {
    int socketWho;
    std::string fileName;

    FileWait(int socketWho, const std::string &fileName) : socketWho(socketWho), fileName(fileName) {}

    bool operator==(const FileWait& rhs) const
    {
        return (socketWho==rhs.socketWho?fileName==rhs.fileName:false);
    }
} FileWait;
#endif //FILEEXCHANGER_FILEWAIT_H
