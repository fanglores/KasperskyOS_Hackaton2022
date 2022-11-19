#pragma once

const uint16_t MQTT_MASK = 0xF;

enum COMMAND : uint16_t
{
    STOP = 0x0,
    FORWARD = 0x1,
    BACK = 0x2,
    LEFT = 0x3,
    RIGHT = 0x4
};
