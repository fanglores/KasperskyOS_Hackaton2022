#ifndef _MOSQUITTO_SUBSCRIBER_H
#define _MOSQUITTO_SUBSCRIBER_H

#pragma once

#include <mosquittopp.h>
#include <rtl/compiler.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include <assert.h>
#include <map>
#include "gpio.h"
#include "common.h"

const std::map <std::string, uint16_t> COMMAND_MAPPING
{
    {"stop",    COMMAND::STOP},
    {"forward", COMMAND::FORWARD},
    {"back",    COMMAND::BACK},
    {"left",    COMMAND::LEFT},
    {"right",   COMMAND::RIGHT}
};

struct JSON_command
{
    uint16_t type;
    uint16_t arg;
    
    uint16_t to_gpio()
    {   
        if (arg >= 1<<13) std::cout << "Warning! Argument part has been lost!" << std::endl;
        return static_cast<uint16_t>(type | (arg << 4));
    }
};

void run_command(const std::string& json_string);

class Subscriber : public mosqpp::mosquittopp
{
private:
    GPIOController* gpioctrl;
public:
    Subscriber(const char *id, const char *host, int port, bool& flag);
    ~Subscriber() {};

    void on_connect(int rc) override;
    void on_message(const struct mosquitto_message *message) override;
    void on_subscribe(__rtl_unused int        mid,
                      __rtl_unused int        qos_count,
                      __rtl_unused const int *granted_qos) override;

    void run_command(const std::string& json_str);
    GPIOController* get_gpio();
};

#endif // _MOSQUITTO_SUBSCRIBER_H
