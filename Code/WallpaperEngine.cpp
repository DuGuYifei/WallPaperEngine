#include <windows.h>
#include <ShellScalingApi.h>
#include <atlstr.h>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
//#include <iostream>

//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // ������ڵ�ַ
#pragma comment( lib, "shcore.lib")

#pragma region �Զ�������

struct ProcessWindow
{
	DWORD dwProcessId;
	HWND hwndWindow;
};

struct MonitorInformation
{
	MONITORINFOEX mRect;
	float mFactor;
};

#pragma endregion


#pragma region ȫ�ֲ���

//��������ʱ��ȡ������
int fileType = -1;
std::string filePath = "";
int wallpaperMode = -1;
bool isWebapp = false;
int screenChosen = 0;
std::vector<int> screenIndex;
bool isAutoStart = false;
bool isCloseWallpaper = false;
int lastSettingStore = 1;

//��������
std::vector<MonitorInformation> monitor_list;		//���õ�����Ļ
std::vector<MonitorInformation> monitor_list_all;	//���е���Ļ
HWND hWorkerw = 0;
RECT hWorkerwRect;

#pragma endregion


#pragma region ��ʽת������
std::string addQuate(std::string ss)
{
	std::string s = ss;
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == ' ')
		{
			for (int j = i;; j--)
			{
				if (j == -1 || s[j] == '\\')
				{
					j++;
					s.insert(s.begin() + j, '\"');
					break;
				}
			}
			for (; i++;)
			{
				if (s[i] == '\\' || i == s.size())
				{
					s.insert(s.begin() + i, '\"');
					break;
				}
			}
		}
	}
	return s;
}

std::string UTF8ToGB(const char* str)
{
	std::string result;
	WCHAR* strSrc;
	LPSTR szRes;

	//�����ʱ�����Ĵ�С
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//�����ʱ�����Ĵ�С
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}

LPCWSTR stringToLPCWSTR(std::string str)
{
	size_t size = str.length();
	int wLen = ::MultiByteToWideChar(CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0);
	wchar_t* buffer = new wchar_t[wLen + 1];
	memset(buffer, 0, (wLen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), size, (LPWSTR)buffer, wLen);
	return buffer;
}

#pragma endregion


#pragma region ��ȡȫ�ֲ���

#pragma region ��ȡ��Ļ��Ϣ�Ļص�����

// ��ȡȫ����Ļ���κ�DPI���ű�����Ϣ
BOOL CALLBACK EnumMonitor(HMONITOR handle, HDC hdc, LPRECT rect, LPARAM param) 
{
	std::vector<MonitorInformation>* list = (std::vector<MonitorInformation>*)param;
	MonitorInformation m;

	//��ȡ��Ļ�������ź󳤶�(С��100%����)
	m.mRect.cbSize = sizeof(m.mRect);
	GetMonitorInfo(handle, &(m.mRect));

	//��ȡ�Ŵ������������ȣ�
	//GetScaleFactorForMonitor(handle, &(m.mFactor));
	DEVMODE devMode;
	devMode.dmSize = sizeof(devMode);
	devMode.dmDriverExtra = 0;
	EnumDisplaySettings(m.mRect.szDevice, ENUM_CURRENT_SETTINGS, &devMode);
	m.mFactor = (float)devMode.dmPelsWidth / (m.mRect.rcMonitor.right - m.mRect.rcMonitor.left);

	list->push_back(m);
	return true;
}

#pragma endregion

#pragma region ��ȡ�ļ���Ϣ��������Ҫ����Ļ

bool ReadParameters(std::string path)
{

#pragma region ��ȡ�ļ�

	std::ifstream file;
	file.open(path, std::ios::in);
	std::string temp = "";
	//1.�ļ�����
	getline(file, temp);
	if (temp == "")
		return 0;
	fileType = atoi(temp.c_str());
	//2.�ļ�·��(�������ű���ո�·�������������Ͽո����������ʹ��)
	getline(file, filePath);
	//filePath = "\"" + UTF8ToGB(filePath.c_str()) + "\" ";
	filePath = UTF8ToGB(filePath.c_str()) + " ";
	//3.��ֽģʽ
	getline(file, temp);
	wallpaperMode = atoi(temp.c_str());
	//4.�Ƿ�webapp
	getline(file, temp);
	isWebapp = atoi(temp.c_str());
	//5.��Ļ����
	getline(file, temp, ' ');
	screenChosen = atoi(temp.c_str());
	for (int i = 0; i < screenChosen; i++)
	{
		getline(file, temp, ' ');
		screenIndex.push_back(atoi(temp.c_str()));
	}
	for (int i : screenIndex)
	{
		monitor_list.push_back(monitor_list_all[i]);
	}
	getline(file, temp);
	//6.��������
	getline(file, temp);
	isAutoStart = atoi(temp.c_str());
	//7.�Ƿ��ǹرձ�ֽ
	getline(file, temp);
	isCloseWallpaper = atoi(temp.c_str());
	//8.�´�����ʱ��������ٴα��������
	getline(file, temp);
	lastSettingStore = atoi(temp.c_str());

	file.close();

#pragma endregion

	return true;
}

#pragma endregion

#pragma endregion


#pragma region д��Ͷ�ȡPID����

void WritePID(int screen, int PID[2])
{
	if(screen == -1)
	{
		for (int i : screenIndex)
		{
			std::ofstream file;
			file.open("./resource/screen" + std::to_string(i) + ".wpeg", std::ios::out);
			file << PID[0];
			file << " ";
			file << PID[1];
			file.close();
		}
		return;
	}

	std::ofstream file;
	file.open("./resource/screen" + std::to_string(screen) + ".wpeg", std::ios::out);
	file << PID[0];
	file << " ";
	file << PID[1];
	file.close();
}

bool ReadPID(int screen, int PID[2])
{
	std::ifstream file;
	file.open("./resource/screen" + std::to_string(screen) + ".wpeg", std::ios::in);
	if (!file.good())
		return false;
	file >> PID[0];
	file >> PID[1];
	file.close();
	return true;
}

#pragma endregion


#pragma region ȫ����ʾ

bool compareLeft(MonitorInformation a, MonitorInformation b)
{
	return a.mRect.rcMonitor.left < b.mRect.rcMonitor.left;
}
bool compareRight(MonitorInformation a, MonitorInformation b)
{
	return a.mRect.rcMonitor.right > b.mRect.rcMonitor.right;
}
bool compareTop(MonitorInformation a, MonitorInformation b)
{
	return a.mRect.rcMonitor.top < b.mRect.rcMonitor.top;
}
bool compareBottom(MonitorInformation a, MonitorInformation b)
{
	return a.mRect.rcMonitor.bottom > b.mRect.rcMonitor.bottom;
}

/// <summary>
/// ������������ʹ�õ���Դ��
/// һ��Ҫ��EnumMonitor֮��
/// </summary>
/// <param name="screen">����ǲ�ֱ�ֽģʽ������-1�����Զ�������ѡ��Ļ��ɵ������Ρ�</param>
void FullScreen(HWND hwnd, int screen)
{
	if (screen != -1)
		SetWindowPos(hwnd, HWND_TOP,
			//monitor_list_all [screen] .mRect.rcMonitor.left - hWorkerwRect.left,
			//monitor_list_all[screen].mRect.rcMonitor.top - hWorkerwRect.top,
			2400,0,
			(monitor_list_all[screen].mRect.rcMonitor.right - monitor_list_all[screen].mRect.rcMonitor.left) * monitor_list_all[screen].mFactor,
			(monitor_list_all[screen].mRect.rcMonitor.bottom - monitor_list_all[screen].mRect.rcMonitor.top) * monitor_list_all[screen].mFactor,
			SWP_FRAMECHANGED);
	else
	{
		std::sort(monitor_list.begin(), monitor_list.end(), compareLeft);
		int left = monitor_list[0].mRect.rcMonitor.left;
		std::sort(monitor_list.begin(), monitor_list.end(), compareRight);
		int right = monitor_list[0].mRect.rcMonitor.right;;
		std::sort(monitor_list.begin(), monitor_list.end(), compareTop);
		int top = monitor_list[0].mRect.rcMonitor.top;
		std::sort(monitor_list.begin(), monitor_list.end(), compareBottom);
		int bottom = monitor_list[0].mRect.rcMonitor.bottom;;
		SetWindowPos(hwnd, HWND_TOP, left - hWorkerwRect.left, top - hWorkerwRect.top, right - left, bottom - top, SWP_FRAMECHANGED);
	}
}

#pragma endregion


#pragma region �رս���

/// <summary>
/// �رս���
/// </summary>
/// <param name="screen">��0���ǿ�����ֽ�ȹر�ģʽ��0�����˳�����ÿ����Ļ����</param>
void MyCloseProcess(int screen)
{
	//�ȶ����ļ����رն�Ӧ����(����id��-1Ҳû��ϵ)
	int PID[2];
	if (screen != 0)
	{
		for (int i = 0; i < screenChosen; i++)
		{
			if (ReadPID(screenIndex[i], PID))
			{
				if (PID[0] != -1)
				{
					std::string l = "taskkill /f /PID " + std::to_string(PID[0]);
					WinExec(l.c_str(), SW_HIDE);
				}
				if (PID[1] != -1)
				{
					std::string l = "taskkill /f /PID " + std::to_string(PID[1]);
					WinExec(l.c_str(), SW_HIDE);
				}
				PID[0] = -1;
				PID[1] = -1;
				WritePID(screenIndex[i], PID);
			}
		}
	}
	else
	{
		for (int i = 0; i < monitor_list_all.size(); i++)
		{
			if (ReadPID(i, PID))
			{
				if (PID[0] != -1)
				{
					std::string l = "taskkill /f /PID " + std::to_string(PID[0]);
					WinExec(l.c_str(), SW_HIDE);
				}
				if (PID[1] != -1)
				{
					std::string l = "taskkill /f /PID " + std::to_string(PID[1]);
					WinExec(l.c_str(), SW_HIDE);
				}
				PID[0] = -1;
				PID[1] = -1;
				WritePID(i, PID);
			}
		}
	}
}

#pragma endregion


#pragma region �������̲�д���ļ�

// ���ҽ��������ڵĻص�����  
BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam)
{
	ProcessWindow* pProcessWindow = (ProcessWindow*)lParam;

	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);

	// �ж��Ƿ���ָ�����̵�������  
	if (pProcessWindow->dwProcessId == dwProcessId && IsWindowVisible(hWnd) && GetParent(hWnd) == NULL)
	{
		pProcessWindow->hwndWindow = hWnd;

		return FALSE;
	}

	return TRUE;
}

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", nullptr);
	if (hDefView != 0)
	{
		hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
		return FALSE;
	}
	return TRUE;
}

/// <summary>
/// ��������
/// </summary>
/// <param name="program"></param>
/// <param name="parameter"></param>
/// <param name="screen">��������Ļ��-1�ǲ�ֱ�ֽģʽ������FullScreen()</param>
void MyCreatePorcess(LPCWSTR program, std::string parameter, int screen)
{
	////�ȶ����ļ����رն�Ӧ����(����id��-1Ҳû��ϵ)
	//MyCloseProcess(screen);

	LPTSTR p = (LPTSTR)stringToLPCWSTR(parameter);
	STARTUPINFO si{ 0 };
	PROCESS_INFORMATION pi{ 0 };
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	if (CreateProcess(program, p, 0, 0, 0, 0, 0, 0, &si, &pi))
	{
		ProcessWindow procwin;
		procwin.dwProcessId = pi.dwProcessId;
		procwin.hwndWindow = NULL;

		// ���������ڣ��ҵ���˵�����ˣ���Ȼ�п����ڼ�������Ҫ�ȴ�
		while (EnumWindows(EnumWindowCallBack, (LPARAM)&procwin))
		{
			Sleep(1000);
		}

		CloseHandle(pi.hProcess);	CloseHandle(pi.hThread);

		HWND hwnd = procwin.hwndWindow;
		SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);	// ȥ�������������ÿɼ���popup�������ڷ��
		//����û������п����޷����������ǿ����ȳ��Բ�Ҫ��
		//FullScreen(hwnd, screen);

		//��Ϊ��Щȫ����Ϸ���ܸķֱ��ʣ���������С�Ϳ��Ըı�����
		ShowWindow(hwnd, SW_MINIMIZE);

		SetParent(hwnd, hWorkerw);							// ����Ƶ��������Ϊprogman���Ӵ���

		//����λ��
		FullScreen(hwnd, screen);

		//����д���ļ�
		int PID[2] = { pi.dwProcessId, -1 };


		if (fileType == 0)
		{
			HWND ffplayer = FindWindow(L"ConsoleWindowClass", 0);
			ShowWindow(ffplayer, SW_HIDE);
		}
		else
		{
			if (fileType == 3 || fileType == 4 || isWebapp)
			{
				HWND hwnd2 = hwnd;
				while (hwnd2 != nullptr)
				{
					hwnd = hwnd2;
					hwnd2 = FindWindowExA(hwnd, 0, 0, 0);
				}
			}
			std::string play = std::to_string((int)hwnd);
			//if (screen != -1)
			//{
			//	play = std::to_string((int)hwnd) + " " +
			//		std::to_string(monitor_list_all[screen].mRect.rcMonitor.left) + " " +
			//		std::to_string(monitor_list_all[screen].mRect.rcMonitor.top);
			//}
			//else
			//{
			//	std::sort(monitor_list.begin(), monitor_list.end(), compareLeft);
			//	int tempLeft = monitor_list[0].mRect.rcMonitor.left;
			//	std::sort(monitor_list.begin(), monitor_list.end(), compareTop);
			//	int tempTop = monitor_list[0].mRect.rcMonitor.top;
			//	play = std::to_string((int)hwnd) + " " +
			//		std::to_string(tempLeft) + " " +
			//		std::to_string(tempTop);
			//}
			if (CreateProcess(L"./Controller.exe ", (LPTSTR)stringToLPCWSTR(play), 0, 0, 0, 0, 0, 0, &si, &pi))
			{
				CloseHandle(pi.hProcess);	CloseHandle(pi.hThread);
				PID[1] = pi.dwProcessId;
			}
		}
		//д���ļ�
		WritePID(screen, PID);
	}
	//else {
	//	std::cout << (GetLastError());
	//}
}

#pragma endregion


#pragma region �����������

//��õ�ǰ·��
std::string GetProgramDir()
{
	wchar_t exeFullPath[MAX_PATH]; // Full path 
	std::string strPath = "";

	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	char CharString[MAX_PATH];
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, CharString, MAX_PATH, exeFullPath, _TRUNCATE);

	strPath = (std::string)CharString;    // Get full path of the file 

	int pos = strPath.find_last_of('\\', strPath.length());
	return strPath.substr(0, pos);  // Return the directory without the file name 
}

// ���򿪻��Զ�����
void AutoStart()
{
	HKEY hKey;

	//1���ҵ�ϵͳ��������  
	if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) ///��������       
	{
		//2���õ������������ȫ·��
		std::string path = GetProgramDir();
		LPCWSTR strRegPath = stringToLPCWSTR("\"" + path + "\\WPEG.exe\"");

		//3���ж�ע������Ƿ��Ѿ�����
		TCHAR strDir[MAX_PATH] = {};
		DWORD nLength = MAX_PATH;
		long result = RegGetValue(hKey, nullptr, L"WPEG", RRF_RT_REG_SZ, 0, strDir, &nLength);

		//4���Ѿ�����
		if (result != ERROR_SUCCESS || _tcscmp(strRegPath, strDir) != 0)
		{
			//5�����һ����Key,������ֵ
			RegSetValueEx(hKey, L"WPEG", 0, REG_SZ, (LPBYTE)strRegPath, (lstrlen(strRegPath) + 1) * sizeof(TCHAR));

			//6���ر�ע���
			RegCloseKey(hKey);
		}
	}
}

// ȡ�������Զ�����
void CancelAutoStart()
{
	HKEY hKey;

	//1���ҵ�ϵͳ��������  
	if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		//2��ɾ��ֵ
		RegDeleteValue(hKey, L"WPEG");

		//3���ر�ע���
		RegCloseKey(hKey);
	}
}

#pragma endregion


#pragma region ������������

void RunSettings()
{
#pragma region ��������
	if (isAutoStart)
	{
		AutoStart();
	}
	else
	{
		CancelAutoStart();
	}
#pragma endregion

#pragma region �رձ�ֽ

	MyCloseProcess(1);

	if (isCloseWallpaper)
	{
		return;
	}

#pragma endregion

#pragma region 	���ñ�ֽ

	//����������Ϣ
	HWND hProgman = FindWindow(L"Progman", 0);					// �ҵ�spy++�����Ǹ�����
	SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);		// ������������Ϣ
	EnumWindows(EnumWindowsProc, (LPARAM)nullptr);				// �ҵ��ڶ���WorkerW����
	GetWindowRect(hWorkerw, &hWorkerwRect);

	//��Ƶ/��̬ͼ
	if (fileType == 0)
	{
		if (wallpaperMode == 0)
		{
			for (int& i : screenIndex)
			{
				MyCreatePorcess(L"./resource/ffplay.exe ", " \"" + filePath + "\"" + " -loop 0", i);
			}
		}
		else
		{
			MyCreatePorcess(L"./resource/ffplay.exe ", " \"" + filePath + "\"" + " -loop 0", -1);
		}
	}

	//ͼƬ
	else if (fileType == 1)
	{
		SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)stringToLPCWSTR(filePath), SPIF_UPDATEINIFILE);
	}

	//����
	else if (fileType == 2)
	{
		if (wallpaperMode == 0)
		{
			for (int& i : screenIndex)
			{
				MyCreatePorcess(stringToLPCWSTR(filePath), " ", i);
			}
		}
		else
		{
			MyCreatePorcess(stringToLPCWSTR(filePath), " ", -1);
		}
	}

	//html����ַ(3��4)
	else
	{
		if (wallpaperMode == 0)
		{
			for (int& i : screenIndex)
			{
				MyCreatePorcess(L"./resource/WebkitChrome/WebkitChrome.exe ", " " + filePath, i);
			}
		}
		else
		{
			MyCreatePorcess(L"./resource/WebkitChrome/WebkitChrome.exe ", " " + filePath, -1);
		}
	}

#pragma endregion

	return;
}

#pragma endregion



int main(int argc, char* argv[])
{
#pragma region ��ȡ��Ļ��Ϣ(��Ϊ�˳�����Ҳ��Ҫ)

	EnumDisplayMonitors(NULL, NULL, EnumMonitor, (LPARAM)&monitor_list_all);

#pragma endregion 

#pragma region ����UI���������������

#pragma region ������ʱ
	//������ʱ
	if (argv[1][0] == 'a')
	{
		if (ReadParameters("./resource/UIContent1.wpeg"))
		{
			if (lastSettingStore == 0)
				return 0;
		}
		else
		{
			return 0;
		}
		
		int nthSetting = lastSettingStore;
		while (nthSetting > 0 )
		{
			while (screenIndex.size() != 0)
			{
				screenIndex.pop_back();
			}
			if (ReadParameters("./resource/UIContent" + std::to_string(nthSetting) + ".wpeg"))
			{
				RunSettings();
				nthSetting--;
			}
			else
			{
				return 0;
			}
		}
		return 0;
	}

#pragma endregion


#pragma region �˳�����ʱ

	//����˳�UI������
	if (argv[1][0] == 'q')
	{
		MyCloseProcess(0);
	}

#pragma endregion


#pragma region ���ȷ������ʱ
	//������UIȷ��
	else if (argv[1][0] == 's')
	{

#pragma region �ȶ�ȡ���ļ�����

		if (!ReadParameters("./resource/UIContent1.wpeg"))
			MessageBox(NULL, stringToLPCWSTR("UIContent1û������"), stringToLPCWSTR("WallpaperEngine"), MB_OK);

#pragma endregion

#pragma region �����������

		RunSettings();

#pragma endregion

	}

#pragma endregion

#pragma endregion

	return 0;
}