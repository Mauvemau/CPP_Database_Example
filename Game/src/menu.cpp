#include "menu.h"

Menu::Menu() {
	exit = false;
}

Menu::~Menu() {

}

void Menu::launchGame() {

}

void Menu::createAccount() {
	if (SessionManager::isConnectedToDatabase()) {
		string userName;
		string password;
		bool validUser = false;
		bool validPassword = false;
		do {
			MiscUtils::clearScreen();
			cout << "[CREATING NEW ACCOUNT]\n\n";
			cout << "Please input the USER NAME you would like to have. (20 character limit)\n";
			bool userExists = false;
			cout << "\n";
			cout << "Username > ";
			userName = MiscUtils::usernameInput();
			if (userName == MAGIC_STRING) return;
			if (userName.size() > 0) {
				userExists = SessionManager::verifyUser(userName);
				if (userExists) {
					cout << "\n";
					cout << "User name already exists!\n";
					MiscUtils::waitUserInput();
				}
				else {
					validUser = true;
				}
			}
		} while (!validUser);
		do {
			MiscUtils::clearScreen();
			cout << "[CREATING NEW ACCOUNT]\n\n";
			cout << "Please input the PASSWORD you would like to have. (20 character limit)\n";
			cout << "\n";
			cout << "Password > ";
			password = MiscUtils::passwordInput();
			if (password == MAGIC_STRING) return;
			if(password.size() > 0) {
				cout << "\n";
				cout << "Please verify your password\n";
				cout << "\n";
				cout << "Password > ";
				validPassword = (password == MiscUtils::passwordInput());
			}
		} while (!validPassword);
		SessionManager::createUser(userName, password);
		cout << "User \"" << userName << "\" has been created created!!\n";
		cout << "\n";
		MiscUtils::waitUserInput();
	}
	else {
		MiscUtils::clearScreen();
		cout << "The client is not currently connected to any database!!!\n";
		cout << "\n";
		MiscUtils::waitUserInput();
	}
}

void Menu::login() {
	if (SessionManager::isConnectedToDatabase()) {
		string userName;
		string password;
		bool validUser = false;
		bool validPassword = false;
		do {
			MiscUtils::clearScreen();
			cout << "[LOGIN]\n\n";
			cout << "Please input your user name.\n";
			cout << "\n";
			cout << "Username > ";
			userName = MiscUtils::usernameInput();
			if (userName == MAGIC_STRING) return;
			if (userName.size() > 0) {
				validUser = SessionManager::verifyUser(userName);
				if (!validUser) {
					cout << "\n";
					cout << "Invalid username!\nIf you don't have an account press ESC and 2 to create one.\n";
					MiscUtils::waitUserInput();
				}
			}
		} while (!validUser);
		do {
			MiscUtils::clearScreen();
			cout << "[LOGIN]\n\n";
			cout << "Please input your password\n";
			cout << "\n";
			cout << "Password > ";
			password = MiscUtils::passwordInput();
			if (password == MAGIC_STRING) return;
			if (password.size() > 0) {
				validPassword = SessionManager::verifyPassword(userName, password);
				if (!validPassword) {
					cout << "\n";
					cout << "Incorrect password!\nPlease verify your password is correct!\n";
					MiscUtils::waitUserInput();
				}
			}
		} while (!validPassword);
		cout << "\n";
		SessionManager::logIn(userName, password);
		MiscUtils::waitUserInput();
	}
	else {
		MiscUtils::clearScreen();
		cout << "The client is not currently connected to any database!!!\n";
		MiscUtils::waitUserInput();
	}
}

void Menu::main() {
	MiscUtils::clearScreen();
	int input;
	cout << "Welcome!\nUse the numbers on your keyboard to pick an option!\n\n";
	cout << "1. Login\n2. Create Account\n3. Exit\n";

	do {
		cout << "\n";
		cout << "Input > ";
		input = MiscUtils::getKey(true);
	} while (input < 49 || input > 51);

	switch (static_cast<char>(input)) {
	case '1':
		login();
		break;
	case '2':
		createAccount();
		break;
	case '3':
		exit = true;
		break;
	default:
		break;
	}
	cout << "\n";
}

void Menu::run() {
	SessionManager::connectToDatabase("dbConfig.txt");
	MiscUtils::waitUserInput();
	do {
		this->main();
	} while (!exit);
	MiscUtils::clearScreen();
	cout << "\n";
	SessionManager::disconnectFromDatabase();
}