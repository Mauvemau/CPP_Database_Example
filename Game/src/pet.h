#pragma once

#include <iostream>

using namespace std;

enum class PetType {
	FIRE = 1,
	WATER,
	GRASS
};

class Pet {
private:
	int id;
	string name;
	float maxHP;
	float curHP;
	float atk;
	PetType type;
public:
	Pet(int id, string name, float maxHP, float curHP, float atk, PetType type);
	~Pet();

	int getID();
	string getName();
	float getMaxHP();
	float getCurrentHP();
	float getAttack();
	PetType getType();
	string getTypeString();
};