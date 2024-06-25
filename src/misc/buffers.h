#ifndef __INCLUDE_BUFFERS_H
#define __INCLUDE_BUFFERS_H

#include <stdint.h>
#include <stdbool.h>


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


/*! Linear Buffer Type
 *
 * @param len   : The number of items that can be in the buffer
 * @param sz    : The size of item type
 * @param buf_s : Start address of the buffer
 * @param buf_e : End address of the buffer
 * @param fill  : Length of the current message
 * @param full  : Buffer has an unserviced message in it
 * @param crc   : Computed CRC of the complete message
 */
typedef struct linear_buffer_t {
    uint16_t len;
    uint16_t sz;
    uint16_t* buf;
    uint16_t fill;
    bool full;
    uint32_t crc;
} linear_buffer;

#endif /* __INCLUDE_BUFFERS_H */