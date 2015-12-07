
#include <windows.h>
#include "instdrv.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>


void Usage(int ec,const char* fmt,...)
{
	FILE *fp=stderr;

	if (ec == 0)
	{
		fp = stdout;
	}

	if (fmt != NULL)
	{
		va_list ap;
		va_start(ap,fmt);
		vfprintf(fp,fmt,ap);
		fprintf(fp,"\n");
	}

	fprintf(fp,"instdrv [COMMANDS]\n");
	fprintf(fp,"\thelp       to display this\n");
	fprintf(fp,"\tinstall svcname drvbin description\n");
	fprintf(fp,"\tuninstall svcname\n");
	fprintf(fp,"\tstart svcname\n");
	fprintf(fp,"\tstop svcname\n");
	exit(ec);

}

int main(int argc,char* argv[])
{
	int ret;
	InstDrv *pDrv=NULL;
	if (argc < 2) 
	{
		Usage(3,"need args");
	}

	if (strcmp(argv[1],"help") == 0)
	{
		Usage(0,NULL);
	}
	else if (strcmp(argv[1],"install") == 0)
	{
		if (argc < 4)
		{
			Usage(3,"install need at least 4 args");
		}
		pDrv = new InstDrv(argv[2]);
		ret = pDrv->SetDrvBin(argv[3]);
		if (ret < 0)
		{
			fprintf(stderr,"can not set drv bin (%s) error (%d)\n",argv[3],ret);
			goto out;
		}

		if (argc > 4)
		{
			ret = pDrv->RegisterDrv(argv[4]);
		}
		else
		{
			ret = pDrv->RegisterDrv(NULL);
		}
		if (ret < 0)
		{
			fprintf(stderr,"can not register (%s) for (%s) desc(%s) error(%d)\n",argv[2],argv[3],argc > 4 ? argv[4] : "NULL",ret);
			goto out;
		}
		fprintf(stdout,"install %s success\n",argv[2]);
		ret = 0;

	}
	else if (strcmp(argv[1],"uninstall") == 0)
	{
		if (argc < 3)
		{
			Usage(3,"uninstall need svcname");
		}
		pDrv = new InstDrv(argv[2]);
		ret = pDrv->UnregisterDrv();
		if (ret < 0)
		{
			fprintf(stderr,"uninstall %s error(%d)\n",argv[2],ret);
			goto out;
		}
		fprintf(stdout,"uninstall %s succ\n",argv[2]);
		ret = 0;
	}
	else if (strcmp(argv[1],"start") == 0)
	{
		if (argc < 3)
		{
			Usage(3,"start need svcname");
		}
		pDrv = new InstDrv(argv[2]);
		ret = pDrv->RunDrv();
		if (ret < 0)
		{
			fprintf(stderr,"start %s error(%d)\n",argv[2],ret);
			goto out;
		}
		fprintf(stdout,"start %s succ\n",argv[2]);
		ret = 0;
	}
	else if (strcmp(argv[1],"stop") == 0)
	{
		if (argc < 3)
		{
			Usage(3,"stop need svcname");
		}
		pDrv = new InstDrv(argv[2]);
		ret = pDrv->StopDrv();
		if (ret < 0)
		{
			fprintf(stderr,"stop %s error(%d)\n",argv[2],ret);
			goto out;
		}
		fprintf(stdout,"stop %s succ\n",argv[2]);
		ret = 0;
	}
	else
	{
		Usage(3,"unrecognize arg (%s)",argv[1]);
	}

out:
	if (pDrv)
	{
		delete pDrv;
	}
	pDrv= NULL;
	return ret;

}