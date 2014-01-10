/* 
 * File:   main.c
 * Author: rajan
 *
 * Created on 20 July, 2013, 1:49 PM
 * 
 * Unit tests for glist
 */

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "generic_list.h"
#include <assert.h>

#define TEST_INSERT_DATA_COUNT 10

enum DataType {
    INT,
    CHAR,
    SHORT,
    LONG,
    FLOAT,
    DOUBLE
};

static data_t create_test_int_data(int i);
static void delete_test_list(const struct generic_list_service *list_service, unsigned int list_id);

static void test_generic_list_service(void);
static void test_invalid_list_id(const struct generic_list_service *);
static void test_insert(const struct generic_list_service *list_service);
static void test_delete(const struct generic_list_service *list_service);
static void test_clear(const struct generic_list_service *list_service);
static void test_get_data(const struct generic_list_service *list_service);
static void test_reverse(const struct generic_list_service *list_service);
static void test_append(const struct generic_list_service *list_service);

/*
 * 
 */
int main(int argc, char** argv)
{

    // test_linked_list();

    int i = -1;
    for (; i < 0; i = 5) {
        printf(":%d:\n", i);
    }
    printf(":%d:\n", i);

    test_generic_list_service();

    return (EXIT_SUCCESS);
}

static void test_generic_list_service(void)
{

    printf("\n===== %s Start =====\n", __FUNCTION__);

    const struct generic_list_service *list_service = glist_create_list_service();

    assert(list_service != NULL);

    struct timespec ts_begin, ts_end;
    clock_gettime(CLOCK_REALTIME, &ts_begin);
    printf("%lld.%.9ld", (long long) ts_begin.tv_sec, ts_begin.tv_nsec);

    test_insert(list_service);

    test_invalid_list_id(list_service);

    test_delete(list_service);

    test_clear(list_service);

    test_get_data(list_service);

    test_reverse(list_service);

    test_append(list_service);

    clock_gettime(CLOCK_REALTIME, &ts_end);
    printf("%lld.%.9ld", (long long) ts_end.tv_sec, ts_end.tv_nsec);

    printf("\n===== %s Passed =====\n", __FUNCTION__);
}

static data_t create_test_int_data(int i)
{
    data_t data;
    data.type_id = INT;
    int *ii = (int *) calloc((size_t) 1, sizeof (int));
    *ii = i;
    data.p = ii;
    return data;
}

static void delete_test_list(const struct generic_list_service *list_service, unsigned int list_id)
{
    int s = list_service->size(list_id);
    int i = 0;
    data_t data;
    for (; i < s; i++) {
        memset(&data, 0, sizeof (data_t));
        assert(list_service->get_data(list_id, i, &data) == 1);
        assert(data.p != NULL);
        free(data.p);
    }
    list_service->delete_list(list_id);
}

static void test_insert(const struct generic_list_service * list_service)
{

    printf("\n===== %s Start =====\n", __FUNCTION__);

    unsigned int list_id = list_service->create_empty_list();
    if (list_id == 0) {
        printf("\ncreate_empty_list failed\n");
        return;
    }

    assert(list_service->insert_at(list_id, 0, create_test_int_data(2)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(4)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(6)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(8)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(10)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(12)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(14)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(16)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(18)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(20)) == 0);
    assert(list_service->insert_at(list_id, 0, create_test_int_data(0)) == 0);

    assert(list_service->size(list_id) != -1);
    assert(list_service->size(list_id) == 11);
    assert(list_service->insert_at(list_id, 11, create_test_int_data(24)) == 0);

    data_t data;
    int i = 0;
    int k = list_service->size(list_id);
    for (; i < k; i++) {
        memset(&data, 0, sizeof (data_t));
        assert(list_service->get_data(list_id, i, &data) == 1);
        printf("\tType at %d index : %d \n", i, data.type_id);
        printf("\tData at %d index : %d \n\n", i, *((int *) data.p));
    }

    assert(list_service->insert(0, create_test_int_data(22)) == -1);
    assert(list_service->insert_at(0, 2, create_test_int_data(22)) == -1);

    delete_test_list(list_service, list_id);
    printf("\n===== %s Passed =====\n", __FUNCTION__);
}

static void test_invalid_list_id(const struct generic_list_service *list_service)
{
    printf("\n===== %s Start =====\n", __FUNCTION__);

    assert(list_service->is_valid_list_id(0) == 0);
    assert(list_service->insert(0, create_test_int_data(50)) == -1);
    assert(list_service->insert_at(0, 1, create_test_int_data(60)) == -1);
    assert(list_service->append(10000, 2) == 0);
    assert(list_service->append(2, 10000) == 0);
    assert(list_service->size(0) == -1);

    data_t data;
    memset(&data, 0, sizeof (data_t));
    assert(list_service->get_data(0, 1, &data) == 0);
    assert(data.p == NULL);

    unsigned int list_id = list_service->create_empty_list();
    assert(list_service->is_valid_list_id(list_id) == 1);
    assert(list_service->insert(list_id, create_test_int_data(20)) == 0);
    list_service->delete_list(0);
    assert(list_service->size(list_id) == 1);

    list_service->clear(0);
    assert(list_service->size(list_id) == 1);

    delete_test_list(list_service, list_id);

    printf("\n===== %s Passed =====\n", __FUNCTION__);
}

static void test_edit_node(const struct generic_list_service *list_service)
{
    printf("\n===== %s Start =====\n", __FUNCTION__);
}

static void test_delete(const struct generic_list_service *list_service)
{
    printf("\n===== %s Start =====\n", __FUNCTION__);

    unsigned int list_id = list_service->create_empty_list();
    if (list_id == 0) {
        printf("\ncreate_empty_list failed\n");
        return;
    }

    assert(list_service->insert(list_id, create_test_int_data(2)) == 0);

    data_t data;
    memset(&data, 0, sizeof (data_t));
    assert(list_service->delete_node(0, 0, &data) == 0);
    assert(list_service->delete_node(list_id, 0, &data) == 1);
    assert(list_service->size(list_id) == 0);
    assert(list_service->is_valid_list_id(list_id) == 1);
    assert(data.type_id == INT);
    assert(*(int *) data.p == 2);
    assert(list_service->delete_node(list_id, 1, &data) == 0);

    int i = 0;
    for (i; i < 10; i++) {
        assert(list_service->insert(list_id, create_test_int_data(i * 2 + 2)) == 0);
    }
    
    memset(&data, 0, sizeof (data_t));
    assert(list_service->delete_node(list_id, 0, &data) == 1);
    assert(data.type_id == INT);
    assert(*(int *) data.p == 2);
    memset(&data, 0, sizeof (data_t));
    int last_index = list_service->size(list_id);
    assert(list_service->delete_node(list_id, last_index - 1, &data) == 1);
    assert(data.type_id == INT);
    assert(*(int *) data.p == 20);

    memset(&data, 0, sizeof (data_t));
    assert(list_service->delete_node(list_id, last_index, &data) == 0);
    assert(data.type_id == 0);
    assert(data.p == NULL);

    memset(&data, 0, sizeof (data_t));
    assert(list_service->delete_node(0, 0, &data) == 0);
    assert(data.type_id == 0);
    assert(data.p == NULL);

    assert(list_service->delete_node(list_id, last_index - 1, NULL) == 0);

    list_service->delete_list(list_id);
    assert(list_service->is_valid_list_id(list_id) == 0);

    delete_test_list(list_service, list_id);

    printf("\n===== %s Passed =====\n", __FUNCTION__);
}

static void test_clear(const struct generic_list_service *list_service)
{
    printf("\n===== %s Start =====\n", __FUNCTION__);

    unsigned int list_id = list_service->create_empty_list();
    if (list_id == 0) {
        printf("\ncreate_empty_list failed\n");
        return;
    }

    assert(list_service->insert(list_id, create_test_int_data(2)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(4)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(6)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(8)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(10)) == 0);
    list_service->clear(list_id);
    assert(list_service->is_valid_list_id(list_id) == 1);
    assert(list_service->size(list_id) == 0);

    assert(list_service->insert(list_id, create_test_int_data(12)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(14)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(16)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(18)) == 0);
    assert(list_service->insert(list_id, create_test_int_data(20)) == 0);
    list_service->clear(list_id);
    assert(list_service->is_valid_list_id(list_id) == 1);
    assert(list_service->size(list_id) == 0);

    list_service->clear(list_id);
    assert(list_service->is_valid_list_id(list_id) == 1);
    assert(list_service->size(list_id) == 0);

    delete_test_list(list_service, list_id);

    printf("\n===== %s Passed =====\n", __FUNCTION__);
}

static void test_get_data(const struct generic_list_service * list_service)
{
    printf("\n===== %s Start =====\n", __FUNCTION__);

    unsigned int list_id = list_service->create_empty_list();
    if (list_id == 0) {
        printf("\ncreate_empty_list failed\n");
        return;
    }

    int i = 0;
    for (; i < TEST_INSERT_DATA_COUNT; i++)
        assert(list_service->insert(list_id, create_test_int_data(i * 2 + 2)) == 0);

    data_t data;
    memset(&data, 0, sizeof (data_t));
    assert(list_service->get_data(list_id, 0, &data) == 1);
    assert(data.type_id == INT);
    assert(*(int *) data.p == 2);

    int last_index = list_service->size(list_id);
    assert(list_service->get_data(list_id, last_index - 1, &data) == 1);
    assert(data.type_id == INT);
    assert(*(int *) data.p == (TEST_INSERT_DATA_COUNT - 1) * 2 + 2);

    memset(&data, 0, sizeof (data_t));
    assert(list_service->get_data(list_id, last_index, &data) == 0);
    assert(data.p == NULL);
    assert(data.type_id == 0);

    memset(&data, 0, sizeof (data_t));
    assert(list_service->get_data(0, 0, &data) == 0);
    assert(data.p == NULL);
    assert(data.type_id == 0);

    assert(list_service->get_data(list_id, 0, NULL) == 0);

    delete_test_list(list_service, list_id);
    printf("\n===== %s Passed =====\n", __FUNCTION__);
}

static void test_reverse(const struct generic_list_service * list_service)
{
    printf("\n===== %s Start =====\n", __FUNCTION__);

    unsigned int list_id = list_service->create_empty_list();
    if (list_id == 0) {
        printf("\ncreate_empty_list failed\n");
        return;
    }

    list_service->reverse(list_id);
    assert(list_service->is_valid_list_id(list_id) == 1);
    assert(list_service->size(list_id) == 0);

    int i = 0;
    for (; i < TEST_INSERT_DATA_COUNT; i++)
        assert(list_service->insert(list_id, create_test_int_data(i * 2 + 2)) == 0);

    int s = list_service->size(list_id);

    list_service->print_list(list_id);
    list_service->reverse(list_id);
    list_service->print_list(list_id);
    assert(list_service->is_valid_list_id(list_id) == 1);
    assert(list_service->size(list_id) == s);

    i = 0;
    s = list_service->size(list_id);
    data_t data1;
    for (; i < s; i++) {
        memset(&data1, 0, sizeof (data_t));
        assert(list_service->get_data(list_id, i, &data1) == 1);
        assert(data1.type_id == INT);
        assert(*(int *) data1.p == (2 * TEST_INSERT_DATA_COUNT) - (i * 2));
    }

    delete_test_list(list_service, list_id);
    printf("\n===== %s Passed =====\n", __FUNCTION__);
}

static void test_append(const struct generic_list_service * list_service)
{

    printf("\n===== %s Start =====\n", __FUNCTION__);

    unsigned int list1_id = list_service->create_empty_list();
    unsigned int list2_id = list_service->create_empty_list();
    if (list1_id == 0 || list2_id == 0) {
        printf("\ncreate_empty_list failed\n");
        return;
    }

    assert(list_service->append(list1_id, list2_id) == 1);
    assert(list_service->size(list1_id) == 0);
    assert(list_service->is_valid_list_id(list2_id) == 0);

    int i = 0;
    for (; i < TEST_INSERT_DATA_COUNT; i++)
        assert(list_service->insert(list1_id, create_test_int_data(i * 2 + 2)) == 0);

    assert(list_service->size(list1_id) == TEST_INSERT_DATA_COUNT);

    list_service->print_list(list1_id);

    list2_id = list_service->create_empty_list();
    i = TEST_INSERT_DATA_COUNT;
    for (; i < (TEST_INSERT_DATA_COUNT * 2); i++)
        assert(list_service->insert(list2_id, create_test_int_data(i * 2 + 2)) == 0);

    assert(list_service->append(list1_id, list2_id) == 1);
    assert(list_service->is_valid_list_id(list2_id) == 0);

    int s = list_service->size(list1_id);
    assert(s == TEST_INSERT_DATA_COUNT * 2);

    i = 0;
    data_t data1;
    for (; i < s; i++) {
        memset(&data1, 0, sizeof (data_t));
        assert(list_service->get_data(list1_id, i, &data1) == 1);
        assert(data1.type_id == INT);
        printf(" For %d \n", *(int *) data1.p);
        assert(*(int *) data1.p == i * 2 + 2);
    }

    delete_test_list(list_service, list1_id);
    printf("\n===== %s Passed =====\n", __FUNCTION__);
}

//long int timespec_difference_microseconds(struct timespec *t1, struct timespec *t2)
//{
//    if (!t1 || !t2)
//        return 0;
//
//    // long int = t1->tv_sec
//    
//}

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return (diff < 0);
}

void timeval_print(struct timeval *tv)
{
    char buffer[30];
    time_t curtime;

    printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
    curtime = tv->tv_sec;
    strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
    printf(" = %s.%06ld\n", buffer, tv->tv_usec);
}
