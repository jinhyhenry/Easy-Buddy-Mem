
#include "ebd_sys_util.h"
#include "ebd_interface.h"
#include "ebd_list.h"

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
}

int main()
{
	//ebd_test_sys_util(1);  /*pass*/
	ebd_test_data_size(1);
	return 0;
}

