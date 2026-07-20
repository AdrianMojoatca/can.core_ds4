/*==========================================================================*/
// $Id: can_v_usr_wake.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_v_usr_private.h"
#include "config_gpio.h"
#include "fiq.h"
#include "gpio.h"

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

void can_v_usr_wake (Wake_Status wake_status)
{
    if(can_v_usr_wake_status != wake_status)
    {  
        ATOMIC (can_v_usr_wake_status = wake_status) ;

        if (wake_status == s_WAKE)
        {  
        #ifdef DBG_CAN
		  gpio_pl_tog(GPIO_PL_RFLOOP_POWER) ;
        #endif
          
          ATOMIC
          (    
          if(pin_map[CAN_DEV_0] == CAN_DEV_0_P0_0_1) // ft?
            {
			#ifdef DEV_LPC_236x
              STB_FTCAN_SET ;
			#endif
              EN_FTCAN_SET ;
							
              gpio_pl_select         (GPIO_PL_EXT_CAN_FT_RX);
			  gpio_pl_int_clr        (GPIO_PL_EXT_CAN_FT_RX);
			  gpio_pl_falling_irq_dis(GPIO_PL_EXT_CAN_FT_RX);
						
            }
          else if (pin_map[CAN_DEV_0] == CAN_DEV_0_P0_21_22) // CAN SW
            {
			#ifdef DEV_LPC_236x
              STB_FTCAN_SET ;
              EN_FTCAN_SET ;
			#endif
              gpio_pl_select         (GPIO_PL_CAN_SW_RX);
			  gpio_pl_int_clr        (GPIO_PL_CAN_SW_RX);
			  gpio_pl_falling_irq_dis(GPIO_PL_CAN_SW_RX);
            }
    
          if(pin_map[CAN_DEV_1] == CAN_DEV_1_P0_4_5) // hs?
            {
              STB_HSCAN_CLR ;
        
              gpio_pl_select         (GPIO_PL_EXT_CAN_HS_RX);
			  gpio_pl_int_clr        (GPIO_PL_EXT_CAN_HS_RX);
			  gpio_pl_falling_irq_dis(GPIO_PL_EXT_CAN_HS_RX);
            }
          )
    
          //fiq_unregister (can_v_usr_fiq_service)  ;
        }
        else
        {
          //fiq_register (can_v_usr_fiq_service)  ;
    
          ATOMIC
          (
          if(pin_map[CAN_DEV_0] == CAN_DEV_0_P0_0_1) // ft?
            {
			#ifdef DEV_LPC_236x
              STB_FTCAN_CLR ;
		    #endif
              EN_FTCAN_CLR  ;
              
			  gpio_pl_func_cfg_gpio  (GPIO_PL_CAN_SW_RX);
			  gpio_pl_int_clr        (GPIO_PL_CAN_SW_RX);
			  gpio_pl_falling_irq_ena(GPIO_PL_CAN_SW_RX);
            }
          else if (pin_map[CAN_DEV_0] == CAN_DEV_0_P0_21_22)
            {
			#ifdef DEV_LPC_236x
              STB_FTCAN_CLR ;
              EN_FTCAN_CLR  ;
			#endif
			  gpio_pl_func_cfg_gpio  (GPIO_PL_J1850_RX);
			  gpio_pl_int_clr        (GPIO_PL_J1850_RX);
			  gpio_pl_falling_irq_ena(GPIO_PL_J1850_RX);
            }
    
          if(pin_map[CAN_DEV_1] == CAN_DEV_1_P0_4_5) // hs?
            {
              STB_HSCAN_SET ;
    
			  gpio_pl_func_cfg_gpio  (GPIO_PL_EXT_CAN_HS_RX);
			  gpio_pl_int_clr        (GPIO_PL_EXT_CAN_HS_RX);
			  gpio_pl_falling_irq_ena(GPIO_PL_EXT_CAN_HS_RX);
            }
          )
        }
    }
}


