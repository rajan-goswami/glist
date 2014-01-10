/* 
 * File:   generic_list.c
 * Author: rajan.goswami85@gmail.com
 * 
 * Copyright (C) 2013  Rajan Goswami
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 *
 * Created on 21 July, 2013, 10:49 PM
 * implementation of generic list
 * 
 */

//=====================================
//              Headers
//=====================================
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "generic_list.h"

//=====================================
//              Macros
//=====================================
#define CHECK_NULL(x) (x == 0)
#define CHECK_NOT_NULL(x) (x != 0)

#define MAX_ALLOTTED_LISTS 1024 /* Maximum no. of LIVE lists */
#define INVALID_LIST_ID (MAX_ALLOTTED_LISTS + 1)
#define FAST_INSERT_OPERATION 1

//=====================================
//          Type Declarations
//=====================================

typedef struct list_node {
    data_t data; /* Data part */
    struct list_node *next; /* Next node */
} list_node_t, *plist_node_t;

typedef struct generic_linked_list {
    plist_node_t phead; /* Head Pointer */
    unsigned int node_count; /* Node count */
} generic_list_t, *pgeneric_list_t;

//=====================================
//        Function Declarations
//=====================================
static plist_node_t create_new_node(data_t data);
static unsigned int create_empty_list(void);
static int insert(unsigned int list_id, data_t data);
static int insert_at(unsigned int list_id, unsigned int index, data_t data);
static unsigned int get_data(unsigned int list_id, unsigned int index, data_t *pData);
static unsigned int delete_node(unsigned int list_id, unsigned int index, data_t *pData);
static void clear(unsigned int list_id);
static void delete_list(unsigned int list_id);
static void edit_node(unsigned int list_id, unsigned int index, data_t data);
static void reverse(unsigned int list_id);
static void print_list(unsigned int list_id);
static unsigned int is_valid_list_id(unsigned int list_id);
static int size(unsigned int list_id);
static int append(unsigned int list_1_id, unsigned int list_2_id);

static unsigned int add_list_to_allotted_list(const pgeneric_list_t pList);
static void remove_list_from_allotted_list(const pgeneric_list_t pList);
static void clean_allotted_list(void);
static pgeneric_list_t get_list_from_allotted_list(unsigned int index);

//=====================================
//              Global Data
//=====================================
static const struct generic_list_service g_list_service = {
    &create_empty_list,
    &insert,
    &insert_at,
    &get_data,
    &delete_node,
    &clear,
    &delete_list,
    &edit_node,
    &reverse,
    &print_list,
    &is_valid_list_id,
    &size,
    &append
};

/* A map from list index -> allocated list address
 *      0 -> 0x20000000      
 *      1 -> 0x20000004
 *      2 -> 0x20000008 etc.
 */
static struct {
    pgeneric_list_t list_addr; /* allocated linked list address */
} g_allotted_lists[MAX_ALLOTTED_LISTS];

static plist_node_t create_new_node(data_t data)
{
    plist_node_t node = (plist_node_t) calloc((size_t) 1, sizeof (list_node_t));
    if (CHECK_NOT_NULL(node)) {
        node->data.p = data.p;
        node->data.type_id = data.type_id;
    }
    return node;
}

static unsigned int create_empty_list(void)
{

    unsigned int index = 0;
    pgeneric_list_t p_list = (pgeneric_list_t) calloc((size_t) 1, sizeof (generic_list_t));
    if (CHECK_NOT_NULL(p_list)) {
        if ((index = add_list_to_allotted_list(p_list)) == 0)
            free(p_list);
    }
    return index;
}

static int insert(unsigned int list_id, data_t data)
{

    pgeneric_list_t p_list = get_list_from_allotted_list(list_id);
    if (CHECK_NULL(p_list))
        return -1;

    plist_node_t node = create_new_node(data);
    if (CHECK_NULL(node))
        return -1;

#if !FAST_INSERT_OPERATION
    if (!p_list->node_count) {
        p_list->phead = node;
    } else {
        plist_node_t temp = p_list->phead;
        for (; temp->next != NULL; temp = temp->next);
        temp->next = node;
    }
    p_list->node_count++;
#else
    node->next = p_list->phead;
    p_list->phead = node;
    p_list->node_count++;
#endif    
    return 0;
}

static int insert_at(unsigned int list_id, unsigned int index, data_t data)
{

    pgeneric_list_t p_list = get_list_from_allotted_list(list_id);
    if (CHECK_NULL(p_list) ||
            (index != 0 &&
            (index > p_list->node_count)))
        return -1;

#if !FAST_INSERT_OPERATION
    if (index == 0)
        return insert(list_id, data);
#else    
    if (index == p_list->node_count)
        return insert(list_id, data);
#endif

    plist_node_t node = create_new_node(data);
    if (CHECK_NULL(node))
        return -1;

    plist_node_t temp = p_list->phead;
#if !FAST_INSERT_OPERATION
    for (; index-- > 0; temp = temp->next)
        ;
#else
    for (index = p_list->node_count - (index + 1); index-- > 0; temp = temp->next)
        ;
#endif
    node->next = temp->next;
    temp->next = node;
    p_list->node_count++;
    return 0;
}

static unsigned int get_data(unsigned int list_id, unsigned int index, data_t *pData)
{

    pgeneric_list_t p_list = get_list_from_allotted_list(list_id);
    if (CHECK_NULL(p_list) ||
            CHECK_NULL(pData) ||
            !(p_list->node_count) ||
            (index != 0 && (index >= p_list->node_count)))
        return 0;

    plist_node_t temp = p_list->phead;
#if !FAST_INSERT_OPERATION    
    for (; index-- > 0; temp = temp->next)
        ;
#else
    for (index = p_list->node_count - (index + 1); index-- > 0; temp = temp->next)
        ;
#endif
    (*pData).p = temp->data.p;
    (*pData).type_id = temp->data.type_id;
    return 1;
}

static unsigned int delete_node(unsigned int list_id, unsigned int index, data_t *pData)
{

    pgeneric_list_t p_list = get_list_from_allotted_list(list_id);
    if (CHECK_NULL(p_list) ||
            !(p_list->node_count) ||
            (index != 0 &&
            (index >= p_list->node_count)))
        return 0;

    plist_node_t temp = p_list->phead;
#if !FAST_INSERT_OPERATION    
    if (index == 0) {
#else
    if (index == (p_list->node_count - 1)) {
#endif
        p_list->phead = temp->next;
    } else {
#if !FAST_INSERT_OPERATION
        for (; --index > 0; temp = temp->next)
            ;
#else
        for (index = p_list->node_count - (index + 1); --index > 0; temp = temp->next)
            ;
#endif        
        plist_node_t prev_node = temp;
        temp = temp->next;
        prev_node->next = temp->next;
    }
    if (CHECK_NOT_NULL(pData)) {
        (*pData).p = temp->data.p;
        (*pData).type_id = temp->data.type_id;
    }
    free(temp);
    p_list->node_count--;
    return 1;
}

static void clear(unsigned int list_id)
{

    pgeneric_list_t p_list = get_list_from_allotted_list(list_id);
    if (CHECK_NULL(p_list))
        return;

    plist_node_t del_node = NULL;
    while (CHECK_NOT_NULL(p_list->phead)) {
        del_node = p_list->phead;
        p_list->phead = del_node->next;
        free(del_node);
    }
    p_list->node_count = 0;
}

static void delete_list(unsigned int list_id)
{

    pgeneric_list_t p_list = get_list_from_allotted_list(list_id);
    if (CHECK_NULL(p_list))
        return;

    // First clear list
    clear(list_id);

    free(p_list);

    remove_list_from_allotted_list(p_list);
}

static void edit_node(unsigned int list_id, unsigned int index, data_t data)
{

    pgeneric_list_t p_list = get_list_from_allotted_list(list_id);
    if (CHECK_NULL(p_list) ||
            !(p_list->node_count) ||
            (index != 0 &&
            (index >= p_list->node_count)))
        return;

    plist_node_t temp = p_list->phead;
#if !FAST_INSERT_OPERATION    
    for (; index-- > 0; temp = temp->next)
        ;
#else
    for (index = p_list->node_count - (index + 1); index-- > 0; temp = temp->next)
        ;
#endif
    temp->data.p = data.p;
    temp->data.type_id = data.type_id;

    return;
}

static void reverse(unsigned int list_id)
{

    pgeneric_list_t p_list = get_list_from_allotted_list(list_id);
    if (CHECK_NULL(p_list) || !(p_list->node_count))
        return;

    plist_node_t prev = NULL, temp = NULL;
    
    do {
        prev = p_list->phead;
        p_list->phead = prev->next;
        prev->next = temp;
        temp = prev;
    } while (p_list->phead != NULL);

    p_list->phead = prev; 
}

static void print_list(unsigned int list_id)
{

    pgeneric_list_t p_list = get_list_from_allotted_list(list_id);
    if (CHECK_NULL(p_list))
        return;

    plist_node_t temp = p_list->phead;
    printf("List : %d\n", list_id);
    for (; CHECK_NOT_NULL(temp); temp = temp->next)
        printf("\t%p : %p \n", temp, temp->next);

    printf("End\n");
}

static unsigned int is_valid_list_id(unsigned int list_id)
{
    pgeneric_list_t p_list = get_list_from_allotted_list(list_id);
    return CHECK_NULL(p_list) ? 0 : 1;
}

static int size(unsigned int list_id)
{
    pgeneric_list_t p_list = get_list_from_allotted_list(list_id);
    return CHECK_NULL(p_list) ? -1 : p_list->node_count;
}

static int append(unsigned int list_1_id, unsigned int list_2_id)
{

    pgeneric_list_t p_list_1 = get_list_from_allotted_list(list_1_id);
    pgeneric_list_t p_list_2 = get_list_from_allotted_list(list_2_id);
    if (CHECK_NULL(p_list_1) || CHECK_NULL(p_list_2))
        return 0;

#if! FAST_INSERT_OPERATION
    if (CHECK_NOT_NULL(p_list_1->phead)) {
        plist_node_t temp = p_list_1->phead;
        for (; CHECK_NOT_NULL(temp->next); temp = temp->next)
            ;
        temp->next = pList_2->phead;
    }
#else
    if (CHECK_NOT_NULL(p_list_2->phead)) {
        plist_node_t temp = p_list_2->phead;
        for (; CHECK_NOT_NULL(temp->next); temp = temp->next)
            ;
        temp->next = p_list_1->phead;
        p_list_1->phead = p_list_2->phead;
    }
#endif   
    p_list_1->node_count += p_list_2->node_count;

    free(p_list_2);

    remove_list_from_allotted_list(p_list_2);

    return 1;
}

static void clean_allotted_list(void)
{
    // Re-initiate g_allotted_lists book
    memset((void *) &g_allotted_lists, 0, sizeof (g_allotted_lists));
}

static void remove_list_from_allotted_list(const pgeneric_list_t pList)
{
    register unsigned int i = MAX_ALLOTTED_LISTS - 1;
    for (; i >= 0; i--) {
        if (g_allotted_lists[i].list_addr == pList) {
            g_allotted_lists[i].list_addr = NULL;
            break;
        }
    }
    return;
}

static unsigned int add_list_to_allotted_list(const pgeneric_list_t pList)
{
    register unsigned int i = 0;
    for (; i < MAX_ALLOTTED_LISTS; i++) {
        if (g_allotted_lists[i].list_addr == NULL) {
            g_allotted_lists[i].list_addr = pList;
            return i + 1;
        }
    }
    return 0;
}

static pgeneric_list_t get_list_from_allotted_list(unsigned int index)
{
    index--;
    return index < MAX_ALLOTTED_LISTS ? g_allotted_lists[index].list_addr : NULL;
}

const struct generic_list_service * glist_create_list_service(void)
{
    // Do clean up required, ideally this function should be called only once
    // clean_allotted_list();

    return &g_list_service;
}