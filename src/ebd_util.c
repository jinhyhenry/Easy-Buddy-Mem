#include "ebd_util.h"
#include "ebd_sys_util.h"
#include "ebd_block.h"

uint64_t ebd_util_pow(int x,int y)
{
	int i;
	uint64_t res = 1;
	for(i=0;i<y;i++){
		res *= x;
	}
	return res;
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

