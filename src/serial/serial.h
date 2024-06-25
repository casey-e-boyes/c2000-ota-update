#ifndef __INCLUDE_SERIAL_H
#define __INCLUDE_SERIAL_H

#include "../bsp/bsp.h"
#include "../misc/crc.h"
#include "../misc/buffers.h"

#define BYTE_LOW  0U
#define BYTE_HIGH 1U

#define SERIAL_RX_PIN 28U
#define SERIAL_RX_CFG GPIO_28_SCIA_RX

#define SERIAL_TX_PIN 29U
#define SERIAL_TX_CFG GPIO_29_SCIA_TX


#define SERIAL_BASE   SCIA_BASE
#define SERIAL_BAUD   9600U
#define SERIAL_CFG    (SCI_CONFIG_WLEN_8|SCI_CONFIG_STOP_ONE|SCI_CONFIG_PAR_NONE)

#define SERIAL_RX_BUF_LEN  256U
#define SERIAL_TX_BUF_LEN  256U
#define SERIAL_BUF_TYPE_SZ 1U

void serial_init(void);
bool serial_is_rx_buf_full(void);

#endif /* __INCLUDE_SERIAL_H */