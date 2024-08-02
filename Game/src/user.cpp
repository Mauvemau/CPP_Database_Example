#include "user.h"

User::User(int id, string name) {
	this->id = id;
	this->name = name;
}

User::~User() {

}

int User::getID() {
	return id;
}

string User::getName() {
	return name;
}

vector<Character> User::getCharacters() {
	return characters;
}

void User::addCharacter(Character character) {
	characters.push_back(character);
}

void User::setCharacters(vector<Character> characters) {
	this->characters = characters;
}