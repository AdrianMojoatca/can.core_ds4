/*==========================================================================*/
// $Id: can_v_usr_wake.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_v_usr_private.h"
#include "fiq.h"
#include "gpio.h"
#include "core_contract_db3_can_gpio.h"

void gpio_iface_rf_loop_toggle(void);

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
		  gpio_iface_rf_loop_toggle();
        #endif
          
          ATOMIC
          (    
          if(pin_map[CAN_DEV_0] == CAN_DEV_0_P0_0_1) // ft?
            {
			#ifdef DEV_LPC_236x
              STB_FTCAN_SET ;
			#endif
        core_contract_db3_can_gpio_ft_enable(TRUE);
							
        core_contract_db3_can_gpio_irq_select_disable(CORE_DB3_CAN_GPIO_IRQ_FT_RX);
						
            }
          else if (pin_map[CAN_DEV_0] == CAN_DEV_0_P0_21_22) // CAN SW
            {
			#ifdef DEV_LPC_236x
              STB_FTCAN_SET ;
              EN_FTCAN_SET ;
			#endif
        core_contract_db3_can_gpio_irq_select_disable(CORE_DB3_CAN_GPIO_IRQ_SW_RX);
            }
    
          if(pin_map[CAN_DEV_1] == CAN_DEV_1_P0_4_5) // hs?
            {
        core_contract_db3_can_gpio_hs_active(TRUE);
        
        core_contract_db3_can_gpio_irq_select_disable(CORE_DB3_CAN_GPIO_IRQ_HS_RX);
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
        core_contract_db3_can_gpio_ft_enable(FALSE);
              
        core_contract_db3_can_gpio_irq_gpio_enable(CORE_DB3_CAN_GPIO_IRQ_SW_RX);
            }
          else if (pin_map[CAN_DEV_0] == CAN_DEV_0_P0_21_22)
            {
			#ifdef DEV_LPC_236x
              STB_FTCAN_CLR ;
              EN_FTCAN_CLR  ;
			#endif
        core_contract_db3_can_gpio_irq_gpio_enable(CORE_DB3_CAN_GPIO_IRQ_J1850_RX);
            }
    
          if(pin_map[CAN_DEV_1] == CAN_DEV_1_P0_4_5) // hs?
            {
        core_contract_db3_can_gpio_hs_active(FALSE);
    
        core_contract_db3_can_gpio_irq_gpio_enable(CORE_DB3_CAN_GPIO_IRQ_HS_RX);
            }
          )
        }
    }
}


