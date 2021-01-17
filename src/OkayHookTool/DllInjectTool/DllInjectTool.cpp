// DllInjectTool.cpp : Defines the entry point for the application.
//

#undef UNICODE
#undef _UNICODE

#include <Public.h>

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define EOL "\n"
#define BANNER "DllInjectTool ~ @ppshuai" EOL
#define USAGE "USAGE: DllInjectTool.exe <file.exe> <file.dll>" EOL

BOOL EnableDebugPrivilege(BOOL bEnabled = TRUE)
{
	BOOL bOK = FALSE;
	HANDLE hToken = INVALID_HANDLE_VALUE;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		TOKEN_PRIVILEGES tp = { 0 };
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = bEnabled ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		bOK = (GetLastError() == ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	return bOK;
}

START_ENTRY_BINARY()
{
	int result = 0;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (argc != 3) {
		printf(BANNER);
		printf(USAGE);
		exit(EXIT_FAILURE);
		return 0;
	}
	EnableDebugPrivilege(TRUE);
	LPCSTR APP = argv[1];
	LPCSTR DLL = argv[2];
	if (!CreateProcess(APP, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED | NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi)) {
		printf("CreateProcess failed (%d)\n", GetLastError());
		exit(EXIT_FAILURE);
	}

	LPVOID addrAlloc = VirtualAllocEx(pi.hProcess, NULL, strlen(DLL) + 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!WriteProcessMemory(pi.hProcess, addrAlloc, DLL, strlen(DLL) + 1, NULL)) {
		printf("WriteProcessMemory failed (%d)\n", GetLastError());
		exit(EXIT_FAILURE);
		return 0;
	}

	LPTHREAD_START_ROUTINE addrLoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryA");
	HANDLE RemoteFunction = NULL;
	if (!(RemoteFunction = CreateRemoteThread(pi.hProcess, NULL, 0, addrLoadLibrary, addrAlloc, 0, NULL))) {
		printf("CreateRemoteThread failed (%d)\n", GetLastError());
		exit(EXIT_FAILURE);
		return 0;
	}

	printf("[+] Start monitoring ..." EOL);

	ResumeThread(pi.hThread);
	WaitForSingleObject(RemoteFunction, INFINITE);
	VirtualFreeEx(pi.hProcess, addrAlloc, 0, MEM_RELEASE);
	CloseHandle(RemoteFunction);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	printf("[-] Clean monitoring ..." EOL);

	return result;
}