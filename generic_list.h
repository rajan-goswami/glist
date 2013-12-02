/* 
 * File:   generic_list.h
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
 * This list should go as a statically linked library
 * 
 */

#ifndef GENERIC_LIST_H
#define	GENERIC_LIST_H

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * A generic data container.
     * Carries type information in the form of type id. Only client knows the
     * information of type. also carries generic data pointer which can point 
     * to any type of data. Every interaction with list happens in the form of 
     * Data container. Client are free to put any type of data inside this 
     * container. At retrieval it is upto them to determine appropriate type of 
     * data.
     */
    typedef struct _data {
        void *p; // Data Pointer
        unsigned int type_id; // Type id of Data
    } data_t, *pdata_t;

    /**
     * client has to get a valid list service handle by calling 
     * {@link create_list_service} api. list service acts as an interface to 
     * backend. multiple lists can be created by client and each 
     * list is identified by a unique id (always > 0). List service currently 
     * supports 1024 maximum no of alive lists. This is enough limit as of now,
     * which may be raised later. 
     * Architecture of list is completely opaque to client. It may change in 
     * any up versions (hopefully to improve performance). 
     * 
     * NOTE - When list data are removed from list, it is 
     * upto client to free memory of data inside {@link data} container. delete
     * functionality of list service only removes data nodes from list. it makes 
     * sure to free any memory created internally by list service, but client is
     * aware of type of memory pointed to be void pointer of data. It may be
     * local memory or on heap. so list service simply can't free that :(.
     * 
     */
    struct generic_list_service {
        /**
         * 
         * Create a new List        
         * @return A unique number (list identifier > 0) representing list. 
         *          Will return 0 in case of list is not created.
         */
        unsigned int (*create_empty_list) (void);

        /**
         * Insert an item into List
         * @param list identifier
         * @param {@link Data} to be inserted
         * 
         * @return 0 if item inserted successfully, else -1
         */
        int (*insert) (unsigned int, data_t);

        /**
         * Insert an item into List at specified index
         * @param list identifier
         * @param index
         * @param {@link Data} to be inserted
         * @return 0 if item inserted successfully, else -1
         */
        int (*insert_at) (unsigned int, unsigned int, data_t);

        /**
         * Retrieves item at specified index
         * @param list identifier
         * @param index
         * @param out parameter {@link Data} at specified index
         * @return 0 if error/data not found, 1 if success.
         */
        int (*get_data) (unsigned int, unsigned int, data_t *pData);

        /**
         * Delete a node from list
         * @param list identifier
         * @param index of node which is to be delete
         * @param out parameter. 
         *         valid {@link Data} deleted from list, NULL if not deleted.
         * @return 0 if error or node not found, 1 if success.
         */
        int (*delete_node) (unsigned int, unsigned int, data_t *pData);

        /**
         * Empty the list. As you anticipate this function will not 
         * free {@link Data} objects. That is upto client. so it is better to 
         * call delete_node and free returned Data. 
         * list identifier will be still valid.
         * @param list identifier
         */
        void (*clear) (unsigned int);

        /**
         * Deletes list. Once delete_list called, list identifier becomes 
         * invalid.
         * @param list identifier
         */
        void (*delete_list) (unsigned int);

        /**
         * Edit a specified node with new data
         * @param list identifier
         * @param index
         * @param new {@link Data} value
         */
        void (*edit_node) (unsigned int, unsigned int, data_t);

        /**
         * Reverse a List
         * @param list identifier
         */
        void (*reverse) (unsigned int);

        /**
         * Print a linked list
         * @param list identifier
         */
        void (*print_list) (unsigned int);

        /**
         * Checks if given list id is a valid one. It checks semantic validity.
         * @param list id
         * @return return 1 if list id is Valid, 0 otherwise
         */
        int (*is_valid_list_id) (unsigned int);

        /**
         * Returns size of list.
         * @param list id
         * @return no of elements inside list, -1 if given list id is invalid
         */
        int (*size) (unsigned int);

        /**
         * Append list-2 at the end of list-1. list-2 will become invalid after
         * this operation.s
         * @param list-1 id
         * @param list-2 id
         * @returns 1 if append was successful, 0 otherwise
         */
        int (*append) (unsigned int, unsigned int);
    };

    /**
     * A function to get handle of List Service
     * @return 
     */
    extern const struct generic_list_service * glist_create_list_service(void);

#ifdef	__cplusplus
}
#endif

#endif	/* GENERIC_LIST_H */

