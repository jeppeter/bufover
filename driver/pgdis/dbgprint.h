#ifndef __DBG_PRINT_H__
#define __DBG_PRINT_H__


#include <ntddk.h>
#include <windef.h>
#include <stdlib.h>


#define DEBUG_PRINT(...)   do{DbgPrint("%s:%d\t",__FILE__,__LINE__);DbgPrint(__VA_ARGS__);}while(0)


#endif /*__DBG_PRINT_H__*/