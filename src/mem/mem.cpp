#include "mem.h"

bool memory::InitializeDriver(const wchar_t* processName, const wchar_t* driverName)
{
	pId = memory::GetProcessId(processName);
	if (pId == 0)
	{
		return false;
	}
	gHandle = CreateFile(driverName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (gHandle == INVALID_HANDLE_VALUE)
	{
		return false;
		

	}
	if (!memory::driver::attach(gHandle, pId))
	{
		CloseHandle(gHandle);
		gHandle = nullptr;
		return false;
	}
	return true;
}

void memory::CleanupDriver()
{
	if (gHandle && gHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(gHandle);
		gHandle = nullptr;
	}
	pId = 0;
}

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