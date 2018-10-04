#ifndef _EBD_UTIL_H_
#define _EBD_UTIL_H_

#define ONE_PAGE_SIZE 4096
#define BLOCK_X(X) (X*ONE_PAGE_SIZE)
#define ALIGN_TO_4K(size) (size - size%ONE_PAGE_SIZE)

uint64_t ebd_util_pow(int x,int y);
void *ebd_util_mallc_lh_blk(big_buf_t *bb);

#endif

