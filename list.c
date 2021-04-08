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
	void (*free_val)(void *val);
	int (*add_cond)(void *val, void *new_val);
	int (*del_cond)(void *val);
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
	
	list->free_val(node->val);
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
	
	list->free_val(node->val);
	free(node);
}

void list_add_cond(struct list_t *list, void *val)
{
	struct list_node_t *last, *node, *new_node;
	
	last = NULL;
	node = list->head;
	
	while (node) {
		if (add_cond_default(node->val, val)) {
			if (last == NULL) {
				list_add_head(list, val);
			} else {
				new_node = malloc(sizeof(struct list_node_t));
				new_node->val = list->copy_val(val);
				last->next = new_node;
				new_node->next = node;
				new_node->prev = last;
				node->prev = new_node;
				++list->len;
			}
			break;
		}
		last = node;
		node = node->next;
		if (node == NULL) {
			list_add_tail(list, val);
		}
	}
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

void free_val_default(void* val)
{
	free(val);
}

int add_cond_default(void *val, void *new_val)
{
	return *(int*)new_val < *(int*)val;
}

int del_cond_default(void *val)
{
	return *(int*)val == 0;
}

void list_show_asc(struct list_t *list)
{
	struct list_node_t *curr = list->head;
	printf("[ ");
	while (curr) {
		printf("%d ", *(int*)curr->val);
		curr = curr->next;
	}
	printf("]\n");
}

void list_show_desc(struct list_t *list)
{
	struct list_node_t *curr = list->tail;
	printf("[ ");
	while (curr) {
		printf("%d ", *(int*)curr->val);
		curr = curr->prev;
	}
	printf("]\n");
}

int main()
{
	struct list_t *list = list_init();
	list->copy_val = copy_val_default;
	list->free_val = free_val_default;
	list->add_cond = add_cond_default;
	list->del_cond = del_cond_default;
	int x = 3, y = 6, z = 8;
	int a = 0, b = 5, c = 9;
	list_add_tail(list, &x);
	list_add_tail(list, &y);
	list_add_tail(list, &z);
	list_add_cond(list, &a);
	list_add_cond(list, &b);
	list_add_cond(list, &c);
	list_show_asc(list);
	list_show_desc(list);
	return 0;
}


