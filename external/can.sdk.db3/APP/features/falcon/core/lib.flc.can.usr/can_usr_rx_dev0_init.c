/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_usr_rx_dev0_init.c 19497 2013-02-25 13:41:22Z martin.bouchard $
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

void can_usr_rx_dev0_init( void )
{
    Tsk tsk ;
    UInt32 btr;
    UInt32 tseg1;
    UInt32 tseg2;
    UInt32 sjw;
    UInt32 sam;
    UInt32 pin_map;

    if (!can_usr_dev0_speed_to_timing(CAN_CFG_CAN_DEV_0_SPEED, &btr, &tseg1, &tseg2, &sjw, &sam))
    {
        can_use[CAN_DEV_0] = FALSE;
        return;
    }

    pin_map = CAN_USR_DEV0_IS_FT() ? CAN_DEV_0_P0_0_1 : CAN_DEV_0_P0_21_22;

    can_usr[CAN_DEV_0].can_dev = can_dev_create( CAN_DEV_0 , btr   , tseg1                 ,\
                                                             tseg2 , sjw   , sam ,\
                                                             pin_map );

        
    can_usr[CAN_DEV_0].rx_can  = can_rx_create( can_usr[CAN_DEV_0].can_dev );
    
    can_usr[CAN_DEV_0].rx_reg_fast          = reg_table_create(REG_ATOMIC_USE_RES);
    can_usr[CAN_DEV_0].rx_reg_slow          = reg_table_create(REG_ATOMIC_USE_RES);

    can_usr[CAN_DEV_0].bus_off_ind          = FALSE ; 
    can_usr[CAN_DEV_0].filter.size          = 0;
    can_usr[CAN_DEV_0].filter.enable        = FALSE;
    
    
    tsk = os_tsk_create( can_usr_rx_tsk , CAN_DEV_0_RX_TSK_PRI  , CAN_DEV_0_RX_TSK_STK  , 0 , "can_dev_0_rx"   ) ;        
    os_tsk_start( tsk , (void*)CAN_DEV_0 ); 

    if( can_usr[CAN_DEV_0].rx_use_buffer && (CAN_CFG_CAN_DEV_0_RX_BUFFER_SIZE > 0u) )
    {
        can_usr[CAN_DEV_0].rx_pipe = pipe_create( CAN_CFG_CAN_DEV_0_RX_BUFFER_SIZE , sizeof( struct s_Can_Msg ) );  
		
		tsk = os_tsk_create( can_usr_rx_handler_tsk , CAN_DEV_0_RX_BUFFER_TSK_PRI  , CAN_DEV_0_RX_BUFFER_TSK_STK  , 0 , "can_dev_0_rx_buffer_tsk") ;                
    
        os_tsk_start( tsk , (void*)CAN_DEV_0 );        
    }
    else
    {
        can_usr[CAN_DEV_0].rx_use_buffer = FALSE;
    }
}

//--------------------------------------------------------------------------//

