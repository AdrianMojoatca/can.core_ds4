/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config_hybp.h 32611 2015-07-08 13:21:24Z martin.bouchard $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __CONFIG_HOBP_BUS_H__
#define __CONFIG_HOBP_BUS_H__

#include "config_gpio.h"
/*==========================================================================*/
/*                                N O T E S                                 */
/*==========================================================================*/

//--------------------------------------------------------------------------//
// <h>  Programmable Rx/Tx 
//--------------------------------------------------------------------------//

//    <o0> ISO LINE
//         <1=> ISO1       
//         <2=> ISO2      

#define HOBP_BUS_ISO    2
// </h>

//--------------------------------------------------------------------------//
#define HOBP_BUS_SPEED           10400 
//--------------------------------------------------------------------------//

#if (HOBP_BUS_ISO == 1 )					//(1u << 29)
  #define HOBP_BUS_IO_RX         !ISO1_RX
  #define HOBP_BUS_TX_SET         ISO1_TX_CLR
  #define HOBP_BUS_TX_CLR         ISO1_TX_SET
 
  #define HOBP_BUS_RX_RISING       gpio_pl_int_stat_r     (GPIO_PL_ISO1_RX)		
  #define HOBP_BUS_RX_FALLING      gpio_pl_int_stat_f     (GPIO_PL_ISO1_RX)		
  #define HOBP_BUS_RX_INT_DIS      gpio_pl_rising_irq_dis (GPIO_PL_ISO1_RX)	; gpio_pl_falling_irq_dis (GPIO_PL_ISO1_RX)	
  #define HOBP_BUS_RX_INT_ENA      gpio_pl_rising_irq_ena (GPIO_PL_ISO1_RX)	; gpio_pl_falling_irq_ena (GPIO_PL_ISO1_RX)		
  #define HOBP_BUS_RX_INT_CLR	   gpio_pl_int_clr        (GPIO_PL_ISO1_RX)

  #if DBG_HOBP_BUS == 1
    #warning "HOBP BUS SET ON ISO1"
  #endif

#elif (HOBP_BUS_ISO ==  2)				//(1u << 11)
  #define HOBP_BUS_IO_RX           ISO2_RX
  #define HOBP_BUS_TX_SET          ISO2_TX_SET
  #define HOBP_BUS_TX_CLR          ISO2_TX_CLR

  #define HOBP_BUS_RX_RISING       gpio_pl_int_stat_f     (GPIO_PL_ISO2_RX)		
  #define HOBP_BUS_RX_FALLING      gpio_pl_int_stat_r     (GPIO_PL_ISO2_RX)		
  #define HOBP_BUS_RX_INT_DIS      gpio_pl_rising_irq_dis (GPIO_PL_ISO2_RX)	; gpio_pl_falling_irq_dis (GPIO_PL_ISO2_RX)		
  #define HOBP_BUS_RX_INT_ENA      gpio_pl_rising_irq_ena (GPIO_PL_ISO2_RX)	; gpio_pl_falling_irq_ena (GPIO_PL_ISO2_RX)		
  #define HOBP_BUS_RX_INT_CLR	   gpio_pl_int_clr        (GPIO_PL_ISO2_RX)
  
  #if DBG_HOBP_BUS == 1
  	#warning "HOBP BUS SET ON ISO2"
  #endif

#else
  #error "define HOBP IO"
#endif

#endif

