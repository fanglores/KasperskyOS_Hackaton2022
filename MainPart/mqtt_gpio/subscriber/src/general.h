/* © 2021 AO Kaspersky Lab */
#ifndef _PUBLISHER_GENERAL_H
#define _PUBLISHER_GENERAL_H

#pragma once
#include <map>
#include <string>

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
    uint16_t type;
    uint16_t arg;
    
    uint16_t to_gpio()
    {   
        if (arg >= 1<<13) std::cout << "Warning! Argument part has been lost!" << std::endl;
        return static_cast<uint16_t>(type | (arg << 4));
    }
};

namespace app {
constexpr const char *AppTag = "[Subscriber] ";
}

#endif // _PUBLISHER_GENERAL_H
