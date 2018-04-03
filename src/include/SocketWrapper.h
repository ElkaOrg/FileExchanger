//
// Created by gnowacki on 03.04.18.
//

#ifndef FILEEXCHANGER_SOCKETWRAPPER_H
#define FILEEXCHANGER_SOCKETWRAPPER_H

class SocketWrapper
{
private:
    int socket;

public:
    SocketWrapper(int sock);
    int getSocket(void);
};

#endif //FILEEXCHANGER_SOCKETWRAPPER_H
