#include <windows.h>
#include <fstream>
#include <string>
#include <vector>
//#include <iostream>
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址


HWND hFfplay = 0;
//std::string gameName = "";

//BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam)
//{
//	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", nullptr);
//	if (hDefView != 0) {
//		// 找它的下一个窗口，类名为WorkerW，隐藏它
//		hFfplay = FindWindowEx(0, hwnd, L"WorkerW", 0);
//		hFfplay = FindWindowExA(hFfplay,0,0, gameName.c_str());
//	}
//	return TRUE;
//}
//
//std::string UTF8ToGB(const char* str)
//{
//	std::string result;
//	WCHAR* strSrc;
//	LPSTR szRes;
//
//	//获得临时变量的大小
//	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
//	strSrc = new WCHAR[i + 1];
//	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);
//
//	//获得临时变量的大小
//	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
//	szRes = new CHAR[i + 1];
//	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);
//
//	result = szRes;
//	delete[]strSrc;
//	delete[]szRes;
//
//	return result;
//}
//
//std::string GetProgramDir()
//{
//	wchar_t exeFullPath[MAX_PATH]; // Full path 
//	std::string strPath = "";
//
//	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
//	char CharString[MAX_PATH];
//	size_t convertedChars = 0;
//	wcstombs_s(&convertedChars, CharString, MAX_PATH, exeFullPath, _TRUNCATE);
//
//	strPath = (std::string)CharString;    // Get full path of the file 
//
//	int pos = strPath.find_last_of('\\', strPath.length());
//	return strPath.substr(0, pos);  // Return the directory without the file name 
//}

//RECT rt;
//
std::vector<MONITORINFO> monitor_list;

BOOL CALLBACK EnumMonitor(HMONITOR handle, HDC hdc, LPRECT rect, LPARAM param) {
	std::vector<MONITORINFO>* list = (std::vector<MONITORINFO>*)param;
	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(handle, &mi);
	//std::cout << "is MONITORINFOF_PRIMARY:" << mi.dwFlags << std::endl;
	list->push_back(mi);
	return true;
}

void GetWorkArea()
{
	//SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);    // 获得工作区大小
	EnumDisplayMonitors(NULL, NULL, EnumMonitor, (LPARAM)&monitor_list);
	//int numbers = GetSystemMetrics(SM_CMONITORS);
}

bool InWorkArea(POINT p)
{
	for (size_t i = 0; i < monitor_list.size(); ++i) 
	{
		if (p.x > monitor_list[i].rcWork.left && p.x < monitor_list[i].rcWork.right && p.y>monitor_list[i].rcWork.top && p.y < monitor_list[i].rcWork.bottom)
		{
			return true;
		}
	}
	return false;
}

int main(int argc, char* argv[])
{
	//std::string ss = GetProgramDir();
	//std::string fileContent;
	//std::ifstream file;
	//file.open((ss + "\\address.txt").c_str(), std::ios::in);
	//getline(file, fileContent);
	//fileContent = UTF8ToGB(fileContent.c_str());;
	//file.close();

	//int i = 0;
	//int start = 0;
	//int size = fileContent.size();
	//if (size == 0)
	//	return 0;
	//for (; i < size; i++)
	//{
	//	if (fileContent[i] == '\\')
	//		start = i;
	//}
	//gameName = fileContent.substr(start + 1, size - 6 - start);

	//EnumWindows(EnumWindowsProc, (LPARAM)nullptr);						// 找到第二个WorkerW窗口(并隐藏它不隐藏了)
	//EnumWindows(EnumWindowsProc, (LPARAM)nullptr);						// 找到第二个WorkerW窗口(并隐藏它不隐藏了)
	//const char* argv1[2] = { "20188686","cloudmusic.exe" };
	hFfplay = (HWND)atoi(argv[0]); 
	RECT rect;
	GetWindowRect(hFfplay, &rect);
	int windowLeft = rect.left;
	int windowTop = rect.top;
	//int windowLeft = atoi(argv[1]);
	//int windowTop = atoi(argv[2]);
	
	//hFfplay = (HWND)0x440C7C;
	//if (argv[1] != nullptr)
	//{
	//	std::string a = argv[1];
	//	if (a == "cloudmusic.exe")
	//	{
	//		std::cout << "进入网易云模式\n";
	//		hFfplay = FindWindowExA(hFfplay, 0, "CefBrowserWindow", 0);
	//		hFfplay = FindWindowExA(hFfplay, 0, "Chrome_WidgetWin_0", 0);
	//		hFfplay = FindWindowExA(hFfplay, 0, "Chrome_RenderWidgetHostHWND", 0);
	//	}
	//}
	GetWorkArea();
	POINT lpPoint;
	//GetCursorPos(&lpPoint);
	bool ctrl = false, shift = false, lMouseButton = false, rMouseButton = false, space = false, enter = false, escape = false, up = false, left = false, right = false, down = false;//按下为true
	bool w = false, a = false, s = false, d = false;
	bool q = false, e = false, r = false;
	while (true)
	{
		GetPhysicalCursorPos(&lpPoint);
		lpPoint.x -= windowLeft;
		lpPoint.y -= windowTop;
		//std::cout << lpPoint.x << " " << lpPoint.y << " ";
		//PhysicalToLogicalPoint(hFfplay, &lpPoint);
		//ScreenToClient(hFfplay, &lpPoint);
		//MapWindowPoints((HWND)0x10010, hFfplay, &lpPoint, 1);
		//std::cout << lpPoint.x << " " << lpPoint.y << std::endl;
		SendMessage(hFfplay, WM_MOUSEMOVE, 0, MAKELPARAM(lpPoint.x, lpPoint.y));
		//为了能够与下面达成组合键，ctrl和shift在上面
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			if (!shift)
			{
				SendMessage(hFfplay, WM_KEYDOWN, VK_SHIFT, 0x002A0001);
				shift = true;
			}
		}
		else
		{
			if (shift)
			{
				SendMessage(hFfplay, WM_KEYUP, VK_SHIFT, 0xC02A0001);
				shift = false;
			}
		}

		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			if (!ctrl)
			{
				SendMessage(hFfplay, WM_KEYDOWN, VK_CONTROL, 0x001D0001);
				ctrl = true;
			}
		}
		else
		{
			if (ctrl)
			{
				SendMessage(hFfplay, WM_KEYUP, VK_CONTROL, 0xC01D0001);
				ctrl = false;
			}
		}

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			//GetCursorPos(&lpPoint);
			if (lMouseButton && InWorkArea(lpPoint))
			{
				//std::cout << lpPoint.x << " " << lpPoint.y << std::endl;

				SendMessage(hFfplay, WM_LBUTTONDOWN, 0, MAKELPARAM(lpPoint.x, lpPoint.y));
			}
			else if (!lMouseButton && InWorkArea(lpPoint))
			{
				lMouseButton = true;
				SendMessage(hFfplay, WM_LBUTTONDOWN, 0, MAKELPARAM(lpPoint.x, lpPoint.y));
				//std::cout << lpPoint.x << " " << lpPoint.y << std::endl;
			}
		}
		else
		{
			if (lMouseButton)
			{
				SendMessage(hFfplay, WM_LBUTTONUP, 0, MAKELPARAM(lpPoint.x, lpPoint.y));
				lMouseButton = false;
			}
		}

		//必须ctrl+右键
		if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			//GetCursorPos(&lpPoint);
			if (rMouseButton)
			{
				SendMessage(hFfplay, WM_RBUTTONDOWN, 0, MAKELPARAM(lpPoint.x, lpPoint.y));
			}
			else if (ctrl && !rMouseButton && InWorkArea(lpPoint))
			{
				rMouseButton = true;
			}
		}
		else
		{
			if (rMouseButton)
			{
				SendMessage(hFfplay, WM_RBUTTONUP, 0, MAKELPARAM(lpPoint.x, lpPoint.y));
				rMouseButton = false;
			}
		}

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			if (!space)
			{
				SendMessage(hFfplay, WM_KEYDOWN, VK_SPACE, 0x00390001);
				space = true;
			}
		}
		else
		{
			if (space)
			{
				SendMessage(hFfplay, WM_KEYUP, VK_SPACE, 0xC0390001);
				space = false;
			}
		}

		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			if (!up)
			{
				//SendMessage(hFfplay, WM_NCACTIVATE, 1, -1);
				PostMessage(hFfplay, WM_KEYDOWN, VK_UP, 0x000D0001);
				up = true;
			}
		}
		else
		{
			if (up)
			{
				PostMessage(hFfplay, WM_KEYUP, VK_UP, 0xC00D0001);
				up = false;
			}
		}


		if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && !down)
		{
			PostMessage(hFfplay, WM_KEYDOWN, VK_DOWN, 0x000D0001);
			down = true;
		}
		if (!(GetAsyncKeyState(VK_DOWN) & 0x8000) && down)
		{
			PostMessage(hFfplay, WM_KEYUP, VK_DOWN, 0xC00D0001);
			down = false;
		}

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			if (!left)
			{
				PostMessage(hFfplay, WM_KEYDOWN, VK_LEFT, 0x000D0001);
				left = true;
			}
		}
		else
		{
			if (left)
			{
				PostMessage(hFfplay, WM_KEYUP, VK_LEFT, 0xC00D0001);
				left = false;
			}
		}


		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			if (!right)
			{
				PostMessage(hFfplay, WM_KEYDOWN, VK_RIGHT, 0x000D0001);
				right = true;
			}
		}
		else
		{
			if (right)
			{
				PostMessage(hFfplay, WM_KEYUP, VK_RIGHT, 0xC00D0001);
				right = false;
			}
		}


		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		{
			if (!escape)
			{
				SendMessage(hFfplay, WM_KEYDOWN, VK_ESCAPE, 0x00010001);
				escape = true;
			}
		}
		else
		{
			if (escape)
			{
				SendMessage(hFfplay, WM_KEYUP, VK_ESCAPE, 0xC0010001);
				escape = false;
			}
		}

		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			if (!enter)
			{
				PostMessage(hFfplay, WM_KEYDOWN, VK_RETURN, 0x000D0001);
				enter = true;
			}
		}
		else
		{
			if (enter)
			{
				PostMessage(hFfplay, WM_KEYUP, VK_RETURN, 0xC00D0001);
				enter = false;
			}
		}

		// W
		if (GetAsyncKeyState(0x57) & 0x8000)
		{
			if (!w)
			{
				PostMessage(hFfplay, WM_KEYDOWN, 0x57, 0x00110001);
				w = true;
			}
		}
		else
		{
			if (w)
			{
				PostMessage(hFfplay, WM_KEYUP, 0x57, 0xC0110001);
				w = false;
			}
		}
		// A
		if (GetAsyncKeyState(0x41) & 0x8000)
		{
			if (!a)
			{
				PostMessage(hFfplay, WM_KEYDOWN, 0x41, 0x001E0001);
				a = true;
			}
		}
		else
		{
			if (a)
			{
				PostMessage(hFfplay, WM_KEYUP, 0x41, 0xC01E0001);
				a = false;
			}
		}
		// S
		if (GetAsyncKeyState(0x53) & 0x8000)
		{
			if (!s)
			{
				PostMessage(hFfplay, WM_KEYDOWN, 0x53, 0x001F0001);
				s = true;
			}
		}
		else
		{
			if (s)
			{
				PostMessage(hFfplay, WM_KEYUP, 0x53, 0xC01F0001);
				s = false;
			}
		}
		//D
		if (GetAsyncKeyState(0x44) & 0x8000)
		{
			if (!d)
			{
				PostMessage(hFfplay, WM_KEYDOWN, 0x44, 0x00200001);
				d = true;
			}
		}
		else
		{
			if (d)
			{
				PostMessage(hFfplay, WM_KEYUP, 0x44, 0xC0200001);
				d = false;
			}
		}

		// Q
		if (GetAsyncKeyState(0x51) & 0x8000)
		{
			if (!q)
			{
				PostMessage(hFfplay, WM_KEYDOWN, 0x51, 0x00100001);
				q = true;
			}
		}
		else
		{
			if (q)
			{
				PostMessage(hFfplay, WM_KEYUP, 0x51, 0xC0100001);
				q = false;
			}
		}
		//E
		if (GetAsyncKeyState(0x45) & 0x8000)
		{
			if (!e)
			{
				PostMessage(hFfplay, WM_KEYDOWN, 0x45, 0x00120001);
				e = true;
			}
		}
		else
		{
			if (e)
			{
				PostMessage(hFfplay, WM_KEYUP, 0x45, 0xC0120001);
				e = false;
			}
		}
		//R
		if (GetAsyncKeyState(0x52) & 0x8000)
		{
			if (!r)
			{
				PostMessage(hFfplay, WM_KEYDOWN, 0x52, 0x00130001);
				r = true;
			}
		}
		else
		{
			if (r)
			{
				PostMessage(hFfplay, WM_KEYUP, 0x52, 0xC0130001);
				r = false;
			}
		}

		Sleep(30);
	}
}