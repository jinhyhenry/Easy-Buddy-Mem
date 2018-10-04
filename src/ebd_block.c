#include "ebd_block.h"
#include "ebd_sys_util.h"
#include "ebd_util.h"

int ebd_block_process_current_div(big_buf_t *bb,void *ptr,int num,int stage)
{
	int i;
	uint64_t div_size = BLOCK_X(ebd_util_pow(2,stage));
	for(i=0;i<num;i++){
		ptr += div_size;
		struct ebd_list_head *lh = (struct ebd_list_head *)ebd_util_mallc_lh_blk(bb);
		ebd_list_add_tail(lh,&bb->block_free_list[stage]);
		if(!ptr){
			ebd_debug("Mem Leak! \n");
			return -1;
		}
	}
	return 0;
}

int ebd_block_init_divide(big_buf_t *bb)
{
	int i,num;
	void *tmp_ptr;
	uint64_t total_size,dividing_size = 1;
	if(!bb){
		ebd_debug("NULL Big Buffer!\n");
		return -1;
	}
	/*perform 4k align first*/
	bb->size = ALIGN_TO_4K(bb->size);
	
	total_size = bb->size;
	tmp_ptr = bb->ptr;
	/*divide mem*/
	for(i = bb->num_of_block-1;i>=0;i--){
		dividing_size = BLOCK_X(ebd_util_pow(2,i));
		if(!total_size){
			break;
		}
		num = total_size / dividing_size;
		total_size = total_size % dividing_size;
		ebd_block_process_current_div(bb,tmp_ptr,num,i);
	}
	return 0;
}

big_buf_t *ebd_block_register(void *buf,size_t size)
{
	int i;
	/*reserve 4K in the headof big buffer to put metas*/
	big_buf_t *bb;
	bb = buf;
	buf += BLOCK_META_SIZE;

	/*reserve 28k for list_head blks*/
	bb->lh_blk_ptr = buf;
	bb->size_lh_blk = BLOCK_X(7);
	buf += bb->size_lh_blk;
	
	bb->ptr = buf;
	bb->size = size - BLOCK_META_SIZE;
	bb->num_of_block = NUM_OF_BLOCK;
	/*Init Lists*/
	for(i=0;i<bb->num_of_block;i++){
		EBD_INIT_LIST_HEAD(&bb->block_free_list[i]);
	}
	for(i=0;i<LH_BLK_TBL_SIZE;i++){
		bb->lh_blk_tbl[i] = 1;
	}
	ebd_block_init_divide(bb);
	return bb;
}

