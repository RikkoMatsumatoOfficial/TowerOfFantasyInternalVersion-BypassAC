#pragma once

#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <random>
#include <cmath>
#include <detours.h>
#include <wininet.h>
using namespace std;
namespace fs = std::filesystem;
bool IsUpdateFile(LPCWSTR path) 
{
    WCHAR fullPath[MAX_PATH];
    if (GetFullPathNameW(path, MAX_PATH, fullPath, nullptr) > 0) {
        return (wcsstr(fullPath, L"HottaUpdate") != nullptr) ||
            (wcsstr(fullPath, L"TowerOfFantasy") != nullptr) ||
            (wcsstr(fullPath, L"update") != nullptr) ||
            (wcsstr(fullPath, L"patch") != nullptr) ||
            (wcsstr(fullPath, L"\\driver\\GameDriverX64.Sys") != nullptr);
    }
    return false;
}
std::string generate_random_string(size_t length) {
    const std::string charset =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::mt19937 generator(std::time(nullptr));
    std::uniform_int_distribution<size_t> distribution(0, charset.size() - 1);

    std::string result;
    result.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        result += charset[distribution(generator)];
    }

    return result;
}

namespace TowerOfFantasy_Hooks
{
    std::filesystem::path currentPath = std::filesystem::current_path();
    static HANDLE(WINAPI* TrueCreateFileW)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE) = CreateFileW;
    static BOOL(WINAPI* TrueInternetCheckConnectionA)(LPCSTR, DWORD, DWORD) = InternetCheckConnectionA;
    static BOOL(WINAPI* TrueInternetCheckConnectionW)(LPCWSTR, DWORD, DWORD) = InternetCheckConnectionW;
    static HINTERNET(WINAPI* TrueInternetOpenW)(LPCWSTR, DWORD, LPCWSTR, LPCWSTR, DWORD) = InternetOpenW;
    BOOL WINAPI HookedInternetCheckConnectionW(LPCWSTR lpszUrl, DWORD dwFlags, DWORD dwReserved) 
    {
        cout << "Full Name Of Agent URL For Updates: " << (const char*)lpszUrl << endl;
        if (lpszUrl && wcsstr(lpszUrl, L"hotta")) 
        {
            return TRUE; // Fake successful connection
        }
        return TrueInternetCheckConnectionW(lpszUrl, dwFlags, dwReserved);
    }
    // InternetOpen hook to prevent actual connections
    HINTERNET WINAPI HookedInternetOpenW(LPCWSTR lpszAgent, DWORD dwAccessType,
        LPCWSTR lpszProxy, LPCWSTR lpszProxyBypass, DWORD dwFlags) {
        cout << "Dummy Handle: " << (const char*)lpszAgent << endl;
        // Return dummy handle for update-related connections
        if (lpszAgent && wcsstr(lpszAgent, L"Hotta")) {
            return (HINTERNET)0x87654321;
        }
        return TrueInternetOpenW(lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwFlags);
    }
    BOOL WINAPI HookedInternetCheckConnectionA(LPCSTR lpszUrl, DWORD dwFlags, DWORD dwReserved)
    {
        // Block update checks by returning TRUE (connection exists) but preventing actual checks
        return TRUE;
    }
    HANDLE WINAPI HookedCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
    {
        if (IsUpdateFile(lpFileName)) 
        {
            // For read operations, return empty file
            if ((dwDesiredAccess & GENERIC_READ) && !(dwDesiredAccess & GENERIC_WRITE)) {
                HANDLE hFile = TrueCreateFileW(
                    lpFileName,
                    GENERIC_READ,
                    FILE_SHARE_READ,
                    lpSecurityAttributes,
                    OPEN_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    hTemplateFile
                );

                if (hFile != INVALID_HANDLE_VALUE) {
                    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
                    SetEndOfFile(hFile);
                    return hFile;
                }
            }

            // For write operations, simulate success
            SetLastError(ERROR_SUCCESS);
            return (HANDLE)0x12345678; // Return non-null dummy handle
        }

        return TrueCreateFileW(lpFileName, dwDesiredAccess, dwShareMode,
            lpSecurityAttributes, dwCreationDisposition,
            dwFlagsAndAttributes, hTemplateFile);
        return TrueCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
            dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }

}
