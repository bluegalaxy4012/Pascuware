#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

namespace memory
{
	namespace driver
	{
		namespace ioctl
		{
			constexpr ULONG ATTACH_DRIVER = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x696, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
			constexpr ULONG READ_MEMORY = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x697, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
			constexpr ULONG WRITE_MEMORY = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x698, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
		}

		struct request
		{
			HANDLE process_id;
			PVOID target_address;
			PVOID buffer;

			SIZE_T size;
			SIZE_T return_length;
		};

		inline bool attach(HANDLE driver_handle, const DWORD process_id)
		{
			request req;
			req.process_id = (HANDLE)process_id;
			return DeviceIoControl(driver_handle, ioctl::ATTACH_DRIVER, &req, sizeof(req), &req, sizeof(req), nullptr, nullptr);
		}

		template <typename T> inline T read(HANDLE driver_handle, uintptr_t address)
		{
			T temp = {};
			request req;
			req.target_address = (PVOID)address;
			req.buffer = &temp;
			req.size = sizeof(T);

			DeviceIoControl(driver_handle, ioctl::READ_MEMORY, &req, sizeof(req), &req, sizeof(req), nullptr, nullptr);
			return temp;
		}


		template <typename T> inline void write(HANDLE driver_handle, uintptr_t address, T value)
		{
			request req;
			req.target_address = (PVOID)address;
			req.buffer = (PVOID)&value;
			req.size = sizeof(T);
			DeviceIoControl(driver_handle, ioctl::WRITE_MEMORY, &req, sizeof(req), &req, sizeof(req), nullptr, nullptr);
		}
	}


	inline HANDLE gHandle;
	inline uintptr_t pId;

	bool InitializeDriver(const wchar_t* processName, const wchar_t* driverName = L"\\\\.\\MyDriver");

	void CleanupDriver();

	uintptr_t GetProcessId(const wchar_t* process);
	uintptr_t GetModuleBaseAddress(uintptr_t processId, const wchar_t* module);

	template <typename T> T Read(uintptr_t address)
	{
		T buffer;
		ReadProcessMemory(gHandle, (LPCVOID)address, &buffer, sizeof(buffer), nullptr);
		return buffer;
	}


	template <typename T> void Write(uintptr_t address, T value)
	{
		WriteProcessMemory(gHandle, (LPVOID)address, &value, sizeof(value), nullptr);
	}

}