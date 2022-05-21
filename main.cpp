#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <string>
#include"inputex.h"
#include <stack>
#include <list>
#include "register.h"
#include "interaction.h"
using namespace std;

int main()
{

	setlocale(LC_ALL, "rus");
	list<string>  ad;
	stack <HKEY> steck;
	Registr reg;
	Interaction fun;
	int counter = 0;
	counter = reg.menu(steck, ad);
	if (counter == 6)
	{
		reg.save();
		return 0;
	}
	if (counter == 0)return 0;
	int log = fun.logic(steck, ad);
	if (log == 15)return 0;
	return 0;
}






