/* Author: Aidan Bush
 * Date: Mar. 2, 2018
 * File test.c
 * Description: ring buffer test file.
 */
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#include "ring_buffer.h"

#define RING_TEST_LEN 16

void test_init() {
    // size 0
    ring_buf_s *buf1 = init_ring_buf(0);
    free_ring_buf(buf1, NULL);

    // size 1
    ring_buf_s *buf2 = init_ring_buf(1);
    free_ring_buf(buf2, NULL);

    // max size
    ring_buf_s *buf3 = init_ring_buf(UCHAR_MAX);
    free_ring_buf(buf3, NULL);

    // random size
    srand(time(NULL));
    uint8_t r = (uint8_t) rand();
    ring_buf_s *buf4 = init_ring_buf(r);
    free_ring_buf(buf4, NULL);
}

void test_align() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-conversion"
    ring_buf_s *buf1 = init_ring_buf(RING_TEST_LEN);
    buf1->s_pos = 0;
    buf1->len = 3;
    memcpy(buf1->ring, (void*[]){1,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0}, RING_TEST_LEN * sizeof(void*));

    ring_buf_s *buf2 = init_ring_buf(RING_TEST_LEN);
    buf2->s_pos = 1;
    buf2->len = 3;
    memcpy(buf2->ring, (void*[]){0,1,2,3,0,0,0,0,0,0,0,0,0,0,0,0}, RING_TEST_LEN * sizeof(void*));

    ring_buf_s *buf3 = init_ring_buf(RING_TEST_LEN);
    buf3->s_pos = 15;
    buf3->len = 3;
    memcpy(buf3->ring, (void*[]){2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, RING_TEST_LEN * sizeof(void*));

    ring_buf_s *buf4 = init_ring_buf(RING_TEST_LEN);
    buf4->s_pos = 5;
    buf4->len = 16;
    memcpy(buf4->ring, (void*[]){12,13,14,15,16,1,2,3,4,5,6,7,8,9,10,11}, RING_TEST_LEN * sizeof(void*));

    ring_buf_s *buf5 = init_ring_buf(RING_TEST_LEN);
    buf5->s_pos = 7;
    buf5->len = 12;
    memcpy(buf5->ring, (void*[]){10,11,12,13,14,15,16,1,2,3,4,5,6,7,8,9}, RING_TEST_LEN * sizeof(void*));

    void *sol1[] = {1,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0};
    void *sol2[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
#pragma GCC diagnostic pop

    uint8_t sol_s_pos = 0;
    uint8_t sol1_len = 3;
    uint8_t sol2_len = 16;
    uint8_t sol3_len = 12;

    align_ring_buf(buf1);
    align_ring_buf(buf2);
    align_ring_buf(buf3);
    align_ring_buf(buf4);
    align_ring_buf(buf5);

    assert(memcmp(buf1->ring, sol1, RING_TEST_LEN * sizeof(void *)) == 0);
    assert(memcmp(buf2->ring, sol1, RING_TEST_LEN * sizeof(void *)) == 0);
    assert(memcmp(buf3->ring, sol1, RING_TEST_LEN * sizeof(void *)) == 0);
    assert(memcmp(buf4->ring, sol2, RING_TEST_LEN * sizeof(void *)) == 0);
    assert(memcmp(buf5->ring, sol2, RING_TEST_LEN * sizeof(void *)) == 0);

    assert(buf1->s_pos == sol_s_pos);
    assert(buf2->s_pos == sol_s_pos);
    assert(buf3->s_pos == sol_s_pos);
    assert(buf4->s_pos == sol_s_pos);
    assert(buf5->s_pos == sol_s_pos);

    assert(buf1->len == sol1_len);
    assert(buf2->len == sol1_len);
    assert(buf3->len == sol1_len);
    assert(buf4->len == sol2_len);
    assert(buf5->len == sol3_len);

    free_ring_buf(buf1, NULL);
    free_ring_buf(buf2, NULL);
    free_ring_buf(buf3, NULL);
    free_ring_buf(buf4, NULL);
    free_ring_buf(buf5, NULL);
}

void test_insert() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-conversion"
    // insert on NULL struct
    ring_buf_s *buf1 = NULL;

    // insert empty
    ring_buf_s *buf2 = init_ring_buf(RING_TEST_LEN);
    memcpy(buf2->ring, (void*[]){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, RING_TEST_LEN * sizeof(void*));

    // insert with item
    ring_buf_s *buf3 = init_ring_buf(RING_TEST_LEN);
    buf3->s_pos = 0;
    buf3->len = 1;
    memcpy(buf3->ring, (void*[]){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, RING_TEST_LEN * sizeof(void*));

    // insert force wrap
    ring_buf_s *buf4 = init_ring_buf(RING_TEST_LEN);
    buf4->s_pos = 15;
    buf4->len = 1;
    memcpy(buf4->ring, (void*[]){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, RING_TEST_LEN * sizeof(void*));

    // insert full
    ring_buf_s *buf5 = init_ring_buf(RING_TEST_LEN);
    buf5->s_pos = 0;
    buf5->len = RING_TEST_LEN;
    memcpy(buf5->ring, (void*[]){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, RING_TEST_LEN * sizeof(void*));

    void *sol2[] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    void *sol3[] = {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    void *sol4[] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    void *sol5[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
#pragma GCC diagnostic pop

    uint8_t sol1_res = 0;
    uint8_t sol2_res = 1;
    uint8_t sol3_res = 2;
    uint8_t sol4_res = 2;
    uint8_t sol5_res = 0;

    uint8_t sol2_s_pos = 0;
    uint8_t sol3_s_pos = 0;
    uint8_t sol4_s_pos = 15;
    uint8_t sol5_s_pos = 0;

    uint8_t sol2_len = 1;
    uint8_t sol3_len = 2;
    uint8_t sol4_len = 2;
    uint8_t sol5_len = RING_TEST_LEN;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-conversion"
    int ins1_ret = insert_ring_buf(buf1, 1);
    int ins2_ret = insert_ring_buf(buf2, 1);
    int ins3_ret = insert_ring_buf(buf3, 1);
    int ins4_ret = insert_ring_buf(buf4, 1);
    int ins5_ret = insert_ring_buf(buf5, 1);
#pragma GCC diagnostic pop

    assert(ins1_ret == sol1_res);
    assert(ins2_ret == sol2_res);
    assert(ins3_ret == sol3_res);
    assert(ins4_ret == sol4_res);
    assert(ins5_ret == sol5_res);

    assert(memcmp(buf2->ring, sol2, RING_TEST_LEN * sizeof(void *)) == 0);
    assert(memcmp(buf3->ring, sol3, RING_TEST_LEN * sizeof(void *)) == 0);
    assert(memcmp(buf4->ring, sol4, RING_TEST_LEN * sizeof(void *)) == 0);
    assert(memcmp(buf5->ring, sol5, RING_TEST_LEN * sizeof(void *)) == 0);

    assert(buf2->s_pos == sol2_s_pos);
    assert(buf3->s_pos == sol3_s_pos);
    assert(buf4->s_pos == sol4_s_pos);
    assert(buf5->s_pos == sol5_s_pos);

    assert(buf2->len == sol2_len);
    assert(buf3->len == sol3_len);
    assert(buf4->len == sol4_len);
    assert(buf5->len == sol5_len);

    free_ring_buf(buf2, NULL);
    free_ring_buf(buf3, NULL);
    free_ring_buf(buf4, NULL);
    free_ring_buf(buf5, NULL);
}

void test_wrap_insert() {
    // insert on NULL struct
    // insert empty
    // insert with item
    // insert with wrap
    // insert full
}

void test_over_insert() {
    // insert on NULL struct
    // insert empty
    // insert with item
    // insert force wrap
    // insert full
}

void test_peek() {}

void test_pop() {}

void test_func() {}

void test_sort() {}

int main() {
    test_init();
    test_align();
    test_insert();

    return 0;
}
