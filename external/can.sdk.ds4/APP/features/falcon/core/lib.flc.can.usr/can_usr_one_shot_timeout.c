/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_usr_one_shot_timeout.c 19497 2013-02-25 13:41:22Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "can_usr_private.h"
#include "can_private.h"
#include "timeout_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void timeout_dev0( void );
static void timeout_dev1( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void can_usr_one_shot_timeout_init( Can_Dev_Id dev_id)
{
	can_usr[dev_id].tx_timeout = timeout_build (0 ,\
							   (dev_id == CAN_DEV_0) ? (Timeout_Func)timeout_dev0 : (Timeout_Func)timeout_dev1,\
								sizeof( struct s_Timeout)) ;				
}

//--------------------------------------------------------------------------//

void can_usr_one_shot_timeout_start( Can_Dev_Id dev_id  , TskTimeout timeout )
{
	timeout_update( can_usr[dev_id].tx_timeout , timeout );

	timeout_start ( can_usr[dev_id].tx_timeout );
}

//--------------------------------------------------------------------------//

void can_usr_one_shot_timeout_stop( Can_Dev_Id dev_id )
{	
	timeout_stop( can_usr[dev_id].tx_timeout );
}

//--------------------------------------------------------------------------//

Boolean can_usr_one_shot_timeout_test( Can_Dev_Id dev_id )
{
	return timeout_test( can_usr[dev_id].tx_timeout ) ; 
}

//--------------------------------------------------------------------------//

void timeout_dev0( void )
{
	os_evt_signal_i (can_usr[CAN_DEV_0].tx_can_slow->evt_tx , CAN_IRQ_TX_BUS_ERROR) ;    
}

//--------------------------------------------------------------------------//

void timeout_dev1( void )
{
	os_evt_signal_i (can_usr[CAN_DEV_1].tx_can_slow->evt_tx , CAN_IRQ_TX_BUS_ERROR) ;    
}

//--------------------------------------------------------------------------//
