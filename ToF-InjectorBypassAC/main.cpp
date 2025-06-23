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
	if (fs::exists(sg)) 
	{
		cout << "This Process is Exists!!! Trying Running and Inject DLL!!!" << endl;
		if (!DetourCreateProcessWithDllsA(processname, (LPSTR)"/launcher", NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi, 0, (LPCSTR*)dll_name, 0)) {
			cout << "Failed to Inject DLL!!!" << endl;
			exit(43);
		}
		else 
		{
			cout << "Success to Inject DLL!!!" << endl;
			exit(-20)
		}
	}
	else {
		cout << "Failed to Found HottaGame.exe Process" << endl;
		exit(403);
	}
}

int main() {
	Inject(ToF::process, "TowerOfFantasyInternalVersion-BypassAC.dll");
	return 0;
}
