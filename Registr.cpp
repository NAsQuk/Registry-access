#include "register.h"
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <iomanip>
#include <string>
#include<cstdio>
#include "inputex.h"
#include <chrono>
#include <ctime>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

int Registr::show(HKEY hkey)
{
	char buff[255];
	int i = 0;
	cout << "__________________________________________________________________________________________________________________________________________" << endl;
	cout << "| № |                 ИМЯ                                                                                                                 |" << endl;
	cout << "__________________________________________________________________________________________________________________________________________" << endl;
	int j = 0;
	while (1)
	{
		DWORD blen = 255;
		j++;
		if (RegEnumKeyExA(hkey, static_cast<DWORD>(i), buff, &blen, NULL, NULL, NULL, NULL) == ERROR_NO_MORE_ITEMS)
			break;

		cout << setw(2) << dec << setfill(' ') << j << setw(3) << "|" << buff << endl;
		cout << "__________________________________________________________________________________________________________________________________________|" << endl;
		if (i == 800) { system("pause"); }
		i++;
	}
	j = 0;
	i = 0;
	DWORD blen = 255;
	DWORD number_subkeys = 0;
	DWORD number_values = 0;
	DWORD max_name = 0;
	DWORD max_data = 0;
	RegQueryInfoKeyA(hkey, NULL, NULL, NULL, &number_subkeys, NULL, NULL, &number_values, &max_name, &max_data, NULL, NULL);

	char buff_name[255];
	DWORD buff_type = 0;
	BYTE* buff_data;
	if (!(buff_data = (BYTE*)malloc(max_data * sizeof(BYTE))))
	{
		cout << "error malloc";
		RegCloseKey(hkey);

		return 0;
	}
	DWORD buff_data_size = max_data;
	cout << "________________________________________________________________________________________" << endl;
	cout << "|                      ИМЯ              |        ТИП        |      ЗНАЧЕНИЕ             |" << endl;
	cout << "________________________________________________________________________________________" << endl;

	RegEnumValueA(hkey, static_cast<DWORD>(i), buff_name, &blen, NULL, &buff_type, buff_data, &buff_data_size);
	while (1)
	{
		buff_data_size = max_data;
		blen = 255;
		if (RegEnumValueA(hkey, static_cast<DWORD>(i), buff_name, &blen, NULL, &buff_type, buff_data, &buff_data_size) == ERROR_NO_MORE_ITEMS)
			break;
		cout << setw(40) << dec << setfill(' ') << buff_name << "|" << setw(10) << type_to_string(buff_type) << setw(10) << "|";


		if (buff_type == 4)
		{
			cout << setfill('0') << setw(2) << hex << (int)buff_data[3] << hex << setfill('0') << setw(2)
				<< (int)buff_data[2] << setfill('0') << setw(2) << hex << (short int)buff_data[1] << setfill('0') << setw(2) << hex << (int)buff_data[0];
		}
		else if (buff_type == 3)
		{
			cout.unsetf(ios::left);
			for (int j = 0; j < buff_data_size; j++)
				cout << setfill('0') << setw(2) << hex << (int)buff_data[j] << " ";
			cout << setfill(' ');
		}
		else {
			for (int j = 0; j < buff_data_size; j++)
				cout << hex << buff_data[j];
		}
		cout << endl;

		cout << "________________________________________________________________________________________|" << endl;
		i++;
	}

	return 0;
}


string Registr::type_to_string(DWORD c)
{
	switch (c)
	{
	case 1:                    	  return "sz";
	case 3:					 	  return "binary";
	case 4:                  	  return "dword";
	}
	return "unknown";

}


int Registr::open(stack <HKEY>& s_hkey, LPCSTR str)
{
	HKEY hkey;
	hkey = s_hkey.top();
	if (str == NULL)
		s_hkey.pop();
	if (RegOpenKeyExA(hkey, str, 0, KEY_READ, &hkey) != ERROR_SUCCESS) //HKEY_CURRENT_USER, "Console" Volatile Environment
	{
		cout << "Error opening a key";
		return 1;
	}
	s_hkey.push(hkey);
	return 0;
}

int Registr::close(stack<HKEY>& s_hkey)
{
	HKEY hkey;
	hkey = s_hkey.top();
	s_hkey.pop();
	RegCloseKey(hkey);
	return 0;
}

int Registr::menu(stack<HKEY>& steck, list <string>& current_dir)
{
	HKEY hkey = HKEY_CLASSES_ROOT;
	int i;
	cout << "Check the directory you need" << endl;
	cout << "1-HKEY_CLASSES_ROOT" << endl;
	cout << "2-HKEY_CURRENT_CONFIG" << endl;
	cout << "3-HKEY_CURRENT_USER" << endl;
	cout << "4-HKEY_LOCAL_MACHINE" << endl;
	cout << "5-HKEY_USERS" << endl;
	cout << "6-сохранить весь реестр в git" << endl;
	cout << "0-exit" << endl;
	i = InputCheck(0, 6);
	if (i == 1) {
		hkey = HKEY_CLASSES_ROOT;
		current_dir.push_back("HKEY_CLASSES_ROOT");
	}

	if (i == 2)
	{
		hkey = HKEY_CURRENT_CONFIG;
		current_dir.push_back("HKEY_CURRENT_CONFIG");
	}
	if (i == 3)
	{
		hkey = HKEY_CURRENT_USER;
		current_dir.push_back("HKEY_CURRENT_USER");
	}
	if (i == 4)
	{
		hkey = HKEY_LOCAL_MACHINE;
		current_dir.push_back("HKEY_LOCAL_MACHINE");
	}
	if (i == 5)
	{
		hkey = HKEY_USERS;
		current_dir.push_back("HKEY_USERS");
	}
	if (i == 0)
	{
		return 0;
	}
	steck.push(hkey);
	return i;
}

HRESULT ModifyPrivilege1(
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

int Registr::open1(HKEY hkey, LPCSTR str)
{
	if (RegOpenKeyExA(hkey, str, 0, KEY_READ, &hkey) != ERROR_SUCCESS) //HKEY_CURRENT_USER, "Console" Volatile Environment
	{
		cout << "Error opening a key";
		return 1;
	}
	return 0;
}

int Registr::close1(HKEY hkey)
{
	RegCloseKey(hkey);
	return 0;
}

bool saveFilesToGit() {
	string comiit;
	cout << "Введите строку" << endl;
	cin >> comiit;

	string commitScript1 = "cd C:\\Users\\user\\register\\ && git init\"";
	system(commitScript1.c_str());
	system("pause");

	string commitScript = "cd C:\\Users\\user\\register\\ && git commit -a -m\"" + comiit + '"';

	system("cd C:\\Users\\user\\register\\ && git add .");
	system("pause");
	system(commitScript.c_str());
	system("pause");

	return true;
}


void Registr::save()
{
	HKEY hkey1 = HKEY_CLASSES_ROOT, hkey2 = HKEY_CURRENT_USER, hkey3 = HKEY_LOCAL_MACHINE, hkey4 = HKEY_USERS, hkey5 = HKEY_CURRENT_CONFIG;
	stack<HKEY> s, s1;
	s.push(hkey3);
	Registr reg;
	string file_name1 = "C:\\Users\\user\\register\\cr.txt";
	string file_name2 = "C:\\Users\\user\\register\\cu.txt";

	string file_name3 = "C:\\Users\\user\\register\\bcd.txt";
	string file_name6 = "C:\\Users\\user\\register\\sam.txt";
	string file_name7 = "C:\\Users\\user\\register\\hw.txt";
	string file_name8 = "C:\\Users\\user\\register\\sq.txt";
	string file_name9 = "C:\\Users\\user\\register\\sfw.txt";
	string file_name10 = "C:\\Users\\user\\register\\stm.txt";

	string file_name12 = "C:\\Users\\user\\register\\def.txt";
	string file_name13 = "C:\\Users\\user\\register\\s1.txt";
	string file_name14 = "C:\\Users\\user\\register\\s2.txt";
	string file_name15 = "C:\\Users\\user\\register\\s3.txt";
	string file_name16 = "C:\\Users\\user\\register\\s4.txt";
	string file_name17 = "C:\\Users\\user\\register\\s5.txt";



	string file_name5 = "C:\\Users\\user\\register\\cc.txt";
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

	//-------------------------------------
	if (remove(file_name1.c_str()) == 0)
	{
		cout << "Файл удален!!" << endl;
	}
	else cout << "Ошибка удаления!!" << endl;

	if (remove(file_name2.c_str()) == 0)
	{
		cout << "Файл удален!!" << endl;
	}
	else cout << "Ошибка удаления!!" << endl;

	if (remove(file_name3.c_str()) == 0)
	{
		cout << "Файл удален!!" << endl;
	}
	else cout << "Ошибка удаления!!" << endl;

	if (remove(file_name5.c_str()) == 0)
	{
		cout << "Файл удален!!" << endl;
	}
	else cout << "Ошибка удаления!!" << endl;

	if (remove(file_name6.c_str()) == 0)
	{
		cout << "Файл удален!!" << endl;
	}
	else cout << "Ошибка удаления!!" << endl;

	if (remove(file_name7.c_str()) == 0)
	{
		cout << "Файл удален!!" << endl;
	}
	else cout << "Ошибка удаления!!" << endl;

	if (remove(file_name9.c_str()) == 0)
	{
		cout << "Файл удален!!" << endl;
	}
	else cout << "Ошибка удаления!!" << endl;

	if (remove(file_name10.c_str()) == 0)
	{
		cout << "Файл удален!!" << endl;
	}
	else cout << "Ошибка удаления!!" << endl;

	if (remove(file_name12.c_str()) == 0)
	{
		cout << "Файл удален!!" << endl;
	}
	else cout << "Ошибка удаления!!" << endl;

	if (remove(file_name13.c_str()) == 0)
	{
		cout << "Файл удален!!" << endl;
	}
	else cout << "Ошибка удаления!!" << endl;

	if (remove(file_name14.c_str()) == 0)
	{
		cout << "Файл удален!!" << endl;
	}
	else cout << "Ошибка удаления!!" << endl;

	if (remove(file_name15.c_str()) == 0)
	{
		cout << "Файл удален!!" << endl;
	}
	else cout << "Ошибка удаления!!" << endl;

	if (remove(file_name16.c_str()) == 0)
	{
		cout << "Файл удален!!" << endl;
	}
	else cout << "Ошибка удаления!!" << endl;

	if (remove(file_name17.c_str()) == 0)
	{
		cout << "Файл удален!!" << endl;
	}
	else cout << "Ошибка удаления!!" << endl;
	//-------------------------------------


	system("pause");


	reg.open1(hkey1, "");
	if (RegSaveKeyA(hkey1, file_name1.c_str(), NULL) != ERROR_SUCCESS)
	{
		cout << endl << "Ошибка создания файла";
	}
	reg.close1(hkey1);

	reg.open1(hkey2, "");
	if (RegSaveKeyA(hkey2, file_name2.c_str(), NULL) != ERROR_SUCCESS)
	{
		cout << endl << "Ошибка создания файла";
	}
	reg.close1(hkey2);
	s.push(hkey3);
	reg.open(s, "SAM");
	if (RegSaveKeyA(s.top(), file_name6.c_str(), NULL) != ERROR_SUCCESS)
	{
		cout << endl << "Ошибка создания файла";
	}
	reg.close(s);
	s.push(hkey3);
	reg.open(s, "bcd00000000");
	if (RegSaveKeyA(s.top(), file_name3.c_str(), NULL) != ERROR_SUCCESS)
	{
		cout << endl << "Ошибка создания файла";
	}
	reg.close(s);

	s.push(hkey3);
	reg.open(s, "hardware");
	if (RegSaveKeyA(s.top(), file_name7.c_str(), NULL) != ERROR_SUCCESS)
	{
		cout << endl << "Ошибка создания файла";
	}
	reg.close(s);

	s.push(hkey3);
	reg.open(s, "system");
	if (RegSaveKeyA(s.top(), file_name10.c_str(), NULL) != ERROR_SUCCESS)
	{
		cout << endl << "Ошибка создания файла";
	}
	reg.close(s);

	s.push(hkey3);
	reg.open(s, "software");
	if (RegSaveKeyA(s.top(), file_name9.c_str(), NULL) != ERROR_SUCCESS)
	{
		cout << endl << "Ошибка создания файла";
	}
	reg.close(s);

	s1.push(hkey4);
	reg.open(s1, ".default");
	if (RegSaveKeyA(s1.top(), file_name12.c_str(), NULL) != ERROR_SUCCESS)
	{
		cout << endl << "cu Ошибка создания файла\n";
	}
	reg.close(s1);

	s1.push(hkey4);
	reg.open(s1, "s-1-5-18");
	if (RegSaveKeyA(s1.top(), file_name13.c_str(), NULL) != ERROR_SUCCESS)
	{
		cout << endl << "cu Ошибка создания файла\n";
	}
	reg.close(s1);

	s1.push(hkey4);
	reg.open(s1, "s-1-5-19");
	if (RegSaveKeyA(s1.top(), file_name14.c_str(), NULL) != ERROR_SUCCESS)
	{
		cout << endl << "cu Ошибка создания файла\n";
	}
	reg.close(s1);

	s1.push(hkey4);
	reg.open(s1, "s-1-5-20");
	if (RegSaveKeyA(s1.top(), file_name15.c_str(), NULL) != ERROR_SUCCESS)
	{
		cout << endl << "cu Ошибка создания файла\n";
	}
	reg.close(s1);

	s1.push(hkey4);
	reg.open(s1, "s-1-5-21-1018534693-222361758-1210851656-1001");
	if (RegSaveKeyA(s1.top(), file_name16.c_str(), NULL) != ERROR_SUCCESS)
	{
		cout << endl << "cu Ошибка создания файла\n";
	}
	reg.close(s1);

	s1.push(hkey4);
	reg.open(s1, "s-1-5-21-1018534693-222361758-1210851656-1001_classes");
	if (RegSaveKeyA(s1.top(), file_name17.c_str(), NULL) != ERROR_SUCCESS)
	{
		cout << endl << "cu Ошибка создания файла\n";
	}
	reg.close(s1);

	reg.open1(hkey5, "");
	if (RegSaveKeyA(hkey5, file_name5.c_str(), NULL) != ERROR_SUCCESS)
	{
		cout << endl << "cc Ошибка создания файла";
	}
	reg.close1(hkey5);

	if (saveFilesToGit())
	{
		cout << "Успешно сохранено в Git" << endl;
	}



}

