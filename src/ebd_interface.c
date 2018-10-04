
#include "ebd_interface.h"
#include "ebd_sys_util.h"

big_buf_t *ebd_register_big_buf(void* buf,size_t size)
{
	if(!buf){
		ebd_debug("Null Input Buffer\n");
		return NULL;
	}
	return ebd_block_register(buf,size);
}


