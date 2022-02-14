#include <windows.h>
#include <stdio.h>
#include <CommDlg.h>
#include <string>
#include <tchar.h>
#include <fstream>

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // ������ڵ�ַ

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
	std::string ss = GetProgramDir();
	pathConvert_Single2Double(ss);

	//�����ļ�
	LPCWSTR address = L" ";
	std::string temp;
	std::ifstream file;
	file.open(ss + "\\address.txt", std::ios::in);
	getline(file, temp);
	temp = UTF8ToGB(temp.c_str());
	if (temp == "")
		return 0;
	file.close();

	//����
	ss.append("\\\\ffplay.exe ");
	address = string2LPCWSTR(temp);


	LPCWSTR ffplayaddress = (LPCWSTR)std::wstring(ss.begin(), ss.end()).c_str();


	// ��Ƶ·����1920��1080��Ҫ����ʵ������ġ�����ʹ��GetSystemMetrics������ȡ�ֱ�������
	LPCWSTR lpParameter = L" ";
	LPCWSTR lpParameter2 = L" -noborder -an -x 1920 -y 1080 -loop 0";
	std::wstring medium = std::wstring(lpParameter) + address + lpParameter2;

	lpParameter = medium.c_str();

	STARTUPINFO si{ 0 };
	//si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi{ 0 };

	//�ȹص�ffplay
	system("taskkill /IM ffplay.exe");

	// ���п�ʼ
	if (CreateProcess(s2ws(ss).c_str(), (LPTSTR)lpParameter, 0, 0, 0, 0, 0, 0, &si, &pi))
	{
		//��������Ƶ����ȫ�����������sleepʱ��
		Sleep(1000);											// �ȴ���Ƶ������������ɡ�����ѭ����ȡ���ڳߴ�������Sleep()

		HWND hProgman = FindWindow(L"Progman", 0);				// �ҵ�PM����
		SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);	// ������������Ϣ
		HWND hFfplay = FindWindow(L"SDL_app", 0);				// �ҵ���Ƶ����
		SetParent(hFfplay, hProgman);							// ����Ƶ��������ΪPM���Ӵ���
		EnumWindows(EnumWindowsProc, 0);						// �ҵ��ڶ���WorkerW���ڲ�������
		SetWindowPos(hFfplay, HWND_TOP, 0, 0, 1920, 1080, SWP_SHOWWINDOW);

		HWND hwnd = FindWindow(L"ConsoleWindowClass", 0);
		ShowWindow(hwnd, SW_HIDE);
		//ShowWindow(hwnd, SW_MINIMIZE);

	}
	return 0;
}