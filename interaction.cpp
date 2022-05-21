#include "interaction.h"
#define _CRT_SECURE_NO_WARNINGS
#include <list>
void add(char s1[], const char s2[], int i)
{
	if (s2 != nullptr)
	{
		int j = 0;
		while (s2[j] != '\0')
		{
			s1[j + i] = s2[j];
			j++;
		}
		s1[j + i] = '\0';
	}
	return;
}

void Interaction::menu(list <string> ad)
{

	cout << endl << "Текущий раздел: ";
	copy(ad.begin(), ad.end(), ostream_iterator<string>(cout, "/"));
	cout << endl << "Выберите действие: " << endl << "1-Перейти в следующий раздел" << endl << "2-Вернуться в предыдущий раздел" << endl << "3-Вывести содержимое раздела"
		<< endl << "4-Сохранить в файл" << endl << "5-Выбрать новый корневой каталог" << endl << "0-Завершение" << endl;
}

BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,
		lpszPrivilege,
		&luid))
	{
		printf("error: %u\n", GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		printf("Error: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		printf("ERROR. \n");
		return FALSE;
	}

	return TRUE;
}
HRESULT ModifyPrivilege(
	IN LPCTSTR szPrivilege,
	IN BOOL fEnable)
{
	HRESULT hr = S_OK;
	TOKEN_PRIVILEGES NewState;
	LUID             luid;
	HANDLE hToken = NULL;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken))
	{
		printf("Failed OpenProcessToken\n");
		return ERROR_FUNCTION_FAILED;
	}

	if (!LookupPrivilegeValue(NULL,
		szPrivilege,
		&luid))
	{
		CloseHandle(hToken);
		printf("Failed LookupPrivilegeValue\n");
		return ERROR_FUNCTION_FAILED;
	}

	NewState.PrivilegeCount = 1;
	NewState.Privileges[0].Luid = luid;
	NewState.Privileges[0].Attributes =
		(fEnable ? SE_PRIVILEGE_ENABLED : 0);
	if (!AdjustTokenPrivileges(hToken,
		FALSE,
		&NewState,
		0,
		NULL,
		NULL))
	{
		printf("Failed AdjustTokenPrivileges\n");
		hr = ERROR_FUNCTION_FAILED;
	}
	CloseHandle(hToken);

	return hr;
}
int Interaction::logic(stack<HKEY>& steck, list <string>& ad)
{
	string file_name = "C:\\save.txt";

	Registr reg;
	String str;
	string sttr;
	Interaction fun;
	int ch = 1;
	while (ch != 0)
	{
		fun.menu(ad);
		ch = InputCheck(0, 5);
		switch (ch)
		{
		case 1: {
			if (steck.empty())
			{
				reg.menu(steck, ad);
				break;
			}
			cout << "Введите название раздела: ";
			fflush(stdin);
			getchar();
			cin >> str;

			if (reg.open(steck, str.const_ch()))
			{
				cout << endl << "Неверное название раздела" << endl;
				break;
			}

			string str1 = str.turn_str();
			ad.push_back(str1);


		}break;
		case 2: {
			int c = 0;
			if (!steck.empty())
			{
				reg.close(steck);
				ad.pop_back();
			}
			if (steck.empty())
			{
				cout << "Вы вышли из корневой папки" << endl;
				cout << "Желаете выбрать новую" << endl << "1-да" << endl << "0-нет" << endl;
				c = InputCheck(0, 1);
				if (c == 1)
				{
					while (ad.size() != 0)
					{
						ad.pop_back();
					}
					c = reg.menu(steck, ad);
					if (c == 0)return 0;
				}
				else break;

			}

		}break;
		case 3: {
			if (steck.empty())
			{
				cout << endl << "Перейдите в любой раздел для вывода его содержимого";
				break;
			}
			reg.show(steck.top());

		}break;
		case 4: {
			TOKEN_PRIVILEGES tp;
			HANDLE hToken;
			LUID luid;
			HANDLE ProcessToken;
			if (!OpenProcessToken(GetCurrentProcess(),
				TOKEN_ADJUST_PRIVILEGES,
				&hToken));


			if (!LookupPrivilegeValue(NULL, SE_BACKUP_NAME, &luid));

			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = luid;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;


			AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES),
				NULL, NULL);

			OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &ProcessToken);
			SetPrivilege(ProcessToken, SE_RESTORE_NAME, TRUE);


			cout << "Введите имя файла с расширением .txt" << endl;
			file_name = InputChCheck(file_name);

			if (RegSaveKeyA(steck.top(), file_name.c_str(), NULL) != ERROR_SUCCESS)
			{
				cout << endl << "Ошибка создания файла";
			}
		}break;
		case 5: {
			while (!(steck.empty()))
			{
				steck.pop();
			}
			while (ad.size() != 0)
			{
				ad.pop_back();
			}
			reg.menu(steck, ad);
		}break;
		case 0: {
			return 15;
			while (!steck.empty())
			{
				reg.close(steck);
				ad.pop_back();
				return 15;
			}
		}break;
		default:
			cout << "ERROR" << endl;
			return 15;
		}
	}
}

