#include "session_manager.h"
#include "misc_utils.h"

#include <string>

DatabaseManager* SessionManager::dbm;
User SessionManager::currentUser(-1, "default");
bool SessionManager::loggedIn = false;

void SessionManager::connectToDatabase(string path) {
	map<string, string> config = MiscUtils::parseDatabaseConfigFile(path);

	std::string host = config.at("host");
	std::string user = config.at("user");
	std::string password = config.at("password");
	std::string database = config.at("database");
	int port = std::stoi(config.at("port"));

	dbm = new DatabaseManager(host.c_str(), user.c_str(), password.c_str(), database.c_str(), port);
}

void SessionManager::disconnectFromDatabase() {
	delete dbm;
}

bool SessionManager::isConnectedToDatabase() {
	if (dbm == nullptr) return false;
	return dbm->isConnected();
}

bool SessionManager::isLoggedIn() {
	return loggedIn;
}

bool SessionManager::verifyUser(string userName) {
	return dbm->validateUser(userName);
}

bool SessionManager::verifyPassword(string username, string password) {
	if (dbm->validateUser(username)) {
		User user = dbm->getUser(username);
		return dbm->validatePassword(user.getID(), password);
	}
	return false;
}


void SessionManager::logIn(string userName, string password) {
	User userToLogin = dbm->getUser(userName);
	if (dbm->validatePassword(userToLogin.getID(), password)) {
		currentUser = userToLogin;
		cout << "You're now logged in as " << userName << ".\n";
		loggedIn = true;
	}
	else {
		cout << "Incorrect details!\n";
	}
}

void SessionManager::logOut() {
	loggedIn = false;
}

void SessionManager::createUser(string userName, string password) {
	if (userName.empty() || password.empty()) {
		cout << "Trying to create invalid username!!!\n";
		return;
	}
	dbm->createUser(userName, password);
}