#pragma once

#include "database_manager.h"

class SessionManager {
private:
	static DatabaseManager* dbm;
	static User currentUser;
	static Character currentCharacter;
	static bool loggedIn;
public:
	static void connectToDatabase(string path);
	static void disconnectFromDatabase();

	static bool isConnectedToDatabase();
	static bool isLoggedIn();
	static bool isCharacterSelected();

	static User getCurrentUser();
	static Character getCurrentCharacter();

	static bool verifyUser(string userName);
	static bool verifyPassword(string username, string password);

	static void logIn(string userName, string password);
	static void logOut();

	static void selectCharacter(int charID);
	static void unSelectCharacter();

	static void createUser(string userName, string password);

	static void loadUserCharacters();	
	static void createCharacter(string name);

	static void loadCharacterPets();
	static void createPet(string name, float maxHP, float attack, PetType type);
	static void deletePet(int id);
};