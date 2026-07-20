#ifndef __CORE_CONTRACT_DS4_MISC_GPIO_H__
#define __CORE_CONTRACT_DS4_MISC_GPIO_H__

#include "device.h"
#include "gpio.h"

static void core_contract_ds4_dac_power_set(Boolean is_enabled)
{
    if (is_enabled)
    {
        gpio_pl_set(GPIO_PL_DAC_PWR_CTL);
    }
    else
    {
        gpio_pl_clr(GPIO_PL_DAC_PWR_CTL);
    }
}

#endif /* __CORE_CONTRACT_DS4_MISC_GPIO_H__ */
