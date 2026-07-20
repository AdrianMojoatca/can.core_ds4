/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_usr_execute.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_v_usr_private.h"
#include "can_v.h"
#include "fiq.h"
#include "config_gpio.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean can_v_usr_execute (void)
{
  if(!can_v_usr_init_nvfs ()) 
    {
      // error ?
    }

  irq_user_register(can_v_usr_user_service) ;

  wake_register     (can_v_usr_wake) ;  // toggle the transceiver to ON/OFF state

  if((pin_map[CAN_DEV_0] == CAN_DEV_0_P0_0_1) || (pin_map[CAN_DEV_0] == CAN_DEV_0_P0_21_22)) // init the transceiver to ON state right now
    {
			#ifdef DEV_LPC_236x
			STB_FTCAN_SET ;
			#else
			gpio_pl_set(GPIO_PL_MODE0);
			gpio_pl_set(GPIO_PL_MODE1);
			#endif
			EN_FTCAN_SET ;
			
    }
  if(pin_map[CAN_DEV_1] == CAN_DEV_1_P0_4_5) // init the transceiver to ON state right now
    {
			STB_HSCAN_CLR ;
		}

  can_set_irq_func	(can_v_usr_irq_service) ; 

  return can_v_execute (2, can_dev[CAN_DEV_0], can_dev[CAN_DEV_1]) ;
}
