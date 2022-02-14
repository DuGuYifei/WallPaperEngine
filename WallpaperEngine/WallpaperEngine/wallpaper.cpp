#include <windows.h>
//#include <stdio.h>
#include <CommDlg.h>
#include <string>
#include <tchar.h>
//#include <fstream> 
//#include <iostream>
#include <atlstr.h>
//#include <algorithm>
//#include <shellAPI.h>
//#include <WinUser.h>
//#include "resource.h"

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址

//托盘
//#define WM_IAWENTRAY    WM_USER+5
//NOTIFYICONDATA e;
//HINSTANCE g_hInst;
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//void ToTray(HWND hWnd)
//{
//	//g_hInst = (HINSTANCE)::GetWindowLong(hWnd, GWL_HINSTANCE);
//	NOTIFYICONDATA nid;
//	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
//	nid.hWnd = hWnd;
//	nid.uID = IDI_ICON1;
//	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
//	nid.uCallbackMessage = WM_IAWENTRAY;//自定义的消息 处理托盘图标事件
//	nid.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1));
//	wcscpy_s(nid.szTip, _T("自定义程序名"));//鼠标放在托盘图标上时显示的文字
//	Shell_NotifyIcon(NIM_ADD, &nid);//在托盘区添加图标
//}
//void DeleteTray(HWND hWnd)
//{
//	NOTIFYICONDATA nid;
//	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
//	nid.hWnd = hWnd;
//	nid.uID = IDI_ICON1;
//	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
//	nid.uCallbackMessage = WM_IAWENTRAY;//自定义的消息名称 处理托盘图标事件
//	nid.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1));
//	wcscpy_s(nid.szTip, _T("自定义程序名"));//鼠标放在托盘图标上时显示的文字
//	Shell_NotifyIcon(NIM_DELETE, &nid);//在托盘中删除图标
//}


BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
	if (hDefView != 0) {
		// 找它的下一个窗口，类名为WorkerW，隐藏它
		HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
		ShowWindow(hWorkerw, SW_HIDE);

		return FALSE;
	}
	return TRUE;
}

OPENFILENAME ofn;
char szFile[300];

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

//单斜杠转双斜杠
void pathConvert_Single2Double(std::string& s) {
	std::string::size_type pos = 0;
	while ((pos = s.find('\\', pos)) != std::string::npos) {
		s.insert(pos, "\\");
		pos = pos + 2;
	}
}
CString GetCorrectInput(const CString& szInput)
{
	// 为了解决传入参数中有空格的问题
	CString szDest = szInput;

	// 判断是否有空格
	if (szDest.Find(' ') < 0)
	{
		// 没有空格
		return szDest;
	}

	// 有空格,用转义符处理

	CString szSpaceFormat = _T("\"%s\"");

	szDest.Format(szSpaceFormat, szInput);
	return szDest;
}


std::string addQuate(std::string ss)
{
	std::string s = ss;
	for (int i = 0;i<s.size();i++)
	{
		if (s[i]==' ')
		{
			for (int j = i;; j--)
			{
				if (s[j] == '\\')
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
//wstring转换成string
std::string WChar2Ansi(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);

	if (nLen <= 0) return std::string("");

	char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;

	std::string strTemp(pszDst);
	delete[] pszDst;

	return strTemp;
}

std::wstring s2ws(const std::string& s)
{
	std::wstring r;
	int slength = s.length();
	if (slength > 0)
	{
		int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		r.resize(len);
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
	}
	return r;
}

// 程序开机自动启动
void autostart(LPCWSTR strRegPath)
{
	HKEY hKey;

	//1、找到系统的启动项  
	if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) ///打开启动项       
	{
		//2、得到本程序自身的全路径
		TCHAR strExeFullDir[MAX_PATH];
		GetModuleFileName(NULL, strExeFullDir, MAX_PATH);

		//3、判断注册表项是否已经存在
		TCHAR strDir[MAX_PATH] = {};
		DWORD nLength = MAX_PATH;
		long result = RegGetValue(hKey, nullptr, L"GISRestart", RRF_RT_REG_SZ, 0, strDir, &nLength);

		//4、已经存在
		if (result != ERROR_SUCCESS || _tcscmp(strExeFullDir, strDir) != 0)
		{
			//5、添加一个子Key,并设置值，"GISRestart"是应用程序名字（不加后缀.exe） 
			RegSetValueEx(hKey, L"GISRestart", 0, REG_SZ, (LPBYTE)strExeFullDir, (lstrlen(strExeFullDir) + 1) * sizeof(TCHAR));

			//6、关闭注册表
			RegCloseKey(hKey);
		}
	}
}

// 取消开机自动启动
void cancelAutoStart(LPCWSTR strRegPath)
{
	HKEY hKey;

	//1、找到系统的启动项  
	if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		//2、删除值
		RegDeleteValue(hKey, L"GISRestart");

		//3、关闭注册表
		RegCloseKey(hKey);
	}
}
LPCWSTR string2LPCWSTR(std::string str)
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

int main(int argc, char* argv[])
{
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = (LPWSTR)szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Video_picture_gif\0*.*\0常见视频格式(包括不限于)\0*.mp4;*.flv;*.mkv;*.avi;*.wmv\0常见图片格式(包括不限于)\0*.gif";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	LPCWSTR address = L" ";
	//std::wstring temp = L"";
	CString temp = L"";
	if (GetOpenFileName(&ofn))
	{
		address = ofn.lpstrFile;
		temp = GetCorrectInput(address);
		address = GetCorrectInput(address);
		//temp = WChar2Ansi(address0);
		//addQuate(temp);
		//address = s2ws(temp);
	}
	else
	{
		printf("user cancelled\n");
	}

	std::string ss = GetProgramDir();



	//写入位置
	FILE* fp = NULL;
	errno_t error;
	CT2A ascii(temp, CP_UTF8);
	//std::string add = "";
	error = fopen_s(&fp, (ss + "\\address.txt").c_str(), "w+"); //第一个逗号前是文件位置。逗号之后是打开文件方式
	fprintf(fp, ascii);  //逗号之前是一个指针，表明往里面输入。逗号之后fprintf是往文件里面输入
	//std::string a = add.assign(temp.begin(), temp.end());
	fclose(fp);  //记得用完关闭文件

	//std::ofstream file;
	//CT2A ascii(temp, CP_UTF8);
	//file.open("demo.txt", std::ios::out);
	//file << (LPCTSTR)temp;


	pathConvert_Single2Double(ss);
	//std::cout << ss << std::endl;
	if (MessageBox(NULL, string2LPCWSTR("是否开机自启动(不想每次都开机设置桌面就选确定)"), string2LPCWSTR("WallpaperEngine"), MB_OKCANCEL) == 1)
	{
		system((addQuate((ss + "\\WallpaperEngineAutoStartUpCmd.bat")).c_str()));
	}
	else
	{
		//cancelAutoStart(string2LPCWSTR(ss + "\\\\WallpaperEngineStart.exe"));
	}
	
	ss.append("\\\\ffplay.exe ");
	//std::cout << ss <<std::endl;


	//LPCWSTR ffplayaddress = (LPCWSTR)std::wstring(ss.begin(), ss.end()).c_str();


	// 视频路径、1920和1080，要根据实际情况改。建议使用GetSystemMetrics函数获取分辨率属性
	LPCWSTR lpParameter = L" ";
	LPCWSTR lpParameter2 = L" -noborder -an -x 1920 -y 1080 -loop 0";
	std::wstring medium = std::wstring(lpParameter) + address + lpParameter2;

	lpParameter = medium.c_str();
	
	STARTUPINFO si{ 0 };
	//si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi{ 0 };

	//托盘
	
	//先关掉ffplay
	system("taskkill /IM ffplay.exe");

	// 运行开始
	if (CreateProcess(s2ws(ss).c_str(), (LPTSTR)lpParameter, 0, 0, 0, 0, 0, 0, &si, &pi))
	//if (CreateProcess(L"E:\\SmallGame\\WallPaperEngine\\WallpaperEngine\\Debug\\ffmpeg\\bin\\ffplay.exe", (LPTSTR)lpParameter, 0, 0, 0, 0, 0, 0, &si, &pi))
	{
		//如果你的视频不能全屏，调长这个sleep时间
		Sleep(1000);												// 等待视频播放器启动完成。可用循环获取窗口尺寸来代替Sleep()

		HWND hProgman = FindWindow(L"Progman", 0);				// 找到PM窗口
		SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);	// 给它发特殊消息
		HWND hFfplay = FindWindow(L"SDL_app", 0);				// 找到视频窗口
		SetParent(hFfplay, hProgman);							// 将视频窗口设置为PM的子窗口
		EnumWindows(EnumWindowsProc, 0);						// 找到第二个WorkerW窗口并隐藏它
		SetWindowPos(hFfplay, HWND_TOP, 0, 0, 1920, 1080, SWP_SHOWWINDOW);
		
		//ss = ss.substr(0, ss.length() - 13);
		//ss.append("\\\\WallpaperEngine.exe");
		//std::cout << ss << std::endl;

		HWND hwnd = FindWindow(L"ConsoleWindowClass",
			//s2ws(ss).c_str()
			0
		);
		//ShowWindow(hwnd,SW_HIDE);
		ShowWindow(hwnd, SW_HIDE);
		//ShowWindow(hwnd, SW_MINIMIZE);
		//SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);

		//g_hInst = (HINSTANCE)::GetWindowLong(hwnd, GWL_HINSTANCE);

		//static TCHAR szAppName[] = TEXT("WallpaperEngine.exe");
		////HWND         hwnd;
		//MSG          msg;
		//WNDCLASS     wndclass;

		//wndclass.style = CS_HREDRAW | CS_VREDRAW;
		//wndclass.lpfnWndProc = WndProc;
		//wndclass.cbClsExtra = 0;
		//wndclass.cbWndExtra = 0;
		//wndclass.hInstance = g_hInst;
		//wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		//wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		//wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		//wndclass.lpszMenuName = NULL;
		//wndclass.lpszClassName = szAppName;

		//if (!RegisterClass(&wndclass))
		//{

		//	MessageBox(NULL, TEXT("This program requires Windows NT!"),
		//		szAppName, MB_ICONERROR);
		//	return 0;
		//}

		/*e.cbSize = sizeof(NOTIFYICONDATA);
		e.hWnd = hwnd;
		e.uID = IDI_ICON1;
		e.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		e.uCallbackMessage = WM_IAWENTRAY;
		e.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON1)); 
		wcscpy_s(e.szTip, _T("WallpaperEngine正在工作！\r\n点击打开主界面"));*/
			
		//Shell_NotifyIcon(NIM_ADD, &e);
	}


	return 0;
}

//LRESULT CALLBACK WndProc(HWND hwndsmall, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//	HDC hdc;
//	
//	switch (message)
//	{
//	case WM_SIZE:
//		if (wParam == SIZE_MINIMIZED)
//		{
//			ToTray(hwndsmall);
//			ShowWindow(hwndsmall, SW_HIDE);
//		}
//		break;
//	case WM_DESTROY:
//		DeleteTray(hwndsmall);
//		PostQuitMessage(0);
//		break;
//	case WM_IAWENTRAY:
//		if (lParam == WM_LBUTTONDBLCLK)
//		{
//			//鼠标双击出现
//			SetForegroundWindow(hwndsmall);
//			ShowWindow(hwndsmall, SW_SHOWNORMAL);
//		}
//		break;
//
//	}
//	return DefWindowProc(hwndsmall, message, wParam, lParam);
//}
////{
////
////	HDC         hdc;
////	PAINTSTRUCT ps;
////	RECT        rect;
////
////	switch (message)
////	{
////
////	case WM_CREATE:
////		return 0;
////
////	case WM_PAINT:
////		hdc = BeginPaint(hwndsmall, &ps);
////
////		GetClientRect(hwndsmall, &rect);
////
////		DrawText(hdc, TEXT("Hello, my fridents！"), -1, &rect,
////			DT_SINGLELINE | DT_CENTER | DT_VCENTER);
////
////		EndPaint(hwndsmall, &ps);
////		return 0;
////
////	case WM_IAWENTRAY:
////		if (wParam == IDI_ICON1) {
////
////			if (lParam == WM_LBUTTONDOWN) {
////
////				//还可以在这个地方加入一个菜单，鼠标右键单击时显示  
////				//ShowWindow(hwnd, SW_SHOWNORMAL);
////				ShowWindow(hwndsmall, SW_SHOWNORMAL);
////				return TRUE;
////			}
////		}
////		return FALSE;
////	case WM_SYSCOMMAND:
////		switch (wParam)
////		{
////
////		case SC_CLOSE:
////			DestroyWindow(hwndsmall);
////			PostQuitMessage(0);
////			break;
////		case SC_MINIMIZE:
////			ShowWindow(hwndsmall, SW_HIDE);
////			Shell_NotifyIcon(NIM_ADD, &e); //最小化时隐藏窗口并设置系统托盘  
////			break;
////		}
////		return 0;
////
////	case WM_DESTROY:
////		Shell_NotifyIcon(NIM_DELETE, &e);
////		PostQuitMessage(0);
////		return 0;
////	}
////	return DefWindowProc(hwndsmall, message, wParam, lParam);
////
////}