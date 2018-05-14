//
// Created by gnowacki on 03.04.18.
//

#include "include/Client.h"

int Client::connectToBroker()
{
    if(socketId > 0){
        perror("Alredy connected?!");
    }

    struct sockaddr_in server;

    socketId = socket( AF_INET, SOCK_STREAM, 0 );
    if (socketId == -1)
    {
        perror("opening stream socket");
        exit(1);
    }

    //TODO hardcoded
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(BROKER_ADDR);
    server.sin_port = htons(BROKER_PORT);

    if (connect(socketId, (struct sockaddr *) &server, sizeof server) == -1)
    {
        perror("connecting stream socket");
        exit(1);
    }
    return socketId;
}

void Client::disconnectFromBroker() {
    close(socketId);
    socketId = 0;
}

bool Client::isConnected() {
    return socketId > 0;
}

