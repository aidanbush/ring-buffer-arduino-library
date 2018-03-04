#include <stdint.h>
#include <stdlib.h>

#include "buffer.h"

/* resets the ring buffer using the free function to free all elements */
void reset_ring_buf(ring_buf_s *buf, void(*free_func)(const void *)) {
    if (buf == NULL)
        return;

    buf->s_pos = 0;
    buf->len = 0;

    if (free_func != NULL) {
        for (int i = 0; i < RING_MAX_LEN; i++) {
            free_func(buf->ring[i]);
        }
    }

    for (int i = 0; i < RING_MAX_LEN; i++) {
        buf->ring[i] = NULL;
    }
}

/* add the given element to the ring buffer */
int insert_ring_buf(ring_buf_s *buf, void *new_elem) {
    if (buf == NULL || buf->len >= (RING_MAX_LEN))
        return 0;

    int pos = (buf->s_pos + buf->len) % (RING_MAX_LEN);
    buf->ring[pos] = new_elem;
    return ++buf->len;
}

/* insert the given element overwriting the first if need be */
int insert_wrap_ring_buf(ring_buf_s *buf, void *new_elem) {
    if (buf == NULL)
        return 0;

    int pos = (buf->s_pos + buf->len) % (RING_MAX_LEN);
    buf->ring[pos] = new_elem;

    if (buf->len < (RING_MAX_LEN))
        buf->len++;
    return buf->len;
}

/* insert the given element overwriting the last if need be */
int insert_over_ring_buf(ring_buf_s *buf, void *new_elem) {
    if (buf == NULL)
        return 0;

    int pos;
    if (buf->len < (RING_MAX_LEN)) {
        pos = (buf->s_pos + buf->len - 1) % (RING_MAX_LEN);
    } else {
        pos = (buf->s_pos + buf->len) % (RING_MAX_LEN);
        buf->len++;
    }

    buf->ring[pos] = new_elem;
    return buf->len;
}

/* peak at the top element of the ring buffer */
void *peak_ring_buf(ring_buf_s *buf) {
    if (buf == NULL)
        return NULL;

    if (buf->len <= 0)
        return NULL;
    return buf->ring[buf->s_pos];
}

/* pop the top element off the ring buffer */
void *pop_ring_buf(ring_buf_s *buf) {
    if (buf == NULL)
        return NULL;

    if (buf->len <= 0)
        return NULL;

    uint8_t pos = buf->s_pos++;
    buf->s_pos %= (RING_MAX_LEN);
    buf->len--;

    return buf->ring[pos];
}

/* applies the given function to all elements in the ring breaking on halt_val */
uint16_t apply_func_ring_buf(ring_buf_s *buf, uint16_t(func)(void **), uint16_t halt_val) {
    if (buf == NULL)
        return 0;

    uint16_t ret_tmp, ret = 0;
    int cur;

    for (int i = 0; i < buf->len; i++) {
        cur = (buf->s_pos + i) % (RING_MAX_LEN);

        ret_tmp = func(&buf->ring[cur]);
        ret |= ret_tmp;

        if (ret_tmp == halt_val)
            break;
    }
    return ret;
}

/* aligns the ring buffer so the start is at index 0 */
void align_ring_buf(ring_buf_s *buf) {
    if (buf == NULL)
        return;

    int i, i2;
    int c = 0, off = 0;
    void *tmp;

    while(c < (RING_MAX_LEN)) {
        i = c;
        tmp = buf->ring[i];
        i2 = (buf->s_pos + i) % (RING_MAX_LEN);

        while (i2 != off) {
            buf->ring[i] = buf->ring[i2];
            i = i2;
            i2 = (buf->s_pos + i) % (RING_MAX_LEN);

            c++;
        }

        buf->ring[i] = tmp;
        c++;
        off++;
    }

    buf->s_pos = 0;
}

/* sorts the given ring buffer */
void sort_ring_buf(ring_buf_s *buf, int(*compare)(const void *, const void *)) {
    if (buf == NULL)
        return;

    if (buf->s_pos != 0)
        align_ring_buf(buf);

    qsort(buf->ring, buf->len, sizeof(void *), compare);

    return;
}
