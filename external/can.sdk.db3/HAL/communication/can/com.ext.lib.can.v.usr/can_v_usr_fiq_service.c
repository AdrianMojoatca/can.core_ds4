/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_v_usr_fiq_service.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "fiq.h"
#include "can_v_usr_private.h"	 
#include "gpio.h"
#include "core_contract_db3_can_gpio.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define CAN_HS_ENA      (pin_map[CAN_DEV_1] == CAN_DEV_1_P0_4_5)
#define CAN_HS_FAL_INT  core_contract_db3_can_gpio_irq_falling_stat(CORE_DB3_CAN_GPIO_IRQ_HS_RX)		//(IO0_INT_STAT_F & (1u << 4))
#define CAN_FT_ENA      (pin_map[CAN_DEV_0] == CAN_DEV_0_P0_0_1)
#define CAN_FT_FAL_INT  core_contract_db3_can_gpio_irq_falling_stat(CORE_DB3_CAN_GPIO_IRQ_FT_RX)		//(IO0_INT_STAT_F & (1u << 0))
#define CAN_SW_ENA      (pin_map[CAN_DEV_0] == CAN_DEV_0_P0_21_22)
#define CAN_SW_FAL_INT  core_contract_db3_can_gpio_irq_falling_stat(CORE_DB3_CAN_GPIO_IRQ_SW_RX)		//(IO0_INT_STAT_F & (1u << 21))

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
void can_v_usr_fiq_service (void)
{
	if((CAN_HS_ENA && CAN_HS_FAL_INT) || (CAN_FT_ENA && CAN_FT_FAL_INT) || (CAN_SW_ENA && CAN_SW_FAL_INT)) 
    {	
        can_v_usr_wake (s_WAKE)  ;
		can_wake_swi = TRUE     ;
		irq_user_trigger()	;
		}
}

