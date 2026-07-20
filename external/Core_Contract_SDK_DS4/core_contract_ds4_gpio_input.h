#ifndef __CORE_CONTRACT_DS4_GPIO_INPUT_H__
#define __CORE_CONTRACT_DS4_GPIO_INPUT_H__

#include "device.h"
#include "gpio.h"

typedef enum {
    CORE_DS4_GPIO_INPUT_NEGATIVE_1 = 0,
    CORE_DS4_GPIO_INPUT_NEGATIVE_2,
    CORE_DS4_GPIO_INPUT_NEGATIVE_3,
    CORE_DS4_GPIO_INPUT_NEGATIVE_4,
    CORE_DS4_GPIO_INPUT_NEGATIVE_5,
    CORE_DS4_GPIO_INPUT_NEGATIVE_6,
    CORE_DS4_GPIO_INPUT_NEGATIVE_7,
    CORE_DS4_GPIO_INPUT_POSITIVE_1,
    CORE_DS4_GPIO_INPUT_POSITIVE_2,
    CORE_DS4_GPIO_INPUT_POSITIVE_3,
    CORE_DS4_GPIO_INPUT_BUTTON_IN
} core_ds4_gpio_input_selector_t;

static Boolean core_contract_ds4_gpio_input_read(core_ds4_gpio_input_selector_t input_selector)
{
    switch (input_selector)
    {
        case CORE_DS4_GPIO_INPUT_NEGATIVE_1: return gpio_pl_pin_read(GPIO_PL_IN_NEGATIVE_1);
        case CORE_DS4_GPIO_INPUT_NEGATIVE_2: return gpio_pl_pin_read(GPIO_PL_IN_NEGATIVE_2);
        case CORE_DS4_GPIO_INPUT_NEGATIVE_3: return gpio_pl_pin_read(GPIO_PL_IN_NEGATIVE_3);
        case CORE_DS4_GPIO_INPUT_NEGATIVE_4: return gpio_pl_pin_read(GPIO_PL_IN_NEGATIVE_4);
        case CORE_DS4_GPIO_INPUT_NEGATIVE_5: return gpio_pl_pin_read(GPIO_PL_IN_NEGATIVE_5);
        case CORE_DS4_GPIO_INPUT_NEGATIVE_6: return gpio_pl_pin_read(GPIO_PL_IN_NEGATIVE_6);
        case CORE_DS4_GPIO_INPUT_NEGATIVE_7: return gpio_pl_pin_read(GPIO_PL_IN_NEGATIVE_7);
        case CORE_DS4_GPIO_INPUT_POSITIVE_1: return gpio_pl_pin_read(GPIO_PL_IN_POSITIVE_1);
        case CORE_DS4_GPIO_INPUT_POSITIVE_2: return gpio_pl_pin_read(GPIO_PL_IN_POSITIVE_2);
        case CORE_DS4_GPIO_INPUT_POSITIVE_3: return gpio_pl_pin_read(GPIO_PL_IN_POSITIVE_3);
        case CORE_DS4_GPIO_INPUT_BUTTON_IN: return gpio_pl_pin_read(GPIO_PL_PUSH_IN);
        default:
            return FALSE;
    }
}

#endif
