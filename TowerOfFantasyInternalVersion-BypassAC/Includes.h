#pragma once

#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <random>
#include <cmath>
#include <detours.h>
using namespace std;
namespace fs = std::filesystem;

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

    HANDLE WINAPI HookedCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) 
    {
        std::string current_driver_path = currentPath.string() + "\\driver\\GameDriverX64.Sys";
        // Check if the file is part of the update process
        if (wcsstr(lpFileName, (const wchar_t*)current_driver_path.c_str()))
        {
            // Return invalid handle to block file operations
            return INVALID_HANDLE_VALUE;
        }

        // Otherwise proceed normally
        return TrueCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
            dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }

}