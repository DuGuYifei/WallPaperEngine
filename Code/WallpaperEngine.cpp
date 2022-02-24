#include <windows.h>
#include <ShellScalingApi.h>
#include <atlstr.h>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
//#include <iostream>

//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址
#pragma comment( lib, "shcore.lib")

#pragma region 自定义类型

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


#pragma region 全局参数

//程序启动时读取的数据
int fileType = -1;
std::string filePath = "";
int wallpaperMode = -1;
bool isWebapp = false;
int screenChosen = 0;
std::vector<int> screenIndex;
bool isAutoStart = false;
bool isCloseWallpaper = false;
int lastSettingStore = 1;

//其他参数
std::vector<MonitorInformation> monitor_list;		//会用到的屏幕
std::vector<MonitorInformation> monitor_list_all;	//所有的屏幕
HWND hWorkerw = 0;
RECT hWorkerwRect;

#pragma endregion


#pragma region 格式转换函数
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

	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//获得临时变量的大小
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


#pragma region 读取全局参数

#pragma region 获取屏幕信息的回调函数

// 获取全部屏幕矩形和DPI缩放倍数信息
BOOL CALLBACK EnumMonitor(HMONITOR handle, HDC hdc, LPRECT rect, LPARAM param) 
{
	std::vector<MonitorInformation>* list = (std::vector<MonitorInformation>*)param;
	MonitorInformation m;

	//获取屏幕物理缩放后长度(小于100%矩形)
	m.mRect.cbSize = sizeof(m.mRect);
	GetMonitorInfo(handle, &(m.mRect));

	//获取放大倍数（用物理长度）
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

#pragma region 读取文件信息和锁定需要的屏幕

bool ReadParameters(std::string path)
{

#pragma region 读取文件

	std::ifstream file;
	file.open(path, std::ios::in);
	std::string temp = "";
	//1.文件类型
	getline(file, temp);
	if (temp == "")
		return 0;
	fileType = atoi(temp.c_str());
	//2.文件路径(加上引号避免空格路径，并在最后加上空格便与命令行使用)
	getline(file, filePath);
	//filePath = "\"" + UTF8ToGB(filePath.c_str()) + "\" ";
	filePath = UTF8ToGB(filePath.c_str()) + " ";
	//3.壁纸模式
	getline(file, temp);
	wallpaperMode = atoi(temp.c_str());
	//4.是否webapp
	getline(file, temp);
	isWebapp = atoi(temp.c_str());
	//5.屏幕序列
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
	//6.开机自启
	getline(file, temp);
	isAutoStart = atoi(temp.c_str());
	//7.是否是关闭壁纸
	getline(file, temp);
	isCloseWallpaper = atoi(temp.c_str());
	//8.下次启动时按最近多少次保存的设置
	getline(file, temp);
	lastSettingStore = atoi(temp.c_str());

	file.close();

#pragma endregion

	return true;
}

#pragma endregion

#pragma endregion


#pragma region 写入和读取PID函数

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


#pragma region 全屏显示

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
/// 清理所有正在使用的资源。
/// 一定要在EnumMonitor之后
/// </summary>
/// <param name="screen">如果是拆分壁纸模式就输入-1，会自动画出所选屏幕组成的最大矩形。</param>
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


#pragma region 关闭进程

/// <summary>
/// 关闭进程
/// </summary>
/// <param name="screen">非0，是开启壁纸先关闭模式；0就是退出程序，每个屏幕遍历</param>
void MyCloseProcess(int screen)
{
	//先读入文件并关闭对应程序(进程id是-1也没关系)
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


#pragma region 创建进程并写入文件

// 查找进程主窗口的回调函数  
BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam)
{
	ProcessWindow* pProcessWindow = (ProcessWindow*)lParam;

	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);

	// 判断是否是指定进程的主窗口  
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
/// 创建进程
/// </summary>
/// <param name="program"></param>
/// <param name="parameter"></param>
/// <param name="screen">创建到屏幕，-1是拆分壁纸模式，用于FullScreen()</param>
void MyCreatePorcess(LPCWSTR program, std::string parameter, int screen)
{
	////先读入文件并关闭对应程序(进程id是-1也没关系)
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

		// 查找主窗口，找到了说明打开了，不然有可能在加载中需要等待
		while (EnumWindows(EnumWindowCallBack, (LPARAM)&procwin))
		{
			Sleep(1000);
		}

		CloseHandle(pi.hProcess);	CloseHandle(pi.hThread);

		HWND hwnd = procwin.hwndWindow;
		SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);	// 去掉标题栏（设置可见和popup弹出窗口风格）
		//下面没有这句有可能无法交互，但是可以先尝试不要它
		//FullScreen(hwnd, screen);

		//因为有些全屏游戏不能改分辨率，所以先缩小就可以改变框架了
		ShowWindow(hwnd, SW_MINIMIZE);

		SetParent(hwnd, hWorkerw);							// 将视频窗口设置为progman的子窗口

		//设置位置
		FullScreen(hwnd, screen);

		//用于写入文件
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
		//写入文件
		WritePID(screen, PID);
	}
	//else {
	//	std::cout << (GetLastError());
	//}
}

#pragma endregion


#pragma region 开机自启相关

//获得当前路径
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

// 程序开机自动启动
void AutoStart()
{
	HKEY hKey;

	//1、找到系统的启动项  
	if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) ///打开启动项       
	{
		//2、得到本程序自身的全路径
		std::string path = GetProgramDir();
		LPCWSTR strRegPath = stringToLPCWSTR("\"" + path + "\\WPEG.exe\"");

		//3、判断注册表项是否已经存在
		TCHAR strDir[MAX_PATH] = {};
		DWORD nLength = MAX_PATH;
		long result = RegGetValue(hKey, nullptr, L"WPEG", RRF_RT_REG_SZ, 0, strDir, &nLength);

		//4、已经存在
		if (result != ERROR_SUCCESS || _tcscmp(strRegPath, strDir) != 0)
		{
			//5、添加一个子Key,并设置值
			RegSetValueEx(hKey, L"WPEG", 0, REG_SZ, (LPBYTE)strRegPath, (lstrlen(strRegPath) + 1) * sizeof(TCHAR));

			//6、关闭注册表
			RegCloseKey(hKey);
		}
	}
}

// 取消开机自动启动
void CancelAutoStart()
{
	HKEY hKey;

	//1、找到系统的启动项  
	if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		//2、删除值
		RegDeleteValue(hKey, L"WPEG");

		//3、关闭注册表
		RegCloseKey(hKey);
	}
}

#pragma endregion


#pragma region 根据设置运行

void RunSettings()
{
#pragma region 开机自启
	if (isAutoStart)
	{
		AutoStart();
	}
	else
	{
		CancelAutoStart();
	}
#pragma endregion

#pragma region 关闭壁纸

	MyCloseProcess(1);

	if (isCloseWallpaper)
	{
		return;
	}

#pragma endregion

#pragma region 	设置壁纸

	//发送特殊消息
	HWND hProgman = FindWindow(L"Progman", 0);					// 找到spy++最后的那个窗口
	SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);		// 给它发特殊消息
	EnumWindows(EnumWindowsProc, (LPARAM)nullptr);				// 找到第二个WorkerW窗口
	GetWindowRect(hWorkerw, &hWorkerwRect);

	//视频/动态图
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

	//图片
	else if (fileType == 1)
	{
		SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)stringToLPCWSTR(filePath), SPIF_UPDATEINIFILE);
	}

	//程序
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

	//html和网址(3和4)
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
#pragma region 获取屏幕信息(因为退出程序也需要)

	EnumDisplayMonitors(NULL, NULL, EnumMonitor, (LPARAM)&monitor_list_all);

#pragma endregion 

#pragma region 根据UI程序传输过来的命令

#pragma region 自启动时
	//自启动时
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


#pragma region 退出托盘时

	//如果退出UI的托盘
	if (argv[1][0] == 'q')
	{
		MyCloseProcess(0);
	}

#pragma endregion


#pragma region 点击确认命令时
	//如果点击UI确认
	else if (argv[1][0] == 's')
	{

#pragma region 先读取总文件内容

		if (!ReadParameters("./resource/UIContent1.wpeg"))
			MessageBox(NULL, stringToLPCWSTR("UIContent1没有设置"), stringToLPCWSTR("WallpaperEngine"), MB_OK);

#pragma endregion

#pragma region 根据玩家命令

		RunSettings();

#pragma endregion

	}

#pragma endregion

#pragma endregion

	return 0;
}