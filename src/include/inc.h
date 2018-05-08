//
// Created by gnowacki on 03.04.18.
//

#ifndef FILEEXCHANGER_INC_H
#define FILEEXCHANGER_INC_H

#define BROKER_PORT 8080
#define BROKER_ADDR "127.0.0.1"

#include "SocketWrapper.h"
#include "../modules/common/include/FileTransfer.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>
#include <csignal>
#include <iostream>
#include "zconf.h"  //good include???
#include <fstream>
#include <fcntl.h>
#include <cstring>

#endif //FILEEXCHANGER_INC_H
