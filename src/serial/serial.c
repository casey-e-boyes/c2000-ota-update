#include "serial.h"

volatile unsigned char rx_buf[SERIAL_RX_BUF_LEN] = {0};

volatile linear_buffer serial_rx_buf = {
    .len  = SERIAL_RX_BUF_LEN,
    .sz   = SERIAL_BUF_TYPE_SZ,
    .buf  = (uint16_t*)rx_buf,
    .fill = 0,
    .full = false,
    .crc  = 0
};


__interrupt void serial_rx_isr(void);
// __interrupt void serial_tx_isr(void);

void serial_init(void) {
    /* Configure Serial RX Pin */
    GPIO_setControllerCore(SERIAL_RX_PIN, GPIO_CORE_CPU1);
    GPIO_setPinConfig(SERIAL_RX_CFG);
    GPIO_setDirectionMode(SERIAL_RX_PIN, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(SERIAL_RX_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(SERIAL_RX_PIN, GPIO_QUAL_ASYNC);

    /* Configure Serial TX Pin */
    GPIO_setControllerCore(SERIAL_TX_PIN, GPIO_CORE_CPU1);
    GPIO_setPinConfig(SERIAL_TX_CFG);
    GPIO_setDirectionMode(SERIAL_TX_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(SERIAL_TX_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(SERIAL_TX_PIN, GPIO_QUAL_ASYNC);

    /* Configure SCI(UART) Peripheral */
    SCI_performSoftwareReset(SERIAL_BASE);
    SCI_setConfig(SERIAL_BASE, DEVICE_LSPCLK_FREQ, SERIAL_BAUD, SERIAL_CFG);
    SCI_resetChannels(SERIAL_BASE);
    SCI_resetRxFIFO(SERIAL_BASE);
    SCI_resetTxFIFO(SERIAL_BASE);
    SCI_clearOverflowStatus(SERIAL_BASE);
    SCI_enableFIFO(SERIAL_BASE);

    /* Register SCI(UART) Interrupts */
    Interrupt_register(INT_SCIA_RX, serial_rx_isr);
    // Interrupt_register(INT_SCIA_TX, serial_tx_isr);

    /* Start SCI(UART) Peripheral */
    SCI_enableModule(SERIAL_BASE);
    SCI_performSoftwareReset(SERIAL_BASE);

    /* Auto Baud Lock & Clear RX FIFO */
    SCI_lockAutobaud(SERIAL_BASE);
    (void) SCI_readCharBlockingFIFO(SERIAL_BASE);

    /* Configure & Enable SCI(UART) Interrupts */
    SCI_setFIFOInterruptLevel(SERIAL_BASE, SCI_FIFO_TX0, SCI_FIFO_RX1);
    SCI_enableInterrupt(SERIAL_BASE, SCI_INT_RXFF); // SCI_INT_TXFF
    SCI_clearInterruptStatus(SERIAL_BASE, SCI_INT_RXFF); // SCI_INT_TXFF
    Interrupt_enable(INT_SCIA_RX);
    // Interrupt_enable(INT_SCIA_TX);
    Interrupt_clearACKGroup(SCIA_INT_GROUP);
}

__interrupt void serial_rx_isr(void) {
    // while (SCI_getRxFIFOStatus(SERIAL_BASE) != SCI_FIFO_RX0) {
        uint16_t chr = SCI_readCharBlockingFIFO(SERIAL_BASE);

        // Packing the 16bit 'Bytes' requires an alternating high/low writes to the upper and lower 8bits
        // from a memory POV this needs to be in the write order of: 1, 0, 3, 2, 5, 4 ... as __byte writes
        // to the lower 8 bits first, which is wrong if loking at the data as usual 8bit bytes
        uint16_t index = (serial_rx_buf.fill % 2) ? serial_rx_buf.fill - 1 : serial_rx_buf.fill + 1;
        serial_rx_buf.fill++;
        __byte((int *)serial_rx_buf.buf, index) = chr;
        if (chr == '!') { // '\r'
            SCI_disableInterrupt(SERIAL_BASE, SCI_INT_RXFF);
            serial_rx_buf.full = true;
            serial_rx_buf.crc = get_crc_32((uint16_t*)serial_rx_buf.buf, serial_rx_buf.fill);
        }
    // }

    SCI_clearInterruptStatus(SERIAL_BASE, SCI_INT_RXFF);
    Interrupt_clearACKGroup(SCIA_INT_GROUP);
}

bool serial_is_rx_buf_full(void) {
    return serial_rx_buf.full;
}