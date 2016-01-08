
#include <dbgprint.h>

#define DEVICE_NAME               L"\\Device\\DisPg"
#define DEVICE_NAME_ANSI          "\\Device\\DisPg"
#define LINK_NAME                 L"\\DosDevices\\DisPg"
#define LINK_NAME_ANSI            "\\DosDevices\\DisPg"
#define LINK_GLOBAL_NAME          L"\\DosDevices\\Global\\DisPg"
#define LINK_GLOBAL_NAME_ANSI     "\\DosDevices\\Global\\DisPg"

#define IOCTL_DUMP_PG             CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_BUFFERED,FILE_ANY_ACCESS)


void DriverUnload(PDRIVER_OBJECT pDrvObj)
{
	UNICODE_STRING strLink;
	DEBUG_PRINT("[%s]Unload\n",DEVICE_NAME_ANSI);
	RtlInitUnicodeString(&strLink,LINK_NAME);
	IoDeleteSymbolicLink(&strLink);
	IoDeleteDevice(pDrvObj->DeviceObject);
	return;
}

NTSTATUS DispatchCreate(PDEVICE_OBJECT pDrvObj,PIRP pIrp)
{
	DEBUG_PRINT("[%s]DispatchCreate\n",DEVICE_NAME_ANSI);
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS DispatchClose(PDEVICE_OBJECT pDrvObj,PIRP pIrp)
{
	DEBUG_PRINT("[%s]DispatchClose\n",DEVICE_NAME_ANSI);
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDrvObj,PIRP pIrp)
{
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
	PIO_STACK_LOCATION pIrpStack;
	ULONG ctrlcode;
	PVOID pIoBuffer;
	ULONG insize;
	ULONG outsize;

	DEBUG_PRINT("[%s]DispatchIoctl\n",DEVICE_NAME_ANSI);

	pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
	ctrlcode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	insize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	outsize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;

	switch(ctrlcode)
	{
		case IOCTL_IO_TEST:
		{
			DWORD dw=0;
			memcpy(&dw,pIoBuffer,sizeof(DWORD));
			dw ++;
			memcpy(pIoBuffer,&dw,sizeof(DWORD));
			status =STATUS_SUCCESS;
			break;
		}

		case IOCTL_SAY_HELLO:
		{
			DEBUG_PRINT("[HelloDrv]SayHello\n");
			status = STATUS_SUCCESS;
			break;
		}
		default:
			break;
	}

	if (status == STATUS_SUCCESS)
	{
		pIrp->IoStatus.Information = outsize;
	}
	else
	{
		pIrp->IoStatus.Information = 0;
	}

	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);
	return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDrvObj,PUNICODE_STRING pRegistryString)
{
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING strLink,strDevname;

	PDEVICE_OBJECT pDevObj;

	pDrvObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
	pDrvObj->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
	pDrvObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;

	pDrvObj->DriverUnload = DriverUnload;

	RtlInitUnicodeString(&strDevname,DEVICE_NAME);
	status = IoCreateDevice(pDrvObj,0,&strDevname,FILE_DEVICE_UNKNOWN,0,FALSE,&pDevObj);
	if (!NT_SUCCESS(status)) 
	{
		return status;
	}

	if (IoIsWdmVersionAvailable(1,0x10))
	{
		RtlInitUnicodeString(&strLink,LINK_GLOBAL_NAME);
	}
	else
	{
		RtlInitUnicodeString(&strLink,LINK_NAME);
	}

	status = IoCreateSymbolicLink(&strLink,&strDevname);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(pDevObj);
		return status;
	}

	DEBUG_PRINT("[HelloDrv]DriverEntry\n");
	return STATUS_SUCCESS;
}