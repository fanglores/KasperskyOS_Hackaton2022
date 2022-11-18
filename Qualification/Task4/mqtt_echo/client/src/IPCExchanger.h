#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Files required for transport initialization. */
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>

/* Description of the server interface used by the `client` entity. */
#include <echo/Ping.idl.h>

#include <assert.h>

#include <string>
#include <map>

#define MQTT_MASK 0xF

const std::map<std::string, uint16_t> COMMAND_OP = {
    {"forward", 0x1},
    {"backward", 0x2},
    {"left", 0x3},
    {"right", 0x4},
    {"exit", 0xF}
};

uint16_t parseMqttString(const std::string& str);

class IPCExchanger
{
public:
    NkKosTransport transport;
    struct echo_Ping_proxy proxy;
    
    echo_Ping_Ping_req req;
    echo_Ping_Ping_res res;
    
    IPCExchanger();
    int callEcho(const std::string& msg);

};
