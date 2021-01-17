
#define FUNC_INIT(CORE, NAME) static TYPE_##NAME* TRUE_##NAME = (TYPE_##NAME*)GetProcAddress(GetModuleHandle(_T(#CORE)), #NAME);

//#define FUNC_LOCAL_INIT(CORE, NAME) TYPE_##NAME* TRUE_##NAME = 0; { HMODULE hModule = GetModuleHandle(_T(#CORE)); if(hModule){ TRUE_##NAME = (TYPE_##NAME*)GetProcAddress(hModule, #NAME);}}

VOID * FUNC_LOCAL_INIT(_TCHAR * ptModuleName, CHAR * pFunctionName)
{
	VOID * pFunc = 0;
	HMODULE hModule = GetModuleHandle(ptModuleName);
	printf("hModule=0x%x====\n", hModule);
	if (hModule) 
	{ 
		pFunc = (void *)::GetProcAddress(hModule, pFunctionName);
		printf("pFunc=0x%x====\n", pFunc);
	}
	return pFunc;
}

BOOL ReadProcessMemoryValue(
	__in      HANDLE hProcess,
	__in      LPCVOID lpBaseAddress,
	__out_bcount_part(nSize, *lpNumberOfBytesRead) LPVOID lpBuffer,
	__in      SIZE_T nSize,
	__out_opt SIZE_T * lpNumberOfBytesRead
)
{
	return ReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
}

/*typedef VOID WINAPI TYPE_ExitProcess(
	_In_ UINT uExitCode
);
FUNC_INIT(KERNEL32, ExitProcess)
VOID WINAPI HOOK_ExitProcess(UINT uExitCode)
{
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	cp.type = ET_ARGS_PCHAR;
	cp.data = "ExitProcess";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(uExitCode);
	cpv.push_back(cp);
	log(&cpv);

	return TRUE_ExitProcess(uExitCode);
}

typedef VOID WINAPI TYPE_Sleep(
	_In_ DWORD dwMilliseconds
);
FUNC_INIT(KERNEL32, Sleep)
VOID WINAPI HOOK_Sleep(DWORD dwMilliseconds)
{
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	cp.type = ET_ARGS_PCHAR;
	cp.data = "Sleep";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(dwMilliseconds);
	cpv.push_back(cp);
	log(&cpv);

	return TRUE_Sleep(dwMilliseconds);
}

typedef FARPROC WINAPI TYPE_GetProcAddress(
	_In_ HMODULE hModule,
	_In_ LPCSTR  lpProcName
);
FUNC_INIT(KERNEL32, GetProcAddress)
FARPROC WINAPI HOOK_GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	FARPROC fResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;
	
	fResult = TRUE_GetProcAddress(hModule, lpProcName);

	cp.type = ET_ARGS_PCHAR;
	cp.data = "GetProcAddress";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = lpProcName;
	cpv.push_back(cp);
	log(&cpv);

	return fResult;
}

typedef HMODULE WINAPI TYPE_LoadLibraryA(
	_In_ LPCSTR lpFileName
);
FUNC_INIT(KERNEL32, LoadLibraryA)
HMODULE WINAPI HOOK_LoadLibraryA(LPCSTR lpFileName)
{
	HMODULE hResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;
	
	hResult = TRUE_LoadLibraryA(lpFileName);

	cp.type = ET_ARGS_PCHAR;
	cp.data = "LoadLibraryA";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = lpFileName;
	cpv.push_back(cp);
	log(&cpv);

	return hResult;
}

typedef HMODULE WINAPI TYPE_LoadLibraryW(
	_In_ LPCWSTR lpFileName
);
FUNC_INIT(KERNEL32, LoadLibraryW)
HMODULE WINAPI HOOK_LoadLibraryW(LPCWSTR lpFileName)
{
	HMODULE hResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	hResult = TRUE_LoadLibraryW(lpFileName);

	cp.type = ET_ARGS_PCHAR;
	cp.data = "LoadLibraryW";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(lpFileName);
	cpv.push_back(cp);
	log(&cpv);

	return hResult;
}

typedef LONG WINAPI TYPE_RegOpenKeyExW(
	_In_     HKEY    hKey,
	_In_opt_ LPCWSTR lpSubKey,
	_In_     DWORD   ulOptions,
	_In_     REGSAM  samDesired,
	_Out_    PHKEY   phkResult
	);
FUNC_INIT(ADVAPI32, RegOpenKeyExW)
LONG WINAPI HOOK_RegOpenKeyExW(
	_In_     HKEY    hKey,
	_In_opt_ LPCWSTR lpSubKey,
	_In_     DWORD   ulOptions,
	_In_     REGSAM  samDesired,
	_Out_    PHKEY   phkResult
)
{
	LONG lResult = 0;

	lResult = TRUE_RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);

	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;
	cp.type = ET_ARGS_PCHAR;
	cp.data = "RegOpenKeyExW";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(lpSubKey);
	cpv.push_back(cp);
	log(&cpv);

	return lResult;
}

typedef LONG WINAPI TYPE_RegQueryValueExW(
	_In_        HKEY    hKey,
	_In_opt_    LPCWSTR lpValueName,
	_Reserved_  LPDWORD lpReserved,
	_Out_opt_   LPDWORD lpType,
	_Out_opt_   LPBYTE  lpData,
	_Inout_opt_ LPDWORD lpcbData
);
FUNC_INIT(ADVAPI32, RegQueryValueExW)
LONG WINAPI HOOK_RegQueryValueExW(
	_In_        HKEY    hKey,
	_In_opt_    LPCWSTR lpValueName,
	_Reserved_  LPDWORD lpReserved,
	_Out_opt_   LPDWORD lpType,
	_Out_opt_   LPBYTE  lpData,
	_Inout_opt_ LPDWORD lpcbData)
{
	LONG lResult = 0;
	
	lResult = TRUE_RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
	
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;
	cp.type = ET_ARGS_PCHAR;
	cp.data = "RegQueryValueExW";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(lpValueName);
	cpv.push_back(cp);
	DWORD dwType = 0;
	SIZE_T stSize = 0;
	ReadProcessMemoryValue(GetCurrentProcess(), lpType, &dwType, sizeof(dwType), &stSize);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(dwType);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA((PWCHAR)lpData);
	cpv.push_back(cp);

	log(&cpv);

	return lResult;
}

typedef HINSTANCE TYPE_ShellExecuteW(
	_In_opt_ HWND    hwnd,
	_In_opt_ LPCWSTR lpOperation,
	_In_     LPCWSTR lpFile,
	_In_opt_ LPCWSTR lpParameters,
	_In_opt_ LPCWSTR lpDirectory,
	_In_     INT     nShowCmd
	);
FUNC_INIT(SHELL32, ShellExecuteW)
HINSTANCE HOOK_ShellExecuteW(
	_In_opt_ HWND    hwnd,
	_In_opt_ LPCWSTR lpOperation,
	_In_     LPCWSTR lpFile,
	_In_opt_ LPCWSTR lpParameters,
	_In_opt_ LPCWSTR lpDirectory,
	_In_     INT     nShowCmd
)
{
	HINSTANCE hInstance = NULL;

	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;
	cp.type = ET_ARGS_PCHAR;
	cp.data = "ShellExecuteW";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(lpOperation);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(lpFile);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(lpParameters);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(lpDirectory);
	cpv.push_back(cp);
	log(&cpv);

	hInstance = TRUE_ShellExecuteW(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);

	return hInstance;
}

typedef HANDLE WINAPI TYPE_CreateFileW(
	_In_     LPCWSTR               lpFileName,
	_In_     DWORD                 dwDesiredAccess,
	_In_     DWORD                 dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_     DWORD                 dwCreationDisposition,
	_In_     DWORD                 dwFlagsAndAttributes,
	_In_opt_ HANDLE                hTemplateFile
	);
FUNC_INIT(KERNEL32, CreateFileW)
HANDLE WINAPI HOOK_CreateFileW(
	_In_     LPCWSTR               lpFileName,
	_In_     DWORD                 dwDesiredAccess,
	_In_     DWORD                 dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_     DWORD                 dwCreationDisposition,
	_In_     DWORD                 dwFlagsAndAttributes,
	_In_opt_ HANDLE                hTemplateFile
)
{
	HANDLE hResult = NULL;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	cp.type = ET_ARGS_PCHAR;
	cp.data = "CreateFileW";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(lpFileName);
	cpv.push_back(cp);
	log(&cpv);

	hResult = TRUE_CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

	return hResult;
}

typedef BOOL WINAPI TYPE_WriteFile(
	_In_        HANDLE       hFile,
	_In_        LPCVOID      lpBuffer,
	_In_        DWORD        nNumberOfBytesToWrite,
	_Out_opt_   LPDWORD      lpNumberOfBytesWritten,
	_Inout_opt_ LPOVERLAPPED lpOverlapped
);
FUNC_INIT(KERNEL32, WriteFile)
BOOL WINAPI HOOK_WriteFile(
	_In_        HANDLE       hFile,
	_In_        LPCVOID      lpBuffer,
	_In_        DWORD        nNumberOfBytesToWrite,
	_Out_opt_   LPDWORD      lpNumberOfBytesWritten,
	_Inout_opt_ LPOVERLAPPED lpOverlapped
)
{
	BOOL bResult = FALSE;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	bResult = TRUE_WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);

	cp.type = ET_ARGS_PCHAR;
	cp.data = "WriteFile";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA((PWCHAR)lpBuffer);
	cpv.push_back(cp);
	log(&cpv);

	return bResult;
}

#define INTERNET_OPTION_REFRESH                 37
#define INTERNET_OPTION_PROXY                   38
#define INTERNET_OPTION_SETTINGS_CHANGED        39
typedef LPVOID HINTERNET;
typedef BOOL TYPE_InternetSetOption(
	_In_ HINTERNET hInternet,
	_In_ DWORD     dwOption,
	_In_ LPVOID    lpBuffer,
	_In_ DWORD     dwBufferLength
);
//FUNC_INIT(WININET, InternetSetOption)

#define WINHTTP_ACCESS_TYPE_DEFAULT_PROXY               0
#define WINHTTP_ACCESS_TYPE_NO_PROXY                    1
#define WINHTTP_ACCESS_TYPE_NAMED_PROXY                 3
typedef struct
{
	DWORD  dwAccessType;      // see WINHTTP_ACCESS_* types below
	LPWSTR lpszProxy;         // proxy server list
	LPWSTR lpszProxyBypass;   // proxy bypass list
}
WINHTTP_PROXY_INFO, *LPWINHTTP_PROXY_INFO;
typedef BOOL WINAPI TYPE_WinHttpSetDefaultProxyConfiguration(
	_In_ WINHTTP_PROXY_INFO *pProxyInfo
);
//FUNC_INIT(WINHTTTP, WinHttpSetDefaultProxyConfiguration)
#include <urlmon.h>
#include <wininet.h>
#pragma comment(lib, "urlmon")
typedef int WSAAPI TYPE_send(
	_In_       SOCKET s,
	_In_ const char   *buf,
	_In_       int    len,
	_In_       int    flags
	);
FUNC_INIT(WS2_32, send)
int WSAAPI HOOK_send(
	_In_       SOCKET s,
	_In_ const char   *buf,
	_In_       int    len,
	_In_       int    flags
)
{
	int nResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	printf("0x%x--\n", TRUE_send);
	{

		INTERNET_PROXY_INFO proxy;
		proxy.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
		proxy.lpszProxy = "127.0.0.1:3128";
		proxy.lpszProxyBypass = NULL;

		HRESULT hr = ::UrlMkSetSessionOption(INTERNET_OPTION_PROXY, &proxy, sizeof(proxy), 0);
		//TYPE_WinHttpSetDefaultProxyConfiguration * TRUE_WinHttpSetDefaultProxyConfiguration = (TYPE_WinHttpSetDefaultProxyConfiguration *)FUNC_LOCAL_INIT(_T("WINHTTP"), "WinHttpSetDefaultProxyConfiguration");
		//printf("0x%x--\n", TRUE_WinHttpSetDefaultProxyConfiguration);
		//TYPE_InternetSetOption * TRUE_InternetSetOption = (TYPE_InternetSetOption *)FUNC_LOCAL_INIT(_T("WININET"), "InternetSetOption");
		//printf("0x%x--\n", TRUE_InternetSetOption);

		//WINHTTP_PROXY_INFO proxyinfo = { 0 };
		//proxyinfo.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
		//proxyinfo.lpszProxy = L"http=127.0.0.1:3128;https=127.0.0.1:3128";
		//proxyinfo.lpszProxyBypass = L"";
		//bResult = ::WinHttpGetDefaultProxyConfiguration(&proxyinfo);
		//printf("0x%x===0x%x===\n", TRUE_WinHttpSetDefaultProxyConfiguration, TRUE_InternetSetOption);
		//TRUE_WinHttpSetDefaultProxyConfiguration(&proxyinfo);
		//通知注册表中代理改变,下次连接时启动代理
		//TRUE_InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
		//从注册表中读入代理
		//TRUE_InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL, 0);
	}
	nResult = TRUE_send(s, buf, len, flags);

	cp.type = ET_ARGS_PCHAR;
	cp.data = "send";
	cpv.push_back(cp);
	//log(&cpv);

	return nResult;
}
typedef int WSAAPI TYPE_recv(
	_In_       SOCKET s,
	_In_ const char   *buf,
	_In_       int    len,
	_In_       int    flags
	);
FUNC_INIT(WS2_32, recv)
int WSAAPI HOOK_recv(
	_In_       SOCKET s,
	_In_ const char   *buf,
	_In_       int    len,
	_In_       int    flags
)
{
	int nResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	nResult = TRUE_recv(s, buf, len, flags);

	cp.type = ET_ARGS_PCHAR;
	cp.data = "recv";
	cpv.push_back(cp);
	log(&cpv);

	return nResult;
}

typedef struct
{
	BOOL    fAutoDetect;
	LPWSTR  lpszAutoConfigUrl;
	LPWSTR  lpszProxy;
	LPWSTR  lpszProxyBypass;
} WINHTTP_CURRENT_USER_IE_PROXY_CONFIG;
typedef BOOL WINAPI TYPE_WinHttpGetIEProxyConfigForCurrentUser(
	_Inout_ WINHTTP_CURRENT_USER_IE_PROXY_CONFIG *pProxyConfig
	);
FUNC_INIT(WINHTTP, WinHttpGetIEProxyConfigForCurrentUser)
int WSAAPI HOOK_WinHttpGetIEProxyConfigForCurrentUser(
	_Inout_ WINHTTP_CURRENT_USER_IE_PROXY_CONFIG *pProxyConfig
)
{
	int nResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	nResult = TRUE_WinHttpGetIEProxyConfigForCurrentUser(pProxyConfig);

	cp.type = ET_ARGS_PCHAR;
	cp.data = "WinHttpGetIEProxyConfigForCurrentUser";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = pProxyConfig->fAutoDetect ? "True" : "False";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(pProxyConfig->lpszAutoConfigUrl);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(pProxyConfig->lpszProxy);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(pProxyConfig->lpszProxyBypass);
	cpv.push_back(cp);
	log(&cpv);

	return nResult;
}

typedef struct
{
	DWORD   dwFlags;
	DWORD   dwAutoDetectFlags;
	LPCWSTR lpszAutoConfigUrl;
	LPVOID  lpvReserved;
	DWORD   dwReserved;
	BOOL    fAutoLogonIfChallenged;
}
WINHTTP_AUTOPROXY_OPTIONS;
typedef BOOL WINAPI TYPE_WinHttpGetProxyForUrl(
	_In_  HINTERNET                 hSession,
	_In_  LPCWSTR                   lpcwszUrl,
	_In_  WINHTTP_AUTOPROXY_OPTIONS *pAutoProxyOptions,
	_Out_ WINHTTP_PROXY_INFO        *pProxyInfo
);
FUNC_INIT(WINHTTP, WinHttpGetProxyForUrl)
BOOL WINAPI HOOK_WinHttpGetProxyForUrl(
	_In_  HINTERNET                 hSession,
	_In_  LPCWSTR                   lpcwszUrl,
	_In_  WINHTTP_AUTOPROXY_OPTIONS *pAutoProxyOptions,
	_Out_ WINHTTP_PROXY_INFO        *pProxyInfo
)
{
	BOOL bResult = FALSE;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	bResult = TRUE_WinHttpGetProxyForUrl(hSession, lpcwszUrl, pAutoProxyOptions, pProxyInfo);
	cp.type = ET_ARGS_PCHAR;
	cp.data = "WinHttpGetProxyForUrl";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(lpcwszUrl);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA((pAutoProxyOptions && pAutoProxyOptions->lpszAutoConfigUrl) ? pAutoProxyOptions->lpszAutoConfigUrl : L"");
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(pProxyInfo->lpszProxy);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(pProxyInfo->lpszProxyBypass);
	cpv.push_back(cp);
	log(&cpv);

	return bResult;
}

typedef BOOL WINAPI TYPE_CreateProcessW(
	_In_opt_    LPCWSTR               lpApplicationName,
	_Inout_opt_ LPWSTR                lpCommandLine,
	_In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_        BOOL                  bInheritHandles,
	_In_        DWORD                 dwCreationFlags,
	_In_opt_    LPVOID                lpEnvironment,
	_In_opt_    LPCWSTR               lpCurrentDirectory,
	_In_        LPSTARTUPINFOW         lpStartupInfo,
	_Out_       LPPROCESS_INFORMATION lpProcessInformation
);
FUNC_INIT(KERNEL32, CreateProcessW)
BOOL WINAPI HOOK_CreateProcessW(
	_In_opt_    LPCWSTR               lpApplicationName,
	_Inout_opt_ LPWSTR                lpCommandLine,
	_In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_        BOOL                  bInheritHandles,
	_In_        DWORD                 dwCreationFlags,
	_In_opt_    LPVOID                lpEnvironment,
	_In_opt_    LPCWSTR               lpCurrentDirectory,
	_In_        LPSTARTUPINFOW        lpStartupInfo,
	_Out_       LPPROCESS_INFORMATION lpProcessInformation
)
{
	BOOL bResult = FALSE;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	bResult = TRUE_CreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
	cp.type = ET_ARGS_PCHAR;
	cp.data = "CreateProcessW";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(lpApplicationName ? lpApplicationName : L"");
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(lpCommandLine ? lpCommandLine : L"");
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(lpCurrentDirectory);
	cpv.push_back(cp);
	log(&cpv);

	return bResult;
}

typedef HLOCAL WINAPI TYPE_LocalAlloc(
	_In_ UINT   uFlags,
	_In_ SIZE_T uBytes
);
FUNC_INIT(KERNEL32, LocalAlloc)
HLOCAL WINAPI HOOK_LocalAlloc(
	_In_ UINT   uFlags,
	_In_ SIZE_T uBytes
)
{
	HLOCAL hResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	hResult = TRUE_LocalAlloc(uFlags, uBytes);
	cp.type = ET_ARGS_PCHAR;
	cp.data = "LocalAlloc";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(uFlags);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(uBytes);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX((unsigned long)hResult);
	cpv.push_back(cp);
	log(&cpv);

	return hResult;
}

typedef HGLOBAL WINAPI TYPE_GlobalAlloc(
	_In_ UINT   uFlags,
	_In_ SIZE_T dwBytes
);
FUNC_INIT(KERNEL32, GlobalAlloc)
HGLOBAL WINAPI HOOK_GlobalAlloc(
	_In_ UINT   uFlags,
	_In_ SIZE_T dwBytes
)
{
	HGLOBAL hResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	hResult = TRUE_LocalAlloc(uFlags, dwBytes);
	cp.type = ET_ARGS_PCHAR;
	cp.data = "GlobalAlloc";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(uFlags);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(dwBytes);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX((unsigned long)hResult);
	cpv.push_back(cp);
	log(&cpv);

	return hResult;
}

typedef BOOL WINAPI TYPE_SetWindowTextW(
	_In_     HWND    hWnd,
	_In_opt_ LPCWSTR lpString
);
FUNC_INIT(USER32, SetWindowTextW)
BOOL WINAPI HOOK_SetWindowTextW(
	_In_     HWND    hWnd,
	_In_opt_ LPCWSTR lpString
)
{
	BOOL bResult = FALSE;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	cp.type = ET_ARGS_PCHAR;
	cp.data = "SetWindowTextW";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX((unsigned long)hWnd);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(lpString);
	cpv.push_back(cp);
	log(&cpv);

	bResult = TRUE_SetWindowTextW(hWnd, lpString);

	return bResult;
}

typedef int WINAPI TYPE_GetWindowTextW(
	_In_  HWND   hWnd,
	_Out_ LPWSTR lpString,
	_In_  int    nMaxCount
);
FUNC_INIT(USER32, GetWindowTextW)
int WINAPI HOOK_GetWindowTextW(
	_In_  HWND   hWnd,
	_Out_ LPWSTR lpString,
	_In_  int    nMaxCount
)
{
	int nResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	nResult = TRUE_GetWindowTextW(hWnd, lpString, nMaxCount);

	cp.type = ET_ARGS_PCHAR;
	cp.data = "GetWindowTextW";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX((unsigned long)hWnd);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(lpString);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(nMaxCount);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(nResult);
	cpv.push_back(cp);
	log(&cpv);

	return nResult;
}

typedef LRESULT WINAPI TYPE_SendMessageW(
	_In_ HWND   hWnd,
	_In_ UINT   Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);
FUNC_INIT(USER32, SendMessageW)
LRESULT WINAPI HOOK_SendMessageW(
	_In_ HWND   hWnd,
	_In_ UINT   Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	LRESULT lResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	lResult = TRUE_SendMessageW(hWnd, Msg, wParam, lParam);

	cp.type = ET_ARGS_PCHAR;
	cp.data = "SendMessageW";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX((unsigned long)hWnd);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(Msg);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(wParam);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(lParam);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(lResult);
	cpv.push_back(cp);
	log(&cpv);

	return lResult;
}

typedef BOOL WINAPI TYPE_PostMessageW(
	_In_opt_ HWND   hWnd,
	_In_     UINT   Msg,
	_In_     WPARAM wParam,
	_In_     LPARAM lParam
);
FUNC_INIT(USER32, PostMessageW)
BOOL WINAPI HOOK_PostMessageW(
	_In_opt_ HWND   hWnd,
	_In_     UINT   Msg,
	_In_     WPARAM wParam,
	_In_     LPARAM lParam
)
{
	BOOL bResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	bResult = TRUE_PostMessageW(hWnd, Msg, wParam, lParam);
	
	cp.type = ET_ARGS_PCHAR;
	cp.data = "PostMessageW";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX((unsigned long)hWnd);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(Msg);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(wParam);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(lParam);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = STRING_FROM_HEX(bResult);
	cpv.push_back(cp);
	log(&cpv);

	return bResult;
}


typedef BOOL WINAPI TYPE_SystemParametersInfoA(
	__in UINT uiAction,
	__in UINT uiParam,
	__inout_opt PVOID pvParam,
	__in UINT fWinIni
);
FUNC_INIT(USER32, SystemParametersInfoA)
BOOL WINAPI HOOK_SystemParametersInfoA(
	__in UINT uiAction,
	__in UINT uiParam,
	__inout_opt PVOID pvParam,
	__in UINT fWinIni
)
{
	BOOL bResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	bResult = TRUE_SystemParametersInfoA(uiAction, uiParam, pvParam, fWinIni);
	if (uiAction == SPI_GETWORKAREA)
	{
		RECT* prc = (RECT*)pvParam;
		prc->right = prc->left + 1200;
		prc->bottom = prc->top + 600;
	}
	cp.type = ET_ARGS_PCHAR;
	cp.data = "SystemParametersInfoA";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(uiAction);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(uiParam);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string((unsigned long)pvParam);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(fWinIni);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(bResult);
	cpv.push_back(cp);
	log(&cpv);

	return bResult;
}

typedef BOOL WINAPI TYPE_MoveWindow(
	_In_ HWND hWnd,
	_In_ int X,
	_In_ int Y,
	_In_ int nWidth,
	_In_ int nHeight,
	_In_ BOOL bRepaint
);
FUNC_INIT(USER32, MoveWindow)
BOOL WINAPI HOOK_MoveWindow(
	_In_ HWND hWnd,
	_In_ int X,
	_In_ int Y,
	_In_ int nWidth,
	_In_ int nHeight,
	_In_ BOOL bRepaint
)
{
	BOOL bResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	TCHAR tClass[MAX_PATH];
	GetClassName(hWnd, tClass, MAX_PATH);
	if (lstrcmp(TEXT("TSDIAppForm"), tClass) == 0)
	{
		if (X == 0 && Y == 0)
		{
			X = 200;
			Y = 200;
		}
	}
	bResult = TRUE_MoveWindow(hWnd, X, Y, nWidth, nHeight, bRepaint);

	cp.type = ET_ARGS_PCHAR;
	cp.data = "MoveWindow";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string((unsigned long)hWnd);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(X);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(Y);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(nWidth);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(nHeight);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(bRepaint);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(bResult);
	cpv.push_back(cp);
	log(&cpv);

	return bResult;
}

typedef BOOL WINAPI TYPE_SetWindowPos(
	_In_ HWND hWnd,
	_In_opt_ HWND hWndInsertAfter,
	_In_ int X,
	_In_ int Y,
	_In_ int cx,
	_In_ int cy,
	_In_ UINT uFlags
);
FUNC_INIT(USER32, SetWindowPos)
BOOL WINAPI HOOK_SetWindowPos(
	_In_ HWND hWnd,
	_In_opt_ HWND hWndInsertAfter,
	_In_ int X,
	_In_ int Y,
	_In_ int cx,
	_In_ int cy,
	_In_ UINT uFlags
)
{
	BOOL bResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	TCHAR tClass[MAX_PATH];
	GetClassName(hWnd, tClass, MAX_PATH);
	if (lstrcmp(TEXT("TSDIAppForm"), tClass) == 0)
	{
		if (X == 0 && Y == 0)
		{
			X = 200;
			Y = 200;
		}
	}
	bResult = TRUE_SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);

	cp.type = ET_ARGS_PCHAR;
	cp.data = "MoveWindow";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string((unsigned long)hWnd);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(X);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(Y);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(cx);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(cy);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(uFlags);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(bResult);
	cpv.push_back(cp);
	log(&cpv);

	return bResult;
}*/

typedef struct _TYPE_USER_INFO_1 {
	LPWSTR   usri1_name;
	LPWSTR   usri1_password;
	DWORD    usri1_password_age;
	DWORD    usri1_priv;
	LPWSTR   usri1_home_dir;
	LPWSTR   usri1_comment;
	DWORD    usri1_flags;
	LPWSTR   usri1_script_path;
}TYPE_USER_INFO_1;
typedef struct _TYPE_USER_INFO_2 {
	LPWSTR usri2_name;
	LPWSTR usri2_password;
	DWORD usri2_password_age;
	DWORD usri2_priv;
	LPWSTR usri2_home_dir;
	LPWSTR usri2_comment;
	DWORD usri2_flags;
	LPWSTR usri2_script_path;
	DWORD usri2_auth_flags;
	LPWSTR usri2_full_name;
	LPWSTR usri2_usr_comment;
	LPWSTR usri2_parms;
	LPWSTR usri2_workstations;
	DWORD usri2_last_logon;
	DWORD usri2_last_logoff;
	DWORD usri2_acct_expires;
	DWORD usri2_max_storage;
	DWORD usri2_units_per_week;
	PBYTE usri2_logon_hours;
	DWORD usri2_bad_pw_count;
	DWORD usri2_num_logons;
	LPWSTR usri2_logon_server;
	DWORD usri2_country_code;
	DWORD usri2_code_page;
}TYPE_USER_INFO_2;
typedef struct _TYPE_USER_INFO_3 {
	LPWSTR usri3_name;
	LPWSTR usri3_password;
	DWORD usri3_password_age;
	DWORD usri3_priv;
	LPWSTR usri3_home_dir;
	LPWSTR usri3_comment;
	DWORD usri3_flags;
	LPWSTR usri3_script_path;
	DWORD usri3_auth_flags;
	LPWSTR usri3_full_name;
	LPWSTR usri3_usr_comment;
	LPWSTR usri3_parms;
	LPWSTR usri3_workstations;
	DWORD usri3_last_logon;
	DWORD usri3_last_logoff;
	DWORD usri3_acct_expires;
	DWORD usri3_max_storage;
	DWORD usri3_units_per_week;
	PBYTE usri3_logon_hours;
	DWORD usri3_bad_pw_count;
	DWORD usri3_num_logons;
	LPWSTR usri3_logon_server;
	DWORD usri3_country_code;
	DWORD usri3_code_page;
	DWORD usri3_user_id;
	DWORD usri3_primary_group_id;
	LPWSTR usri3_profile;
	LPWSTR usri3_home_dir_drive;
	DWORD usri3_password_expired;
}TYPE_USER_INFO_3;
typedef struct _TYPE_USER_INFO_4 {
	LPWSTR usri4_name;
	LPWSTR usri4_password;
	DWORD  usri4_password_age;
	DWORD  usri4_priv;
	LPWSTR usri4_home_dir;
	LPWSTR usri4_comment;
	DWORD  usri4_flags;
	LPWSTR usri4_script_path;
	DWORD  usri4_auth_flags;
	LPWSTR usri4_full_name;
	LPWSTR usri4_usr_comment;
	LPWSTR usri4_parms;
	LPWSTR usri4_workstations;
	DWORD  usri4_last_logon;
	DWORD  usri4_last_logoff;
	DWORD  usri4_acct_expires;
	DWORD  usri4_max_storage;
	DWORD  usri4_units_per_week;
	PBYTE  usri4_logon_hours;
	DWORD  usri4_bad_pw_count;
	DWORD  usri4_num_logons;
	LPWSTR usri4_logon_server;
	DWORD  usri4_country_code;
	DWORD  usri4_code_page;
	PSID   usri4_user_sid;
	DWORD  usri4_primary_group_id;
	LPWSTR usri4_profile;
	LPWSTR usri4_home_dir_drive;
	DWORD  usri4_password_expired;
} TYPE_USER_INFO_4;
typedef DWORD WINAPI TYPE_NetUserAdd(
	LPCWSTR servername,
	DWORD   level,
	LPBYTE  buf,
	LPDWORD parm_err
);
//FUNC_INIT(NETAPI32, NetUserAdd)
FUNC_INIT(SAMCLI, NetUserAdd)
DWORD WINAPI HOOK_NetUserAdd(
	LPCWSTR servername,
	DWORD   level,
	LPBYTE  buf,
	LPDWORD parm_err
)
{
	DWORD dwResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	dwResult = TRUE_NetUserAdd(servername, level, buf, parm_err);
	if (dwResult == 0)
	{
		cp.type = ET_ARGS_PCHAR;
		cp.data = "NetUserAdd";
		cpv.push_back(cp);
		cp.type = ET_ARGS_PCHAR;
		cp.data = "";
		if (servername != NULL)
		{
			cp.data = std::move(StringConvertUtils::Instance()->WToA(servername));
		}
		cpv.push_back(cp);
		cp.type = ET_ARGS_PCHAR;
		cp.data = std::to_string(level);
		cpv.push_back(cp);
		cp.type = ET_ARGS_PCHAR;
		cp.data = "";
		if (buf != NULL)
		{
			if (level == 1)
			{
				cp.data = StringConvertUtils::Instance()->WToA(((TYPE_USER_INFO_1*)buf)->usri1_name)
					+ ":" + StringConvertUtils::Instance()->WToA(((TYPE_USER_INFO_1*)buf)->usri1_password);
			}
			else if (level == 2)
			{
				cp.data = StringConvertUtils::Instance()->WToA(((TYPE_USER_INFO_2*)buf)->usri2_name)
					+ ":" + StringConvertUtils::Instance()->WToA(((TYPE_USER_INFO_2*)buf)->usri2_password);
			}
			else if (level == 3)
			{
				cp.data = StringConvertUtils::Instance()->WToA(((TYPE_USER_INFO_3*)buf)->usri3_name)
					+ ":" + StringConvertUtils::Instance()->WToA(((TYPE_USER_INFO_3*)buf)->usri3_password);
			}
			else if (level == 4)
			{
				cp.data = StringConvertUtils::Instance()->WToA(((TYPE_USER_INFO_4*)buf)->usri4_name)
					+ ":" + StringConvertUtils::Instance()->WToA(((TYPE_USER_INFO_4*)buf)->usri4_password);
			}
		}
		cpv.push_back(cp);
		log(&cpv);
	}

	return dwResult;
}

/*typedef PVOID 	SAM_HANDLE;
typedef PVOID* PSAM_HANDLE;
typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING* PUNICODE_STRING;
NTSTATUS NTAPI 	SamCreateUser2InDomain(
	IN SAM_HANDLE DomainHandle,
	IN PUNICODE_STRING AccountName,
	IN ULONG AccountType,
	IN ACCESS_MASK DesiredAccess,
	OUT PSAM_HANDLE UserHandle,
	OUT PULONG GrantedAccess,
	OUT PULONG RelativeId);
typedef DWORD WINAPI TYPE_SamCreateUser2InDomain(
	IN SAM_HANDLE DomainHandle,
	IN PUNICODE_STRING AccountName,
	IN ULONG AccountType,
	IN ACCESS_MASK DesiredAccess,
	OUT PSAM_HANDLE UserHandle,
	OUT PULONG GrantedAccess,
	OUT PULONG RelativeId
);
FUNC_INIT(samlib, SamCreateUser2InDomain)
DWORD WINAPI HOOK_SamCreateUser2InDomain(
	IN SAM_HANDLE DomainHandle,
	IN PUNICODE_STRING AccountName,
	IN ULONG AccountType,
	IN ACCESS_MASK DesiredAccess,
	OUT PSAM_HANDLE UserHandle,
	OUT PULONG GrantedAccess,
	OUT PULONG RelativeId
)
{
	DWORD dwResult = 0;
	COMMONPARAMSVECTOR cpv;
	CCommonParams cp;

	dwResult = TRUE_SamCreateUser2InDomain(DomainHandle, AccountName, AccountType, DesiredAccess, UserHandle, GrantedAccess, RelativeId);

	cp.type = ET_ARGS_PCHAR;
	cp.data = "SamCreateUser2InDomain";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = "";
	if (DomainHandle != NULL)
	{
		cp.data = std::to_string((ULONG_PTR)DomainHandle);
	}
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = StringConvertUtils::Instance()->WToA(std::wstring(AccountName->Buffer, AccountName->Length));
	cpv.push_back(cp);
	log(&cpv);

	return dwResult;
}*/
/*NET_API_STATUS NET_API_FUNCTION NetGroupAddUser(
	LPCWSTR servername,
	LPCWSTR GroupName,
	LPCWSTR username
);*/
/*typedef DWORD WINAPI TYPE_NetGroupUserAdd(
	LPCWSTR servername,
	LPCWSTR GroupName,
	LPCWSTR username
);
FUNC_INIT(samcli, NetGroupUserAdd)
DWORD WINAPI HOOK_NetGroupUserAdd(
	LPCWSTR servername,
	LPCWSTR GroupName,
	LPCWSTR username
)
{
	DWORD dwResult = 0;
	//COMMONPARAMSVECTOR cpv;
	//CCommonParams cp;

	dwResult = TRUE_NetGroupUserAdd(servername, GroupName, username);

	cp.type = ET_ARGS_PCHAR;
	cp.data = "NetUserAdd";
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
		cp.data = "";
	if (servername != NULL)
	{
		cp.data = std::move(StringConvertUtils::Instance()->WToA(servername));
	}
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(level);
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
		cp.data = "";
	if (buf != NULL && level == 1)
	{
		cp.data = StringConvertUtils::Instance()->WToA(((TYPE_USER_INFO_1*)buf)->usri1_name) 
			+ ":" + StringConvertUtils::Instance()->WToA(((TYPE_USER_INFO_1*)buf)->usri1_password);
	}
	cpv.push_back(cp);
	cp.type = ET_ARGS_PCHAR;
	cp.data = std::to_string(*parm_err);
	cpv.push_back(cp);
	log(&cpv);

	return dwResult;
}*/