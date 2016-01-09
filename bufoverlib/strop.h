#ifndef __STR_OP_H__
#define __STR_OP_H__

#include <stdarg.h>

#ifdef __cplusplus
extern "C"{
#endif

int find_endof_inbuf(void* pbuf,int bufsize);
int vsnprintf_safe(char** ppbuf,int *pbufsize,const char* fmt,va_list ap);
int snprintf_safe(char** ppbuf,int *pbufsize,const char* fmt,...);
int append_vsnprintf_safe(char** ppbuf,int *pbufsize,const char* fmt,va_list ap);
int append_snprintf_safe(char**ppbuf,int*pbufsize,const char* fmt,...);
bool str_match_wildcard(const char* regpat,const char* str);

#ifdef __cplusplus
};
#endif


#endif /*__STR_OP_H__*/