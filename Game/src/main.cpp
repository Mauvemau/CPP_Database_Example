#include <iostream>

#include "database_manager.h"
#include "misc_utils.h"

using namespace std;

int main() {
    DatabaseManager databaseManager("localhost", "root", "1234", "my_game", 3306);
    cout << "\n";

    system("pause");
    cout << "\n";
    string username;
    string password;

    cout << "Insert the username you want to create.\n> ";
    cin >> username;

    cout << "Insert the password you want to create.\n> ";
    cin >> password;

    cout << "\n";

    cout << "Your username is: " << username << ", your password is: " << password << "\n";
    cout << "The hashed password is: " << MiscUtils::hashString(password) << "\n";

    cout << "\n";

    cout << "Inserting new user into the database...\n";

    databaseManager.createUser(username, MiscUtils::hashString(password));

    cout << "\n";
    cout << "Retrieving new entry from the database...\n";

    User user = databaseManager.getUser(username);

    if (user.getID() != 0) {
        cout << "User retrieved successfully!\n";
        cout << "User ID: " << user.getID() << ".\nUser name: " << user.getName() << "\n";
    }
    else {
        cout << "Couldn't retrieve user, insertion mistake.";
    }

    cout << "\n";
    system("pause");
    return 0;
}