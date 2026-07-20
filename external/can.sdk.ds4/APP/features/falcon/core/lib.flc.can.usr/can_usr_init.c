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

//--------------------------------------------------------------------------//

void can_usr_init(Can_Usr_Setup setup )
{    
    
	PCLKSEL0 |= (UInt32) 0x15 << 26 ;  // CLK = CCLK / 1  for CAN 1,2 and CAN filters
        
    can_usr_err_init();

    can_use[CAN_DEV_0]               = !!(setup&CAN0_ENABLE) ;
    can_usr[CAN_DEV_0].rx_use_buffer = !!(setup&CAN0_RX_BUFFER_ENABLE) ;
	can_usr[CAN_DEV_0].tx_use_buffer = !!(setup&CAN0_TX_BUFFER_ENABLE) ;

	can_use[CAN_DEV_1]               = !!(setup&CAN1_ENABLE) ;	
	can_usr[CAN_DEV_1].rx_use_buffer = !!(setup&CAN1_RX_BUFFER_ENABLE) ;	
	can_usr[CAN_DEV_1].tx_use_buffer = !!(setup&CAN1_TX_BUFFER_ENABLE) ;
    
    if( can_use[ CAN_DEV_0 ]  == TRUE )
    {        
        #if CAN_DEV_0_FT_SW == FALSE //SW CAN bus        
            #ifdef DEV_LPC_176x				
				gpio_pl_set(GPIO_PL_MODE0);
				gpio_pl_set(GPIO_PL_MODE1);
				gpio_pl_direction(GPIO_PL_J1850_TX_NU);
				gpio_pl_direction(GPIO_PL_CAN_SW_TX);			
			#endif        
        #endif
        
        can_usr_rx_dev0_init();
        can_usr_tx_dev0_init();
        
        can_usr_bus_off_init( CAN_DEV_0 );

        can_dev_ena( CAN_DEV_0 );
        
        OUT_EN_FT (TRUE);
        OUT_STB_FT(TRUE);

		TRACE("\r\nCAN USR INIT DEV 0 OK\r\n");
    }
    else
    {
        OUT_EN_FT (FALSE);
        OUT_STB_FT(FALSE);
		TRACE("\r\nCAN USR INIT DEV 0 FAILED\r\n");
    }
    
    if( can_use[ CAN_DEV_1 ]  == TRUE )
    {
        can_usr_rx_dev1_init();    
        can_usr_tx_dev1_init();

        can_usr_bus_off_init( CAN_DEV_1 );
                                                       
        can_dev_ena( CAN_DEV_1 );

        OUT_STB_HS( FALSE );
		TRACE("\r\nCAN USR INIT DEV 1 OK\r\n");
    }
    else
    {
        OUT_STB_HS( TRUE );
		TRACE("\r\nCAN USR INIT DEV 1 FAILED\r\n");        
    }

	
    can_lut_bypass_ena();
    wake_register( can_usr_wake );            
}

//--------------------------------------------------------------------------//

