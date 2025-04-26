#include <ntifs.h>


extern "C" {
	NTKERNELAPI NTSTATUS IoCreateDriver(
		PUNICODE_STRING DriverName,
		PDRIVER_INITIALIZE InitializationFunction
	);

	NTKERNELAPI NTSTATUS MmCopyVirtualMemory(
		PEPROCESS SourceProcess,
		PVOID SourceAddress,
		PEPROCESS TargetProcess,
		PVOID TargetAddress,
		SIZE_T BufferSize,
		KPROCESSOR_MODE PreviousMode,
		PSIZE_T ReturnLength
	);
}

void printdbg(PCSTR text)
{
#ifndef DEBUG
	UNREFERENCED_PARAMETER(text);
#endif

	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, text));
}

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

	NTSTATUS create(PDEVICE_OBJECT device_object, PIRP irp)
	{
		UNREFERENCED_PARAMETER(device_object);

		IoCompleteRequest(irp, IO_NO_INCREMENT);
		return irp->IoStatus.Status;
	}

	NTSTATUS close(PDEVICE_OBJECT device_object, PIRP irp)
	{
		UNREFERENCED_PARAMETER(device_object);
		IoCompleteRequest(irp, IO_NO_INCREMENT);
		return irp->IoStatus.Status;
	}

	NTSTATUS control(PDEVICE_OBJECT device_object, PIRP irp)
	{
		UNREFERENCED_PARAMETER(device_object);
		IO_STACK_LOCATION* stack = IoGetCurrentIrpStackLocation(irp);
		ULONG control_code = stack->Parameters.DeviceIoControl.IoControlCode;

		NTSTATUS status = STATUS_UNSUCCESSFUL;
		request* req = (request*)irp->AssociatedIrp.SystemBuffer;

		if (stack == nullptr || req == nullptr)
		{
			printdbg("invalid request\n");
			IoCompleteRequest(irp, IO_NO_INCREMENT);
			return status;
		}

		static PEPROCESS target_process = nullptr;

		switch (control_code)
		{
		case ioctl::ATTACH_DRIVER:
			//printdbg("ATTACH_DRIVER\n");
			status = PsLookupProcessByProcessId(req->process_id, &target_process);
			break;
		case ioctl::READ_MEMORY:
			if (target_process != nullptr)
				status = MmCopyVirtualMemory(
					target_process,
					req->target_address,
					PsGetCurrentProcess(),
					req->buffer,
					req->size,
					KernelMode,
					&req->return_length
				);

			//printdbg("READ_MEMORY\n");
			break;
		case ioctl::WRITE_MEMORY:
			if (target_process != nullptr)
				status = MmCopyVirtualMemory(
					PsGetCurrentProcess(),
					req->buffer,
					target_process,
					req->target_address,
					req->size,
					KernelMode,
					&req->return_length
				);

			//printdbg("WRITE_MEMORY\n");
			break;
		default:
			//printdbg("unknown IOCTL\n");
			break;
		}

		irp->IoStatus.Status = status;
		irp->IoStatus.Information = sizeof(request);
		IoCompleteRequest(irp, IO_NO_INCREMENT);
		return irp->IoStatus.Status;
	}


}

NTSTATUS driver_entry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path)
{
	printdbg("driver entry\n");
	UNREFERENCED_PARAMETER(registry_path);

	UNICODE_STRING device_name = RTL_CONSTANT_STRING(L"\\Device\\MyDriver");

	PDEVICE_OBJECT device_object = nullptr;
	NTSTATUS status = IoCreateDevice(driver_object, 0, &device_name, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &device_object);

	if (!NT_SUCCESS(status))
	{
		printdbg("failed creating device\n");
		return status;
	}

	UNICODE_STRING symbolic_link_name = RTL_CONSTANT_STRING(L"\\DosDevices\\MyDriver");
	status = IoCreateSymbolicLink(&symbolic_link_name, &device_name);

	if (!NT_SUCCESS(status))
	{
		printdbg("failed creating symbolic link\n");
		IoDeleteDevice(device_object);
		return status;
	}

	SetFlag(device_object->Flags, DO_BUFFERED_IO);

	driver_object->MajorFunction[IRP_MJ_CREATE] = driver::create;
	driver_object->MajorFunction[IRP_MJ_CLOSE] = driver::close;
	driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = driver::control;

	ClearFlag(device_object->Flags, DO_DEVICE_INITIALIZING);

	printdbg("driver initialized\n");
	return status;
}

NTSTATUS DriverEntry()
{
	printdbg("entry point\n");

	UNICODE_STRING driver_name = RTL_CONSTANT_STRING(L"\\Driver\\MyDriver");


	return IoCreateDriver(&driver_name, &driver_entry);
}


