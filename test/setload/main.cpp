#include <stdio.h>
#include <WINDOWS.h>
#include <args.h>
#include <err.h>
#include <uniansi.h>
#include <output_debug.h>
#include <priv.h>
#include <strop.h>


typedef long NTSTATUS;

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#define NT_INFORMATION(Status) ((((ULONG)(Status)) >> 30) == 1)
#define NT_WARNING(Status) ((((ULONG)(Status)) >> 30) == 2)
#define NT_ERROR(Status) ((((ULONG)(Status)) >> 30) == 3)

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING;

typedef VOID (_stdcall *RtlInitUnicodeStringFunc)( IN OUT UNICODE_STRING *DestinationString,
                                       IN PCWSTR SourceString );

typedef NTSTATUS (_stdcall *ZwSetSystemInformationFunc)( IN DWORD functionCode,
        IN OUT PVOID driverName,
        IN LONG driverNameLength );


typedef struct _DRIVER_NAME {
    UNICODE_STRING name;
} DRIVER_NAME;

#define LOAD_DRIVER_IMAGE_CODE 38

NTSTATUS loadDriver(PCWSTR binaryPath)
{
    DRIVER_NAME DriverName;
    const char dllName[] = "ntdll.dll";
    HANDLE ntHandle = NULL;
    RtlInitUnicodeStringFunc RtlInitUnicodeString=NULL;
    ZwSetSystemInformationFunc ZwSetSystemInformation=NULL;


    ntHandle = (void *) GetModuleHandle(dllName);

    if (ntHandle == NULL) {
        ERROR_INFO("can not get dll handle\n");
        return -1;
    }


    RtlInitUnicodeString = (RtlInitUnicodeStringFunc) GetProcAddress ( GetModuleHandle(dllName),
                           "RtlInitUnicodeString" );

    ZwSetSystemInformation = (ZwSetSystemInformationFunc)GetProcAddress (GetModuleHandle(dllName),
                             "ZwSetSystemInformation" );

    printf("RtlInitUnicodeString == 0x%p\n", RtlInitUnicodeString);
    printf("ZwSetSystemInformation == 0x%p\n", ZwSetSystemInformation);

    if (RtlInitUnicodeString == NULL) {
    	ERROR_INFO("can not get RtlInitUnicodeString\n");
        return -1;
    }

    RtlInitUnicodeString(&(DriverName).name, binaryPath);

    if (ZwSetSystemInformation == NULL) {
    	ERROR_INFO("can not get ZwSetSystemInformation\n");
        return -1;
    }

    return ZwSetSystemInformation( LOAD_DRIVER_IMAGE_CODE,
                                   &DriverName,
                                   sizeof(DRIVER_NAME) );
}


int _tmain(int argc,TCHAR* argv[])
{

    //WCHAR binaryPath[] = L"C:\\srv3.sys";
    wchar_t *binaryPath=NULL;
    int binarysize=0;
    char *ptname=NULL;
    int tnamesize=0;
    NTSTATUS status;
    DWORD s = 0;
    char** args=NULL;
    int ret;
    HANDLE hToken=NULL;
    int enabled=0;

    if (argc <2){
    	ret = -1;
    	fprintf(stderr,"setload sysfile\n");
    	goto out;
    }

    args = copy_args(argc,argv);
    if (args == NULL){
    	GETERRNO(ret);
    	goto out;
    }

    ret = snprintf_safe(&ptname,&tnamesize,"\\??\\%s",args[1]);
    if (ret < 0){
    	GETERRNO(ret);
    	fprintf(stderr, "can not change (%s) error(%d)\n", args[1],ret);
    	goto out;
    }

    ret = AnsiToUnicode(ptname,&binaryPath,&binarysize);
    if (ret < 0){
    	GETERRNO(ret);
    	fprintf(stderr,"can not change file (%s) error(%d)\n",args[1],ret);
    	goto out;
    }

    ret = EnableLoadDriver(&hToken);
    if (ret < 0){
    	GETERRNO(ret);
    	fprintf(stderr,"can not enable load driver priv error(%d)\n",ret);
    	goto out;
    }
    enabled = 1;

    status = loadDriver(binaryPath);
    if (!NT_SUCCESS(status)){
    	GETERRNO(ret);
    	fprintf(stderr,"can not load (%s) error(0x%x:%d)\n",ptname,status,status);
    	goto out;
    }

    fprintf(stdout,"load (%s) succ\n",args[1]);
    ret = 0;
out:
	if (enabled){
		DisableLoadDriver(&hToken);
	}
	enabled = 0;
	snprintf_safe(&ptname,&tnamesize,NULL);
	AnsiToUnicode(NULL,&binaryPath,&binarysize);
	free_args(&args);
    return ret;
}