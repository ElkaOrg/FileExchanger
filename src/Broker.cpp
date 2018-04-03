//
// Created by gnowacki on 03.04.18.
//

#include "include/Broker.h"

Broker& Broker::getInstance()
{
    static Broker instance;

    instance.instancePtr = &instance;

    return instance;
}

void Broker::terminateWrapper(int arg)
{
    instancePtr->terminate(arg);
}

void Broker::terminate(int arg)
{
    std::vector<pthread_t>::iterator it;
    for(it = threads.begin(); it != threads.end(); ++it)
    {
        std::cout<<"partial-terminate"<<std::endl;
        pthread_join(*it, NULL);
    }
    std::cout<<"total-terminate"<<std::endl;
    exit(0);
}

void* Broker::handleClient(void* ptr)
{
    SocketWrapper* socketWrapper = static_cast<SocketWrapper*>(ptr);
    int socket = socketWrapper->getSocket();

    //TODO body
    std::cout<<"handleClient"<<std::endl;

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

    signal(SIGINT, Broker::terminateWrapper);

    while(true)
    {
        msgsock = accept(sock, (struct sockaddr *) &client_addr, &client_addr_len);
        if (msgsock == -1 )
        {
            perror("accept");
        }
        else
        {
            pthread_t thread;
            SocketWrapper* socketWrapper = new SocketWrapper(msgsock);

            pthread_create(&thread, NULL, Broker::handleClient, (void *) socketWrapper);

            threads.push_back(thread);
        }
    };
    /*
     * gniazdo sock nie zostanie nigdy zamkniete jawnie,
     * jednak wszystkie deskryptory zostana zamkniete gdy proces
     * zostanie zakonczony (np w wyniku wystapienia sygnalu)
     */
}
