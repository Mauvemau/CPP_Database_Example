#include "menu.h"

#include <string>

Menu::Menu() {
	exit = false;
}

Menu::~Menu() {

}

void Menu::petCapture() {
	if (!SessionManager::isCharacterSelected()) return;
	bool returnToCharMenu = false;
	bool passed = true;

	string name;
	float maxHP;
	float attack;
	PetType type;
	int catchChance;
	do {
		MiscUtils::clearScreen();
		if (passed) {
			name = MiscUtils::getRandomAnimalName();
			maxHP = static_cast<float>(MiscUtils::getRandomInt(8, 384));
			attack = static_cast<float>(MiscUtils::getRandomInt(6, 154));
			type = static_cast<PetType>(MiscUtils::getRandomInt(1, 3));
			catchChance = MiscUtils::getRandomInt(50, 100);
			passed = false;
		}
		
		int input;
		cout << "[A WILD CREATURE HAS APPEARED]\n\n";
		cout << "[" << name << "] Type: " << MiscUtils::getPetTypeString(type) << "\n\n";
		cout << "Use the numbers on your keyboard to pick an option!\n\n";
		cout << "1. Try collect!\n2. Pass\n3. Return to Character Menu\n";

		do {
			cout << "\n";
			cout << "Input > ";
			input = MiscUtils::getKey(true);
		} while (input < 49 || input > 51);

		int catchRoll = MiscUtils::getRandomInt(1, 100);

		switch (static_cast<char>(input)) {
		case '1':
			cout << "\n\n";
			if (catchRoll <= catchChance) {
				cout << "You successfully caught the pet!\n";
				SessionManager::createPet(name, maxHP, attack, type);
			}
			else {
				cout << "The pet has escaped!\n";
			}
			passed = true;
			MiscUtils::waitUserInput();
			break;
		case '2':
			passed = true;
			break;
		case '3':
			returnToCharMenu = true;
			break;
		default:
			break;
		}
		cout << "\n";
	} while (!returnToCharMenu);
}

void Menu::petDelete() {
	if (!SessionManager::isCharacterSelected()) return;
	SessionManager::loadCharacterPets();
	Character currentCharacter = SessionManager::getCurrentCharacter();
	vector<Pet> characterPets = currentCharacter.getPets();
	bool validInput = false;
	if (!characterPets.empty()) {
		string input;
		bool validInput = false;
		do {
			MiscUtils::clearScreen();
			cout << "[REMOVING PET]\nEnter the index of the Pet you want to remove from your collection. (or press ESC key to return)\n\n";
			int i = 0;

			cout << "\n";
			cout << "Selection > ";
			input = MiscUtils::safeInputNumbers(true, false);
			if (input == MAGIC_STRING) return;
			if (!input.empty()) {
				int num = stoi(input);

				num--;
				if (num > -1 && num < characterPets.size()) {
					cout << "\n";
					SessionManager::deletePet(characterPets[num].getID());
					validInput = true;
				}
			}
		} while (!validInput);
		MiscUtils::waitUserInput();
	}
	else {
		MiscUtils::clearScreen();
		cout << "You haven't captured any pets yet!\nUse the first option in the character menu to capture pets!\n";
		MiscUtils::waitUserInput();
	}
}

void Menu::petList() {
	if (!SessionManager::isCharacterSelected()) return;
	SessionManager::loadCharacterPets();
	Character currentCharacter = SessionManager::getCurrentCharacter();
	vector<Pet> characterPets = currentCharacter.getPets();
	MiscUtils::clearScreen();
	if (!characterPets.empty()) {
		int i = 0;
		cout << "[COLLECTED PETS]\n";
		for (vector<Pet>::iterator it = characterPets.begin(); it != characterPets.end(); ++it) {
			cout << "\n";
			cout << i + 1 << ". " << it->getName() << "\n";
			cout << "Type: " << it->getTypeString() << "\n";
			cout << "Health: " << it->getCurrentHP() << "/" << it->getMaxHP() << "\n";
			cout << "Attack Power: " << it->getAttack() << "\n";
			i++;
		}
	}
	else {
		cout << "You haven't captured any pets yet!\nUse the first option in the character menu to capture pets!\n";
	}
	MiscUtils::waitUserInput();
}

void Menu::characterMenu() {
	if (!SessionManager::isCharacterSelected()) return;
	User currentUser = SessionManager::getCurrentUser();
	Character currentCharacter = SessionManager::getCurrentCharacter();
	bool returnToUserMenu = false;
	do {
		MiscUtils::clearScreen();
		int input;
		cout << "[USER: " << currentUser.getName() << ", CHARACTER: " << currentCharacter.getName() << "]\nUse the numbers on your keyboard to pick an option!\n\n";
		cout << "1. Catch new Pet\n2. See Pet list\n3. Free a Pet\n4. Return to User Menu\n";

		do {
			cout << "\n";
			cout << "Input > ";
			input = MiscUtils::getKey(true);
		} while (input < 49 || input > 52);

		switch (static_cast<char>(input)) {
		case '1':
			petCapture();
			break;
		case '2':
			petList();
			break;
		case '3':
			petDelete();
			break;
		case '4':
			returnToUserMenu = true;
			break;
		default:
			break;
		}
		cout << "\n";
	} while (!returnToUserMenu);
}

void Menu::characterSelect() {
	if (!SessionManager::isLoggedIn()) return;
	SessionManager::loadUserCharacters();
	User currentUser = SessionManager::getCurrentUser();
	vector<Character> userCharacters = currentUser.getCharacters();
	if (!userCharacters.empty()) {
		string input;
		bool validInput = false;
		do {
			MiscUtils::clearScreen();
			cout << "[CHARACTER SELECT]\nEnter the number next to the character you wish to select! (or press ESC key to return)\n\n";
			int i = 0;
			for (vector<Character>::iterator it = userCharacters.begin(); it != userCharacters.end(); ++it) {
				cout << i + 1 << ". " << it->getName() << "\n";
				i++;
			}

			cout << "\n";
			cout << "Selection > ";
			input = MiscUtils::safeInputNumbers(true, false);
			if (input == MAGIC_STRING) return;
			if (!input.empty()) {
				int num = stoi(input);

				num--;
				if (num > -1 && num < userCharacters.size()) {
					cout << "\n";
					SessionManager::selectCharacter(userCharacters[num].getID());
					validInput = true;
				}
			}
		} while (!validInput);
		MiscUtils::waitUserInput();
		characterMenu();
	}
	else {
		MiscUtils::clearScreen();
		cout << "You haven't created any characters yet!\nUse the second option in the user menu to create a new character!\n";
		MiscUtils::waitUserInput();
	}
}

void Menu::createCharacter() {
	if (SessionManager::isConnectedToDatabase()) {
		string characterName;
		bool validName = false;
		do {
			MiscUtils::clearScreen();
			cout << "[CREATING A CHARACTER]\n\n";
			cout << "Please input the name you would like your character to have. (20 character limit)\n";
			cout << "\n";
			cout << "Character Name > ";
			characterName = MiscUtils::SafeInput(true, true, false, false, 20);
			if (characterName == MAGIC_STRING) return;
			if (characterName.size() > 0) {
				validName = true;
			}
		} while (!validName);

		SessionManager::createCharacter(characterName);
		cout << "\n";
		cout << "User \"" << characterName << "\" has been created created!!\n";
		MiscUtils::waitUserInput();
	}
	else {
		MiscUtils::clearScreen();
		cout << "\n";
		cout << "The client is not currently connected to any database!!!\n";
		MiscUtils::waitUserInput();
	}
}

void Menu::userMenu() {
	if (!SessionManager::isLoggedIn()) return;
	User currentUser = SessionManager::getCurrentUser();
	bool returnToMainMenu = false;
	do {
		MiscUtils::clearScreen();
		int input;
		cout << "[USER: " << currentUser.getID() << ":" << currentUser.getName() << "]\nUse the numbers on your keyboard to pick an option!\n\n";
		cout << "1. Select a character\n2. Create a character\n3. Logout\n";

		do {
			cout << "\n";
			cout << "Input > ";
			input = MiscUtils::getKey(true);
		} while (input < 49 || input > 51);

		switch (static_cast<char>(input)) {
		case '1':
			characterSelect();
			break;
		case '2':
			createCharacter();
			break;
		case '3':
			returnToMainMenu = true;
			break;
		default:
			break;
		}
		cout << "\n";
	} while (!returnToMainMenu);
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
		cout << "\n";
		cout << "User \"" << userName << "\" has been created created!!\n";
		MiscUtils::waitUserInput();
	}
	else {
		MiscUtils::clearScreen();
		cout << "\n";
		cout << "The client is not currently connected to any database!!!\n";
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

		// We jump to user menu

		userMenu();
	}
	else {
		MiscUtils::clearScreen();
		cout << "The client is not currently connected to any database!!!\n";
		MiscUtils::waitUserInput();
	}
}

void Menu::mainMenu() {
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
		this->mainMenu();
	} while (!exit);
	MiscUtils::clearScreen();
	cout << "\n";
	SessionManager::disconnectFromDatabase();
}