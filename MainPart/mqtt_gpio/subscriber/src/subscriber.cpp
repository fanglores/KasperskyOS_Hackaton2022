#include <iostream>
#include <string_view>

#include "general.h"
#include "subscriber.h"
#include "json.hpp"

GPIOCtrl::GPIOCtrl()
{
Retcode rc = BspInit(NULL);
    if (BSP_EOK != rc)
    {
        fprintf(stderr, "Failed to initialize BSP, error code: %d.\n", RC_GET_CODE(rc));
        throw 1;
    }
    
    rc = BspSetConfig(HW_MODULE_NAME, HW_MODULE_CFG);
    if (rcOk != rc)
    {
        fprintf(stderr, "Failed to set mux configuration for %s module, error code: %d.\n", HW_MODULE_NAME, RC_GET_CODE(rc));
        throw 1;
    }
    
    rc = GpioInit();
    if (rcOk != rc)
    {
        fprintf(stderr, "GpioInit failed, error code: %d.\n", RC_GET_CODE(rc));
        throw 1;
    }

    rc = GpioOpenPort(HW_MODULE_NAME, &gphandle);
    if (rcOk != rc)
    {
        fprintf(stderr, "GpioOpenPort port %s failed, error code: %d.\n", HW_MODULE_NAME, RC_GET_CODE(rc));
        throw 1;
    }
    else if (GPIO_INVALID_HANDLE == gphandle)
    {
        fprintf(stderr, "GPIO module %s handle is invalid.\n", HW_MODULE_NAME);
        throw 1;
    }

    for (auto pinNum : pinArray)
    {
        rc = GpioSetMode(gphandle, pinNum, GPIO_DIR_OUT);
        if (rcOk != rc)
        {
            fprintf(stderr, "GpioSetMode for module %s pin %u failed, error code: %d.\n", HW_MODULE_NAME, pinNum, RC_GET_CODE(rc));
            throw 1;
        }
    }
    
    GpioOut(gphandle,  6, 1);
    GpioOut(gphandle, 26, 1);

    stop();
}

GPIOCtrl::~GPIOCtrl()
{
    GpioOut(gphandle,  6, 0);
    GpioOut(gphandle, 26, 0);
    
    if (GPIO_INVALID_HANDLE != gphandle)
    {   
        Retcode rc = GpioClosePort(gphandle);
        if (rcOk != rc)
        {
            fprintf(stderr, "GpioClosePort for %s port failed, error code: %d.\n", HW_MODULE_NAME, RC_GET_CODE(rc));
        }
    }
}

int GPIOCtrl::run(uint16_t command)
{
    uint16_t op = command & MQTT_MASK;
    unsigned int arg = static_cast<unsigned int>(command >> 4)*1000;
    
    fprintf(stderr, "Robot command: %d with %d ms\n", op, arg/1000);
    
    switch(op)
    {
        case COMMAND::FORWARD:
            return forward(arg);
        case COMMAND::BACK:
            return back(arg);
        case COMMAND::LEFT:
            return left(arg);
        case COMMAND::RIGHT:
            return right(arg);
        case COMMAND::STOP:
            return stop();
    }
}

int GPIOCtrl::forward(unsigned int time)
{
    fprintf(stderr, "Robot forward %d\n", time);
    
    GpioOut(gphandle, 12, 1);
    GpioOut(gphandle, 13, 0);
    GpioOut(gphandle, 20, 1);
    GpioOut(gphandle, 21, 0);
    
    usleep(time);
    stop();
    //return (rcOk == rc);
    return 0;
}

int GPIOCtrl::back(unsigned int time)
{
    fprintf(stderr, "Robot back\n");
    Retcode rc = rcOk;
    
    rc |= GpioOut(gphandle, 12, 0);
    rc |= GpioOut(gphandle, 13, 1);
    rc |= GpioOut(gphandle, 20, 0);
    rc |= GpioOut(gphandle, 21, 1);
    
    usleep(time);
    rc |= stop();
    return 0; 
}

int GPIOCtrl::right(unsigned int time)
{
    fprintf(stderr, "Robot right\n");
    Retcode rc = rcOk;
    
    rc |= GpioOut(gphandle, 12, 1);
    rc |= GpioOut(gphandle, 13, 0);
    rc |= GpioOut(gphandle, 20, 0);
    rc |= GpioOut(gphandle, 21, 0);
    
    usleep(time);
    rc |= stop();
    return 0; 
}

int GPIOCtrl::left(unsigned int time)
{
    fprintf(stderr, "Robot left\n");
    Retcode rc = rcOk;
    
    rc |= GpioOut(gphandle, 12, 1);
    rc |= GpioOut(gphandle, 13, 0);
    rc |= GpioOut(gphandle, 20, 0);
    rc |= GpioOut(gphandle, 21, 0);
    
    usleep(time);
    rc |= stop();
    return 0; 
}

int GPIOCtrl::stop()
{
    fprintf(stderr, "Robot stop\n");
    
    GpioOut(gphandle, 12, 0);
    GpioOut(gphandle, 21, 0);
    GpioOut(gphandle, 13, 0);
    GpioOut(gphandle, 20, 0);
    
    return 0;
}

using namespace std::literals;

constexpr auto Topic = "my/awesome/kokosmasters"sv;

Subscriber::Subscriber(const char *id, const char *host, int port)
    : mosquittopp(id)
{
    std::cout << app::AppTag << "Connecting to MQTT Broker with address "
              << host << " and port " << port << std::endl;

    const int keepAlive = 60;
    
    //this->username_pw_set(DefaultUsername, DefaultPassword);
    connect(host, port, keepAlive);
}

void Subscriber::run_command(const std::string& json_string)
{
    nlohmann::json j = nlohmann::json::parse(json_string);
    
    std::string cmd_type = j.value("cmd", "");
    if(cmd_type.empty())
    {
        std::cout << "No command type!" << std::endl;
        return;
    }
    
    uint16_t cmd_op = COMMAND_MAPPING.at(cmd_type);
    JSON_command jc;
    
    switch (cmd_op)
    {
        case COMMAND::STOP:
        {
            jc.type = cmd_op;
            break;
        }
    
        case COMMAND::FORWARD:
        case COMMAND::BACK:
        case COMMAND::RIGHT:
        case COMMAND::LEFT:
        {
            std::string cmd_arg = j.value("val", "");
            if(cmd_arg.empty())
            {
                std::cout << "No command argument!" << std::endl;
                return;
            }
            jc.type = cmd_op;
            jc.arg = static_cast<uint16_t>(stod(cmd_arg)*100);
            break;
        }
        
        default:
            std::cout << "Unknown command!" << std::endl;
            return;
    }
    
    if(0 != gpioctrl->run(jc.to_gpio())) std::cout << "Error while executing command on GPIO!";
}

void Subscriber::on_connect(int rc)
{
    std::cout << app::AppTag << "ON COUT Subscriber connected with code "
              << rc << std::endl;

    std::cerr << app::AppTag << "Subscriber connected with code "
              << rc << std::endl;

    if (rc == 0)
    {
        subscribe(NULL, Topic.data());
    }
}

void Subscriber::on_message(const struct mosquitto_message *message)
{
    if (Topic == message->topic)
    {
        const std::string_view payload{reinterpret_cast<char*>(message->payload),
                                       static_cast<size_t>(message->payloadlen)};
        std::cerr << app::AppTag << "Got message with topic: " << message->topic
                  << ", payload: " << payload << std::endl;

        run_command(std::string(payload));
    }
}

void Subscriber::on_subscribe(__rtl_unused int        mid,
                              __rtl_unused int        qos_count,
                              __rtl_unused const int *granted_qos)
{
    std::cout << app::AppTag << "Subscription succeeded." << std::endl;
}
