/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: gpio_pl.c 33398 2015-08-26 16:43:57Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "device.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void gpio_pl_pin_write( Gpio_Pl_T pl, Boolean val);
void gpio_pl_direction(Gpio_Pl_T pl);
void gpio_pl_set(Gpio_Pl_T pl);
void gpio_pl_clr(Gpio_Pl_T pl);
void gpio_pl_tog(Gpio_Pl_T pl);
void gpio_pl_select(Gpio_Pl_T pl);
void gpio_pl_mode(Gpio_Pl_T pl);
Boolean gpio_pl_pin_read(Gpio_Pl_T pl);
void gpio_pl_func_cfg_gpio(Gpio_Pl_T pl);
void gpio_pl_rising_irq_ena(Gpio_Pl_T pl);
void gpio_pl_rising_irq_dis(Gpio_Pl_T pl);
void gpio_pl_falling_irq_ena(Gpio_Pl_T pl);
Boolean gpio_pl_check_irq_ena_R(Gpio_Pl_T pl);
void gpio_pl_falling_irq_dis(Gpio_Pl_T pl);
Boolean gpio_pl_check_irq_ena_F(Gpio_Pl_T pl);
void gpio_pl_int_clr(Gpio_Pl_T pl);
Boolean gpio_pl_int_stat_r(Gpio_Pl_T pl);
Boolean gpio_pl_int_stat_f(Gpio_Pl_T pl);
void gpio_pl_chan(Gpio_Pl_T pl);
void gpio_pl_uart_cfg(Gpio_Pl_T pl, UInt8 bdrate_type);

#ifdef PLATFORM_404
void cap_int_clr(Gpio_Pl_T pl);
void cap_int_ena(Gpio_Pl_T pl);
void cap_int_dis(Gpio_Pl_T pl);
#endif

#define GPIO_PL_INT_STAT_R(pl) ((1<<((pl) & (GPIO_PL_MASK_PIN<<GPIO_PL_POS_PIN))) & *((volatile unsigned int *)(((pl) & (GPIO_PL_MASK_PORT<<GPIO_PL_POS_PORT)) ? IO2_INT_STAT_R : IO0_INT_STAT_R)))
#define GPIO_PL_INT_STAT_F(pl) ((1<<((pl) & (GPIO_PL_MASK_PIN<<GPIO_PL_POS_PIN))) & *((volatile unsigned int *)(((pl) & (GPIO_PL_MASK_PORT<<GPIO_PL_POS_PORT)) ? IO2_INT_STAT_F : IO0_INT_STAT_F)))

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/


