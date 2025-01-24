#include "Menu.hpp"
#include <iostream>

Menu::Menu() {
    std::cout << "Menu initialized." << std::endl;
}

Menu::~Menu() {
    std::cout << "Menu closed." << std::endl;
}

void Menu::run() {
    webcamOps.setResourcesPath("../resources");
    char choice;

    do {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
        case '1': // Open Webcam
            webcamOps.openWebcam();
            break;
        case '2': // Close Webcam
            webcamOps.closeWebcam();
            break;
        case 'q': // Quit
            std::cout << "Exiting menu." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 'q');
}

void Menu::displayMenu() {
    std::cout << "\nMenu Options:" << std::endl;
    std::cout << "1. Open Webcam" << std::endl;
    std::cout << "2. Close Webcam" << std::endl;
    std::cout << "q. Quit" << std::endl;
    std::cout << "Enter your choice: ";
}
