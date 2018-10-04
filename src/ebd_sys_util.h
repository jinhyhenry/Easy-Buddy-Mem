
#ifndef _EBD_SYS_UTIL_H_
#define _EBD_SYS_UTIL_H_

#define OS_LINUX

#ifdef OS_LINUX
#include "stdlib.h"
#endif

#define DEBUG_LEVEL 3 /*1 -- debug , 2 -- debug|high , 3 -- debug|high|low*/

#if 0
typedef unsigned char           uint8_t;  
typedef unsigned short int      uint16_t;  
typedef unsigned int            uint32_t;
typedef unsigned long int       uint64_t;
#endif

void ebd_low(const char *cmd,...);
void ebd_high(const char *cmd,...);
void ebd_debug(const char *cmd,...);
void *sys_malloc(size_t size);

#endif
