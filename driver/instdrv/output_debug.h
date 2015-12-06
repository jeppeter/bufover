#ifndef  __OUTPUT_DEBUG_H__
#define  __OUTPUT_DEBUG_H__

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>



#ifdef INJECTBASE_EXPORTS
#ifndef INJECTBASE_API
#define   INJECTBASE_API  extern "C"  __declspec(dllexport)
#endif
#else   /*INJECTBASE_EXPORTS*/
#ifndef INJECTBASE_API
#define   INJECTBASE_API  extern "C"  __declspec(dllimport)
#endif
#endif   /*INJECTBASE_EXPORTS*/



extern "C" void DebugOutString(const char* file,int lineno,const char* fmt,...);
//INJECTBASE_API void DebugBuffer(const char* file,int lineno,unsigned char* pBuffer,int buflen);
extern "C" void DebugBufferFmt(const char* file,int lineno,unsigned char* pBuffer,int buflen,const char* fmt,...);





#define WIN7_VER  1

#ifdef WIN7_VER
#define  DEBUG_INFO(fmt,...) DebugOutString(__FILE__,__LINE__,fmt,__VA_ARGS__)
#define  ERROR_INFO(fmt,...) DebugOutString(__FILE__,__LINE__,fmt,__VA_ARGS__)
#else
#ifdef WINXP_VER
#define  DEBUG_INFO(fmt,...) do{fprintf(stderr,fmt,__VA_ARGS__);DebugOutString(__FILE__,__LINE__,fmt,__VA_ARGS__);}while(0)
#define  ERROR_INFO(fmt,...) do{fprintf(stderr,fmt,__VA_ARGS__);DebugOutString(__FILE__,__LINE__,fmt,__VA_ARGS__);}while(0)
#else
#define  DEBUG_INFO(fmt,...) do{;}while(0)
#define  ERROR_INFO(fmt,...) do{;}while(0)
#endif
#endif
#define  DEBUG_BUFFER(ptr,blen) DebugBufferFmt(__FILE__,__LINE__,(unsigned char*)ptr,blen,NULL)
#define  DEBUG_BUFFER_FMT(ptr,blen,...) DebugBufferFmt(__FILE__,__LINE__,(unsigned char*)ptr,blen,__VA_ARGS__)


#define GETERRNO() (GetLastError() != 0 ? -GetLastError(): -1) 

#define ASSERT_IF(expr)  \
	do\
	{\
		if (!(expr)){\
			ERROR_INFO("expression %s not asserted\n",#expr);\
			abort();\
		}\
	}\
	while(0)


#endif /*__OUTPUT_DEBUG_H__*/
