/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_usr_rx_dev1_init.c 19497 2013-02-25 13:41:22Z martin.bouchard $
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

void can_usr_rx_dev1_init( void )
{
    Tsk tsk ; 

    can_usr[CAN_DEV_1].can_dev = can_dev_create( CAN_DEV_1 , CAN_DEV_1_BTR   , CAN_DEV_1_TSEG1                 ,\
                                                             CAN_DEV_1_TSEG2 , CAN_DEV_1_SJW   , CAN_DEV_1_SAM ,\
                                                             CAN_DEV_1_P0_4_5   );
        
    can_usr[CAN_DEV_1].rx_can  = can_rx_create( can_usr[CAN_DEV_1].can_dev );
    
    can_usr[CAN_DEV_1].rx_reg_fast          = reg_table_create(REG_ATOMIC_USE_RES);
    can_usr[CAN_DEV_1].rx_reg_slow          = reg_table_create(REG_ATOMIC_USE_RES);

    can_usr[CAN_DEV_1].bus_off_ind          = FALSE ; 
    can_usr[CAN_DEV_1].filter.size          = 0;
    can_usr[CAN_DEV_1].filter.enable        = FALSE;
        
    tsk = os_tsk_create( can_usr_rx_tsk   , CAN_DEV_1_RX_TSK_PRI  , CAN_DEV_1_RX_TSK_STK  , 0 , "can_dev_1_rx"   ) ;        
    os_tsk_start( tsk , (void*)CAN_DEV_1 );
	 

    if( can_usr[CAN_DEV_1].rx_use_buffer )
    {
        can_usr[CAN_DEV_1].rx_pipe = pipe_create( CAN_DEV_1_RX_BUFFER_SIZE , sizeof( struct s_Can_Msg ) );
		
		tsk = os_tsk_create( can_usr_rx_handler_tsk , CAN_DEV_1_RX_BUFFER_TSK_PRI  , CAN_DEV_1_RX_BUFFER_TSK_STK  , 0 , "can_dev_1_rx_buffer_tsk") ;        
        
        os_tsk_start( tsk , (void*)CAN_DEV_1 ); 
	
		TRACE("\r\n CAN USR RX DEV1 INIT\r\n");
		
    }

	  
}

//--------------------------------------------------------------------------//

