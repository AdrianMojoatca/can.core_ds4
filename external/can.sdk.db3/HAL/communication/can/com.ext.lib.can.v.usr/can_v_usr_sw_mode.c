/*==========================================================================*/
// $Id: can_v_usr_sw_mode.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "can_v_usr_private.h"
#include "core_contract_db3_can_gpio.h"

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

void can_v_usr_sw_mode (SWCAN_Mode mode)
{
	switch (mode)
	{
		case SWCAN_WAKE_UP_MODE:
			ATOMIC 
			(
				#ifdef DEV_LPC_236x
						STB_FTCAN_SET ;
						EN_FTCAN_CLR ;
				#else
						core_contract_db3_can_gpio_mode0_set(FALSE);
						core_contract_db3_can_gpio_mode1_set(TRUE);
				#endif
			)
			break ;

		case SWCAN_NORMAL_MODE:
			ATOMIC
			(
				#ifdef DEV_LPC_236x
						STB_FTCAN_SET ;
						EN_FTCAN_SET ;
				#else
						core_contract_db3_can_gpio_mode0_set(TRUE);
						core_contract_db3_can_gpio_mode1_set(TRUE);
				#endif
			)
			break ;
	}
}


