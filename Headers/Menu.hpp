#ifndef MENU_HPP
#define MENU_HPP

#include "WebcamOperations.hpp"

class Menu {
public:
    Menu();
    ~Menu();
    void run();

private:
    WebcamOperations webcamOps;
    void displayMenu();
};

#endif // MENU_HPP
