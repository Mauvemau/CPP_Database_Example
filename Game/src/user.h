#pragma once

#include <iostream>

using namespace std;

class User {
private:
	int id;
	string name;

public:

	User(int id, string name);
	~User();

	int getID();
	string getName();
};