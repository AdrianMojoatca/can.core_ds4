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
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void sense_handbrake( Boolean instant_level );
static void handbrake_reset( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Ied     sense_rs_ied_handbrake ; 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_rs_can_handbrake_init( void )
{
    sense_rs_ied_handbrake = ied_create( FALSE , sense_rs_can_cfg->handbrake_debounce , sense_rs_can_cfg->handbrake_debounce );	
	
	sense_rs_handbrake_reset();	
}

//--------------------------------------------------------------------------//

Boolean sense_rs_handbrake_can( Boolean instant_level )
{
    sense_handbrake( instant_level || !get_ign_status() );                
	
    return get_handbrake_status();
}
  
//--------------------------------------------------------------------------//

void sense_handbrake( Boolean instant_level )
{
    SInt8 edge_detect ; 

	Boolean old_status = get_handbrake_status();

	ATOMIC( edge_detect = ied_detect( sense_rs_ied_handbrake , instant_level ) );

	if(  edge_detect )
	{
	    set_handbrake_data_status( ied_get_level( sense_rs_ied_handbrake ) ); 
	
        if( old_status != get_handbrake_status() )
        {
            brake.bit.handbrake = get_handbrake_status();

			//if( IS_NOT_RSR_SETUP() )
			{
				if( brake.bit.handbrake )
				{
					gpio_iface_handbrake_on(); // new approach
				}
				else
				{
					gpio_iface_handbrake_off(); // new approach
				}
			}
						
			cmd_q_insert( get_handbrake_status()  ? INPUT_HANDBRAKE_STATUS_ON : INPUT_HANDBRAKE_STATUS_OFF , SENSE_RS_CAN_TIMEOUT_CMD_Q);
	
			TRACE_SENSE_RS_CAN("\n\r[HANDBRAKE %s]\n\r" , get_handbrake_status()  ? "ON":"OFF" );				
        }
	}    
}

//--------------------------------------------------------------------------//

void sense_rs_handbrake_reset( void )
{
	UInt8 i ;

	ATOMIC( for( i = 0  ; i < sense_rs_can_cfg->handbrake_debounce + 1 ; i ++ )sense_rs_handbrake_can(TRUE ));
}

//--------------------------------------------------------------------------//
