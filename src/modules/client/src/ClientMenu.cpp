//
// Created by gnowacki on 07.05.18.
//
#include <inc.h>
#include "../include/ClientMenu.h"

ClientMenu::ClientMenu(){

}

void ClientMenu::showMainMenu() {
    int chosenNumber = 0;

    while (1) {
        std::cout << "0 - Quit" << std::endl;
        std::cout << "1 - Manage connection to broker" << std::endl;
        std::cout << "2 - Client settings" << std::endl;
        std::cout << "3 - Download file" << std::endl;

        std::cin >> chosenNumber;

        switch (chosenNumber) {
            case 0:
                std::cout << "Exiting..." << std::endl;
                clientConnection.disconnectFromBroker();
                clientDb.get()->close();
                return;
            case 1:
                menuManageConnection();
                break;
            case 2:
                menuSettings();
                break;

            case 3:
                menuDownloadFile();
                //FileTransfer fileTransfer(socketDescriptor);
                //fileTransfer.sendOneFile(filePath);
                break;
            default:
                std::cout << "Error has occured" << std::endl;
                exit(0);
        }
    }
}
void ClientMenu::menuDownloadFile() {
    std::cout << "0 - Back" << std::endl;
    std::cout << "1 - Get all files" << std::endl;
    std::cout << "2 - Download file" << std::endl;
    int chosenNumber = 0;
    std::cin >> chosenNumber;
    switch(chosenNumber){
        case 1:
            clientConnection.getAllFiles();
            break;
        case 2: {
            std::string fileName;
            std::cout << "Type filename:" << std::endl;
            std::cin >> fileName;
            if (fileName.size() > 40) {
                std::cout << "File name cant have more than 40 characters!" << std::endl;
                return;
            }
            clientConnection.requestForFile(fileName);
            break;
        }
        case 0: default:
            return;
    }

}
void ClientMenu::menuManageConnection() {
    if (clientConnection.isConnected()) {
        std::cout << "Currently you are connected!" << std::endl;
        std::cout << "0 - Back" << std::endl;
        std::cout << "1 - Disconnect" << std::endl;
    } else {
        std::cout << "Currently you are disconnected!" << std::endl;
        std::cout << "0 - Back" << std::endl;
        std::cout << "1 - Connect" << std::endl;
    }
    int chosenNumber = 0;
    std::cin >> chosenNumber;

    //back
    if (chosenNumber == 0) {
        return;
    } else if (chosenNumber == 1) {
        if (clientConnection.isConnected()) {
            clientConnection.disconnectFromBroker();
            std::cout << "You are disconnected from broker!" << std::endl;
        } else {
            std::string brokerIp = clientDb.get()->getKey(brokerIpKey);
            if(brokerIp.size() == 0){
                std::cout << "Broker ip not defined! " << std::endl;
                return;
            }
            uint16_t brokerPort = 8080; //always
            clientConnection.connectToBroker(brokerIp, brokerPort);
            std::cout << "You are connected to broker!" << std::endl;
        }
    } else {
        sayUnknownOption();
        menuManageConnection();
    }


}

void ClientMenu::menuSettings() {
    std::string brokerIp = clientDb.get()->getKey(brokerIpKey);
    std::string clientSharedFolder = clientDb.get()->getKey(clientSharedFolderKey);
    if(brokerIp != ""){
        std::cout << "Current broker ip: " << brokerIp << std::endl;
    }
    if(clientSharedFolder != "") {
        std::cout << "Current shared folder: " << clientSharedFolder << std::endl;
    }

    std::cout << std::endl << "0 - Back" << std::endl;
    std::cout << "1 - Set broker ip" << std::endl;
    std::cout << "2 - Set shared folder" << std::endl;


    int choosenOption = 0;
    //3 tries
    for (int i = 0; i < 3; i++) {
        std::cin >> choosenOption;
        switch (choosenOption) {
            case 0: //back
                return;
            case 1:
                std::cout << "Enter broker ip: " << std::endl;
                std::cin >> brokerIp;
                clientDb.get()->saveKey(brokerIpKey, brokerIp);
                std::cout << "Broker ip updated! Current broker ip: " << brokerIp << std::endl;
                return;
            case 2:
                std::cout << "Enter dir to shared folder: " << std::endl;
                std::cin >> clientSharedFolder;
                if (!DirManagment::isValidDirectory(clientSharedFolder)) {
                    std::cout << "Error!!! Directory not valid! Try again" << std::endl;
                    std::cin.ignore(INT_MAX); //flush
                }
                else {
                    clientDb.get()->saveKey(clientSharedFolderKey, clientSharedFolder);
                    std::cout << "Shared folder updated!" << std::endl;
                    return;
                }
                break;
            default:
                sayUnknownOption();
        }
    }

}

void ClientMenu::sayUnknownOption() {
    std::cout << "Error! Unknown option! Please try again." << std::endl << std::endl;
}


