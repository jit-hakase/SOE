#ifndef SOE_DS_LIST_H
#define SOE_DS_LIST_H

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
    int (*cmp_val)(void *old_val, void *new_val);
    uint64_t len;
};

struct list_t * list_init();
void list_destroy(struct list_t *list);

void list_add_head(struct list_t *list, void *val);
void list_add_tail(struct list_t *list, void *val);
void list_del_head(struct list_t *list);
void list_del_tail(struct list_t *list);

void list_add_cond(struct list_t *list, void *val);
void list_del_cond(struct list_t *list);

int list_is_empty(const struct list_t *list);
int list_get_size(const struct list_t *list);

/* default member function example

void* list_copy_val_default(void* val)
{
    void *new_val;
    new_val = malloc(sizeof(int));
    *(int*)new_val = *(int*)val;
    return new_val;
}

void list_free_val_default(void* val)
{
    free(val);
}

int list_add_cond_default(void *val, void *new_val)
{
    return *(int*)new_val < *(int*)val;
}

int list_del_cond_default(void *val)
{
    return *(int*)val == 0;
}

void list_show_data_default(const struct list_t *list)
{
    struct list_node_t *curr = list->head;
    printf("[ ");
    while (curr) {
        printf("%d ", *(int*)curr->val);
        curr = curr->next;
    }
    printf("]\n");
}

list->copy_val = list_copy_val_default;
list->free_val = list_free_val_default;
list->add_cond = list_add_cond_default;
list->del_cond = list_del_cond_default;
*/

#endif //SOE_DS_LIST_H
