﻿// DllInjectTool.cpp : Defines the entry point for the application.
//

#undef UNICODE
#undef _UNICODE

#include <Public.h>

#pragma comment(lib, "netapi32.lib")

#include <stdio.h>
#include <windows.h> 
#include <lm.h>

START_ENTRY_BINARY()
{
	int result = 0;
    USER_INFO_1 ui;
    DWORD dwLevel = 1;
    DWORD dwError = 0;
    NET_API_STATUS nStatus;

    //system("cmd /c net user test /delete");
    //
    // Set up the USER_INFO_1 structure.
    //  USER_PRIV_USER: name identifies a user, 
    //    rather than an administrator or a guest.
    //  UF_SCRIPT: required 
    //
    ui.usri1_name = L"test";
    ui.usri1_password = L"test";
    ui.usri1_priv = USER_PRIV_USER;
    ui.usri1_home_dir = NULL;
    ui.usri1_comment = NULL;
    ui.usri1_flags = UF_SCRIPT;
    ui.usri1_script_path = NULL;

    //
    // Call the NetUserAdd function, specifying level 1.
    //
    nStatus = NetUserAdd(NULL,
        dwLevel,
        (LPBYTE)&ui,
        &dwError);
    //
    // If the call succeeds, inform the user.
    //
    if (nStatus == NERR_Success)
        fwprintf(stderr, L"User %s has been successfully added on %s\n",
            argv[2], argv[1]);
    //
    // Otherwise, print the system error.
    //
    else
        fprintf(stderr, "A system error has occurred: %d\n", nStatus);

	return result;
}