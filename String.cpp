#define _CRT_SECURE_NO_WARNINGS
#include"String.h"
#include<cstring>
#include<iostream>
#include<string>
using namespace std;

String::String(int _length)
{
	this->length = _length;
	this->mystring = new char[this->length];
	strcpy(this->mystring, "");
}
String::String(char str[])
{
	this->length = strlen(str);
	this->mystring = new char[this->length + 1];
	strcpy(this->mystring, str);
}
String::~String()
{
	delete[] this->mystring;
}
ostream& operator << (ostream& os, String& str)
{
	return os << str.mystring;
}

istream& operator >> (istream& is, String& str)
{
	fflush(stdin);
	delete[] str.mystring;
	char buf[256];
	is.sync();
	is.getline(buf, sizeof buf);
	str.length = strlen(buf);
	str.mystring = new char[str.length + 1];
	strcpy(str.mystring, buf);
	return is;
}

const char* String::const_ch()
{
	return mystring;

}

string String::turn_str()
{
	string s = this->mystring;
	return s;
}