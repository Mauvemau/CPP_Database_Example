#pragma once

#include <iostream>

using namespace std;

class Character {
private:
	int id;
	string name;
public:
	Character(int id, string name);
	~Character();

	int getID();
	string getName();
};