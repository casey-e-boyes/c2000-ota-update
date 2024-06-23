#ifndef __INCUDE_CRC_H
#define __INCUDE_CRC_H

#include "../bsp/bsp.h"
#include "vcu0_crc.h"

/* Supported CRC Polynomials for the VCU0 Peripheral */
#define CRC_32_POLY    0x04C11DB7U /* CRC-32 */
#define CRC_16_POLY_P1 0x8005U     /* CRC-16 */
#define CRC_16_POLY_P2 0x1021U     /* CRC-16-CCITT */
#define CRC_8          0x07U       /* CRC-8-CCITT */

inline uint32_t get_crc_32(uint16_t* message, const uint16_t length) {
    CRC_reset();
    uint32_t crc_result_raw;
    uint16_t tmp;
    for (uint16_t i = 0; i < length; i++) {
        tmp = message[i];
        message[i] = __flip16(tmp);
    }

    crc_result_raw = getCRC32_vcu(0xFFFFFFFF, (uint16_t*)message, CRC_parity_even, length); // Byte is refering to 8bit

    uint32_t crc_result = __flip32(crc_result_raw) ^ 0xFFFFFFFF;
    return crc_result;
}

#endif /* __INCUDE_CRC_H */