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

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // ������ڵ�ַ

//����
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
//	nid.uCallbackMessage = WM_IAWENTRAY;//�Զ������Ϣ ��������ͼ���¼�
//	nid.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1));
//	wcscpy_s(nid.szTip, _T("�Զ��������"));//����������ͼ����ʱ��ʾ������
//	Shell_NotifyIcon(NIM_ADD, &nid);//�����������ͼ��
//}
//void DeleteTray(HWND hWnd)
//{
//	NOTIFYICONDATA nid;
//	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
//	nid.hWnd = hWnd;
//	nid.uID = IDI_ICON1;
//	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
//	nid.uCallbackMessage = WM_IAWENTRAY;//�Զ������Ϣ���� ��������ͼ���¼�
//	nid.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1));
//	wcscpy_s(nid.szTip, _T("�Զ��������"));//����������ͼ����ʱ��ʾ������
//	Shell_NotifyIcon(NIM_DELETE, &nid);//��������ɾ��ͼ��
//}


BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
	if (hDefView != 0) {
		// ��������һ�����ڣ�����ΪWorkerW��������
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

//��б��ת˫б��
void pathConvert_Single2Double(std::string& s) {
	std::string::size_type pos = 0;
	while ((pos = s.find('\\', pos)) != std::string::npos) {
		s.insert(pos, "\\");
		pos = pos + 2;
	}
}
CString GetCorrectInput(const CString& szInput)
{
	// Ϊ�˽������������пո������
	CString szDest = szInput;

	// �ж��Ƿ��пո�
	if (szDest.Find(' ') < 0)
	{
		// û�пո�
		return szDest;
	}

	// �пո�,��ת�������

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
//wstringת����string
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

// ���򿪻��Զ�����
void autostart(LPCWSTR strRegPath)
{
	HKEY hKey;

	//1���ҵ�ϵͳ��������  
	if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) ///��������       
	{
		//2���õ������������ȫ·��
		TCHAR strExeFullDir[MAX_PATH];
		GetModuleFileName(NULL, strExeFullDir, MAX_PATH);

		//3���ж�ע������Ƿ��Ѿ�����
		TCHAR strDir[MAX_PATH] = {};
		DWORD nLength = MAX_PATH;
		long result = RegGetValue(hKey, nullptr, L"GISRestart", RRF_RT_REG_SZ, 0, strDir, &nLength);

		//4���Ѿ�����
		if (result != ERROR_SUCCESS || _tcscmp(strExeFullDir, strDir) != 0)
		{
			//5�����һ����Key,������ֵ��"GISRestart"��Ӧ�ó������֣����Ӻ�׺.exe�� 
			RegSetValueEx(hKey, L"GISRestart", 0, REG_SZ, (LPBYTE)strExeFullDir, (lstrlen(strExeFullDir) + 1) * sizeof(TCHAR));

			//6���ر�ע���
			RegCloseKey(hKey);
		}
	}
}

// ȡ�������Զ�����
void cancelAutoStart(LPCWSTR strRegPath)
{
	HKEY hKey;

	//1���ҵ�ϵͳ��������  
	if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		//2��ɾ��ֵ
		RegDeleteValue(hKey, L"GISRestart");

		//3���ر�ע���
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
	ofn.lpstrFilter = L"Video_picture_gif\0*.*\0������Ƶ��ʽ(����������)\0*.mp4;*.flv;*.mkv;*.avi;*.wmv\0����ͼƬ��ʽ(����������)\0*.gif";
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



	//д��λ��
	FILE* fp = NULL;
	errno_t error;
	CT2A ascii(temp, CP_UTF8);
	//std::string add = "";
	error = fopen_s(&fp, (ss + "\\address.txt").c_str(), "w+"); //��һ������ǰ���ļ�λ�á�����֮���Ǵ��ļ���ʽ
	fprintf(fp, ascii);  //����֮ǰ��һ��ָ�룬�������������롣����֮��fprintf�����ļ���������
	//std::string a = add.assign(temp.begin(), temp.end());
	fclose(fp);  //�ǵ�����ر��ļ�

	//std::ofstream file;
	//CT2A ascii(temp, CP_UTF8);
	//file.open("demo.txt", std::ios::out);
	//file << (LPCTSTR)temp;


	pathConvert_Single2Double(ss);
	//std::cout << ss << std::endl;
	if (MessageBox(NULL, string2LPCWSTR("�Ƿ񿪻�������(����ÿ�ζ��������������ѡȷ��)"), string2LPCWSTR("WallpaperEngine"), MB_OKCANCEL) == 1)
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


	// ��Ƶ·����1920��1080��Ҫ����ʵ������ġ�����ʹ��GetSystemMetrics������ȡ�ֱ�������
	LPCWSTR lpParameter = L" ";
	LPCWSTR lpParameter2 = L" -noborder -an -x 1920 -y 1080 -loop 0";
	std::wstring medium = std::wstring(lpParameter) + address + lpParameter2;

	lpParameter = medium.c_str();
	
	STARTUPINFO si{ 0 };
	//si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi{ 0 };

	//����
	
	//�ȹص�ffplay
	system("taskkill /IM ffplay.exe");

	// ���п�ʼ
	if (CreateProcess(s2ws(ss).c_str(), (LPTSTR)lpParameter, 0, 0, 0, 0, 0, 0, &si, &pi))
	//if (CreateProcess(L"E:\\SmallGame\\WallPaperEngine\\WallpaperEngine\\Debug\\ffmpeg\\bin\\ffplay.exe", (LPTSTR)lpParameter, 0, 0, 0, 0, 0, 0, &si, &pi))
	{
		//��������Ƶ����ȫ�����������sleepʱ��
		Sleep(1000);												// �ȴ���Ƶ������������ɡ�����ѭ����ȡ���ڳߴ�������Sleep()

		HWND hProgman = FindWindow(L"Progman", 0);				// �ҵ�PM����
		SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);	// ������������Ϣ
		HWND hFfplay = FindWindow(L"SDL_app", 0);				// �ҵ���Ƶ����
		SetParent(hFfplay, hProgman);							// ����Ƶ��������ΪPM���Ӵ���
		EnumWindows(EnumWindowsProc, 0);						// �ҵ��ڶ���WorkerW���ڲ�������
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
		wcscpy_s(e.szTip, _T("WallpaperEngine���ڹ�����\r\n�����������"));*/
			
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
//			//���˫������
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
////		DrawText(hdc, TEXT("Hello, my fridents��"), -1, &rect,
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
////				//������������ط�����һ���˵�������Ҽ�����ʱ��ʾ  
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
////			Shell_NotifyIcon(NIM_ADD, &e); //��С��ʱ���ش��ڲ�����ϵͳ����  
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