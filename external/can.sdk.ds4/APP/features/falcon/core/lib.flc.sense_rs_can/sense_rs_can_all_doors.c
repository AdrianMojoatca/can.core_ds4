/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_prk_flash.c 1106 2010-04-27 13:30:39Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_can_private.h"
#include "tsk_rs_private.h"  // For tsk_rs_tke_door_shutdown() 
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Boolean  sense_rs_can_all_door_old_status;      
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_rs_can_all_doors_init( void )
{
    sense_rs_can_all_door_old_status=FALSE;
}            

//--------------------------------------------------------------------------//

void sense_rs_can_door_open_data( void )
{
	Boolean cur_status = get_door_status() ;
	
	if( !sense_rs_can_all_door_old_status && cur_status )
	{

		if( get_door_source() == SOURCE_DATA )
		{                    
			TRACE("\r\nSENSE RS CAN DOOR OPEN DATA\r\n");
			TRACE("GENERATING INPUT_ALL_DOOR_STATUS_OPEN command");
			
			cmd_q_insert( INPUT_ALL_DOOR_STATUS_OPEN , SENSE_RS_CAN_TIMEOUT_CMD_Q); 
			
			TRACE("INPUT_ALL_DOOR_STATUS_OPEN command sent to queue");
			
			// Call Falcon takeover system directly since cmd_q doesn't reach Falcon
			TRACE("CALLING tsk_rs_tke_door_shutdown() DIRECTLY");
			tsk_rs_tke_door_shutdown();
			
			//if( IS_NOT_RSR_SETUP() )
			{
				DOOR_SENSE_SET ; 				
			}

            TRACE("\n\r[ALL_DOORS_OPEN]");
		}
	}

	ATOMIC( sense_rs_can_all_door_old_status = cur_status );
}

/*--------------------------------------------------------------------------*/

void sense_rs_can_door_close_data( void )
{
	Boolean cur_status = get_door_status() ;
	
	if( sense_rs_can_all_door_old_status && !cur_status )
	{
		if( get_door_source() == SOURCE_DATA )
		{		
			cmd_q_insert( INPUT_ALL_DOOR_STATUS_CLOSE , SENSE_RS_CAN_TIMEOUT_CMD_Q); 
			
			//if( IS_NOT_RSR_SETUP() )
			{
				DOOR_SENSE_CLR ; 			
			}

            TRACE_SENSE_RS_CAN("\n\r[ALL_DOORS_CLOSE]");
		}
	}

	ATOMIC( sense_rs_can_all_door_old_status = cur_status );
}

/*--------------------------------------------------------------------------*/

