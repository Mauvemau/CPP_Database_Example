#pragma once

#ifdef _WIN32
#include "mysql.h"
#else // Library directory on linux is different
#include "mysql/mysql.h"
#endif

#include "user.h"

#include <vector>

/// <summary>
/// Class used to handle a database.
/// </summary>
class DatabaseManager {
private:
	MYSQL* mysql;

	/// <summary>
	/// Returns a single user using a query.
	/// </summary>
	User getUserByQuery(const string& query);
public:

	DatabaseManager(const char* host, const char* user, const char* password, const char* database, int port);
	~DatabaseManager();

	/// <summary>
	/// Returns whether or not a connection has been established.
	/// </summary>
	/// <returns></returns>
	bool isConnected();

	// ---- User Related

	/// <summary>
	/// Returns if the User exists in the database.
	/// </summary>
	/// <returns></returns>
	bool validateUser(string name);
	/// <summary>
	/// Returns if the password is correct or not.
	/// </summary>
	bool validatePassword(int id, string password);

	/// <summary>
	/// Returns a specified user.
	/// </summary>
	User getUser(int id);
	User getUser(string name);

	void setUserName(int id, string newName);
	void setUserPassword(int id, string newPassword);

	/// <summary>
	/// Inserts a new user into the database.
	/// </summary>
	void createUser(string name, string password);

	// ---- Character Related

	vector<Character> getUserCharacters(int userID);

	void createCharacter(int userID, string characterName);

	// ---- Pet Related

	vector<Pet> getCharacterPets(int characterID);

	void deletePet(int petID);
	void createPet(int characterID, string petName, float maxHP, float atk, int typeID);
};
