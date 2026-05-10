#include "stdafx.h"
#include <shellapi.h>
#include <string>

// Utility to restart ModelEx in Windows with TCP support.
// Reason:  issue in asynclib - listening sockets are closed after closing app only.
// After fixing that issue - utility can be removed. (See StopSimulation command)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nShowCmd)
{
	int argc;
	LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	if (!argv || argc < 2)
	{
		const wchar_t* usage = L"Usage:  restart.exe 124 c:\\my.exe args...\n"
			"124 - pid of the process which must be stopped before start";
		MessageBoxW(0, usage, L"Usage", 0);
		return 0;
	}

	int pid = _wtoi(argv[1]);
	HANDLE hProcess = OpenProcess(SYNCHRONIZE, FALSE, pid);
	if (hProcess != NULL)
	{
		WaitForSingleObject(hProcess, INFINITE);
	}

	// Synthetic pause for sockets(listening) cleaning up.
	Sleep(300);
	std::wstring commandLine;
	for(int i=2; i< argc; i++)
	{
		commandLine += argv[i];
		commandLine += L" ";
	}

	STARTUPINFOW startupInfo = {};
	PROCESS_INFORMATION processInfo = {};
	BOOL result = CreateProcessW(0, (LPWSTR)commandLine.c_str(), 0, 0, FALSE, 0, 0, 0, &startupInfo, &processInfo);
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	return result == TRUE;
}

