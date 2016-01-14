/*======================================================================
 Copyright (c) 2015 by Steward_Fu
 All rights reserved
 2015/05/05
======================================================================*/
#include <wdm.h>

// save next device object
PDEVICE_OBJECT gpNextObj = NULL;

#define DEBUG_PRINT(...)   do{DbgPrint("%s:%d\t",__FILE__,__LINE__);DbgPrint(__VA_ARGS__);}while(0)
#define DEVICE_NAME        L"\\Device\\WDMHello"
#define DOS_DEVICE_NAME    L"\\DosDevices\\WDMHello"
#define DEVICE_ANSI        "\\Device\\WDMHello"
/*======================================================================
description:
 call this function to add new device

parameter:
 pDrvObj: our driver object
 pPhyObj: next device object

return:
 status
======================================================================*/
NTSTATUS AddDevice(PDRIVER_OBJECT pDrvObj, PDEVICE_OBJECT pPhyObj)
{
    PDEVICE_OBJECT pFunObj = NULL;
    UNICODE_STRING usDeviceName;
    UNICODE_STRING usSymboName;

    DEBUG_PRINT("Add device %s\n",DEVICE_ANSI);

    RtlInitUnicodeString(&usDeviceName, DEVICE_NAME);
    IoCreateDevice(pDrvObj, 0, &usDeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &pFunObj);

    RtlInitUnicodeString(&usSymboName, DOS_DEVICE_NAME);
    IoCreateSymbolicLink(&usSymboName, &usDeviceName);

    gpNextObj = IoAttachDeviceToDeviceStack(pFunObj, pPhyObj);

    pFunObj->Flags &= ~DO_DEVICE_INITIALIZING;
    pFunObj->Flags |= DO_BUFFERED_IO;
    return STATUS_SUCCESS;
}

/*======================================================================
description:
 unload driver

parameter:
 pDrvObj: our driver object

return:
 none
======================================================================*/
void Unload(PDRIVER_OBJECT pDrvObj)
{
    DEBUG_PRINT("unload %s\n",DEVICE_ANSI);
    return;
}

/*======================================================================
description:
 process pnp irp

parameter:
 pDevObj: current device object
 pIrp: current irp

return:
 status
======================================================================*/
NTSTATUS IrpDispatch(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    PIO_STACK_LOCATION psk = IoGetCurrentIrpStackLocation(pIrp);
    UNICODE_STRING usSymboName;

    DEBUG_PRINT("call major function (%d) minor function (%d)\n",psk->MajorFunction,psk->MinorFunction);
    if (psk->MinorFunction == IRP_MN_REMOVE_DEVICE) {
        // free resources
        DEBUG_PRINT("unlink %s\n",DEVICE_ANSI);
        RtlInitUnicodeString(&usSymboName, DOS_DEVICE_NAME);
        IoDeleteSymbolicLink(&usSymboName);
        IoDetachDevice(gpNextObj);
        IoDeleteDevice(pDevObj);
    }

// pass irp to next
    IoSkipCurrentIrpStackLocation(pIrp);
    return IoCallDriver(gpNextObj, pIrp);
}

/*======================================================================
description:
 driver entry point

parameter:
 pDrvObj: our driver object
 pRegPath: registry path for our driver

return:
 status
======================================================================*/
NTSTATUS DriverEntry(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath)
{
    // set pnp callback function
    pDrvObj->MajorFunction[IRP_MJ_PNP] = IrpDispatch;

    // set adddevice and unload callback function
    pDrvObj->DriverExtension->AddDevice = AddDevice;
    pDrvObj->DriverUnload = Unload;
    DEBUG_PRINT("Driver Entry %s\n",DEVICE_ANSI);
    return STATUS_SUCCESS;
}