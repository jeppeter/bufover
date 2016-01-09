#include <args.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <err.h>


#define CTL_CODE_GEN(func)  ((FILE_DEVICE_UNKNOWN << 16) | (FILE_ANY_ACCESS << 14)| (func << 2))
#define IOCTL_DUMP_PG       CTL_CODE_GEN(0x800)


#ifdef _UNICODE
#define DISPG_DRV_FILE L"\\\\.\\DisPg"
#else
#define DISPG_DRV_FILE "\\\\.\\DisPg"
#endif


#define DISPG_DRV_ANSI "\\\\.\\DisPg"


void Usage(int ec, const char* fmt, ...)
{
    FILE* fp = stderr;
    va_list ap;

    if (ec == 0) {
        fp = stdout;
    }

    if (fmt != NULL) {
        va_start(ap, fmt);
        vfprintf(fp, fmt, ap);
        fprintf(fp, "\n");
    }

    fprintf(fp, "dispgcmd [OPTIONS] [CMD] : to handle options\n");
    fprintf(fp, "\t-h|--help                to display this help information\n");
    fprintf(fp, "\tdump                     to dump the patchguard ,please see windbg or dbgview for it\n");
    exit(ec);
}


typedef struct __options{
    char* m_cmd;
} options_t,*poptions_t;

int parse_param(int argc,char* argv[],poptions_t popt)
{
    int i;
    int idx=-1;

    for (i=1;i<argc;i++){
        if (strcmp(argv[i],"-h") == 0 ||
            strcmp(argv[i],"--help") == 0){
            Usage(0,NULL);
        }else if (strcmp(argv[i],"dump")==0){
            popt->m_cmd = argv[i];
            idx = i + 1;
            break;
        }else {
            Usage(3,"unrecognize %s cmd",argv[i]);
        }
    }

    return idx;
}

void CloseDisPg(HANDLE *phFile)
{
    BOOL bret;
    int ret;
    if (phFile == NULL || *phFile == INVALID_HANDLE_VALUE){
        return;
    }

    bret = CloseHandle(*phFile);
    if (!bret){
        GETERRNO(ret);
        fprintf(stderr,"can not close handle 0x%p error(%d)\n",*phFile,ret);
    }
    *phFile = INVALID_HANDLE_VALUE;
    return;
}

int OpenDisPg(HANDLE *phFile)
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    int ret;

    if (phFile == NULL || *phFile != INVALID_HANDLE_VALUE){
        ret = -ERROR_INVALID_PARAMETER;
        SETERRNO(-ret);
        return ret;
    }

    hFile = CreateFile(DISPG_DRV_FILE,GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile == INVALID_HANDLE_VALUE){
        GETERRNO(ret);
        fprintf(stderr,"can not open(%s) error(%d)\n",DISPG_DRV_ANSI,ret);
        goto fail;
    }
    *phFile = hFile;
    return 0;
fail:
    CloseHandle(&hFile);
    SETERRNO(-ret);
    return ret;
}

int dump_pg(int argc,char* argv[],int idx,poptions_t popt)
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    int ret;
    BOOL bret;
    DWORD dret;

    ret = OpenDisPg(&hFile);
    if (ret < 0){
        goto out;
    }

    bret = DeviceIoControl(hFile,IOCTL_DUMP_PG,NULL,0,NULL,0,&dret,NULL);
    if (!bret){
        GETERRNO(ret);
        fprintf(stderr,"can not dump pg error(%d)\n",ret);
        goto out;
    }
    ret = 0;
    fprintf(stdout,"dump pg succ\n");

out:
    CloseDisPg(&hFile);
    return ret;
}



int _tmain(int argc, TCHAR* argv[])
{
    int ret;
    char** args=NULL;
    int idx;
    options_t opt;
    memset(&opt,0,sizeof(opt));

    args = copy_args(argc,argv);
    if (args == NULL){
        GETERRNO(ret);
        fprintf(stderr,"can not copy args error(%d)\n",ret);
        goto out;
    }
    ret = parse_param(argc,args,&opt);
    if (ret < 0){
        fprintf(stderr,"can not parse args error(%d)\n",ret);
        goto out;
    }
    idx = ret;
    if (strcmp(opt.m_cmd,"dump") == 0 ){
        ret = dump_pg(argc,args,idx,&opt);        
    }

    if (ret < 0){
        goto out;
    }
    ret = 0;

out:
    free_args(&args);
    return ret;
}