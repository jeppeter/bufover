
#include <args.h>
#include <uniansi.h>
#include <stdlib.h>
#include <err.h>
#include <output_debug.h>

void free_args(char*** pppargs)
{
	int i;
	char **ppargs;
	int lensize=0;
	if (pppargs == NULL){
		return;
	}

	ppargs = *pppargs;
	if (ppargs == NULL){
		return ;
	}
	for (i=0;ppargs[i];i++){
		lensize = (int)strlen(ppargs[i]);
		lensize ++;
		TcharToAnsi(NULL,&(ppargs[i]),&lensize);
	}

	free(ppargs);
	*pppargs = NULL;
	return;
}

char** copy_args(int argc,TCHAR *argv[])
{
	int i;
	int ret;
	char** ppargs=NULL;
	int argssize=0;
	int cursize=0;

	/*we include the end to indicate*/
	argssize = sizeof(ppargs[0]) * (argc+1);

	ppargs = (char**) malloc(argssize);
	if (ppargs == NULL){
		GETERRNO(ret);
		ERROR_INFO("can not alloc(%d) error(%d)\n",argssize,ret);
		goto fail;
	}
	memset(ppargs,0,argssize);

	for (i=0;i<argc;i++){
		cursize = 0;
		ret = TcharToAnsi(argv[i],&(ppargs[i]),&cursize);
		if (ret < 0){			
			ERROR_INFO("can not convert[%d] args error(%d)\n",i,ret);
			goto fail;
		}
	}

	return ppargs;
fail:
	free_args(&ppargs);
	SETERRNO(-ret);
	return NULL;
}
