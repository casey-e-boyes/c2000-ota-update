#ifndef __INCLUDE_CIRCULAR_BUFFER_H
#define __INCLUDE_CIRCULAR_BUFFER_H

#include <stdint.h>


/*! Circular Buffer Type
 *
 * @param len   : The number of items that can be in the buffer
 * @param sz    : The size of item type
 * @param buf_s : Start address of the buffer
 * @param buf_e : End address of the buffer
 * @param head  : Pointer to the next write entry location
 * @param tail  : Pointer to the first write entry 
 */

typedef struct circular_buffer_t {
    uint16_t len;
    uint16_t sz;
    void* buf_s;
    void* buf_e;
    void* head;
    void* tail;
} circular_buffer;

#endif /* __INCLUDE_CIRCULAR_BUFFER_H */