#ifndef _EBD_LIST_H_
#define _EBD_LIST_H_

struct ebd_list_head {
	struct ebd_list_head *next, *prev;
	void *data;
};

static inline void EBD_INIT_LIST_HEAD(struct ebd_list_head *list)
{
	list->next = list;
	list->prev = list;
}

static inline void __ebd_list_add(struct ebd_list_head *new, struct ebd_list_head *prev, struct ebd_list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static inline void ebd_list_add(struct ebd_list_head *new, struct ebd_list_head *head)
{
	__ebd_list_add(new, head, head->next);
}

static inline void ebd_list_add_tail(struct ebd_list_head *new, struct ebd_list_head *head)
{
	__ebd_list_add(new, head->prev, head);
}

static inline void __ebd_list_del(struct ebd_list_head * prev, struct ebd_list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void ebd_list_del(struct ebd_list_head *entry)
{
	__ebd_list_del(entry->prev, entry->next);
	//entry->next = NULL;
	//entry->prev = NULL;
}


#endif

