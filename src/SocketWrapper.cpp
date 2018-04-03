//
// Created by gnowacki on 03.04.18.
//

#include "SocketWrapper.h"

SocketWrapper::SocketWrapper(int sock): socket(sock) {}

int SocketWrapper::getSocket(void)
{
    return socket;
}