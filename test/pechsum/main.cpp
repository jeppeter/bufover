#include <args.h>
#include <stdio.h>
#include <err.h>

#pragma warning(disable:4996)


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

    fprintf(fp, "pechsum file... to check file\n");
    exit(ec);
}

int pe_check(const char* file, unsigned short *chksum)
{
    unsigned long sum = 0;
    unsigned char* pbuf = NULL;
    unsigned short* pcurptr = NULL;
    unsigned char* pptr = NULL;
    int bufsize = 0;
    int leftsize = 0;
    FILE* fp = NULL;
    int ret;

    fp = fopen(file, "r+b");
    if (fp == NULL) {
        GETERRNO(ret);
        fprintf(stderr, "can not open(%s) error(%d)\n", file, ret);
        goto out;
    }

    ret = fseek(fp, 0, SEEK_END);
    if (ret != 0 ) {
        GETERRNO(ret);
        fprintf(stderr, "fseek (%s) error(%d)\n", file, ret);
        goto out;
    }

    bufsize = ftell(fp);
    if (bufsize == -1) {
        GETERRNO(ret);
        fprintf(stderr, "can not tell (%s) error(%d)\n", file, ret);
        goto out;
    }

    fclose(fp);
    fp = NULL;
    fp = fopen(file, "r+b");
    if (fp == NULL) {
        GETERRNO(ret);
        fprintf(stderr, "can not open(%s) error(%d)\n", file, ret);
        goto out;
    }

    pbuf = (unsigned char*)malloc(bufsize);
    if (pbuf == NULL) {
        GETERRNO(ret);
        fprintf(stderr, "can not alloc(%d) error(%d)\n", bufsize, ret);
        goto out;
    }

    ret = (int)fread(pbuf, bufsize, 1, fp);
    if (ret != 1) {
        GETERRNO(ret);
        fprintf(stderr, "can not read (%s) bufsize(%d) error(%d)\n", file, bufsize, ret);
        goto out;
    }

    /*now we should check sum*/
    leftsize = bufsize;
    pcurptr = (unsigned short*) pbuf;
    while (leftsize > 1) {
        sum += *pcurptr;
        leftsize -= 2;
        pcurptr ++;
    }

    if (leftsize > 0) {
        pptr = (unsigned char*)pcurptr;
        sum += *pptr;
    }

    while ((sum >> 16)) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    *chksum = (unsigned short)~sum;
    ret = 0;



out:
    if (pbuf) {
        free(pbuf);
    }
    pbuf = NULL;
    bufsize = 0;
    if (fp) {
        fclose(fp);
    }
    fp = NULL;
    SETERRNO(-ret);
    return ret;
}


int _tmain(int argc, TCHAR* argv[])
{
    char** args = NULL;
    int ret;
    int i;
    unsigned short chksum=0;
    if (argc < 2){
    	Usage(3,"need one file");
    }

    args = copy_args(argc, argv);
    if (args == NULL) {
        GETERRNO(ret);
        fprintf(stderr, "can not copy args error(%d)\n", ret);
        goto out;
    }

    for (i=1;i<argc;i++){
    	ret = pe_check(args[i],&chksum);
    	if (ret < 0){
    		fprintf(stderr,"can not check %s\n",args[i]);
    		goto out;
    	}
    	fprintf(stdout,"%-30s %08d:0x%04x\n",args[i],chksum,chksum);
    }

    ret = 0;


out:
    free_args(&args);
    return ret;
}
