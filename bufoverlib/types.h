#ifndef __TYPES_H__
#define __TYPES_H__

#include <windows.h>

typedef UINT8 uint8_t;
typedef UINT16 uint16_t;
typedef UINT32 uint32_t;
typedef UINT64 uint64_t;

#ifdef _M_X64
typedef UINT64 addr_t;
#elif defined(_M_IX86)
typedef UINT32 addr_t;
#else
#error "not support architecture for windows compiler"
#endif

#define MAX_UINT64       (0xffffffffffffffffULL)

#endif /*__TYPES_H__*/