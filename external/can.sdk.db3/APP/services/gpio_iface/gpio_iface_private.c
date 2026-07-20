/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: gpio_iface_private.c - internal gpio iface helpers                  $
/*==========================================================================*/

#include "gpio_iface_private.h"

void gpio_iface_ignition_on(void)
{
    core_contract_db3_fw_gpio_ignition_on();
}

void gpio_iface_ignition_off(void)
{
    core_contract_db3_fw_gpio_ignition_off();
}

void gpio_iface_hood_open(void)
{
    core_contract_db3_fw_gpio_hood_open();
}

void gpio_iface_hood_closed(void)
{
    core_contract_db3_fw_gpio_hood_closed();
}

void gpio_iface_key_wrap_on(void)
{
    core_contract_db3_fw_gpio_key_wrap_on();
}

void gpio_iface_key_wrap_off(void)
{
    core_contract_db3_fw_gpio_key_wrap_off();
}

void gpio_iface_led_red_on(void)
{
    core_contract_db3_gpio_led_red_on();
}

void gpio_iface_led_red_off(void)
{
    core_contract_db3_gpio_led_red_off();
}

void gpio_iface_led_green_on(void)
{
    core_contract_db3_gpio_led_green_on();
}

void gpio_iface_led_green_off(void)
{
    core_contract_db3_gpio_led_green_off();
}

void gpio_iface_led_blue_on(void)
{
    core_contract_db3_gpio_led_blue_on();
}

void gpio_iface_led_blue_off(void)
{
    core_contract_db3_gpio_led_blue_off();
}

void gpio_iface_rsr_lock_on(void)
{
    core_contract_db3_gpio_rsr_lock_on();
}

void gpio_iface_rsr_lock_off(void)
{
    core_contract_db3_gpio_rsr_lock_off();
}

void gpio_iface_rsr_unlock_on(void)
{
    core_contract_db3_gpio_rsr_unlock_on();
}

void gpio_iface_rsr_unlock_off(void)
{
    core_contract_db3_gpio_rsr_unlock_off();
}

void gpio_iface_rsr_trunk_on(void)
{
    core_contract_db3_gpio_rsr_trunk_on();
}

void gpio_iface_rsr_trunk_off(void)
{
    core_contract_db3_gpio_rsr_trunk_off();
}

void gpio_iface_rf_loop_toggle(void)
{
    core_contract_db3_fw_gpio_rf_loop_toggle();
}
