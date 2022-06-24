#include "proc.h"

DWORD GetProcId(const wchar_t* procName)
{
	//For error checking
	DWORD procId = 0;
	
	//get a snapshot of the process, then store in hsnap
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	//Error checking
	if (hSnap != INVALID_HANDLE_VALUE) 
	{
		//gets the name of the exe
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);
		
		//grabs the process in the backround then stores it in procEntry
		if (Process32First(hSnap, &procEntry)) {
			do {
				if (!_wcsicmp(procEntry.szExeFile, procName)) {
					procId = procEntry.th32ProcessID;
					break;
				}
			//will loop though all the process in the background	
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	//when process is found it will close out the handle, will stop memory leaks
	CloseHandle(hSnap);
	return procId;
}

//make sure you complie the game to match the game archtecure
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* moduleName)
{
	uintptr_t modBaseAddr = 0;
	
	//will get 32 or 64 bit process
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	
	//does basicly the same as the func above this one
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		
		if (Module32First(hSnap, &modEntry))
		{
			do 
			{
				if (!_wcsicmp(modEntry.szModule, moduleName))
				{
					modBaseAddr = (uintptr_t)modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

//take a handle to a process, will take a vector of offsets and will return the address of the offset
uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	//dynmaticly get the offsets and then store it in addr
	for (unsigned int i = 0; i < offsets.size(); i++)
	{
		ReadProcessMemory(hProc, (LPCVOID)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	
	return addr;
}
