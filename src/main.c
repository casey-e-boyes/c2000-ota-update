#include "bsp/bsp.h"
#include "serial/serial.h"

int main(void) {
    __bsp_init();

    GPIO_setPadConfig(GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);
    GPIO_setAnalogMode(GPIO_PIN_LED1, GPIO_ANALOG_DISABLED);

    DINT;
    serial_init();
    EINT;

    while (1) {
        DEVICE_DELAY_US(1000000U);
        GPIO_togglePin(GPIO_PIN_LED1);
    }
}