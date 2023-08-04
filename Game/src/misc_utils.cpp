#include "misc_utils.h"

#include <functional>
#include <string>

string MiscUtils::hashString(string inputString) {
	hash<string> hasher;
	return to_string(hasher(inputString));
}

string MiscUtils::usernameInput() {

}

string MiscUtils::passwordInput() {

}