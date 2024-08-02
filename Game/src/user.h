#pragma once

#include "character.h"

#include <iostream>
#include <vector>

using namespace std;

class User {
private:
	int id;
	string name;
	vector<Character> characters; // Character associated with the user.

public:
	User(int id, string name);
	~User();

	int getID();
	string getName();

	vector<Character> getCharacters();

	void addCharacter(Character character);
	void setCharacters(vector<Character> characters);
};