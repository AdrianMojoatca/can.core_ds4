#ifndef __CORE_CONTRACT_DS4_D2D_GPIO_H__
#define __CORE_CONTRACT_DS4_D2D_GPIO_H__

#include "device.h"
#include "gpio.h"

typedef enum
{
    CORE_DS4_D2D_UART_CHAN_0 = 0,
    CORE_DS4_D2D_UART_CHAN_1,
    CORE_DS4_D2D_UART_CHAN_2,
    CORE_DS4_D2D_UART_CHAN_3
} core_ds4_d2d_uart_channel_t;

static core_ds4_d2d_uart_channel_t core_contract_ds4_d2d_uart_channel(void)
{
    return CORE_DS4_D2D_UART_CHAN_0;
}

static void core_contract_ds4_d2d_uart_cfg_main_tx(void)
{
    gpio_pl_uart_cfg(GPIO_PL_D2D_TX, 0);
}

static void core_contract_ds4_d2d_uart_cfg_rf_tx(void)
{
    gpio_pl_uart_cfg(GPIO_PL_RF_D2D_TX, 0);
}

static void core_contract_ds4_d2d_select_main_uart_pins(void)
{
    gpio_pl_select(GPIO_PL_D2D_TX);
    gpio_pl_select(GPIO_PL_D2D_RX);
}

static void core_contract_ds4_d2d_select_rf_uart_pins(void)
{
    gpio_pl_select(GPIO_PL_RF_D2D_TX);
    gpio_pl_select(GPIO_PL_RF_D2D_RX);
}

static void core_contract_ds4_d2d_rx_select(void)
{
    gpio_pl_select(GPIO_PL_D2D_RX);
}

static void core_contract_ds4_d2d_rx_gpio_mode(void)
{
    gpio_pl_func_cfg_gpio(GPIO_PL_D2D_RX);
}

static void core_contract_ds4_d2d_sleep_gpio_mode(void)
{
    gpio_pl_func_cfg_gpio(GPIO_PL_D2D_RX);
    gpio_pl_func_cfg_gpio(GPIO_PL_D2D_TX);
}

static Boolean core_contract_ds4_d2d_rx_read(Boolean rf_path)
{
    if (rf_path)
    {
        return gpio_pl_pin_read(GPIO_PL_RF_D2D_RX);
    }

    return gpio_pl_pin_read(GPIO_PL_D2D_RX);
}

static void core_contract_ds4_d2d_tx_write(UInt8 value)
{
    gpio_pl_pin_write(GPIO_PL_D2D_TX, value);
}

static void core_contract_ds4_d2d_rf_enable(Boolean is_enabled)
{
    if (is_enabled)
    {
        gpio_pl_set(GPIO_PL_RF_D2D_EN);
    }
    else
    {
        gpio_pl_clr(GPIO_PL_RF_D2D_EN);
    }
}

#endif /* __CORE_CONTRACT_DS4_D2D_GPIO_H__ */