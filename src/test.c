
#include "ebd_sys_util.h"
#include "ebd_interface.h"
#include "ebd_list.h"
#include "ebd_util.h"

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

int ebd_test_util(int typ)
{
	uint64_t src = 10*ebd_util_pow(2,9)+33;
	switch(typ){
		case 1:
			ebd_debug("pow res %d",ebd_util_pow(2,9));
			break;
		case 2:
			ebd_debug("align test src %d res1 %d, res2 %d",src,ALIGN_TO_4K(src)
				,ALIGN_TO_4K_ADD(src));
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

int ebd_test_travel_list(struct ebd_list_head *head)
{
	struct ebd_list_head *tmp;
	int *dat;
	tmp = head;
	ebd_low("%s E h:%d",__func__,head);
	tmp = tmp->next;	
	while(tmp != head){
		ebd_debug("%s list data tmp:%d",__func__,tmp);
		tmp = tmp->next;
	}
	return 0;
}

int ebd_test_list(int typ)
{
	int dat[3] = {2,4,6};
	int *tmp_dat;
	struct ebd_list_head head,entry1,entry2,entry3,*tmp_head;
	switch(typ){
		case 1:
			ebd_low("E!");
			EBD_INIT_LIST_HEAD(&head);
			tmp_head = &head;
			entry1.data = dat;
			entry2.data = dat+1;
			entry3.data = dat+2;
			ebd_low("%s begin to add list! h:%d en1:%d en2:%d en3:%d",__func__,&head,&entry1,&entry2,&entry3);
			//ebd_low("dat1 %d");
			ebd_list_add_tail(&entry1,&head);
			ebd_list_add_tail(&entry3,&head);
			ebd_list_add(&entry2,&entry1);

			ebd_test_travel_list(&head);

			ebd_low("%s del en2",__func__);
			ebd_list_del(&entry2);
			ebd_low("%s is_empty: %d",__func__,ebd_list_empty(&head));
			ebd_test_travel_list(&head);
			ebd_low("%s del en3",__func__);
			ebd_list_del(&entry3);
			ebd_low("%s is_empty: %d",__func__,ebd_list_empty(&head));
			ebd_test_travel_list(&head);
			
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
			ebd_debug("input buf %d , size %d",buf,SIZE_BB);
			big_buf_t *bb = ebd_register_big_buf(buf,SIZE_BB);
			sys_free(buf);
			break;
	}
	return 0;
}

int main()
{
	//ebd_test_sys_util(1);  /*pass*/
	//ebd_test_data_size(1);
	//ebd_test_util(2); /*pass*/
	//ebd_test_interface(1);
	ebd_test_list(1);
	return 0;
}

