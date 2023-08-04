#include "misc_utils.h"

#include <functional>
#include <string>

string MiscUtils::toLowerCase(string input) {
	string result = input;
	for (char& c : result) {
		c = std::tolower(c);
	}
	return result;
}

bool MiscUtils::containsSpace(const char input[]) {
	for (size_t i = 0; input[i] != '\0'; i++) {
		if (input[i] == ' ') {
			cout << "\nInput provided contains a space! Please input a value without spaces!\n> ";
			return true;
		}
	}
	return false;
}

void MiscUtils::validateCharInput(char input[], size_t size) {
	bool validInput = false;
	while (!validInput) {
		cin.getline(input, size);

		if (input[0] != '\0' && input[0] != ' ') {
			validInput = true;
		}
	}
}

template <typename T>
T MiscUtils::validateInput() {
	T input;
	cin >> input;
	while (cin.fail()) {
		cout << "[Input failed!] Please input a value of type" << typeid(input).name() << ".\n> ";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> input;
	}
	return input;
}

string MiscUtils::hashString(string inputString) {
	hash<string> hasher;
	return to_string(hasher(inputString));
}

string MiscUtils::usernameInput() {
	char input[20];
	do {
		validateCharInput(input, 20);
	} while (containsSpace(input));
	return static_cast<string>(input);
}

string MiscUtils::passwordInput() {
	string input = usernameInput();
	return hashString(input);
}