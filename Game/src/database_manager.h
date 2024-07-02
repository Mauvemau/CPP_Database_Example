#pragma once

#include "mysql/mysql.h"

#include "user.h"

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
};
