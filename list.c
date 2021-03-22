#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct list_node_t
{
	void *val;
	struct list_node_t *prev;
	struct list_node_t *next;
};

struct list_t
{
	struct list_node_t *head;
	struct list_node_t *tail;
	void* (*copy_val)(void *val);
	uint64_t len;
};

struct list_t * list_init()
{
	struct list_t *list;
	list = malloc(sizeof(struct list_t));
	list->head = list->tail = NULL;
	list->len = 0;
	return list;
}

void list_add_head(struct list_t *list, void *val)
{
	struct list_node_t *node;
	node = malloc(sizeof(struct list_node_t));
	node->val = list->copy_val(val);
	
	if (list->len == 0) {
		list->tail = list->head = node;
		node->prev = node->next = NULL;
	} else {
		node->next = list->head;
		node->prev = NULL;
		list->head->prev = node;
		list->head = node;
	}
	++list->len;
}

void list_add_tail(struct list_t *list, void *val)
{
	struct list_node_t *node;
	node = malloc(sizeof(struct list_node_t));
	node->val = list->copy_val(val);
	
	if (list->len == 0) {
		list->tail = list->head = node;
		node->prev = node->next = NULL;
	} else {
		node->prev = list->tail;
		node->next = NULL;
		list->tail->next = node;
		list->tail = node;
	}
	++list->len;
}

void list_del_head(struct list_t *list)
{
	struct list_node_t *node;

	--list->len;
	node = list->head;
	
	if (list->len == 0) {
		list->tail = list->head = NULL;
	} else {
		list->head = list->head->next;
		list->head->prev = NULL;
	}
	
	free(node->val);
	free(node);
}

void list_del_tail(struct list_t *list)
{
	struct list_node_t *node;
	
	--list->len;
	node = list->tail;
	
	if (list->len == 0) {
		list->tail = list->head = NULL;
	} else {
		list->tail = list->tail->prev;
		list->tail->next = NULL;
	}
	
	free(node->val);
	free(node);
}

int list_is_empty(struct list_t *list)
{
	return list->len == 0;
}

void* copy_val_default(void* val)
{
	void *new_val;
	
	new_val = malloc(sizeof(int));
	*(int*)new_val = *(int*)val;
	return new_val;
}
