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

vector<Pet> Character::getPets() {
	return pets;
}

void Character::setPets(vector<Pet> pets) {
	this->pets = pets;
}

void Character::addPet(Pet pet) {
	pets.push_back(pet);
}