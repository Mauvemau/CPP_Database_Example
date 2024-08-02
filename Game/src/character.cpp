#include "character.h"

Character::Character(int id, string name) {
	this->id = id;
	this->name = name;
}

Character::~Character() {

}

int Character::getID() {
	return id;
}

string Character::getName() {
	return name;
}