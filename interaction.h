#pragma once
#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <string>
#include"inputex.h"
#include <stack>
#include <list>
#include "register.h"
using namespace std;

class Interaction
{
public:
	Interaction() {}
	~Interaction() {}
	void menu(list <string> ad);
	int logic(stack<HKEY>& steck, list <string>& current_dir);

};
