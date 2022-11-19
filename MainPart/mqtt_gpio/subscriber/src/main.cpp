#include <cstring>
#include <exception>
#include <iostream>
#include <memory>

#include <kos_net.h>

#include "general.h"
#include "subscriber.h"

namespace consts {
constexpr const char *DefaultMqttAddress = "10.0.2.2";
constexpr int DefaultMqttUnencryptedPort = 1883;
constexpr int PublicationIntervalInSec = 5;
} // namespace consts

static std::string GetBrokerAddress()
{
    const char *brokerAddress = getenv("MQTT_BROKER_ADDRESS");
    if (!brokerAddress)
    {
        std::cerr << app::AppTag
                  << "Failed to get MQTT broker address. Using default MQTT "
                     "broker address ("
                  << consts::DefaultMqttAddress << ")" << std::endl;
        return consts::DefaultMqttAddress;
    }
    return brokerAddress;
}

static int GetBrokerPort()
{
    const char *brokerPortEnvVariable = getenv("MQTT_BROKER_PORT");
    if (!brokerPortEnvVariable)
    {
        std::cerr << app::AppTag
                  << "Failed to get MQTT broker port. Using default MQTT "
                     "broker port ("
                  << consts::DefaultMqttUnencryptedPort << ")" << std::endl;
        return consts::DefaultMqttUnencryptedPort;
    }

    try
    {
        return std::stoi(brokerPortEnvVariable);
    }
    catch (const std::invalid_argument &ex)
    {
        std::cerr << app::AppTag
                  << "Failed to get MQTT broker port: " << ex.what()
                  << "Using default port ("
                  << consts::DefaultMqttUnencryptedPort << ")" << std::endl;
        return consts::DefaultMqttUnencryptedPort;
    }
}

int main(void)
{
    bool f;
    GPIOController* gpioctrl = new GPIOController(f);
    uint16_t cmd1 = static_cast<uint16_t>(COMMAND::FORWARD | (1 << 4));
    gpioctrl->run(cmd1);
    sleep(1);
    uint16_t cmd2 = static_cast<uint16_t>(COMMAND::BACK | (2 << 4));
    gpioctrl->run(cmd2);
    sleep(1);
    uint16_t cmd3 = static_cast<uint16_t>(COMMAND::LEFT | (1 << 4));
    gpioctrl->run(cmd3);
    sleep(1);
    uint16_t cmd4 = static_cast<uint16_t>(COMMAND::RIGHT | (2 << 4));
    gpioctrl->run(cmd4);
    
    return EXIT_SUCCESS;

    if (!wait_for_network())
    {
        std::cerr << app::AppTag << "Error: Wait for network failed!" << std::endl;
        return EXIT_FAILURE;
    }

    mosqpp::lib_init();
    
    bool flag = false;
    auto sub = std::make_unique<Subscriber>("subscriber", GetBrokerAddress().c_str(), GetBrokerPort(), flag);
    
    if(flag)
    {
        mosqpp::lib_cleanup();
        return EXIT_FAILURE;
    }
    
    if (sub)
    {
        sub->loop_forever();
    }

    mosqpp::lib_cleanup();
    return EXIT_SUCCESS;
}
