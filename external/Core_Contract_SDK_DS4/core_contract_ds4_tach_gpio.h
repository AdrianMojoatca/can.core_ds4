#ifndef __CORE_CONTRACT_DS4_TACH_GPIO_H__
#define __CORE_CONTRACT_DS4_TACH_GPIO_H__

#include "device.h"
#include "gpio.h"

static void core_contract_ds4_tach_pwm_select(void)
{
    gpio_pl_select(GPIO_PL_PWM_OUT);
}

static UInt32 core_contract_ds4_tach_pwm_channel(void)
{
    return GPIO_PL_CHAN_NUM(GPIO_PL_PWM_OUT);
}

#endif /* __CORE_CONTRACT_DS4_TACH_GPIO_H__ */