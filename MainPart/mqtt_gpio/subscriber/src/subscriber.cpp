#include <iostream>
#include <string_view>

#include "general.h"
#include "subscriber.h"

using namespace std::literals;

constexpr auto Topic = "my/awesome/kokosmasters"sv;

Subscriber::Subscriber(const char *id, const char *host, int port, bool& flag)
    : mosquittopp(id)
{
    std::cout << app::AppTag << "Connecting to MQTT Broker with address "
              << host << " and port " << port << std::endl;

    const int keepAlive = 60;
    
    //this->username_pw_set("kokos", "testpswd");
    int rc = connect(host, port, keepAlive)
    if(0 != rc)
    {
        std::cout << app::AppTag << "Connect ended with code: " << rc << std::endl;
        flag = true;
        return;
    }
    
    gpioctrl = new GPIOController(flag);
    if (flag)
    {
        delete gpioctrl;
        return;
    }
    
}

GPIOController* Subscriber::get_gpio()
{
    return gpioctrl;
}

void Subscriber::on_connect(int rc)
{
    std::cout << app::AppTag << "Subscriber connected with code " << rc << std::endl;

    if (rc == 0)
    {
        subscribe(NULL, Topic.data());
    }
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
    
    if(!gpioctrl->run(jc.to_gpio())) std::cout << "Error while executing command on GPIO!";
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
