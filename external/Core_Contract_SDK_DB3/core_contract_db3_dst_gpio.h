#ifndef __CORE_CONTRACT_DB3_DST_GPIO_H__
#define __CORE_CONTRACT_DB3_DST_GPIO_H__

#include "device.h"
#include "gpio.h"

typedef enum
{
    CORE_DB3_DST_GPIO_J1850_RX = 0,
    CORE_DB3_DST_GPIO_ISO1_RX,
    CORE_DB3_DST_GPIO_ISO2_RX
} core_db3_dst_gpio_pin_t;

static Boolean core_contract_db3_dst_gpio_int_stat_f(core_db3_dst_gpio_pin_t pin)
{
    switch (pin)
    {
        case CORE_DB3_DST_GPIO_J1850_RX: return gpio_pl_int_stat_f(GPIO_PL_J1850_RX);
        case CORE_DB3_DST_GPIO_ISO1_RX: return gpio_pl_int_stat_f(GPIO_PL_ISO1_RX);
        case CORE_DB3_DST_GPIO_ISO2_RX: return gpio_pl_int_stat_f(GPIO_PL_ISO2_RX);
        default: return FALSE;
    }
}

static Boolean core_contract_db3_dst_gpio_int_stat_r(core_db3_dst_gpio_pin_t pin)
{
    switch (pin)
    {
        case CORE_DB3_DST_GPIO_J1850_RX: return gpio_pl_int_stat_r(GPIO_PL_J1850_RX);
        case CORE_DB3_DST_GPIO_ISO1_RX: return gpio_pl_int_stat_r(GPIO_PL_ISO1_RX);
        case CORE_DB3_DST_GPIO_ISO2_RX: return gpio_pl_int_stat_r(GPIO_PL_ISO2_RX);
        default: return FALSE;
    }
}

static void core_contract_db3_dst_gpio_irq_dis(core_db3_dst_gpio_pin_t pin)
{
    switch (pin)
    {
        case CORE_DB3_DST_GPIO_J1850_RX:
            gpio_pl_rising_irq_dis(GPIO_PL_J1850_RX);
            gpio_pl_falling_irq_dis(GPIO_PL_J1850_RX);
            break;
        case CORE_DB3_DST_GPIO_ISO1_RX:
            gpio_pl_rising_irq_dis(GPIO_PL_ISO1_RX);
            gpio_pl_falling_irq_dis(GPIO_PL_ISO1_RX);
            break;
        case CORE_DB3_DST_GPIO_ISO2_RX:
            gpio_pl_rising_irq_dis(GPIO_PL_ISO2_RX);
            gpio_pl_falling_irq_dis(GPIO_PL_ISO2_RX);
            break;
        default:
            break;
    }
}

static void core_contract_db3_dst_gpio_irq_ena(core_db3_dst_gpio_pin_t pin)
{
    switch (pin)
    {
        case CORE_DB3_DST_GPIO_J1850_RX:
            gpio_pl_rising_irq_ena(GPIO_PL_J1850_RX);
            gpio_pl_falling_irq_ena(GPIO_PL_J1850_RX);
            break;
        case CORE_DB3_DST_GPIO_ISO1_RX:
            gpio_pl_rising_irq_ena(GPIO_PL_ISO1_RX);
            gpio_pl_falling_irq_ena(GPIO_PL_ISO1_RX);
            break;
        case CORE_DB3_DST_GPIO_ISO2_RX:
            gpio_pl_rising_irq_ena(GPIO_PL_ISO2_RX);
            gpio_pl_falling_irq_ena(GPIO_PL_ISO2_RX);
            break;
        default:
            break;
    }
}

static void core_contract_db3_dst_gpio_int_clr(core_db3_dst_gpio_pin_t pin)
{
    switch (pin)
    {
        case CORE_DB3_DST_GPIO_J1850_RX: gpio_pl_int_clr(GPIO_PL_J1850_RX); break;
        case CORE_DB3_DST_GPIO_ISO1_RX: gpio_pl_int_clr(GPIO_PL_ISO1_RX); break;
        case CORE_DB3_DST_GPIO_ISO2_RX: gpio_pl_int_clr(GPIO_PL_ISO2_RX); break;
        default: break;
    }
}

#endif /* __CORE_CONTRACT_DB3_DST_GPIO_H__ */