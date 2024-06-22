#ifndef __INCLUDE_BSP_H
#define __INCLUDE_BSP_H

#include <stdint.h>
#include "driverlib.h"

#define DEVICE_OSCSRC_FREQ          20000000U
#define DEVICE_SYSCLK_FREQ          ((DEVICE_OSCSRC_FREQ * 10 * 1) / 2)
#define DEVICE_LSPCLK_FREQ          (DEVICE_SYSCLK_FREQ / 4)

#define DEVICE_SETCLOCK_CFG         (SYSCTL_OSCSRC_XTAL | SYSCTL_IMULT(10) |  \
                                     SYSCTL_FMULT_NONE | SYSCTL_SYSDIV(2) |   \
                                     SYSCTL_PLL_ENABLE)

#define DEVICE_DELAY_US(x)          SysCtl_delay(((((float)(x)) / (1000000.0f /  \
                                    (float)DEVICE_SYSCLK_FREQ)) - 9.0f) / 5.0f)


#define GPIO_PIN_LED1               23U


extern void __bsp_init(void);
extern void __unlock_gpio_ports(void);
extern void __enable_peripherals(void);
extern void __interrupt_init(void);
extern void __error__(const char* filename, uint32_t line);

#endif /* __INCLUDE_BSP_H */