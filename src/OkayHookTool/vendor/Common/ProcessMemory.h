// CryptTools.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>

//  Forward declarations:
__inline static
void PrintError(LPCTSTR msg)
{
    DWORD dwNum;
    TCHAR tzErrMsg[102400];
    TCHAR* p;

    dwNum = GetLastError();
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, dwNum,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        tzErrMsg, sizeof(tzErrMsg) / sizeof(tzErrMsg), NULL);

    // Trim the end of the line and terminate it with a null
    p = tzErrMsg;
    while ((*p > 31) || (*p == 9))
    {
        ++p;
    }

    do 
    {
        *p-- = 0;
    } while ((p >= tzErrMsg) && ((*p == '.') || (*p < 33)));

    // Display the message
    _tprintf(TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, dwNum, tzErrMsg);
}
__inline static
LPBYTE GetProcessModuleHandleByPid(DWORD dwPID, LPCTSTR lpModuleName = TEXT("wechatwin.dll"))
{
    LPBYTE lpBaseAddr = NULL;
    MODULEENTRY32 me32 = { 0 };
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;

    // Take a snapshot of all modules in the specified process.
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
    if (hModuleSnap != INVALID_HANDLE_VALUE)
    {
        // Set the size of the structure before using it.
        me32.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(hModuleSnap, &me32) == TRUE)
        {
            do
            {
                if (lstrcmpi(lpModuleName, me32.szExePath + lstrlen(me32.szExePath) - lstrlen(lpModuleName)) == 0)
                {
                    _tprintf(TEXT("\n     MODULE NAME:     %s"), me32.szModule);
                    _tprintf(TEXT("\n     Executable     = %s"), me32.szExePath);
                    _tprintf(TEXT("\n     Process ID     = 0x%08X"), me32.th32ProcessID);
                    _tprintf(TEXT("\n     Ref count (g)  = 0x%04X"), me32.GlblcntUsage);
                    _tprintf(TEXT("\n     Ref count (p)  = 0x%04X"), me32.ProccntUsage);
                    _tprintf(TEXT("\n     Base address   = 0x%08X"), (DWORD)me32.modBaseAddr);
                    _tprintf(TEXT("\n     Base size      = %d"), me32.modBaseSize);

                    lpBaseAddr = me32.modBaseAddr;
                    break;
                }
            } while (Module32Next(hModuleSnap, &me32) == TRUE);
        }
    }

    CloseHandle(hModuleSnap);
    return lpBaseAddr;
}
__inline static
DWORD GetProcessIdByName(LPCTSTR lpProcessName = TEXT("wechat.exe"))
{
    DWORD dwProcessID = 0;
    PROCESSENTRY32 pe32 = { 0 };
    HANDLE hProcessSnap = INVALID_HANDLE_VALUE;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap != INVALID_HANDLE_VALUE)
    {
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hProcessSnap, &pe32) == TRUE)
        {
            do
            {
                if (lstrcmpi(pe32.szExeFile, lpProcessName) == 0)
                {
                    // List the modules and threads associated with this process
                    dwProcessID = pe32.th32ProcessID;
                    break;
                }
            } while (Process32Next(hProcessSnap, &pe32));
        }
    }
    CloseHandle(hProcessSnap);
    return (dwProcessID);
}
__inline static
BOOL GetProcessMemoryBuffer(HANDLE hProcess, LPCVOID lpBaseAddr, LPVOID lpBuff, SIZE_T nSize)
{
    SIZE_T nNumberOfBytesRead = 0;
    return (ReadProcessMemory(
        hProcess,
        lpBaseAddr,
        lpBuff,
        nSize,
        &nNumberOfBytesRead) == TRUE &&
        (nNumberOfBytesRead > 0));
}

__inline static
HANDLE OpenProcessByName(LPCTSTR lpProcName = _T("wechat.exe"))
{
    PROCESSENTRY32 pe32 = { 0 };
    HANDLE hProcess = INVALID_HANDLE_VALUE; 
    HANDLE hProcessSnap = INVALID_HANDLE_VALUE;

    //打开快照
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap != INVALID_HANDLE_VALUE)
    {
        pe32.dwSize = sizeof(pe32);
        if (Process32First(hProcessSnap, &pe32) == TRUE)
        {
            while (Process32Next(hProcessSnap, &pe32) == TRUE)
            {
                if (lstrcmpi(pe32.szExeFile, lpProcName) == 0)
                {
                    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)pe32.th32ProcessID);
                    break;
                }
            }
        }
    }
    CloseHandle(hProcessSnap);
    return hProcess;
}

#include <psapi.h>
#pragma comment(lib, "psapi")
int GetProcessFilePathByPid(LPTSTR lpProcessFilePath, DWORD dwSize, DWORD processID)
{
    HMODULE hMods[1024];
    HANDLE hProcess;
    DWORD cbNeeded;

    // Print the process identifier.
    printf("\nProcess ID: %u\n", processID);

    // Get a handle to the process.
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess != NULL)
    {
        // Get a list of all the modules in this process.

        if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
        {
            /*for (auto i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
            {
                TCHAR szModName[MAX_PATH];
                // Get the full path to the module's file.
                if (GetModuleFileNameEx(hProcess, hMods[i], szModName,
                    sizeof(szModName) / sizeof(TCHAR)))
                {
                    // Print the module name and handle value.
                    _tprintf(TEXT("\t%s (0x%08X)\n"), szModName, hMods[i]);
                }
            }*/
            if ((cbNeeded / sizeof(HMODULE)) > 0)
            {
                if (GetModuleFileNameEx(hProcess, *hMods, lpProcessFilePath, dwSize))
                {
                    CloseHandle(hProcess);
                    return 0;
                }
            }
        }

        // Release the handle to the process.
        CloseHandle(hProcess);
    }
    return (-1);
}

#include <winver.h>
#pragma comment(lib, "version")
int GetFileCurrentVersion(LPTSTR lpVersion, DWORD dwVersionNum, LPCTSTR lpFileName) {
    TCHAR tMessage[MAX_PATH] = { 0 };
    DWORD fileVersionSize = GetFileVersionInfoSize(lpFileName, 0);
    TCHAR* lpData = new TCHAR[fileVersionSize];
    if (!GetFileVersionInfo(lpFileName, NULL, fileVersionSize, lpData)) {
        wsprintf(tMessage, TEXT("错误1：%d"), GetLastError());
        MessageBox(NULL, tMessage, tMessage, MB_OK);
        delete lpData;
        return (-1);
    }
    VS_FIXEDFILEINFO* versionInfo;
    UINT uLen = 0;
    if (!VerQueryValue(lpData, TEXT("\\"), (LPVOID*)&versionInfo, &uLen)) {
        wsprintf(tMessage, TEXT("错误2：%d"), GetLastError());
        MessageBox(NULL, tMessage, tMessage, MB_OK);
        delete lpData;
        return (-1);
    }
    //产品版本
    wsprintf(lpVersion, TEXT("%d.%d.%d.%d"),
        HIWORD(versionInfo->dwProductVersionMS), LOWORD(versionInfo->dwProductVersionMS),
        HIWORD(versionInfo->dwProductVersionLS), LOWORD(versionInfo->dwProductVersionLS)
    );
    //文件版本
    wsprintf(lpVersion, TEXT("%d.%d.%d.%d"),
        HIWORD(versionInfo->dwFileVersionMS), LOWORD(versionInfo->dwFileVersionMS),
        HIWORD(versionInfo->dwFileVersionLS), LOWORD(versionInfo->dwFileVersionLS)
    );
    delete lpData;
    return 0;
}

#include <windows.h>
#include <commdlg.h>
#include <commctrl.h>
#include <shellapi.h>
#include <string>
#include <vector>

#if !defined(_UNICODE) && !defined(UNICODE)
#define _TSTRING	std::string
#else
#define _TSTRING	std::wstring
#endif //!defined(_UNICODE) && !defined(UNICODE)

#define TSTRING _TSTRING
#define tstring _TSTRING

//显示在屏幕中央
__inline static void CenterWindowInScreen(HWND hWnd)
{
    RECT rcWindow = { 0 };
    RECT rcScreen = { 0 };
    SIZE szAppWnd = { 300, 160 };
    POINT ptAppWnd = { 0, 0 };

    // Get workarea rect.
    BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA,   // Get workarea information
        0,              // Not used
        &rcScreen,    // Screen rect information
        0);             // Not used

    GetWindowRect(hWnd, &rcWindow);
    szAppWnd.cx = rcWindow.right - rcWindow.left;
    szAppWnd.cy = rcWindow.bottom - rcWindow.top;

    //居中显示
    ptAppWnd.x = (rcScreen.right - rcScreen.left - szAppWnd.cx) / 2;
    ptAppWnd.y = (rcScreen.bottom - rcScreen.top - szAppWnd.cy) / 2;
    MoveWindow(hWnd, ptAppWnd.x, ptAppWnd.y, szAppWnd.cx, szAppWnd.cy, TRUE);
}

//显示在父窗口中央
__inline static void CenterWindowInParent(HWND hWnd, HWND hParentWnd)
{
    RECT rcWindow = { 0 };
    RECT rcParent = { 0 };
    SIZE szAppWnd = { 300, 160 };
    POINT ptAppWnd = { 0, 0 };

    GetWindowRect(hParentWnd, &rcParent);
    GetWindowRect(hWnd, &rcWindow);
    szAppWnd.cx = rcWindow.right - rcWindow.left;
    szAppWnd.cy = rcWindow.bottom - rcWindow.top;

    //居中显示
    ptAppWnd.x = (rcParent.right - rcParent.left - szAppWnd.cx) / 2;
    ptAppWnd.y = (rcParent.bottom - rcParent.top - szAppWnd.cy) / 2;
    MoveWindow(hWnd, ptAppWnd.x, ptAppWnd.y, szAppWnd.cx, szAppWnd.cy, TRUE);
}

__inline static BOOL SelectSaveFile(_TCHAR(&tFileName)[MAX_PATH])
{
    BOOL bResult = FALSE;
    OPENFILENAME ofn = { 0 };
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFilter = _T("Execute Files (*.EXE)\0*.EXE\0All Files (*.*)\0*.*\0\0");
    ofn.lpstrFile = tFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_ENABLEHOOK | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;
    bResult = GetSaveFileName(&ofn);
    if (bResult == FALSE)
    {
        //dwError = CommDlgExtendedError();
        //return bResult;
    }
    return bResult;
}
__inline static BOOL SelectOpenFile(_TCHAR(&tFileName)[MAX_PATH])
{
    BOOL bResult = FALSE;
    OPENFILENAME ofn = { 0 };
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFilter = _T("Execute Files (*.EXE)\0*.EXE\0All Files (*.*)\0*.*\0\0");
    ofn.lpstrFile = tFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_ENABLEHOOK | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;
    bResult = GetOpenFileName(&ofn);
    if (bResult == FALSE)
    {
        //dwError = CommDlgExtendedError();
        //return bResult;
    }
    return bResult;
}

__inline static size_t GetDropFiles(std::vector<TSTRING>& tsv, HDROP hDropInfo)
{
    UINT nIndex = 0;
    UINT nNumOfFiles = 0;
    _TCHAR tszFilePathName[MAX_PATH + 1] = { 0 };

    //得到文件个数
    nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

    for (nIndex = 0; nIndex < nNumOfFiles; nIndex++)
    {
        //得到文件名
        DragQueryFile(hDropInfo, nIndex, (LPTSTR)tszFilePathName, _MAX_PATH);
        tsv.push_back(tszFilePathName);
    }

    DragFinish(hDropInfo);

    return nNumOfFiles;
}

__inline static void RegisterDropFilesEvent(HWND hWnd)
{
#ifndef MSGFLT_ADD
#define MSGFLT_ADD 1
#endif // !MSGFLT_ADD

#ifndef WM_COPYGLOBALDATA
#define WM_COPYGLOBALDAYA	0x0049
#endif // !WM_COPYGLOBALDATA
    SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_ACCEPTFILES);
    typedef BOOL(WINAPI* PFN_ChangeWindowMessageFilter)(_In_ UINT message, _In_ DWORD dwFlag);
    HINSTANCE hInstance = GetModuleHandle(TEXT("USER32"));
    PFN_ChangeWindowMessageFilter fnChangeWindowMessageFilter = (PFN_ChangeWindowMessageFilter)GetProcAddress(hInstance, ("ChangeWindowMessageFilter"));
    if (fnChangeWindowMessageFilter != nullptr)
    {
        fnChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
        fnChangeWindowMessageFilter(WM_COPYDATA, MSGFLT_ADD);
        fnChangeWindowMessageFilter(WM_COPYGLOBALDAYA, MSGFLT_ADD);       // 0x0049 == WM_COPYGLOBALDATA
    }
}