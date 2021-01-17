// dllmain.cpp : Defines the entry point for the DLL application.

#include "header.h"
#include "signature.h"

#define ATTACH(NAME) detourMonitorAttach(#NAME, &(PVOID&)TRUE_##NAME, HOOK_##NAME);
#define DETACH(NAME) detourMonitorDetach(#NAME, &(PVOID&)TRUE_##NAME, HOOK_##NAME);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DebugPrintString("=================================================================");
		//ConsoleDebugUtils::Instance()->InitDebugConsole();
		//ATTACH(ExitProcess)
		//ATTACH(Sleep)
		//ATTACH(GetProcAddress)
		//ATTACH(LoadLibraryA)
		//ATTACH(LoadLibraryW)
		//ATTACH(RegOpenKeyExW)
		//ATTACH(RegQueryValueExW)
		//ATTACH(ShellExecuteW)
		//ATTACH(CreateFileW)
		//ATTACH(WriteFile)
		//ATTACH(send)
		//ATTACH(recv)
		//ATTACH(WinHttpGetIEProxyConfigForCurrentUser)
		//ATTACH(WinHttpGetProxyForUrl)
		//ATTACH(CreateProcessW)
		//ATTACH(LocalAlloc)
		//ATTACH(GlobalAlloc)
		//ATTACH(SetWindowTextW)
		//ATTACH(SendMessageW)
		//ATTACH(PostMessageW)
		//ATTACH(GetWindowTextW)
		//ATTACH(SystemParametersInfoA)
		//ATTACH(MoveWindow)
		//ATTACH(SetWindowPos)
		ATTACH(NetUserAdd)
		//ATTACH(NetGroupUserAdd)
		//ATTACH(SamCreateUser2InDomain)
	}
	break;
	case DLL_THREAD_ATTACH:
	{

	}
	break;
	case DLL_THREAD_DETACH:
	{

	}
	break;
	case DLL_PROCESS_DETACH:
	{
		//DETACH(ExitProcess)
		//DETACH(Sleep)
		//DETACH(GetProcAddress)
		//DETACH(LoadLibraryA)
		//DETACH(LoadLibraryW)
		//DETACH(RegOpenKeyExW)
		//DETACH(RegQueryValueExW)
		//DETACH(ShellExecuteW)
		//DETACH(CreateFileW)
		//DETACH(WriteFile)
		//DETACH(send)
		//DETACH(recv)
		//DETACH(WinHttpGetIEProxyConfigForCurrentUser)
		//DETACH(WinHttpGetProxyForUrl)
		//DETACH(CreateProcessW)
		//DETACH(LocalAlloc)
		//DETACH(GlobalAlloc)
		//DETACH(SetWindowTextW)
		//DETACH(GetWindowTextW)
		//DETACH(SendMessageW)
		//DETACH(PostMessageW)
		//DETACH(SetWindowPos)
		//DETACH(MoveWindow)
		//DETACH(SystemParametersInfoA)
		DETACH(NetUserAdd)
		//DETACH(NetGroupUserAdd)
		//DETACH(SamCreateUser2InDomain)
		//ConsoleDebugUtils::Instance()->ExitDebugConsole();
	}
		break;
	}
	return TRUE;
}

