#include "gpio.h"

GPIOController::GPIOController()
{
    Retcode rc = BspInit(NULL);
    if (BSP_EOK != rc)
    {
        fprintf(stderr, "Failed to initialize BSP, error code: %d.\n", RC_GET_CODE(rc));
        return;
    }
    
    rc = BspSetConfig(HW_MODULE_NAME, HW_MODULE_CFG);
    if (rcOk != rc)
    {
        fprintf(stderr, "Failed to set mux configuration for %s module, error code: %d.\n", HW_MODULE_NAME, RC_GET_CODE(rc));
        return;
    }
    
    rc = GpioInit();
    if (rcOk != rc)
    {
        fprintf(stderr, "GpioInit failed, error code: %d.\n", RC_GET_CODE(rc));
        return;
    }

    /* Initialize and setup HW_MODULE_NAME port. */
    rc = GpioOpenPort(HW_MODULE_NAME, &handle);
    if (rcOk != rc)
    {
        fprintf(stderr, "GpioOpenPort port %s failed, error code: %d.\n", HW_MODULE_NAME, RC_GET_CODE(rc));
    }
    else if (GPIO_INVALID_HANDLE == handle)
    {
        fprintf(stderr, "GPIO module %s handle is invalid.\n", HW_MODULE_NAME);
        rc = rcFail;
    }

    for (auto pinNum : pinArray)
    {
        rc = GpioSetMode(handle, pinNum, GPIO_DIR_OUT);
        if (rcOk != rc)
        {
            fprintf(stderr, "GpioSetMode for module %s pin %u failed, error code: %d.\n", HW_MODULE_NAME, pinNum, RC_GET_CODE(rc));
            return;
        }
    }
    
    GpioOut(handle,  6, 1);
    GpioOut(handle, 26, 1);

    GpioOut(handle, 12, 0);
    GpioOut(handle, 21, 0);
    GpioOut(handle, 13, 0);
    GpioOut(handle, 20, 0);
}

int GPIOController::run(uint16_t command)
{
    uint16_t op = command & MQTT_MASK;
    uint16_t arg = command >> 4;
    
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

int GPIOController::forward(int time)
{
    GpioOut(handle, 12, 0);
    GpioOut(handle, 21, 0);
    GpioOut(handle, 13, 0);
    GpioOut(handle, 20, 0);    
    
    sleep(time);
    stop();
    return 1;
}

int GPIOController::back(int time)
{
    GpioOut(handle, 12, 0);
    GpioOut(handle, 21, 0);
    GpioOut(handle, 13, 0);
    GpioOut(handle, 20, 0);    
    
    sleep(time);
    stop();
    return 1;
}

int GPIOController::right(int time)
{
    GpioOut(handle, 12, 0);
    GpioOut(handle, 21, 0);
    GpioOut(handle, 13, 0);
    GpioOut(handle, 20, 0);    
    
    sleep(time);
    stop();
    return 1;
}

int GPIOController::left(int time)
{
    GpioOut(handle, 12, 0);
    GpioOut(handle, 21, 0);
    GpioOut(handle, 13, 0);
    GpioOut(handle, 20, 0);    
    
    sleep(time);
    stop();
    return 1;
}

int GPIOController::stop()
{
    GpioOut(handle, 12, 0);
    GpioOut(handle, 21, 0);
    GpioOut(handle, 13, 0);
    GpioOut(handle, 20, 0);
    
    return 1;
}

GPIOController::~GPIOController()
{
    if (GPIO_INVALID_HANDLE != handle)
    {   
        Retcode rc = GpioClosePort(handle);
        if (rcOk != rc)
        {
            fprintf(stderr, "GpioClosePort for %s port failed, error code: %d.\n", HW_MODULE_NAME, RC_GET_CODE(rc));
        }
    }
}

/*
    if (rcOk == rc)
    {
        for (rtl_uint32_t i = GPIO_PIN_NUM; i != 0; i--)
        {
            rtl_uint32_t pinNum = i - 1;
            if (false == IsExceptionPin(pinNum))
            {
                rc = GpioOut(handle, pinNum, GPIO_HIGH_LEVEL);
                if (rcOk != rc)
                {
                    fprintf(stderr, "GpioOut 1 for module %s pin %u failed, "
                            "error code: %d.\n", HW_MODULE_NAME, pinNum,
                            RC_GET_CODE(rc));
                    break;
                }
                else
                {
                    fprintf(stderr, "Module %s pin %u is set to 1.\n",
                            HW_MODULE_NAME, pinNum);
                    sleep(DELAY_S);

                    rc = GpioOut(handle, pinNum, GPIO_LOW_LEVEL);
                    if (rcOk != rc)
                    {
                        fprintf(stderr, "GpioOut 0 for module %s pin %u "
                                "failed, error code: %d.\n",
                                HW_MODULE_NAME, pinNum, RC_GET_CODE(rc));
                        break;
                    }

                    fprintf(stderr, "Module %s pin %u is set to 0.\n",
                            HW_MODULE_NAME, pinNum);
                }
            }
        }
    }
*/
