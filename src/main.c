#include "bsp/bsp.h"
#include "serial/serial.h"
#include "misc/crc.h"

volatile uint16_t message[6] = {0x6865, 0x6C6C, 0x6F20, 0x776F, 0x726C, 0x6400}; // 'hello world'
uint32_t crc;

int main(void) {
    __bsp_init();

    GPIO_setPadConfig(GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);
    GPIO_setAnalogMode(GPIO_PIN_LED1, GPIO_ANALOG_DISABLED);

    DINT;
    serial_init();
    EINT;
    crc = get_crc_32((uint16_t*)message, 11);

    while (1) {
        DEVICE_DELAY_US(1000000U);
        GPIO_togglePin(GPIO_PIN_LED1);
    }
}