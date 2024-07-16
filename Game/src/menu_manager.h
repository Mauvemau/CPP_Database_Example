#pragma once

enum class menuState{
    START = 0,
    LOGIN,
    REGISTER,
    MAIN,
    CREATURE
};

/// <summary>
/// Handles the interface of the program.
/// </summary>
class MenuManager{
private:
    menuState currentState;
    bool exit;

    void pause();
    void setState(menuState stateToSet);
    void update();
public:
    MenuManager();
    ~MenuManager();

    void start();
};
