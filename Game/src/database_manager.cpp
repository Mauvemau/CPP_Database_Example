#include "database_manager.h"

#include <iostream>
#include <string>
#include "mysql_utils.h"
#include "misc_utils.h"

using namespace std;

DatabaseManager::DatabaseManager(const char* host, const char* user, const char* password, const char* database, int port) {
	try {
		cout << "Attempting connection with database [" << host << ":" << port << "]...\n";
		mysql = MySQLUtils::connectToDatabase(host, user, password, database, port);
		cout << "Connection has been established successfully!\n";
	}
	catch (const runtime_error& e) {
		cerr << "Error: " << e.what() << "\n";
	}
}

DatabaseManager::~DatabaseManager() {
	try {
		if (!mysql) {
			throw runtime_error("Unable to disconnect with database, no connection has been established.");
		}
		cout << "Closing connection with database [" << mysql->host << ":" << mysql->port << "]...\n";
		mysql_close(mysql);
		cout << "Connection closed successfully!\n";
	}
	catch (const runtime_error& e) {
		cerr << "Error: " << e.what() << "\n";
	}
}

// Private

User DatabaseManager::getUserByQuery(const string& query) {
	int id_user = 0;
	string username = "null";

	try {
		MYSQL_RES* result = MySQLUtils::executeQuery(mysql, query.c_str());

		if (result) {
			MYSQL_ROW row = mysql_fetch_row(result);

			if (row) {
				id_user = atoi(row[0]);
				username = row[1];

				User user(id_user, username);
				mysql_free_result(result);
				return user;
			}
			else {
				mysql_free_result(result);
				throw runtime_error("User not found.");
			}
		}
		else {
			throw runtime_error("User not found.");
		}
	}
	catch (const runtime_error& e) {
		cerr << "Error: " << e.what() << "\n";
	}

	return User(id_user, username);
}

// Public

bool DatabaseManager::validatePassword(int id, string password) {
	string query = "SELECT user_password_hash FROM users WHERE id_user = " + to_string(id);
	string actualPassword = "null";

	try {
		MYSQL_RES* result = MySQLUtils::executeQuery(mysql, query.c_str());

		if (result) {
			MYSQL_ROW row = mysql_fetch_row(result);

			if (row) {
				actualPassword = row[0];
				mysql_free_result(result);
			}
			else {
				mysql_free_result(result);
				throw runtime_error("User not found.");
			}
		}
		else {
			throw runtime_error("User not found.");
		}
	}
	catch (const runtime_error& e) {
		cerr << "Error: " << e.what() << "\n";
	}

	return (password == actualPassword);
}

User DatabaseManager::getUser(int id) {
	string query = "SELECT * FROM users WHERE id_user = " + to_string(id);
	return getUserByQuery(query);
}

User DatabaseManager::getUser(string name) {
	string escapedName = MySQLUtils::escapeString(mysql, MiscUtils::toLowerCase(name));
	string query = "SELECT * FROM users WHERE user_name = '" + escapedName + "'";
	return getUserByQuery(query);
}

void DatabaseManager::setUserName(int id, string newName) {
	string escapedName = MySQLUtils::escapeString(mysql, MiscUtils::toLowerCase(newName));
	string query = "UPDATE users SET user_name = '" + escapedName + "' WHERE id_user = " + to_string(id);
	try {
		MySQLUtils::executeQuery(mysql, query.c_str());
	}
	catch (const runtime_error& e) {
		cerr << "Error: " << e.what() << "\n";
	}
}

void DatabaseManager::setUserPassword(int id, string newPassword) {
	string escapedPass = MySQLUtils::escapeString(mysql, newPassword);
	string query = "UPDATE users SET user_password_hash = '" + escapedPass + "' WHERE id_user = " + to_string(id);
	try {
		MySQLUtils::executeQuery(mysql, query.c_str());
	}
	catch (const runtime_error& e) {
		cerr << "Error: " << e.what() << "\n";
	}
}

void DatabaseManager::createUser(string name, string password) {
	string escapedName = MySQLUtils::escapeString(mysql, MiscUtils::toLowerCase(name));
	string escapedPass = MySQLUtils::escapeString(mysql, password);
	string query = "INSERT INTO users(user_name, user_password_hash) VALUES('" + escapedName + "', '" + escapedPass + "')";
	try {
		MySQLUtils::executeQuery(mysql, query.c_str());
	}
	catch (const runtime_error& e) {
		cerr << "Error: " << e.what() << "\n";
	}
}