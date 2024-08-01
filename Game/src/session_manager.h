#pragma once

#include "database_manager.h"

class SessionManager {
private:
	static DatabaseManager* dbm;
	static User currentUser;
	static bool loggedIn;
public:
	static void connectToDatabase(string path);
	static void disconnectFromDatabase();

	static bool isConnectedToDatabase();
	static bool isLoggedIn();

	static bool verifyUser(string userName);
	static bool verifyPassword(string username, string password);

	static void logIn(string userName, string password);
	static void logOut();

	static void createUser(string userName, string password);
};