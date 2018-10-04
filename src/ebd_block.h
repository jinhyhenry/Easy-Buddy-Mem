#ifndef _EBD_BLOCK_H_
#define _EBD_BLOCK_H_

#define BLOCK_META_SIZE 4096
#define NUM_OF_BLOCK 11

#define LH_BLK_TBL_SIZE 1024

#include "ebd_list.h"
#include "stddef.h"

typedef struct _big_buf_t {
  void *ptr;
  size_t size;

  struct ebd_list_head block_free_list[NUM_OF_BLOCK];
  int num_of_block;
  /* 0 -- 1 * 4k */ /* 1 -- 2 * 4k */ /* 2 -- 4 * 4k */
  /*2 ^ n*/

  void *lh_blk_ptr;
  size_t size_lh_blk;
  uint8_t lh_blk_tbl[LH_BLK_TBL_SIZE];
  
} big_buf_t;

big_buf_t *ebd_block_register(void *buf,size_t size);

#endif

