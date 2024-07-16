#include "menu_manager.h"

#include <iostream>

using namespace std;

MenuManager::MenuManager(){
    currentState = menuState::START;
    exit = false;
}

MenuManager::~MenuManager(){

}

// Private

void MenuManager::pause(){
    do {
        cout << "Press the Enter key to continue.\n";
    } while (cin.get() != '\n');
}

void MenuManager::setState(menuState stateToSet){
    currentState = stateToSet;
}

void MenuManager::update(){
    do{
        cout << "This is a menu!\n";
    }while(!exit);
}

// Public

void MenuManager::start(){
    cout << "menu started!\n";
}
