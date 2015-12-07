#include <stdio.h>
#include <windows.h>

#define CTL_CODE_GEN(func)  ((FILE_DEVICE_UNKNOWN << 16) | (FILE_ANY_ACCESS << 14)| (func << 2))
#define IOCTL_IO_TEST       CTL_CODE_GEN(0x800)
#define IOCTL_SAY_HELLO     CTL_CODE_GEN(0x801)


#ifdef _UNICODE
#define HELLO_DRV_FILE L"\\\\.\\HelloDrv"
#else
#define HELLO_DRV_FILE "\\\\.\\HelloDrv"
#endif


#define HELLO_DRV_ANSI "\\\\.\\HelloDrv"

#define GETERRNO() ((int)(GetLastError() != 0 ? -(int)GetLastError() : -1))

int main(int argc, char* argv[])
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    int ret;
    unsigned int inbuf[100];
    unsigned int outbuf[100];
    int dval=100,doutval;
    DWORD dret;
    BOOL bret;

    hFile = CreateFile(HELLO_DRV_FILE, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile == INVALID_HANDLE_VALUE) {
        ret = GETERRNO();
        fprintf(stderr, "open %s file error(%d)\n", HELLO_DRV_ANSI, ret);
        goto out;
    }

    memcpy(inbuf,&dval,sizeof(dval));
    bret = DeviceIoControl(hFile,IOCTL_IO_TEST,inbuf,sizeof(dval),outbuf,sizeof(outbuf),&dret,NULL);
    if (!bret){
    	ret = GETERRNO();
    	fprintf(stderr,"iotest error (%d)\n",ret);
    	goto out;
    }

    memcpy(&doutval,outbuf,sizeof(doutval));
    fprintf(stdout,"write (%d) get (%d) for iotest\n",dval,doutval);

    bret = DeviceIoControl(hFile,IOCTL_SAY_HELLO,NULL,0,NULL,0,&dret,NULL);
    if (!bret){
    	ret = GETERRNO();
    	fprintf(stderr,"can not call say hello error(%d)\n",ret);
    	goto out;
    }
    fprintf(stdout,"test all success\n");

    ret = 0;
out:
	if (hFile!= INVALID_HANDLE_VALUE){
		CloseHandle(hFile);
	}
	hFile = INVALID_HANDLE_VALUE;

    return ret;
}