#pragma once
#include "exeption.h"
#include<string>
using namespace std;

class InputException :public Exception
{
public:
	InputException() :Exception() {};
	InputException(int n) :Exception(n) {};
	void NumError();
};

string InputChCheck(string input);
bool CheckCorrect(string input, bool fl);
int InputCheck(int inter1, int inter2);
bool CheckintCorrect(string input, bool fl);
string InputintCheck(string input);
