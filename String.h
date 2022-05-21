#pragma once

#define _CRT_SECURE_NO_WARNINGS
#ifndef _STRING_H_
#define _STRING_H_
#include<iostream>
#include<string>
using namespace std;

class String
{
private:
	char* mystring;
	int length;
public:
	String(int _length = 100);
	String(char str[]);
	~String();
	friend std::ostream& operator << (std::ostream& os, String& str);
	friend std::istream& operator >> (std::istream& is, String& str);
	const char* const_ch();
	string turn_str();

};
#endif