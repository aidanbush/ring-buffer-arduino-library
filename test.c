/* Author: Aidan Bush
 * Date: Mar. 2, 2018
 * File test.c
 * Description: ring buffer test file.
 */
#define RING_MAX_LEN 16

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "buffer.h"

void test_align() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-conversion"
    ring_buf_s buf1 = {
        .s_pos = 0,
        .len = 3,
        .ring = {1,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0},
    };

    ring_buf_s buf2 = {
        .s_pos = 1,
        .len = 3,
        .ring = {0,1,2,3,0,0,0,0,0,0,0,0,0,0,0,0},
    };

    ring_buf_s buf3 = {
        .s_pos = 15,
        .len = 3,
        .ring = {2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    };

    ring_buf_s buf4 = {
        .s_pos = 5,
        .len = 16,
        .ring = {12,13,14,15,16,1,2,3,4,5,6,7,8,9,10,11},
    };

    ring_buf_s buf5 = {
        .s_pos = 7,
        .len = 12,
        .ring = {10,11,12,13,14,15,16,1,2,3,4,5,6,7,8,9},
    };

    void *sol1[] = {1,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0};
    void *sol2[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
#pragma GCC diagnostic pop
    uint8_t sol_s_pos = 0;
    uint8_t sol1_len = 3;
    uint8_t sol2_len = 16;
    uint8_t sol3_len = 12;

    align_ring_buf(&buf1);
    align_ring_buf(&buf2);
    align_ring_buf(&buf3);
    align_ring_buf(&buf4);
    align_ring_buf(&buf5);

    assert(memcmp(buf1.ring, sol1, RING_MAX_LEN * sizeof(void *)) == 0);
    assert(memcmp(buf2.ring, sol1, RING_MAX_LEN * sizeof(void *)) == 0);
    assert(memcmp(buf3.ring, sol1, RING_MAX_LEN * sizeof(void *)) == 0);
    assert(memcmp(buf4.ring, sol2, RING_MAX_LEN * sizeof(void *)) == 0);
    assert(memcmp(buf5.ring, sol2, RING_MAX_LEN * sizeof(void *)) == 0);

    assert(buf1.s_pos == sol_s_pos);
    assert(buf2.s_pos == sol_s_pos);
    assert(buf3.s_pos == sol_s_pos);
    assert(buf4.s_pos == sol_s_pos);
    assert(buf5.s_pos == sol_s_pos);

    assert(buf1.len == sol1_len);
    assert(buf2.len == sol1_len);
    assert(buf3.len == sol1_len);
    assert(buf4.len == sol2_len);
    assert(buf5.len == sol3_len);
}

void test_reset_ring_buf() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-conversion"
    ring_buf_s buf1 = {
        .s_pos = 0,
        .len = 0,
        .ring = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    };

    ring_buf_s buf2 = {
        .s_pos = 5,
        .len = 16,
        .ring = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},
    };

    ring_buf_s buf3 = {
        .s_pos = 5,
        .len = 8,
        .ring = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},
    };

    void *sol_ring[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
#pragma GCC diagnostic pop
    uint8_t sol_s_pos = 0;
    uint8_t sol_len = 0;

    reset_ring_buf(&buf1, NULL);
    reset_ring_buf(&buf2, NULL);
    reset_ring_buf(&buf3, NULL);

    assert(memcmp(buf1.ring, sol_ring, RING_MAX_LEN * sizeof(void *)) == 0);
    assert(memcmp(buf2.ring, sol_ring, RING_MAX_LEN * sizeof(void *)) == 0);
    assert(memcmp(buf3.ring, sol_ring, RING_MAX_LEN * sizeof(void *)) == 0);

    assert(buf1.s_pos == sol_s_pos);
    assert(buf2.s_pos == sol_s_pos);
    assert(buf3.s_pos == sol_s_pos);

    assert(buf1.len == sol_len);
    assert(buf2.len == sol_len);
    assert(buf3.len == sol_len);
}

int main() {
    test_align();
    test_reset_ring_buf();

    return 0;
}
