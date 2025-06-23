// dllmain.cpp : Defines the entry point for the DLL application.
#include "Includes.h"
using namespace TowerOfFantasy_Hooks;
void AllocateConsole() {
    FILE* fp;
    AllocConsole();
    freopen_s(&fp, "CONOUT$", "w", stdout);
    std::string rand = generate_random_string(8);
    SetConsoleTitleA(rand.c_str());
}

void DetHack_GameDriver() 
{
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TowerOfFantasy_Hooks::TrueCreateFileW, TowerOfFantasy_Hooks::HookedCreateFileW);
    DetourTransactionCommit();
}

DWORD WINAPI Unhook_GameDriver() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)TowerOfFantasy_Hooks::TrueCreateFileW, TowerOfFantasy_Hooks::HookedCreateFileW);
    DetourTransactionCommit();
    return TRUE;
}
DWORD WINAPI ThreadMain() 
{
    AllocateConsole();
    DetHack_GameDriver();
    return TRUE;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadMain, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Unhook_GameDriver, 0, 0, 0);
        break;
    }
    return TRUE;
}

