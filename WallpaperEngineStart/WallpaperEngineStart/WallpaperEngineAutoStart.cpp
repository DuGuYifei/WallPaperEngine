#include <windows.h>
#include <stdio.h>
#include <CommDlg.h>
#include <string>
#include <tchar.h>
#include <fstream>

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址

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

	//读入文件
	LPCWSTR address = L" ";
	std::string temp;
	std::ifstream file;
	file.open(ss + "\\address.txt", std::ios::in);
	getline(file, temp);
	temp = UTF8ToGB(temp.c_str());
	if (temp == "")
		return 0;
	file.close();

	//参数
	ss.append("\\\\ffplay.exe ");
	address = string2LPCWSTR(temp);


	LPCWSTR ffplayaddress = (LPCWSTR)std::wstring(ss.begin(), ss.end()).c_str();


	// 视频路径、1920和1080，要根据实际情况改。建议使用GetSystemMetrics函数获取分辨率属性
	LPCWSTR lpParameter = L" ";
	LPCWSTR lpParameter2 = L" -noborder -an -x 1920 -y 1080 -loop 0";
	std::wstring medium = std::wstring(lpParameter) + address + lpParameter2;

	lpParameter = medium.c_str();

	STARTUPINFO si{ 0 };
	//si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi{ 0 };

	//先关掉ffplay
	system("taskkill /IM ffplay.exe");

	// 运行开始
	if (CreateProcess(s2ws(ss).c_str(), (LPTSTR)lpParameter, 0, 0, 0, 0, 0, 0, &si, &pi))
	{
		//如果你的视频不能全屏，调长这个sleep时间
		Sleep(1000);											// 等待视频播放器启动完成。可用循环获取窗口尺寸来代替Sleep()

		HWND hProgman = FindWindow(L"Progman", 0);				// 找到PM窗口
		SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);	// 给它发特殊消息
		HWND hFfplay = FindWindow(L"SDL_app", 0);				// 找到视频窗口
		SetParent(hFfplay, hProgman);							// 将视频窗口设置为PM的子窗口
		EnumWindows(EnumWindowsProc, 0);						// 找到第二个WorkerW窗口并隐藏它
		SetWindowPos(hFfplay, HWND_TOP, 0, 0, 1920, 1080, SWP_SHOWWINDOW);

		HWND hwnd = FindWindow(L"ConsoleWindowClass", 0);
		ShowWindow(hwnd, SW_HIDE);
		//ShowWindow(hwnd, SW_MINIMIZE);

	}
	return 0;
}