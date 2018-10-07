
#include "ebd_interface.h"
#include "ebd_sys_util.h"
#include "ebd_util.h"

big_buf_t *ebd_register_big_buf(void* buf,size_t size)
{
	if(!buf){
		ebd_debug("Null Input Buffer\n");
		return NULL;
	}
	return ebd_block_register(buf,size);
}

void *ebd_malloc(big_buf_t *bb,size_t size)
{
	ebd_low("%s E",__func__);
	if(!bb){
		ebd_debug("Null big buf!");
		return NULL;
	}
	if(size >= bb->size){
		ebd_debug("Not enough size!");
		return NULL;
	}
	return ebd_block_malloc(bb,ALIGN_TO_4K_ADD(size));
}

