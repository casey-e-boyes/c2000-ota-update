#include "bsp.h"

void __bsp_init(void) {
    SysCtl_disableWatchdog();
    SysCtl_setClock(DEVICE_SETCLOCK_CFG);
    SysCtl_setLowSpeedClock(SYSCTL_LSPCLK_PRESCALE_4);

    ASSERT(SysCtl_getClock(DEVICE_OSCSRC_FREQ) == DEVICE_SYSCLK_FREQ);
    ASSERT(SysCtl_getLowSpeedClock(DEVICE_OSCSRC_FREQ) == DEVICE_LSPCLK_FREQ);

    SysCtl_deviceCal();
    ASysCtl_disableDCDC();

    __unlock_gpio_ports();
    __enable_peripherals();
    __interrupt_init();
}

void __unlock_gpio_ports(void) {
    GPIO_unlockPortConfig(GPIO_PORT_A, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_B, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_H, 0xFFFFFFFF);
}

void __enable_peripherals(void) {
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DMA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER0);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SD1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SCIA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DCC0);
}

void __interrupt_init(void) {
    Interrupt_initModule();
    Interrupt_initVectorTable();

    EINT;
    ERTM;
}

void __error__(const char* filename, uint32_t line) {
    ESTOP0;
}