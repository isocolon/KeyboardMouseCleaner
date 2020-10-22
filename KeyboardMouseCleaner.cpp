#include <Windows.h>
#include <shellapi.h>
#include <stdlib.h>     // _wtoi

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")      // BlockInput
#pragma comment(lib, "shell32.lib")     // CommandLineToArgvW
#pragma comment(lib, "ucrt.lib")        // _wtoi

HANDLE g_hStdout = NULL;

void Print(LPCWSTR message)
{
    DWORD dwWritten = 0;
    if (g_hStdout)
    {
        WriteConsoleW(
            g_hStdout, 
            (LPCVOID)message, 
            lstrlenW(message), 
            &dwWritten, 
            NULL
            );    
    }
}

void main(LPVOID pPeb)
{
    int nArgs;
    g_hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    Print(L"[*] KeyboardMouseCleaner v1.0\r\n");

    LPWSTR *szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);

    if (NULL == szArglist || nArgs != 2)
    {
        LPCWSTR szUsage = L"[-] Usage: KeyboardMouseCleaner.exe <TIMEOUT>\r\n"
                          L"    Timeout is in seconds.\r\n"
                          L"    Example: KeyboardMouseCleaner.exe 60\r\n";

        Print(szUsage);
        ExitProcess(1);
    }

    DWORD dwTimeout = 0;
    dwTimeout = _wcstol_l(szArglist[1], NULL, 10, LC_NONE);

    if (!BlockInput(TRUE))
    {
        Print(L"[-] Call of BlockInput() to BLOCK input failed!\n");
        ExitProcess(1);
    }

    Print(L"[*] Blocking input now...\n");

    Sleep(dwTimeout * 1000);

    Print(L"[*] Unblocking input now...\n");

    if (!BlockInput(FALSE))
    {
        Print(L"[-] Call of BlockInput() to UNBLOCK input failed!\n");
        ExitProcess(1);
    }

    LocalFree(szArglist);

    Print(L"[*] Cleaning up ... done!\n");

    ExitProcess(0);
}