#pragma once

#include <iostream>

using namespace std;

/// <summary>
/// General tools and utilities.
/// </summary>
class MiscUtils {
public:
	static string toLowerCase(string input);
	/// <summary>
	/// Returns wether the character string has a space or not.
	/// </summary>
	static bool containsSpace(const char input[]);
	/// <summary>
	/// Makes sure that the input string of characters is valid, also eliminating input buffer issues.
	/// </summary>
	static void validateCharInput(char input[], size_t size);
	/// <summary>
	/// Makes sure that the provided input is of the specified type.
	/// </summary>
	template <typename T>
	static T validateInput();
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