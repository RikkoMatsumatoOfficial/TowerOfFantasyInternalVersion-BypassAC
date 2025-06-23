#pragma once

#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <random>
#include <cmath>
#include <detours.h>
#include <wininet.h>
#include <winhttp.h>
#include <ntstatus.h>
#include <string>
#include <fstream>
using namespace std;
namespace fs = std::filesystem;
#include <mutex>

std::mutex logMutex;

void LogToFile(const std::wstring& message) {
    std::lock_guard<std::mutex> lock(logMutex);  // References global mutex

    std::wofstream logFile(L"ServerMonitor.log", std::ios_base::app);
    if (logFile.is_open()) {
        SYSTEMTIME st;
        GetLocalTime(&st);
        logFile << L"[" << st.wYear << L"-" << st.wMonth << L"-" << st.wDay
            << L" " << st.wHour << L":" << st.wMinute << L":" << st.wSecond
            << L"] " << message << std::endl;
    }
}
// Helper function to check if process is HottaUpdate
bool IsHottaUpdateProcess(LPCWSTR lpApplicationName, LPWSTR lpCommandLine) {
    std::wstring target;
    if (lpApplicationName) target = lpApplicationName;
    else if (lpCommandLine) {
        target = lpCommandLine;
        if (target[0] == L'"') {
            size_t end = target.find(L'"', 1);
            if (end != std::wstring::npos) target = target.substr(1, end - 1);
        }
        else {
            size_t space = target.find(L' ');
            if (space != std::wstring::npos) target = target.substr(0, space);
        }
    }
    std::transform(target.begin(), target.end(), target.begin(), ::towlower);
    return target.find(L"HottaUpdate") != std::wstring::npos;
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
    // Pointer to original CreateFileW
    static HANDLE(WINAPI* TrueCreateFileW)(
        LPCWSTR lpFileName,
        DWORD dwDesiredAccess,
        DWORD dwShareMode,
        LPSECURITY_ATTRIBUTES lpSecurityAttributes,
        DWORD dwCreationDisposition,
        DWORD dwFlagsAndAttributes,
        HANDLE hTemplateFile
        ) = CreateFileW;
    HANDLE WINAPI HookedCreateFileW(
        LPCWSTR lpFileName,
        DWORD dwDesiredAccess,
        DWORD dwShareMode,
        LPSECURITY_ATTRIBUTES lpSecurityAttributes,
        DWORD dwCreationDisposition,
        DWORD dwFlagsAndAttributes,
        HANDLE hTemplateFile
    ) 
    {
        // Check if the file path contains GameDriverX64.sys
        if (lpFileName) {
            std::wstring filename(lpFileName);

            // Case-insensitive search for the target filename
            std::wstring lowerFilename = filename;
            std::transform(lowerFilename.begin(), lowerFilename.end(), lowerFilename.begin(), ::tolower);

            if (lowerFilename.find(L"driver\\GameDriverX64.Sys") != std::wstring::npos) {
                // Block the creation/opening
                SetLastError(ERROR_ACCESS_DENIED);
                return INVALID_HANDLE_VALUE;
            }
        }

        // Call original function for all other files
        return TrueCreateFileW(
            lpFileName,
            dwDesiredAccess,
            dwShareMode,
            lpSecurityAttributes,
            dwCreationDisposition,
            dwFlagsAndAttributes,
            hTemplateFile
        );
    }


}
    
