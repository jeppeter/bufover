#include <ntddk.h>
#include <windef.h>
#include <stdlib.h>


#define DEVICE_NAME L"\\Device\\DisPg"
#define LINK_NAME L"\\DosDevices\\DisPg"
#define LINK_GLOBAL_NAME L"\\DosDevices\\Global\\DisPg"

#define DEBUG_PRINT(...)   do{DbgPrint("%s:%d\t",__FILE__,__LINE__);DbgPrint(__VA_ARGS__);}while(0)

