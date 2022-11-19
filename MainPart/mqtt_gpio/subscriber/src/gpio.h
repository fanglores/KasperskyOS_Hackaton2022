#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpio/gpio.h>
#include <stdbool.h>
#include <bsp/bsp.h>
#include <sys/cdefs.h>
#include <rtl/countof.h>

#define GPIO_PIN_NUM    RTL_UINT32_C(28)
#define DELAY_S         2
#define HW_MODULE_NAME  "gpio0"
#define HW_MODULE_CFG   "raspberry_pi4b.default"

#define GPIO_PIN_UART_TX 14
#define GPIO_PIN_UART_RX 15

#include "common.h"

// 6, 26 - turn on/off
// 6, 12, 21 - eng1
// 12, 13, 26 - eng2
const rtl_uint32_t pinArray[6] = {6, 12, 13, 20, 21, 26};

class GPIOController
{
private:
    GpioHandle handle;
    
    int forward(uint16_t time);
    int back(uint16_t time);
    int right(uint16_t time);
    int left(uint16_t time);
    int stop();
public:
    GPIOController(bool& flag);
    int run(uint16_t command);
    ~GPIOController();
};
