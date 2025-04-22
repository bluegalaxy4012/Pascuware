#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

namespace memory
{

	inline HANDLE gHandle;
	inline uintptr_t pId;

	uintptr_t GetProcessId(const wchar_t* process);
	uintptr_t GetModuleBaseAddress(uintptr_t processId, const wchar_t* module);

	template <typename T> T Read(uintptr_t address)
	{
		T buffer;
		ReadProcessMemory(gHandle, (LPCVOID)address, &buffer, sizeof(buffer), nullptr);
		return buffer;
	}
	
	//std::vector<char> ReadString(uintptr_t address, size_t size)
	//{
	//	std::vector<char> buffer(size);
	//	ReadProcessMemory(gHandle, (LPCVOID)address, buffer.data(), size, nullptr);
	//	return buffer;
	//}


	template <typename T> void Write(uintptr_t address, T value)
	{
		WriteProcessMemory(gHandle, (LPVOID)address, &value, sizeof(value), nullptr);
	}

}