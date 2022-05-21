#include "inputex.h"
#include <iostream>
#include<string>
using namespace std;

void InputException::NumError()
{
	switch (this->error)
	{
	case 1:
		cout << "������ 1.1" << endl;
		cout << "��������� ����!" << endl;
		cout << "� ������ ���������� ����������������� �������." << endl << endl;
		break;
	case 2:
		cout << "������ 1.2" << endl;
		cout << "������, ����� ������ ����� ��� int" << endl << endl;
		break;
	case 3:
		cout << "������ 1.3" << endl;
		cout << "������� ������� ������!" << endl;
		cout << "����� ������ �� ������ ��������� - 30 ��������." << endl << endl;
		break;
	case 4:
		cout << "������ 1.4" << endl;
		cout << "������� �� ������������� ���� int!(�������� ������� ����� ����� �����)" << endl << endl;
		break;
	case 5:
		cout << "������ 1.9" << endl;
		cout << "����������� ����. ����� ������ ���� �� 0 �� 9" << endl << endl;
		break;
	case 6:
		cout << "������ 1.6" << endl;
		cout << "������ ������." << endl << endl;
		break;
	case 7:
		cout << "������ 1.7" << endl;
		cout << "��� ����� �� ������������� �������, ���� ��� ���������� ������ �����������. ������������� ���� �" << endl << endl;
		break;
	case 8:
		cout << "������ 1.8" << endl;
		cout << "��� ����� �� ������������� �������, ����� ����� ����� ������ ���� :" << endl << endl;
		break;
	case 9:
		cout << "������ 1.9" << endl;
		cout << " ���������� ����� �� ������������� �������, .txt ��� .reg" << endl << endl;
		break;


	default:
		puts("+++++");
		puts("++++++++");
		break;
	}
}








int InputCheck(int inter1, int inter2)
{
	bool flag = true;
	int tmp = 0;
	do
	{
		try
		{
			cin.sync();
			cin.clear();
			rewind(stdin);
			cin >> tmp;
			flag = true;

			if (!cin || (cin.peek() < '0' && cin.peek() > '9'))
			{
				flag = false;
				throw InputException(2);
			}
			if (!cin || cin.peek() != '\n')
			{
				flag = false;
				throw InputException(4);
			}
			if (tmp<inter1 || tmp>inter2)
			{
				flag = false;
				throw 5;// �������� �� ��� catch (...)
			}
		}
		catch (InputException e)
		{
			e.NumError();
			cin.sync();
			cin.clear();
			rewind(stdin);
		}
		catch (...)
		{
			cin.sync();
			cin.clear();
			rewind(stdin);
			cout << "����� ����� �� �������� �� " << inter1 << " �� " << inter2 << endl;
		}
	} while (!flag);
	return tmp;
}

string InputintCheck(string input)// �������� �� char ����� ����� ����������
{
	bool fl = true;
	do {
		try
		{
			fflush(stdin);
			cin.sync();
			cin.clear();
			cin >> input;
			CheckintCorrect(input, fl);
			fl = false;
		}
		catch (InputException e)
		{
			e.NumError();
			fflush(stdin);
			cin.sync();
			cin.clear();
		}
		fflush(stdin);
	} while (fl);
	return input;
}

bool CheckintCorrect(string input, bool fl) throw (InputException)
{
	int len = input.length();
	if (input == "")
	{
		fl = false;
		throw InputException(13);
	}
	for (int i = 0; i < len; i++)
	{
		if (input[i] < '0' || input[i] > '9')
		{
			fl = false;
			throw InputException(5);

		}
	}

	return fl;
}


string InputChCheck(string input)// �������� �� char ����� ����� ����������
{
	bool fl = true;
	do {
		try
		{
			fflush(stdin);
			cin.sync();
			cin.clear();
			cin >> input;
			CheckCorrect(input, fl);
			fl = false;
		}
		catch (InputException e)
		{
			e.NumError();
			fflush(stdin);
			cin.sync();
			cin.clear();
		}
		fflush(stdin);
	} while (fl);
	//input = toupper(input[0]);
	return input;
}

bool CheckCorrect(string input, bool fl) throw (InputException)
{
	int len = strlen(input.c_str());
	if (input[0] != 'C')
	{
		fl = false;
		throw InputException(7);
	}
	if (input[1] != ':')
	{
		fl = false;
		throw InputException(8);
	}

	for (int i = 0; i < len; i++)
	{
		if (input[i] == '.')
		{
			if (input[i + 1] != 't')
			{
				if (input[i + 1] != 'r')
				{
					fl = false;
					throw InputException(9);
				}
				else
				{
					if (input[i + 2] != 'e')
					{
						fl = false;
						throw InputException(9);
					}
					else
					{
						if (input[i + 3] != 'g')
						{
							fl = false;
							throw InputException(9);
						}
					}
				}


			}
			else {

				if (input[i + 2] != 'x')
				{
					fl = false;
					throw InputException(9);
				}
				else
				{
					if (input[i + 3] != 't')
					{
						fl = false;
						throw InputException(9);
					}
				}
			}

		}
	}

	if (len > 15)
	{
		fl = false;
		throw InputException(3);
	}
	return fl;
}

