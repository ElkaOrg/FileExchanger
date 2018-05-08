//
// Created by gnowacki on 07.05.18.
//
#include "../include/ClientMenu.h"

ClientMenu::ClientMenu(int socketDescriptor): socketDescriptor(socketDescriptor) {}

void ClientMenu::showMainMenu(void)
{
    int chosenNumber = 0;

    while(1)
    {
        std::cout<<"0 - Quit"<<std::endl;
        std::cout<<"1 - Choose directory to be shared"<<std::endl;
        std::cout<<"2 - Send file by name"<<std::endl;
        //TODO use case

        std::cin>>chosenNumber;

        switch (chosenNumber) {
            case 0:
                std::cout << "Exiting..." << std::endl;
                //TODO do clean up
                break;
            case 1:
                std::cout << "Option 1" << std::endl;
                break;

            case 2:
            {
                char filePath[100];
                std::cout << "Send file by name chosen. Specify path: " << std::endl;
                std::cin >> filePath;
                FileTransfer fileTransfer(socketDescriptor);
                fileTransfer.sendOneFile(filePath);
                break;
            }

            case 3:
                //jakiś kod
                break;

            default:
                std::cout << "Error has occured" << std::endl;
                exit(0);
                break;
        }
    }
}
