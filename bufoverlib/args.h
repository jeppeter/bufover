#ifndef __ARGS_H__
#define __ARGS_H__


#include <tchar.h>

#ifdef __cplusplus
extern "C" {
#endif

void free_args(char*** pppargs);
char** copy_args(int argc,TCHAR *argv[]);

#ifdef __cplusplus
};
#endif


#endif /*__ARGS_H__*/