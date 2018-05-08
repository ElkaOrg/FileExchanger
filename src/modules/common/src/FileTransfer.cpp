//
// Created by gnowacki on 07.05.18.
//

#include "../include/FileTransfer.h"

FileTransfer::FileTransfer(int socketDescriptor): socketDescriptor(socketDescriptor) {}

int FileTransfer::sendOneFile(char *filePath)
{
    //send file name first
    write(socketDescriptor, filePath,256);

    FILE *fp = fopen(filePath,"rb");
    if(fp == NULL)
    {
        printf("File open error");
        return 1;
    }

    while(1)
    {
        unsigned char buff[1024]={0};
        int nread = fread(buff,1,1024,fp);

        /* If read was success, send data. */
        if(nread > 0)
        {
            //printf("Sending \n");
            write(socketDescriptor, buff, nread);
        }
        if (nread < 1024)
        {
            if (feof(fp))
            {
                printf("End of file\n");
                printf("File transfer completed for id: %d\n",socketDescriptor);
                fclose(fp);
            }
            if (ferror(fp))
                printf("Error reading\n");
            break;
        }
    }//while

    return 0;
}

int FileTransfer::receiveOneFile(char* filePath)
{
    int bytesReceived = 0;
    char recvBuff[1024];
    memset(recvBuff, '0', sizeof(recvBuff));

    /* Create file where data will be stored */
    FILE *fp;
    char fname[100];
    read(socketDescriptor, fname, 256);

    strcpy(filePath, fname);

    //fp = fopen(fname, "ab");
    fp = fopen("/home/gnowacki/BROKER.txt", "ab");  //TODO uncomment 
    if(fp == NULL)
    {
        printf("Error opening file");
        return 1;
    }

    long double sz = 1;

    /* Receive data in chunks of 256 bytes */
    while((bytesReceived = read(socketDescriptor, recvBuff, 1024)) > 0)
    {
        sz++;
        //gotoxy(0,4);
        printf("Received: %llf Mb",(sz/1024));
        fflush(stdout);
        // recvBuff[n] = 0;
        fwrite(recvBuff, 1,bytesReceived,fp);
        // printf("%s \n", recvBuff);
    }

    if(bytesReceived < 0)
    {
        printf("\n Read Error \n");
    }
    printf("\nFile OK....Completed\n");
    return 0;
}