//
// FileExchanger
// Created by dram on 22.05.18.
//

#ifndef FILEEXCHANGER_MESSAGE_H
#define FILEEXCHANGER_MESSAGE_H

#include <stdint-gcc.h>

typedef struct message_header {
    uint32_t type; //
    uint32_t size; //size of body
    uint8_t body[];
} message_header;
#endif //FILEEXCHANGER_MESSAGE_H
