#include <iostream>
#include <string_view>

#include "general.h"
#include "subscriber.h"

using namespace std::literals;

constexpr auto Topic = "my/awesome/topic"sv;

Subscriber::Subscriber(const char *id, const char *host, int port)
    : mosquittopp(id)
{
    std::cout << app::AppTag << "Connecting to MQTT Broker with address "
              << host << " and port " << port << std::endl;

    const int keepAlive = 60;

    connect(host, port, keepAlive);
}

void Subscriber::on_connect(int rc)
{
    std::cout << app::AppTag << "Subscriber connected with code "
              << rc << std::endl;

    if (rc == 0)
    {
        subscribe(NULL, Topic.data());
    }
}

void run_command(const std::string& json_string)
{
    nlohmann::json j = nlohmann::json::parse(json_string);
    
    std::string cmd_type = j.value("cmd", "");
    if(cmd_type.empty())
    {
        std::cout << "No command type!" << std::endl;
        return;
    }
    
    switch (cmd_type)
    {
        case "stop":
        {
            // send command to gpio
        }
    
        case "forward":
        case "back":
        case "left":
        case "right":
        {
            std::string cmd_arg = j.value("val", "");
            if(cmd_arg.empty())
            {
                std::cout << "No command argument!" << std::endl;
                return;
            }
            // send command to gpio
        }
    }
}

void Subscriber::on_message(const struct mosquitto_message *message)
{
    if (Topic == message->topic)
    {
        const std::string_view payload{reinterpret_cast<char*>(message->payload),
                                       static_cast<size_t>(message->payloadlen)};
        std::cout << app::AppTag << "Got message with topic: " << message->topic
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
