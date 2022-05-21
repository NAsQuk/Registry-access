#pragma once
#include <iostream>
#include<string>
using namespace std;

class Exception
{
protected:
	int error;
public:
	Exception()
	{
		error = 0;
	}
	Exception(int n)
	{
		error = n;
	}
};