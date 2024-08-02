#include "pet.h"

Pet::Pet(int id, string name, float maxHP, float curHP, float atk, PetType type) {
	this->id = id;
	this->name = name;
	this->maxHP = maxHP;
	this->curHP = curHP;
	this->atk = atk;
	this->type = type;
}

Pet::~Pet() {

}


int Pet::getID() {
	return id;
}

string Pet::getName() {
	return name;
}

float Pet::getMaxHP() {
	return maxHP;
}

float Pet::getCurrentHP() {
	return curHP;
}

float Pet::getAttack() {
	return atk;
}

PetType Pet::getType() {
	return type;
}

string Pet::getTypeString() {
	string str = "";
	switch (type)
	{
	case PetType::FIRE:
		str = "FIRE";
		break;
	case PetType::WATER:
		str = "WATER";
		break;
	case PetType::GRASS:
		str = "GRASS";
		break;
	default:
		break;
	}
	return str;
}