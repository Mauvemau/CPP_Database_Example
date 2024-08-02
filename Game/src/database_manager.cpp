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

bool DatabaseManager::isConnected() {
	return mysql != nullptr;
}

bool DatabaseManager::validateUser(string name) {
	try {
		string escapedName = MySQLUtils::escapeString(mysql, MiscUtils::toLowerCase(name));
		string query = "SELECT COUNT(*) FROM users WHERE user_name = '" + escapedName + "'";

		MYSQL_RES* result = MySQLUtils::executeQuery(mysql, query.c_str());

		if (result) {
			MYSQL_ROW row = mysql_fetch_row(result);

			if (row && row[0]) {
				int count = atoi(row[0]);
				mysql_free_result(result);

				return count > 0;
			}
			mysql_free_result(result);
		}
	}
	catch (const runtime_error& e) {
		cerr << "Error: " << e.what() << "\n";
	}

	return false;
}

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

vector<Character> DatabaseManager::getUserCharacters(int userID) {
	vector<Character> characters;
	string query = "SELECT c.id_character, c.character_name FROM characters c "
				   "INNER JOIN users_characters uc ON c.id_character = uc.id_character "
				   "WHERE uc.id_user = " + to_string(userID);
	try {
		MYSQL_RES* result = MySQLUtils::executeQuery(mysql, query.c_str());
		if (result) {
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(result))) {
				int id_character = atoi(row[0]);
				string character_name = row[1];
				characters.emplace_back(id_character, character_name);
			}
			mysql_free_result(result);
		}
	}
	catch (const runtime_error& e) {
		cerr << "Error: " << e.what() << "\n";
	}
	return characters;
}

void DatabaseManager::createCharacter(int userID, string characterName) {
	string escapedName = MySQLUtils::escapeString(mysql, MiscUtils::toLowerCase(characterName));
	// We first create a character and insert it into the characters table.
	string query = "INSERT INTO characters(character_name) VALUES('" + escapedName + "')";

	try {
		MySQLUtils::executeQuery(mysql, query.c_str());

		string getLastInsertIdQuery = "SELECT LAST_INSERT_ID()";
		MYSQL_RES* result = MySQLUtils::executeQuery(mysql, getLastInsertIdQuery.c_str());

		if (result) {
			MYSQL_ROW row = mysql_fetch_row(result);
			if (row) {
				int characterID = atoi(row[0]);
				mysql_free_result(result);

				// We then want to associate the character with the userID provided, therefore we retrieve the character ID and insert both ids into the joint table.
				string userCharQuery = "INSERT INTO users_characters(id_user, id_character) VALUES(" + to_string(userID) + ", " + to_string(characterID) + ")";
				MySQLUtils::executeQuery(mysql, userCharQuery.c_str());
			}
			else {
				mysql_free_result(result);
				throw runtime_error("Failed to retrieve character ID.");
			}
		}
		else {
			throw runtime_error("Failed to retrieve character ID.");
		}
	}
	catch (const runtime_error& e) {
		cerr << "Error: " << e.what() << "\n";
	}
}

vector<Pet> DatabaseManager::getCharacterPets(int characterID) {
	vector<Pet> pets;
	string query = "SELECT p.id_pet, p.pet_name, p.pet_max_hp, p.pet_current_hp, p.pet_attack_damage, p.id_type "
		"FROM pets p INNER JOIN characters_pets cp ON p.id_pet = cp.id_pet "
		"WHERE cp.id_character = " + to_string(characterID);

	try {
		MYSQL_RES* result = MySQLUtils::executeQuery(mysql, query.c_str());
		if (result) {
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(result))) {
				int id_pet = atoi(row[0]);
				string pet_name = row[1];
				float pet_max_hp = static_cast<float>(atof(row[2]));
				float pet_current_hp = static_cast<float>(atof(row[3]));
				float pet_attack_damage = static_cast<float>(atof(row[4]));
				int id_type = atoi(row[5]);

				PetType petType = static_cast<PetType>(id_type);
				pets.emplace_back(id_pet, pet_name, pet_max_hp, pet_current_hp, pet_attack_damage, petType);
			}
			mysql_free_result(result);
		}
	}
	catch (const runtime_error& e) {
		cerr << "Error: " << e.what() << "\n";
	}
	return pets;
}

void DatabaseManager::deletePet(int petID) {
	string charPetQuery = "DELETE FROM characters_pets WHERE id_pet = " + to_string(petID);

	try {
		MySQLUtils::executeQuery(mysql, charPetQuery.c_str());

		string petQuery = "DELETE FROM pets WHERE id_pet = " + to_string(petID);
		MySQLUtils::executeQuery(mysql, petQuery.c_str());
	}
	catch (const runtime_error& e) {
		cerr << "Error: " << e.what() << "\n";
	}
}

void DatabaseManager::createPet(int characterID, string petName, float maxHP, float atk, int typeID) {
	string escapedName = MySQLUtils::escapeString(mysql, MiscUtils::toLowerCase(petName));
	string query = "INSERT INTO pets(pet_name, pet_max_hp, pet_current_hp, pet_attack_damage, id_type) VALUES('"
		+ escapedName + "', " + to_string(maxHP) + ", " + to_string(maxHP) + ", " + to_string(atk) + ", " + to_string(typeID) + ")";

	try {
		MySQLUtils::executeQuery(mysql, query.c_str());

		string getLastInsertIdQuery = "SELECT LAST_INSERT_ID()";
		MYSQL_RES* result = MySQLUtils::executeQuery(mysql, getLastInsertIdQuery.c_str());

		if (result) {
			MYSQL_ROW row = mysql_fetch_row(result);
			if (row) {
				int petID = atoi(row[0]);
				mysql_free_result(result);

				string charPetQuery = "INSERT INTO characters_pets(id_character, id_pet) VALUES(" + to_string(characterID) + ", " + to_string(petID) + ")";
				MySQLUtils::executeQuery(mysql, charPetQuery.c_str());
			}
			else {
				mysql_free_result(result);
				throw runtime_error("Failed to retrieve pet ID.");
			}
		}
		else {
			throw runtime_error("Failed to retrieve pet ID.");
		}
	}
	catch (const runtime_error& e) {
		cerr << "Error: " << e.what() << "\n";
	}
}