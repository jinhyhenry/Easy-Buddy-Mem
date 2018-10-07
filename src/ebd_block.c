#include "ebd_block.h"
#include "ebd_sys_util.h"
#include "ebd_util.h"

void ebd_block_add_buf_to_list(big_buf_t *bb,void *ptr,int stage);
int ebd_block_process_mem_divide(big_buf_t *bb,size_t total_size,void *ptr);

int ebd_block_malloc_postproc(big_buf_t *bb,int stage,void *ptr,size_t real_size)
{
	uint64_t block_size,left_size;
	void *tmp;
	/*(1) cut buffer*/
	block_size = ebd_util_get_data_size(stage);
	if(block_size>real_size){
		tmp = ptr + real_size;
		left_size = block_size - real_size;
	}
	/*(2) do divide*/
	if(left_size>0)
		ebd_block_process_mem_divide(bb,left_size,tmp);
	return 0;
}

void *ebd_block_get_buf_by_stage(big_buf_t *bb,int stage)
{
	void *tmp;
	struct ebd_list_head *entry,*head;
	head = &bb->block_free_list[stage];
	entry = head->next;
	ebd_list_del(entry);
	tmp = entry->data;
	ebd_util_free_lh_blk(bb,(void*)entry);

	ebd_low("%s get buf %d in stage %d",__func__,tmp,stage);
	return tmp;
}

int ebd_block_get_stage_lvl(size_t size)
{
	int i;
	for(i=0;i<NUM_OF_BLOCK;i++){
		if(size <= ebd_util_get_data_size(i)){
			break;
		}
	}
	ebd_low("%s stage %d",__func__,i);
	return i;
}

void ebd_block_add_buf_to_list(big_buf_t *bb,void *ptr,int stage)
{
	struct ebd_list_head *entry, *tmp, *head;
	void *p;
	head = &bb->block_free_list[stage];
	entry = (struct ebd_list_head *)ebd_util_mallc_lh_blk(bb);
	entry->data = ptr;

	ebd_low("%s E",__func__);

	/*Travel*/
	tmp = head->next;
	if(ebd_list_empty(head)){
		ebd_low("%s add to tail directly!",__func__);
		ebd_list_add_tail(entry,head);
		return;
	}
	ebd_low("%s start travel!",__func__);
	while(tmp != head){
		if(tmp->data > entry->data){
			break;
		}
		tmp = tmp->next;
	}
	ebd_list_add_tail(entry,tmp);
	ebd_low("%s end travel!",__func__);
	
	if(ebd_util_check_merge_avail(entry->data,entry->next->data,(size_t)ebd_util_get_data_size(stage)) && (stage < bb->num_of_block-1)){
		p = entry->data;
		tmp = entry->next;
		ebd_list_del(tmp);
		ebd_list_del(entry);
		ebd_util_free_lh_blk(bb,(void*)entry);
		ebd_util_free_lh_blk(bb,(void*)tmp);
		ebd_block_add_buf_to_list(bb,p,stage+1);
	}
	else if(ebd_util_check_merge_avail(entry->prev->data,entry->data,(size_t)ebd_util_get_data_size(stage)) && (stage < bb->num_of_block-1)){
		p = entry->prev->data;
		tmp = entry->prev;
		ebd_list_del(tmp);
		ebd_list_del(entry);
		ebd_util_free_lh_blk(bb,(void*)entry);
		ebd_util_free_lh_blk(bb,(void*)tmp);
		ebd_block_add_buf_to_list(bb,p,stage+1);
	}
	else{
		return;
	}
}

int ebd_block_process_current_div(big_buf_t *bb,void **ptr,int num,int stage)
{
	int i;
	uint64_t div_size = ebd_util_get_data_size(stage);
	ebd_low("%s E %d",__func__,stage);
	for(i=0;i<num;i++){
		*ptr += div_size;
		ebd_block_add_buf_to_list(bb,*ptr,stage);
		ebd_low("%s stage %d add , *ptr %d",__func__,stage,*ptr);
		if(!(*ptr)){
			ebd_debug("Mem Leak! \n");
			return -1;
		}
	}
	return 0;
}

int ebd_block_process_mem_divide(big_buf_t *bb,size_t total_size,void *ptr)
{
	int i,num;
	void *tmp_ptr;
	uint64_t dividing_size;

	tmp_ptr = ptr;
	for(i = bb->num_of_block-1;i>=0;i--){
		dividing_size = ebd_util_get_data_size(i);
		if(!total_size){
			continue;
		}
		num = total_size / dividing_size;
		ebd_low("i %d , num %d , total %d , div %d , tmp_ptr %d",i,num,total_size,dividing_size,tmp_ptr);
		total_size = total_size % dividing_size;
		ebd_low("final total %d",total_size);
		ebd_block_process_current_div(bb,&tmp_ptr,num,i);
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
	ebd_low("%s tmp ptr %d",__func__,tmp_ptr);
	/*divide mem*/
	for(i = bb->num_of_block-1;i>=0;i--){
		dividing_size = ebd_util_get_data_size(i);
		if(!total_size){
			continue;
		}
		num = total_size / dividing_size;
		ebd_low("%s i %d , num %d , total %d , div %d , tmp_ptr %d",__func__,i,num,total_size,dividing_size,tmp_ptr);
		total_size = total_size % dividing_size;
		ebd_low("%s final total %d",__func__,total_size);
		ebd_block_process_current_div(bb,&tmp_ptr,num,i);
	}
	return 0;
}

void *ebd_block_malloc(big_buf_t *bb,size_t size)
{
	int stage,i;
	void *buf;
	ebd_low("%s E",__func__);

	/*(1) get block stage according to its' size*/
	stage = ebd_block_get_stage_lvl(size);

	/*(2) get a empty buffer in free list*/
	for(i=stage;i<NUM_OF_BLOCK;i++){
		if(!ebd_list_empty(&bb->block_free_list[i])){
			buf = ebd_block_get_buf_by_stage(bb,i);
			break;
		}
	}

	/*(3) do postproc*/
	ebd_block_malloc_postproc(bb,i,buf,size);
	
	return buf;
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
	bb->size = size - BLOCK_META_SIZE - bb->size_lh_blk;
	bb->num_of_block = NUM_OF_BLOCK;
	/*Init Lists*/
	for(i=0;i<bb->num_of_block;i++){
		EBD_INIT_LIST_HEAD(&bb->block_free_list[i]);
	}
	for(i=0;i<LH_BLK_TBL_SIZE;i++){
		bb->lh_blk_tbl[i] = 1;
	}
	ebd_low("%s real buffer %d, size %d",__func__,bb->ptr,bb->size);
	ebd_block_init_divide(bb);
	return bb;
}

