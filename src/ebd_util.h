#ifndef _EBD_UTIL_H_
#define _EBD_UTIL_H_

#define ONE_PAGE_SIZE 4096
#define BLOCK_X(X) (X*ONE_PAGE_SIZE)
#define ALIGN_TO_4K(size) (size - size%ONE_PAGE_SIZE)
#define ALIGN_TO_4K_ADD(size) (size + (ONE_PAGE_SIZE-(size%ONE_PAGE_SIZE)))


#include "ebd_block.h"
#include "ebd_sys_util.h"

uint64_t ebd_util_pow(int x,int y);
void *ebd_util_mallc_lh_blk(big_buf_t *bb);
uint64_t ebd_util_get_data_size(int stage);
int ebd_util_check_merge_avail(void *ptr1,void *ptr2,size_t size);

#endif

