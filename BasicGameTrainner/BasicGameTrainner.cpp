// My First attempt at making a game trainner
//

#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"

int main()
{
    //get ProcId of target process
	DWORD procId = GetProcId(L"ac_client.exe");

	//get module base address of target process
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

	//get handle to target process
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
	
	//resolve base address of the pointer chain
	uintptr_t dynamicBase = moduleBase + ; //insert offset here

	std::cout << "dynamicBase = " << "0x" << std::hex << dynamicBase << std::endl;
	
	//resovle our ammo address
	std::vector<unsigned int> ammoOffsets = { 0x364,0x14,0x0 }; //insert offsets here
	uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicBase, ammoOffsets);

	std::cout << "ammoAddr = " << "0x" << std::hex << ammoAddr << std::endl;
	
	//read ammo value
	int ammoValue = 0;

	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
	std::cout <<"Current ammo = " << std::dec << ammoValue << std::endl;
	
	//write ammo value
	int newAmmo = 1337;
	WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

	
	//write to it
	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
	std::cout <<"Current ammo = " << std::dec << ammoValue << std::endl;

	getchar();
	return 0;
}
