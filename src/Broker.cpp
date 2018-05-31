//
// Created by gnowacki on 03.04.18.
//

#include "include/Broker.h"
#include <Message.h>
#include <chrono>

/* Null, because instance will be initialized on demand. */
Broker* Broker::instancePtr = 0;
std::map<int,std::vector<std::string>> Broker::clients;

Broker* Broker::getInstance()
{
    if (instancePtr == 0)
    {
        instancePtr = new Broker();
    }

    return instancePtr;
}

Broker::Broker() {}

Broker::~Broker()
{
    delete instancePtr;
}

void Broker::terminateWrapper(int arg)
{
    if(arg == SIGINT)
        instancePtr->terminate();
}

void Broker::terminate()
{
    std::vector<pthread_t>::iterator it;
    for(it = threads.begin(); it != threads.end(); ++it)
    {
        std::cout<<"partial-terminate"<<std::endl;
        pthread_join(*it, nullptr);
    }
    std::cout<<"total-terminate"<<std::endl;
    exit(0);
}

void Broker::sendRequest(int socketId, uint32_t requestType)
{
    message_header msg;
    msg.type = htonl(requestType);
    msg.size = htonl(0);

    size_t size = sizeof(msg);
    auto buffer = new char[size];
    memset(buffer, 0x00, size);
    memcpy(buffer, &msg, size);
    std::cout << "Sending request with header: " << requestType << " to client with ID: " << socketId << std::endl;
    write(socketId, buffer, size);
    delete[] buffer;
}

int Broker::checkFilename(std::string filename)
{
    int ownerId = -1;
    for (auto const& item : clients)
    {
        for(auto const& name : item.second)
        {
            if (name == filename) {
                ownerId = item.first;
            }
        }
    }

    return ownerId;
}

void Broker::sendErrorToClient(int socket, std::string message)
{
    message_header msg;
    msg.type = htonl(5);
    msg.size = htonl(sizeof(message));

    size_t size = sizeof(msg) + msg.size;
    auto buffer = new char[size];
    memset(buffer, 0x00, size);
    memcpy(buffer, &msg, size);
    memcpy(buffer + sizeof(msg), &message, msg.size);
    write(socket, buffer, size);
    delete[] buffer;
}

void* Broker::handleClient(void* ptr)
{
    auto socketWrapper = static_cast<SocketWrapper*>(ptr);
    int socket = socketWrapper->getSocket();
    auto now = std::chrono::system_clock::now();

    std::vector<std::string> filenames;
    std::size_t client_hashcode = 0;

    auto ehlo = receiveMessage(socket);
    auto header = ehlo.first;

    if (header->type != 0)
    {
        throw std::runtime_error("Didn't receive ehlo!");
    }
    else
    {
        std::cout << "Got ehlo from client with ID: " << socket << std::endl;
        sendRequest(socket, 1); //filenames request
        sendRequest(socket, 2); //hashcode request
    }

    do
    {
        auto timeTmp = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - timeTmp).count() >= 18000)
        {
            std::cout << "Sending hashcode request to client with ID: " << socket << std::endl;
            now = timeTmp;
            sendRequest(socket, 2);
        }

        auto message = receiveMessage(socket);
        if (message.first)
        {
            std::cout << "Got message with header type: " << message.first->type << std::endl;
            switch (message.first->type)
            {
                case (0): // ehlo
                {
                    std::cout << "Ehlo was already received. :c";
                    throw std::runtime_error("Something went wrong - ehlo was already received!");
                }
                case (1): // client sent us his filenames
                {
                    int nrOfFiles = message.first->size/40; // ??? na pewno dobrze ???
                    for(int i=0;i<=nrOfFiles;i++)
                    {
                        char filename[40];
                        memset(filename, 0x00, sizeof(filename));
                        memcpy(filename, message.second+8+40*i, sizeof(filename));
                        filenames[i] = filename;
                    }
                    clients[socket] = filenames;
                }
                case (2): // client sent us hashcode
                {
                    char tmp_hash[8];
                    memset(tmp_hash, 0x00, sizeof(tmp_hash));
                    memcpy(tmp_hash, message.second+8, sizeof(tmp_hash));
                    if ((size_t) tmp_hash != client_hashcode)
                    {
                        std::cout << "Client with ID " << socket << " hashcode changed. Asking for filenames" << std::endl;
                        client_hashcode = (size_t) tmp_hash;
                        sendRequest(socket, 1);
                    }
                }
                case (3): // client wants list of avaliable files
                {
                    // TODO
                    // wyslac liste wszystkich dostepnych plikow
                }
                case (4): // client asking us for a file
                {
                    char filename[40];
                    if (header->size != 40) {
                        throw std::runtime_error("Invalid file size");
                    }
                    memset(filename, 0x00, sizeof(filename));
                    memcpy(filename, message.second + 8, sizeof(filename));

                    int socketId = checkFilename(filename);
                    if (socketId == -1)
                    {
                        std::cout << "Filename " << filename << "is not avaliable" << std::endl;
                        sendErrorToClient(socket, "No such file found.");
                    }
                    else
                    {
                        std::cout << "Filename " << filename << "found in files of client with ID: " << socket << std::endl;
                        // TODO
                        // zamow pliczek
                    }
                }
                case (5): // client sent us a file
                {
                    // TODO
                    // odbierz pliczek
                    // przeslij dalej
                }
                case (6): // client doesn't have that file anymore
                {
                    sendErrorToClient(socket, "No such file found.");
                }
                default: // bad header
                {
                    throw std::runtime_error("Unknown header!");
                }
            }
        }
    } while (client_hashcode != 0);

    std::cout << "Didn't receive hashcode - client probably dead" << std::endl;
    close(socket);
    delete socketWrapper;
}

std::pair<message_header*,char*> Broker::receiveMessage(int socket)
{
    char buff[512] = {0};
    char typeAndSize[8] = {0};

    auto bytesRead = read(socket, buff, sizeof(buff));
    memcpy(typeAndSize, buff, sizeof(typeAndSize));
    auto *header = (struct message_header *) typeAndSize;
    header->type = ntohl(header->type);
    header->size = ntohl(header->size);

    //TEST
    std::cout << header->type << std::endl;

    return std::make_pair(header, buff);
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

    std::cout << "Begin waiting for clients..." << std::endl;
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
            std::cout << "Client acquired! His ID: " << socketWrapper->getSocket() << std::endl;

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
