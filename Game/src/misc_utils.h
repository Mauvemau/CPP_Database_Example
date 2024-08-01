#pragma once

#include <iostream>
#include <map>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#endif

using namespace std;

static const string MAGIC_STRING = "_ABORT_INPUT";

const int KEY_ESC = 27;
#ifdef _WIN32
const int KEY_UP = 256 + 72;
const int KEY_DOWN = 256 + 80;
const int KEY_LEFT = 256 + 75;
const int KEY_RIGHT = 256 + 77;
const int KEY_DELETE = 256 + 83;
const int KEY_INSERT = 256 + 82;
const int KEY_ENTER = 13;
const int KEY_BEG = 256 + 71;
const int KEY_END = 256 + 79;
#else
const int KEY_UP = 65;
const int KEY_DOWN = 66;
const int KEY_LEFT = 68;
const int KEY_RIGHT = 67;
const int KEY_DELETE = 51;
const int KEY_INSERT = 50;
const int KEY_ENTER = 10;
const int KEY_BEG = 49;
const int KEY_END = 52;
#endif

const int LEFT_UP = 201;
const int LEFT_DW = 200;
const int RIGHT_UP = 187;
const int RIGHT_DW = 188;
const int LINE_HOR = 205;
const int LINE_VER = 186;

/// <summary>
/// General tools and utilities.
/// </summary>
class MiscUtils {
private:
	/// <summary>
	/// Returns wether or not a character is allowed or not.
	/// </summary>
	static bool isSafeCharacter(char input, bool allowSpecialChars);
public:
	/// <summary>
	/// Clears the screen
	/// </summary>
	static void clearScreen();
	/// <summary>
	/// Reads your input and returns the decimal ascii of your input.
	/// </summary>
	static int getKey(bool wait);
	/// <summary>
	/// Prints the message "Press any key to continue" and waits for user input.
	/// </summary>
	static void waitUserInput();
	/// <summary>
	/// Similar to cout but 
	/// </summary>
	static string SafeInput(bool allowAbort);
	static string SafeInput(bool allowAbort, bool allowSpacing, bool allowSpecialChars, bool censorInput);
	static string SafeInput(bool allowAbort, bool allowSpacing, bool allowSpecialChars, bool censorInput, int charLimit);
	/// <summary>
	/// Receives a string and returns the same string but in lowerCase.
	/// </summary>
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
	/// <summary>
	/// Looks for a database config file at a path and parses the info into a dictionary.
	/// </summary>
	static map<string, string> parseDatabaseConfigFile(string path);
};