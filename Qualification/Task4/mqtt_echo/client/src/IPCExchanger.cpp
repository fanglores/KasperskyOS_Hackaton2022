#include "IPCExchanger.h"

IPCExchanger::IPCExchanger()
{
    Handle handle = ServiceLocatorConnect("server_connection");
    assert(handle != INVALID_HANDLE);

    NkKosTransport_Init(&transport, handle, NK_NULL, 0);

    nk_iid_t riid = ServiceLocatorGetRiid(handle, "echo.Ping.ping");
    assert(riid != INVALID_RIID);

    echo_Ping_proxy_init(&proxy, &transport.base, riid);
}

uint16_t parseMqttString(const std::string& str)
{
    return 0x0;
}

int IPCExchanger::callEcho(const std::string& msg)
{
    req.value = parseMqttString(msg);

    if (echo_Ping_Ping(&proxy.base, &req, NULL, &res, NULL) == rcOk)
    {
        return res.result;
    }
    else
    {
        fprintf(stderr, "Failed to call echo.Ping.Ping()\n");
        return EXIT_FAILURE;
    }
}
