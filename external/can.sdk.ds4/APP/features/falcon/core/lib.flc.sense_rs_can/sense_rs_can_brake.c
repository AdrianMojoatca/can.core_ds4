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
static void sense_brake( Boolean instant_level );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Ied     sense_rs_ied_brake  ;
static Boolean skip_brake          ;

static Boolean brake_status_can    ;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_rs_can_brake_init( void )
{
    sense_rs_ied_brake = ied_create( FALSE , sense_rs_can_cfg->brake_debounce , sense_rs_can_cfg->brake_debounce );

	skip_brake       = FALSE ;    
    brake_status_can = FALSE ;    

	sense_rs_brake_reset();
}

//--------------------------------------------------------------------------//

Boolean sense_rs_brake_can( Boolean instant_level )
{
    UInt8 i ; 
    
    if(sense_rs_can_brake_skip==TRUE)return get_brake_status() ;       
		
    if((get_rs_start()==FALSE)&&(sense_rs_can_noise_window==FALSE))
    {
        // if we are not during noise window and the cranking period do not use debounce for brake
        
        ATOMIC( for( i=0;i< sense_rs_can_cfg->brake_debounce + 1 ; i ++ )sense_brake(instant_level && get_ign_status())); 
    }
    else
    {
        sense_brake( instant_level && get_ign_status()); 
    }
	
    return get_brake_status();
}

//--------------------------------------------------------------------------//

void sense_rs_can_brake_force_on( void )
{
    UInt8 i ;    
	
	ATOMIC( skip_brake = FALSE ); 
    
    ATOMIC( sense_rs_can_brake_skip=TRUE);
    
    ATOMIC( for( i = 0  ; i < sense_rs_can_cfg->brake_debounce + 1 ; i ++ )sense_brake( TRUE ));                                                       
    
	TRACE_SENSE_RS_CAN("\n\r----------------[ BRAKE_FORCED_ON ]\n\r");    
}    

//--------------------------------------------------------------------------//

void sense_rs_can_brake_force_off( void )
{
    UInt8 i ;     
    
    ATOMIC( for( i = 0  ; i < sense_rs_can_cfg->brake_debounce + 1 ; i ++ )sense_brake( FALSE ));                                                       
    
    ATOMIC( sense_rs_can_brake_skip=FALSE);
	
	TRACE_SENSE_RS_CAN("\n\r----------------[ BRAKE_FORCED_OFF ]\n\r");    
}    

//--------------------------------------------------------------------------//

void sense_rs_can_brake_skip_enter( void )
{
	ATOMIC( skip_brake = TRUE );
}

//--------------------------------------------------------------------------//

void sense_rs_can_brake_skip_leave( void )
{
	ATOMIC( skip_brake = FALSE );
}

//--------------------------------------------------------------------------//

void sense_brake( Boolean instant_level )
{
	SInt8 edge_detect ;

	Boolean old_status = get_brake_status();

    brake_status_can = instant_level ; 

	ATOMIC( edge_detect = ied_detect( sense_rs_ied_brake , instant_level && !skip_brake ) );
		
	if( edge_detect )
	{			
	    set_brake_data_status(  ied_get_level( sense_rs_ied_brake ) ) ;
		
		TRACE("\r\nINSIDE SENSE BRAKE\r\n");
			
		if( old_status != get_brake_status() )
        {			

			brake.bit.brake = get_brake_status();

			if( IS_NOT_RSR_SETUP() )
			{
				if( brake.bit.brake )
				{
					BRAKE_SENSE_SET ; 
				}
				else
				{
					BRAKE_SENSE_CLR ;
				}
			}

			cmd_q_insert( get_brake_status() ? INPUT_BRAKE_STATUS_PRESS :  INPUT_BRAKE_STATUS_RELEASE , SENSE_RS_CAN_TIMEOUT_CMD_Q);

			//TRACE_SENSE_RS_CAN("\n\r[BRAKE %s]\n\r" , get_brake_status() ? "PRESSED":"RELEASED" );					
			TRACE("\n\r[BRAKE %s]\n\r" , get_brake_status() ? "PRESSED":"RELEASED" );					
        }			
	}	
}

//--------------------------------------------------------------------------//

void sense_rs_brake_reset( void )
{
	UInt8 i ;

	ATOMIC( for( i = 0  ; i < sense_rs_can_cfg->brake_debounce + 1 ; i ++ )sense_rs_brake_can(FALSE ));

	ATOMIC( sense_rs_can_brake_skip=FALSE );
}

//--------------------------------------------------------------------------//

Boolean sense_rs_can_brake_get_status( void )
{
    return brake_status_can ; 
}

//--------------------------------------------------------------------------//
