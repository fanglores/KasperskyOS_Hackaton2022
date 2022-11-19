/* © 2021 AO Kaspersky Lab */
#ifndef _MOSQUITTO_SUBSCRIBER_H
#define _MOSQUITTO_SUBSCRIBER_H

#pragma once

#include <mosquittopp.h>
#include <rtl/compiler.h>
#include <string>
#include "json.hpp"
#include <assert.h>
#include <map>

const uint16_t MQTT_MASK = 0xF;

enum COMMAND : uint16_t
{
    STOP = 0x0,
    FORWARD = 0x1,
    BACK = 0x2,
    LEFT = 0x3,
    RIGHT = 0x4
};

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
    std::string cmd_type;
    std::string cmd_arg;
};

struct GPIO_command
{
    uint16_t cmd_type;
    uint16_t cmd_arg;
    
    //GPIO_command(const JSON_command& jc);
};

void run_command(const std::string& json_string);

class Subscriber : public mosqpp::mosquittopp
{
public:
    Subscriber(const char *id, const char *host, int port);
    ~Subscriber() {};

    void on_connect(int rc) override;
    void on_message(const struct mosquitto_message *message) override;
    void on_subscribe(__rtl_unused int        mid,
                      __rtl_unused int        qos_count,
                      __rtl_unused const int *granted_qos) override;
};

#endif // _MOSQUITTO_SUBSCRIBER_H
