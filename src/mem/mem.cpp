#include "mem.h"

uintptr_t memory::GetProcessId(const wchar_t* process)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	Process32First(snapshot, &entry);


	do {
		if (!wcscmp(entry.szExeFile, process))
		{	
			CloseHandle(snapshot);
			pId = entry.th32ProcessID;
			gHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, pId);	
			return entry.th32ProcessID;
		}
	} while (Process32Next(snapshot, &entry));

	CloseHandle(snapshot);
	return 0;
}



uintptr_t memory::GetModuleBaseAddress(uintptr_t processId, const wchar_t* module)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
	MODULEENTRY32 entry;
	entry.dwSize = sizeof(MODULEENTRY32);
	Module32First(snapshot, &entry);


	do {
		if (!wcscmp(entry.szModule, module))
		{
			CloseHandle(snapshot);
			return (uintptr_t)entry.modBaseAddr;
		}
	} while (Module32Next(snapshot, &entry));
	CloseHandle(snapshot);
	return 0;
}