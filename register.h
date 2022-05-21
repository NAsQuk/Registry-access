#pragma once
#include<string>
#include<fstream>
#include <iostream>
#include <Windows.h>
#include <stack>
#include <list>
#include"String.h"
using namespace std;

class Registr
{
private:
	HKEY hkey;
public:

	Registr() {	}
	Registr(HKEY hkey, String str) {}

	~Registr() {}
	HKEY get_h()
	{
		return this->hkey;
	}
	void set_h(HKEY h)
	{
		this->hkey = h;
	}

	int show(HKEY hkey);
	string type_to_string(DWORD c);

	int open(stack<HKEY>& st, LPCSTR str);
	int close(stack<HKEY>& s_hkey);

	int open1(HKEY, LPCSTR str);
	int close1(HKEY);

	int menu(stack<HKEY>& steck, list <string>& current_dir);
	void save();
};