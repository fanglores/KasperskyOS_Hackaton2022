/* © 2021 AO Kaspersky Lab */
#ifndef _MOSQUITTO_SUBSCRIBER_H
#define _MOSQUITTO_SUBSCRIBER_H
#pragma once
#include <stdio.h>
#include <stdlib.h>

#include <gpio/gpio.h>
#include <stdbool.h>
#include <sys/cdefs.h>
#include <rtl/countof.h>
#include <unistd.h>
#include <mosquittopp.h>
#include <rtl/compiler.h>
#include <bsp/bsp.h>

#define HW_MODULE_NAME  "gpio0"
#define HW_MODULE_CFG   "raspberry_pi4b.default"

const rtl_uint32_t pinArray[6] = {6, 12, 13, 20, 21, 26};

class GPIOCtrl
{
private:
    GpioHandle gphandle;
    int forward(unsigned int t);
    int back(unsigned int t);
    int right(unsigned int t);
    int left(unsigned int t);
    int stop();
public:
    GPIOCtrl();
    int run(uint16_t cmd);
    ~GPIOCtrl();
};

static GPIOCtrl* gpioctrl;

class Subscriber : public mosqpp::mosquittopp
{
public:
    Subscriber(const char *id, const char *host, int port);
    ~Subscriber() {};
    void run_command(const std::string& str);
    void on_connect(int rc) override;
    void on_message(const struct mosquitto_message *message) override;
    void on_subscribe(__rtl_unused int        mid,
                      __rtl_unused int        qos_count,
                      __rtl_unused const int *granted_qos) override;
};

#endif // _MOSQUITTO_SUBSCRIBER_H
