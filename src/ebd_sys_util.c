
#include "ebd_sys_util.h"
#ifdef OS_LINUX
#include "stdio.h"
#include "stdarg.h"
#endif

void ebd_low(const char *cmd,...)  
{
	if(DEBUG_LEVEL<3)
		return;
#ifdef OS_LINUX
	printf("%s %s %s\t", __DATE__, __TIME__, __func__);
	va_list args; 
	va_start(args,cmd); 
	vprintf(cmd,args);    
	va_end(args);       
	printf("\n");
#endif
}  

void ebd_high(const char *cmd,...)  
{
	if(DEBUG_LEVEL<2)
		return;
#ifdef OS_LINUX
	printf("%s %s %s\t", __DATE__, __TIME__, __func__);
	va_list args; 
	va_start(args,cmd); 
	vprintf(cmd,args);    
	va_end(args);       
	printf("\n");
#endif
}  

void ebd_debug(const char *cmd,...)  
{
	if(DEBUG_LEVEL<1)
		return;
#ifdef OS_LINUX
	printf("%s %s %s\t", __DATE__, __TIME__, __func__);
	va_list args; 
	va_start(args,cmd); 
	vprintf(cmd,args);    
	va_end(args);       
	printf("\n");
#endif
}  

