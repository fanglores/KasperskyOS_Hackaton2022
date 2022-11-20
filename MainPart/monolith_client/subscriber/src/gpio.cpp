#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <gpio/gpio.h>
#include <stdbool.h>
#include <string>
#include <iostream>
#include "json.hpp"

#ifdef __arm__
#include <bsp/bsp.h>
#endif

#define GPIO_PIN_NUM_IN2 12U
#define GPIO_PIN_NUM_IN1 13U
#define GPIO_PIN_NUM_ENA 6U
#define GPIO_PIN_NUM_IN4 20U
#define GPIO_PIN_NUM_IN3 21U
#define GPIO_PIN_NUM_ENB 26U
#define DELAY_S 2

#define BUF_SIZE 1024
#define HIGH 1
#define LOW 0

void forward(GpioHandle* handle){
    fprintf(stderr, "forward\n");
    GpioOut(*handle, GPIO_PIN_NUM_IN1, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_IN2, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN3, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN4, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_ENA, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_ENB, HIGH);
}

void stop(GpioHandle* handle) {
    GpioOut(*handle, GPIO_PIN_NUM_IN1, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN2, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN3, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN4, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_ENA, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_ENB, LOW);
}

void backward(GpioHandle* handle) {
    fprintf(stderr, "backward\n");
    GpioOut(*handle, GPIO_PIN_NUM_IN1, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN2, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_IN3, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_IN4, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_ENA, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_ENB, HIGH);
}

void left(GpioHandle* handle) {
    fprintf(stderr, "left\n");
    GpioOut(*handle, GPIO_PIN_NUM_IN1, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_IN2, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN3, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_IN4, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_ENA, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_ENB, HIGH);
}

void right(GpioHandle* handle) {
    fprintf(stderr, "right\n");
    GpioOut(*handle, GPIO_PIN_NUM_IN1, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN2, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_IN3, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN4, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_ENA, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_ENB, HIGH);
}

int gpiomain(const std::string& cmd)
{
    fprintf(stderr, "Start GPIO_output test\n");

#ifdef __arm__
    /**
     * InitializeGPIO_PIN_NUM board support package (BSP) driver and set configuration
     * for GPIO pins. It`s required for stdout by UART.
     */
    {
        BspError rc = BspInit(NULL);
        if (rc != BSP_EOK)
        {
            fprintf(stderr, "Failed to initialize BSP\n");
            return EXIT_FAILURE;
        }

        rc = BspSetConfig("gpio0", "raspberry_pi4b.default");
        if (rc != BSP_EOK)
        {
            fprintf(stderr, "Failed to set mux configuration for gpio0 module\n");
            return EXIT_FAILURE;
        }
    }
#endif

    if (GpioInit())
    {
        fprintf(stderr, "GpioInit failed\n");
        return EXIT_FAILURE;
    }

    /* initialize and setup gpio0 port */
    GpioHandle handle = NULL;
    if (GpioOpenPort("gpio0", &handle) || handle == GPIO_INVALID_HANDLE)
    {
        fprintf(stderr, "GpioOpenPort failed\n");
        return EXIT_FAILURE;
    }

    GpioSetMode(handle, GPIO_PIN_NUM_IN1, GPIO_DIR_OUT);
    GpioSetMode(handle, GPIO_PIN_NUM_IN2, GPIO_DIR_OUT);
    GpioSetMode(handle, GPIO_PIN_NUM_IN3, GPIO_DIR_OUT);
    GpioSetMode(handle, GPIO_PIN_NUM_IN4, GPIO_DIR_OUT);
    GpioSetMode(handle, GPIO_PIN_NUM_ENA, GPIO_DIR_OUT);
    GpioSetMode(handle, GPIO_PIN_NUM_ENB, GPIO_DIR_OUT);

    fprintf(stderr, "Starting move (%s)\n", cmd);
    GpioHandle* p_handle = &handle;
    
    nlohmann::json j = nlohmann::json::parse(cmd);
    
    std::string cmd_type = j.value("cmd", "");
    if(cmd_type.empty())
    {
        std::cout << "No command type!" << std::endl;
        return 1;
    }
    
    double cmd_arg = stod(j.value("val", "-1.0"));
    if(cmd_arg == -1.0)
    {
        std::cout << "No command argument!" << std::endl;
        return 1;
    }
    cmd_arg *= 100;
    if(cmd_type == "back")
    {
      forward(p_handle);
      usleep(static_cast<unsigned int>(cmd_arg*1000));
      stop(p_handle);
    }
    else if(cmd_type == "forward")
    {
      backward(p_handle);
      usleep(static_cast<unsigned int>(cmd_arg*1000));
      stop(p_handle);
    }
    else if(cmd_type == "right")
    {
      right(p_handle);
      usleep(static_cast<unsigned int>(cmd_arg*1000));
      stop(p_handle);
    }
    else if(cmd_type == "left")
    {
      left(p_handle);
      usleep(static_cast<unsigned int>(cmd_arg*1000));
      stop(p_handle);
    }
     else if(cmd_type == "stop")
    {
      stop(p_handle);
    }

    if(GpioClosePort(handle))
    {
        fprintf(stderr, "GpioClosePort failed\n");
        return EXIT_FAILURE;
    }

    fprintf(stderr, "Test finished.\n");

    return EXIT_SUCCESS;
}
