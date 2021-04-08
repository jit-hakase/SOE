//
// Created by dx on 2021/4/8.
//

#include "ds_list.h"

struct list_t * list_init()
{
    struct list_t *list;
    list = malloc(sizeof(struct list_t));
    list->head = list->tail = NULL;
    list->len = 0;
    return list;
}

void list_destroy(struct list_t *list)
{
    while (!list_is_empty(list)) {
        list_del_head(list);
    }
    free(list);
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
    node = list->head;

    if (list->len == 1) {
        list->tail = list->head = NULL;
    } else {
        list->head = list->head->next;
        list->head->prev = NULL;
    }
    list->free_val(node->val);
    free(node);
    --list->len;
}

void list_del_tail(struct list_t *list)
{
    struct list_node_t *node;
    node = list->tail;

    if (list->len == 1) {
        list->tail = list->head = NULL;
    } else {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }
    list->free_val(node->val);
    free(node);
    --list->len;
}

int list_is_empty(const struct list_t *list)
{
    return list->len == 0;
}

int list_get_size(const struct list_t *list)
{
    return list->len;
}

void list_add_asc(struct list_t *list, void *val)
{
    struct list_node_t *curr, *node;

    if (list->cmp_val(list->head->val, val) <= 0) {
        list_add_head(list, val);
    } else if (list->cmp_val(list->tail->val, val) > 0) {
        list_add_tail(list, val);
    } else {
        curr = list->head;
        while (curr) {
            if (list->cmp_val(curr->val, val) <= 0) {
                node = malloc(sizeof(struct list_node_t));
                node->val = list->copy_val(val);
                curr->prev->next = node;
                node->prev = curr->prev;
                node->next = curr;
                curr->prev = node;
                ++list->len;
            }
            curr = curr->next;
        }
    }
}

void list_del_cond(struct list_t *list)
{
    struct list_node_t *last, *curr;
    last = NULL;
    curr = list->head;

    while (curr) {
        if (list->del_cond(curr->val)) {
            if (last == NULL) {
                list_del_head(list);
            } else if (curr->next == NULL) {
                list_del_tail(list);
            } else {
                curr->next->prev = last;
                last->next = curr->next;
                list->free_val(curr->val);
                free(curr);
                --list->len;
            }
            break;
        }
        last = curr;
        curr = curr->next;
    }
}
