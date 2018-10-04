
#include "ebd_sys_util.h"
#include "ebd_interface.h"
#include "ebd_list.h"

#define SIZE_BB 25165824

int ebd_test_sys_util(int typ)
{
	switch(typ){
		case 1:
			ebd_debug("hello world %d",typ);
			break;
	}
	return 0;
}

int ebd_test_data_size(int typ)
{
	struct ebd_list_head lh;
	switch(typ){
		case 1:
			ebd_debug("size of ebd_list_head %d",sizeof(lh));
			ebd_debug("size of big_buf_t %d",sizeof(big_buf_t));
			break;
	}
	return 0;
}

int ebd_test_interface(int typ)
{
	void *buf = NULL;
	switch(typ){
		case 1:
			buf = sys_malloc(SIZE_BB);
			big_buf_t *bb = ebd_register_big_buf(buf,SIZE_BB);
			break;
	}
	return 0;
}

int main()
{
	//ebd_test_sys_util(1);  /*pass*/
	//ebd_test_data_size(1);
	ebd_test_interface(1);
	return 0;
}

