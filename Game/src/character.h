#pragma once

#include "pet.h"

#include <vector>

class Character {
private:
	int id;
	string name;
	vector<Pet> pets;

public:
	Character(int id, string name);
	~Character();

	int getID();
	string getName();

	vector<Pet> getPets();

	void setPets(vector<Pet> pets);
	void addPet(Pet pet);
};