#pragma once

#include "session_manager.h"
#include "misc_utils.h"

class Menu {
private:
	bool exit;
public:
	Menu();
	~Menu();

	void launchGame();
	void createAccount();
	void login();
	void main();

	void run();
};