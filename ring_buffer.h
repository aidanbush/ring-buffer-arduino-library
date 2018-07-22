/* Author: Aidan Bush
 * Date: Mar. 2, 2018
 * File buffer.h
 * Description: Header file for ring buffer.
 */
#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

typedef struct ring_buf_s {
    uint8_t s_pos;
    uint8_t len;
    uint8_t size;
    void **ring;
} ring_buf_s;

ring_buf_s *init_ring_buf(uint8_t size);

/*
 * Resets the ring buffer using the free function to free all elements
 * sets the length and starting position to 0
 * calls the free_func on all elements in the ring unless they are NULL
 * after freeing each elements it sets them to NULL
 */
void free_ring_buf(ring_buf_s *buf, void(*free_func)(const void *));

/*
 * Adds the given element to the ring buffer if there is space
 * returns 0 on error and the new length on success
 */
int insert_ring_buf(ring_buf_s *buf, void *new_elem);

/*
 * Insert the given element overwriting the first if need be
 * returns the new length
 */
int insert_wrap_ring_buf(ring_buf_s *buf, void *new_elem);

/*
 * Insert the given element overwriting the last if need be
 * returns the new length
 */
int insert_over_ring_buf(ring_buf_s *buf, void *new_elem);

/*
 * Peek at the top element of the ring buffer
 * returns that element or NULL if empty
 */
void *peek_ring_buf(ring_buf_s *buf);

/*
 * Pop the top element off the ring buffer
 * returns the element or NULL if it is empty or if buf = NULL
 */
void *pop_ring_buf(ring_buf_s *buf);

/*
 * Applies the given function to all elements in the element
 * breaks from appling the function if the halt_val is returned
 * returns the or of all return values from the function
 */
uint16_t apply_func_ring_buf(ring_buf_s *buf, uint16_t(func)(void **), uint16_t halt_val);

/*
 * Aligns the ring buffer so the start is at index 0
 */
void align_ring_buf(ring_buf_s *buf);

/*
 * Sorts the given ring buffer
 * takes a compare function with the same setup as to what qsort requires
 * designed to be time not space efficient
 */
void sort_ring_buf(ring_buf_s *buf, int(*compare)(const void *, const void *));

#endif /* BUFFER_H */
