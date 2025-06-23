#include <Windows.h>
#include <iostream>
#include <detours.h>
#include <filesystem>
using namespace std;
namespace fs = filesystem;

namespace ToF {
	const char* process = "HottaGame.exe";
}
void Inject(const char* processname, const char* dll_name) 
{
	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	fs::path sg(ToF::process);
	SetConsoleTitleA("ToF-InjectorBypassAC by RikkoMatsumatoOfficial");
	if (fs::exists(sg)) 
	{
		cout << "This Process is Exists!!! Trying Running and Inject DLL!!!" << endl;
		if (!DetourCreateProcessWithDllsA(processname, (LPSTR)"/launcher", NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi, 0, &dll_name, 0)) {
			cout << "Failed to Inject DLL!!!" << endl;
			exit(43);
		}
		else 
		{
			cout << "Success to Inject DLL!!!" << endl;
			ResumeThread(pi.hThread);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			exit(-20);
		}
	}
	else {
		cout << "Failed to Found HottaGame.exe Process" << endl;
		exit(403);
	}
}

int main() {
	if (fs::exists("TowerOfFantasyInternalVersion-BypassAC.dll")) {
		Inject(ToF::process, "TowerOfFantasyInternalVersion-BypassAC.dll");
	}
	else {
		printf("Not Founded TowerOfFantasyInternalVersion-BypassAC.dll file!!! Pls make sure what this dll is exists in same folder in HottaGame.exe!!!");
		exit(455);
	}
	return 0;
}
