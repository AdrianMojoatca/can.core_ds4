#ifndef __CORE_CONTRACT_DS4_CAN_GPIO_H__
#define __CORE_CONTRACT_DS4_CAN_GPIO_H__

#include "device.h"
#include "gpio.h"

typedef enum
{
    CORE_DS4_CAN_GPIO_IRQ_FT_RX = 0,
    CORE_DS4_CAN_GPIO_IRQ_SW_RX,
    CORE_DS4_CAN_GPIO_IRQ_HS_RX,
    CORE_DS4_CAN_GPIO_IRQ_J1850_RX
} core_ds4_can_gpio_irq_pin_t;

static void core_contract_ds4_can_gpio_mode0_set(Boolean is_high)
{
    if (is_high)
    {
        gpio_pl_set(GPIO_PL_MODE0);
    }
    else
    {
        gpio_pl_clr(GPIO_PL_MODE0);
    }
}

static void core_contract_ds4_can_gpio_mode1_set(Boolean is_high)
{
    if (is_high)
    {
        gpio_pl_set(GPIO_PL_MODE1);
    }
    else
    {
        gpio_pl_clr(GPIO_PL_MODE1);
    }
}

static void core_contract_ds4_can_gpio_prepare_sw_tx(void)
{
    core_contract_ds4_can_gpio_mode0_set(TRUE);
    core_contract_ds4_can_gpio_mode1_set(TRUE);
    gpio_pl_direction(GPIO_PL_J1850_TX_NU);
    gpio_pl_direction(GPIO_PL_CAN_SW_TX);
}

static void core_contract_ds4_can_gpio_ft_enable(Boolean is_enabled)
{
    if (is_enabled)
    {
        gpio_pl_set(GPIO_PL_EN_FTCAN);
    }
    else
    {
        gpio_pl_clr(GPIO_PL_EN_FTCAN);
    }
}

static void core_contract_ds4_can_gpio_ft_standby(Boolean is_enabled)
{
    if (is_enabled)
    {
        gpio_pl_set(GPIO_PL_STB_FTCAN);
    }
    else
    {
        gpio_pl_clr(GPIO_PL_STB_FTCAN);
    }
}

static void core_contract_ds4_can_gpio_hs_active(Boolean is_active)
{
    if (is_active)
    {
        gpio_pl_clr(GPIO_PL_STB_HSCAN);
    }
    else
    {
        gpio_pl_set(GPIO_PL_STB_HSCAN);
    }
}

static void core_contract_ds4_can_gpio_irq_select_disable(core_ds4_can_gpio_irq_pin_t irq_pin)
{
    switch (irq_pin)
    {
        case CORE_DS4_CAN_GPIO_IRQ_FT_RX:
            gpio_pl_select(GPIO_PL_EXT_CAN_FT_RX);
            gpio_pl_int_clr(GPIO_PL_EXT_CAN_FT_RX);
            gpio_pl_falling_irq_dis(GPIO_PL_EXT_CAN_FT_RX);
            break;

        case CORE_DS4_CAN_GPIO_IRQ_SW_RX:
            gpio_pl_select(GPIO_PL_CAN_SW_RX);
            gpio_pl_int_clr(GPIO_PL_CAN_SW_RX);
            gpio_pl_falling_irq_dis(GPIO_PL_CAN_SW_RX);
            break;

        case CORE_DS4_CAN_GPIO_IRQ_HS_RX:
            gpio_pl_select(GPIO_PL_EXT_CAN_HS_RX);
            gpio_pl_int_clr(GPIO_PL_EXT_CAN_HS_RX);
            gpio_pl_falling_irq_dis(GPIO_PL_EXT_CAN_HS_RX);
            break;

        case CORE_DS4_CAN_GPIO_IRQ_J1850_RX:
            gpio_pl_select(GPIO_PL_J1850_RX);
            gpio_pl_int_clr(GPIO_PL_J1850_RX);
            gpio_pl_falling_irq_dis(GPIO_PL_J1850_RX);
            break;

        default:
            break;
    }
}

static void core_contract_ds4_can_gpio_irq_gpio_enable(core_ds4_can_gpio_irq_pin_t irq_pin)
{
    switch (irq_pin)
    {
        case CORE_DS4_CAN_GPIO_IRQ_FT_RX:
            gpio_pl_func_cfg_gpio(GPIO_PL_EXT_CAN_FT_RX);
            gpio_pl_int_clr(GPIO_PL_EXT_CAN_FT_RX);
            gpio_pl_falling_irq_ena(GPIO_PL_EXT_CAN_FT_RX);
            break;

        case CORE_DS4_CAN_GPIO_IRQ_SW_RX:
            gpio_pl_func_cfg_gpio(GPIO_PL_CAN_SW_RX);
            gpio_pl_int_clr(GPIO_PL_CAN_SW_RX);
            gpio_pl_falling_irq_ena(GPIO_PL_CAN_SW_RX);
            break;

        case CORE_DS4_CAN_GPIO_IRQ_HS_RX:
            gpio_pl_func_cfg_gpio(GPIO_PL_EXT_CAN_HS_RX);
            gpio_pl_int_clr(GPIO_PL_EXT_CAN_HS_RX);
            gpio_pl_falling_irq_ena(GPIO_PL_EXT_CAN_HS_RX);
            break;

        case CORE_DS4_CAN_GPIO_IRQ_J1850_RX:
            gpio_pl_func_cfg_gpio(GPIO_PL_J1850_RX);
            gpio_pl_int_clr(GPIO_PL_J1850_RX);
            gpio_pl_falling_irq_ena(GPIO_PL_J1850_RX);
            break;

        default:
            break;
    }
}

static Boolean core_contract_ds4_can_gpio_irq_falling_stat(core_ds4_can_gpio_irq_pin_t irq_pin)
{
    switch (irq_pin)
    {
        case CORE_DS4_CAN_GPIO_IRQ_FT_RX:
            return gpio_pl_int_stat_f(GPIO_PL_EXT_CAN_FT_RX);

        case CORE_DS4_CAN_GPIO_IRQ_SW_RX:
            return gpio_pl_int_stat_f(GPIO_PL_CAN_SW_RX);

        case CORE_DS4_CAN_GPIO_IRQ_HS_RX:
            return gpio_pl_int_stat_f(GPIO_PL_EXT_CAN_HS_RX);

        case CORE_DS4_CAN_GPIO_IRQ_J1850_RX:
            return gpio_pl_int_stat_f(GPIO_PL_J1850_RX);

        default:
            return FALSE;
    }
}

#endif /* __CORE_CONTRACT_DS4_CAN_GPIO_H__ */