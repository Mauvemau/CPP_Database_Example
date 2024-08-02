#pragma once

#include "session_manager.h"
#include "misc_utils.h"

class Menu {
private:
	bool exit;

	void launchGame();

	void characterSelect();
	void createCharacter();

	void userMenu();

	void createAccount();
	void login();

	void mainMenu();
public:
	Menu();
	~Menu();

	void run();
};