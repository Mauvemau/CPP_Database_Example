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