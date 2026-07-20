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
static void sense_ign( Boolean instant_level );
static Timeout_Q_Status noise_window_timeout( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_rs_can_ign_init( void )
{
    sense_rs_can_noise_window = FALSE;
}

//--------------------------------------------------------------------------//

Boolean sense_rs_ign_can( Boolean instant_level )
{			
	
	sense_ign( instant_level ); 
    
    return get_ign_status();
}

//--------------------------------------------------------------------------//

void sense_ign( Boolean instant_level )
{
    SInt8 edge_detect ; 

	Boolean old_status = get_ign_status();
    
    if(old_status != instant_level)
    {
        TRACE_SENSE_RS_CAN("\n\r[TRIGGER NOISE WINDOW TIMEOUT]\n\r");
        
         //ignition state changed  , start the noise window timoeut
            
         timeout_q_remove((Timeout_Q_Func)noise_window_timeout);
            
         ATOMIC(sense_rs_can_noise_window=TRUE);
           
         timeout_q_insert((Timeout_Q_Func)noise_window_timeout,NULL,100,1,1);//1000 ms timoeut

		if( instant_level == FALSE )
		{		
			sense_rs_rpm_can         (0);
			sense_rs_brake_reset     ();
			sense_rs_speed_can       (FALSE);
			sense_rs_glowplug_off_can(TRUE);
		}					
    }
	
    set_ign_data_status( instant_level ); 

	//TRACE("\r\n old_status vs get_ign_status: %02X vs %02X\r\n", old_status, get_ign_status());

    if( old_status != get_ign_status() )
    {
        TRACE("\r\nSENSE IGN STATUS\r\n");
		
		key.bit.ign = get_ign_status();

		//if( IS_NOT_RSR_SETUP() )
		{
			if( key.bit.ign )
			{
				IGNITION_SENSE_SET ; 
			}
			else
			{
				IGNITION_SENSE_CLR ; 
			}
		}
        
        if(!get_ign_status())
        {
            ATOMIC(sense_rs_can_brake_skip=FALSE);
        }
		
		cmd_q_insert( get_ign_status() ? INPUT_IGN_STATUS_ON :  INPUT_IGN_STATUS_OFF , SENSE_RS_CAN_TIMEOUT_CMD_Q);

        TRACE("\n\r[IGNITION %s]\n\r" , get_ign_status() ? "ON":"OFF" );
				
    }
}

//--------------------------------------------------------------------------// 

Timeout_Q_Status noise_window_timeout( void )
{
    TRACE_SENSE_RS_CAN("\n\r[NOISE WINDOW TIMEOUT EXPIRED]\n\r");
        
    ATOMIC(sense_rs_can_noise_window=FALSE);
    
    return TIMEOUT_Q_COMPLETE;
}

//--------------------------------------------------------------------------// 
