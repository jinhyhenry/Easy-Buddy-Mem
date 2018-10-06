#include "ebd_util.h"
#include "ebd_sys_util.h"
#include "ebd_block.h"

int pow_tbl_2[NUM_OF_BLOCK] = {1,2,4,8,16,32,64,128,256,512,1024};

uint64_t ebd_util_pow(int x,int y)
{
	int i;
	uint64_t res = 1;
	for(i=0;i<y;i++){
		res *= x;
	}
	return res;
}

int ebd_util_2_pow(int x)
{
	if(x>NUM_OF_BLOCK-1){
		return -1;
	}
	return pow_tbl_2[x];
}

uint64_t ebd_util_get_data_size(int stage)
{
	return BLOCK_X(ebd_util_2_pow(stage));
}

/*ptr1 must smaller*/
int ebd_util_check_merge_avail(void *ptr1,void *ptr2,size_t size)
{
	if(!ptr1 || !ptr2)
		return 0;
	if((ptr2 - ptr1) == size){
		return 1;
	}
	else
		return 0;
}

void *ebd_util_malloc_lh_blk_by_index(big_buf_t *bb,int index)
{
	void *ptr_t = bb->lh_blk_ptr;
	if(!ptr_t){
		ebd_debug("Null lh buffer!");
		return NULL;
	}
	if(!bb->lh_blk_tbl[index]){
		ebd_debug("Dirty index!");
		return NULL;
	}
	bb->lh_blk_tbl[index] = 0;
	return ptr_t+(index*sizeof(struct ebd_list_head));
}

void *ebd_util_mallc_lh_blk(big_buf_t *bb)
{
	int i;
	/*Get a availeble addr*/
	for(i=0;i<LH_BLK_TBL_SIZE;i++){
		if(bb->lh_blk_tbl[i]){
			ebd_low("found lh_blk %d",i);
			return ebd_util_malloc_lh_blk_by_index(bb,i);
		}
	}
	ebd_debug("No ebd lh blk!");
	return NULL;
}

void ebd_util_free_lh_blk(big_buf_t *bb,void *ptr)
{
	/*locate the index*/
	int index = (ptr - bb->lh_blk_ptr)/(sizeof(struct ebd_list_head));
	sys_memset(ptr,0,sizeof(struct ebd_list_head));
	bb->lh_blk_tbl[index] = 1;
}

