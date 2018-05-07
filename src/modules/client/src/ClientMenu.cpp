//
// Created by gnowacki on 07.05.18.
//
#include "../include/ClientMenu.h"

void ClientMenu::showMainMenu(void)
{
    int chosenNumber = 0;
    std::cout<<"0 - Quit"<<std::endl;
    std::cout<<"1 - Choose directory to be shared"<<std::endl;
    //TODO use case

    std::cin>>chosenNumber;

    while(1)
    {
        switch (chosenNumber) {
            case 0:
                std::cout << "Exiting..." << std::endl;
                //TODO do clean up
                break;
            case 1:
                std::cout << "Option 1" << std::endl;
                break;

            case 2:
                //jakiś kod
                break;

                //...
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
