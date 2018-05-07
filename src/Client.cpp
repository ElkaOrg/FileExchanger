//
// Created by gnowacki on 03.04.18.
//

#include "include/Client.h"

void Client::connectToBroker()
{
    int sock;
    struct sockaddr_in server;
    struct hostent *hp;
    char buf[1024];

    sock = socket( AF_INET, SOCK_STREAM, 0 );
    if (sock == -1)
    {
        perror("opening stream socket");
        exit(1);
    }

    //TODO hardcoded
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(BROKER_ADDR);
    server.sin_port = htons(BROKER_PORT);

    if (connect(sock, (struct sockaddr *) &server, sizeof server) == -1)
    {
        perror("connecting stream socket");
        exit(1);
    }

    //TODO body
    ClientMenu clientMenu;
    clientMenu.showMainMenu();

    close(sock);
    exit(0);
}

