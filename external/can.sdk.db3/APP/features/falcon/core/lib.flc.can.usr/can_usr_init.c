/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_usr_init.c 19497 2013-02-25 13:41:22Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "can_usr_private.h"
#include "core_contract_db3_can_gpio.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean can_usr_dev0_contract_valid(void);

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean can_usr_dev0_speed_to_timing(UInt32 speed, UInt32 *btr, UInt32 *tseg1, UInt32 *tseg2, UInt32 *sjw, UInt32 *sam)
{
    if ((btr == 0) || (tseg1 == 0) || (tseg2 == 0) || (sjw == 0) || (sam == 0))
    {
        return FALSE;
    }

    switch (speed)
    {
        case 0u:
            *btr = 119u + 1u;
            *sjw = 1u + 1u;
            *tseg1 = 10u + 1u;
            *tseg2 = 5u + 1u;
            *sam = 1u + 1u;
            return TRUE;

        case 1u:
            *btr = 79u + 1u;
            *sjw = 1u + 1u;
            *tseg1 = 10u + 1u;
            *tseg2 = 5u + 1u;
            *sam = 1u + 1u;
            return TRUE;

        case 2u:
            *btr = 47u + 1u;
            *sjw = 1u + 1u;
            *tseg1 = 10u + 1u;
            *tseg2 = 5u + 1u;
            *sam = 1u + 1u;
            return TRUE;

        case 3u:
            *btr = 39u + 1u;
            *sjw = 1u + 1u;
            *tseg1 = 10u + 1u;
            *tseg2 = 5u + 1u;
            *sam = 1u;
            return TRUE;

        case 4u:
            *btr = 31u + 1u;
            *sjw = 1u + 1u;
            *tseg1 = 10u + 1u;
            *tseg2 = 5u + 1u;
            *sam = 1u;
            return TRUE;

        case 5u:
            *btr = 15u + 1u;
            *sjw = 1u + 1u;
            *tseg1 = 10u + 1u;
            *tseg2 = 5u + 1u;
            *sam = 1u;
            return TRUE;

        case 6u:
            *btr = 7u + 1u;
            *sjw = 2u + 1u;
            *tseg1 = 10u + 1u;
            *tseg2 = 5u + 1u;
            *sam = 0u;
            return TRUE;

        default:
            return FALSE;
    }
}

static Boolean can_usr_dev0_contract_valid(void)
{
    UInt32 btr;
    UInt32 tseg1;
    UInt32 tseg2;
    UInt32 sjw;
    UInt32 sam;

    if (!CAN_USR_DEV0_RUNTIME_ENABLED())
    {
        return FALSE;
    }

    return can_usr_dev0_speed_to_timing(CAN_CFG_CAN_DEV_0_SPEED, &btr, &tseg1, &tseg2, &sjw, &sam);
}

//--------------------------------------------------------------------------//

void can_usr_init(Can_Usr_Setup setup )
{    
    
	PCLKSEL0 |= (UInt32) 0x15 << 26 ;  // CLK = CCLK / 1  for CAN 1,2 and CAN filters
        
    can_usr_err_init();

    can_use[CAN_DEV_0]               = !!(setup&CAN0_ENABLE) && can_usr_dev0_contract_valid();
    can_usr[CAN_DEV_0].rx_use_buffer = !!(setup&CAN0_RX_BUFFER_ENABLE) ;
	can_usr[CAN_DEV_0].tx_use_buffer = !!(setup&CAN0_TX_BUFFER_ENABLE) ;

	can_use[CAN_DEV_1]               = !!(setup&CAN1_ENABLE) ;	
	can_usr[CAN_DEV_1].rx_use_buffer = !!(setup&CAN1_RX_BUFFER_ENABLE) ;	
	can_usr[CAN_DEV_1].tx_use_buffer = !!(setup&CAN1_TX_BUFFER_ENABLE) ;
    
    if( can_use[ CAN_DEV_0 ]  == TRUE )
    {        
        if( !CAN_USR_DEV0_IS_FT() )
        {
            #ifdef DEV_LPC_176x				
                core_contract_db3_can_gpio_prepare_sw_tx();
			#endif        
        }
        
        can_usr_rx_dev0_init();
        can_usr_tx_dev0_init();
        
        can_usr_bus_off_init( CAN_DEV_0 );

        can_dev_ena( CAN_DEV_0 );
        
        core_contract_db3_can_gpio_ft_enable(TRUE);
        core_contract_db3_can_gpio_ft_standby(TRUE);

		TRACE("\r\nCAN USR INIT DEV 0 OK\r\n");
    }
    else
    {
        core_contract_db3_can_gpio_ft_enable(FALSE);
        core_contract_db3_can_gpio_ft_standby(FALSE);
		TRACE("\r\nCAN USR INIT DEV 0 FAILED\r\n");
    }
    
    if( can_use[ CAN_DEV_1 ]  == TRUE )
    {
        can_usr_rx_dev1_init();    
        can_usr_tx_dev1_init();

        can_usr_bus_off_init( CAN_DEV_1 );
                                                       
        can_dev_ena( CAN_DEV_1 );

                core_contract_db3_can_gpio_hs_active(TRUE);
		TRACE("\r\nCAN USR INIT DEV 1 OK\r\n");
    }
    else
    {
                core_contract_db3_can_gpio_hs_active(FALSE);
		TRACE("\r\nCAN USR INIT DEV 1 FAILED\r\n");        
    }

	
    can_lut_bypass_ena();
    wake_register( can_usr_wake );            
}

//--------------------------------------------------------------------------//

