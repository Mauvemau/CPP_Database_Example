#pragma once

#include <iostream>

using namespace std;

/// <summary>
/// General tools and utilities.
/// </summary>
class MiscUtils {
public:
	/// <summary>
	/// Hashes a string using <functional>
	/// </summary>
	static string hashString(string inputString);
	/// <summary>
	/// Special input function for usernames.
	/// </summary>
	static string usernameInput();
	/// <summary>
	/// Special input function for passwords.
	/// </summary>
	static string passwordInput();
};