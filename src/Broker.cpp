//
// Created by gnowacki on 03.04.18.
//

#include "include/Broker.h"

void* Broker::handleClient(void* ptr)
{
    SocketWrapper* socketWrapper = static_cast<SocketWrapper*>(ptr);
    int socket = socketWrapper->getSocket();

    //TODO body

    close(socket);
    delete socketWrapper;
}

void Broker::waitForClients()
{
    int sock, length;
    struct sockaddr_in server;
    int msgsock;
    char buf[1024];
    int rval;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("opening stream socket");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(BROKER_ADDR);    //TODO inet_addr probably deprecated; also DNS resolver should be added
    server.sin_port = htons(BROKER_PORT);

    if (bind(sock, (struct sockaddr *) &server, sizeof server) == -1)
    {
        perror("binding stream socket");
        exit(1);
    }

    if(listen(sock, 32) != 0)
    {
        perror("listen call error");
        exit(1);
    }

    while(true)
    {
        msgsock = accept(sock, (struct sockaddr *) &client_addr, &client_addr_len);
        if (msgsock == -1 )
        {
            perror("accept");
        }
        else
        {
            SocketWrapper* socketWrapper = new SocketWrapper(msgsock);

            pthread_create(NULL, NULL, handleClient, (void *) socketWrapper);
        }
    };
    /*
     * gniazdo sock nie zostanie nigdy zamkniete jawnie,
     * jednak wszystkie deskryptory zostana zamkniete gdy proces
     * zostanie zakonczony (np w wyniku wystapienia sygnalu)
     */

    exit(0);
}
