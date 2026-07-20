/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_can_speed.c 1106 2010-04-27 13:30:39Z florin.olariu $
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
static void speed_cmd  ( void );
static Timeout_Q_Status end_brake_pulse( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Boolean last_moving_status ;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_rs_can_speed_init( void )
{
    last_moving_status = 0 ;     
}

//--------------------------------------------------------------------------//

void sense_rs_speed_can( UInt32 new_speed )
{
    #if STATUS_SPEED_ENABLE == 1
    
   // #define TIMEOUT_5SEC	( (tmr > T1TC) ? ((tmr-T1TC)>5000*1000*72) : ((T1TC-tmr)>5000*1000*72) ) 
    #define TIMEOUT_5SEC	( (tmr > T2TC) ? ((tmr-T2TC)>5000*1000*72) : ((T2TC-tmr)>5000*1000*72) ) 

	UInt16 temp_speed = get_ign_status() ? new_speed : 0 ;

	TRACE("\rCRT SPEED: %d km/h", new_speed);
	
    if( (get_rs_start()) || (sense_rs_can_noise_window==TRUE))  // DO NOT REPORT speed during noise window period or the cranking period
    {
        temp_speed = 0;
	}
	
 	TRACE_ZONE
	( 
		static UInt32 tmr       = 0 ;
		static UInt32 cur_speed = 0 ;

		if( TIMEOUT_5SEC )
		{
			//tmr = T1TC ;  
			tmr = T2TC ;  
				
			if( cur_speed != temp_speed )
			{
				TRACE_SENSE_RS_CAN("\n\r[SPEED : %d]\n\r" , temp_speed );
				
				if( (get_rs_start()) || (sense_rs_can_noise_window==TRUE))  // noise window period or the cranking period
				{
					TRACE_SENSE_RS_CAN("\n\r[SPEED IGNORED !]\n\r" );
				}
			}

			cur_speed = temp_speed ; 
		}            
	)
			
    set_speed( temp_speed );
        
	if( get_ign_status())
		speed.bit.is_detected = TRUE;
	
    speed.byte = (UInt8)temp_speed;
	
    if( last_moving_status != !!temp_speed )
    {
        last_moving_status = !!temp_speed ; 
        
        speed_cmd();

        if( !!new_speed && get_rs_gwr() && get_rpm() && sense_rs_can_cfg->speed_shutdown_on )
        {
            TRACE_SENSE_RS_CAN("\n\r----------------[SPEED SHUTDOWN ACTIVE]\n\r");    

			timeout_q_remove( (Timeout_Q_Func)end_brake_pulse                   ) ;
			timeout_q_loop  ( (Timeout_Q_Func)end_brake_pulse ,  NULL , 200 , 1 ) ;
			
            sense_rs_can_brake_force_on();
        }
    }
    	
	#undef TIMEOUT_5SEC   

    #endif
}

//--------------------------------------------------------------------------//

void speed_cmd( void )
{
    cmd_q_insert( last_moving_status ? INPUT_SPEED_SET : INPUT_SPEED_CLR , SENSE_RS_CAN_TIMEOUT_CMD_Q);            
    
    TRACE_SENSE_RS_CAN("\n\r[%s]\n\r" , last_moving_status ? "INPUT_SPEED_SET" : "INPUT_SPEED_CLR" );
}    

//--------------------------------------------------------------------------//   

Timeout_Q_Status end_brake_pulse( void )
{
	sense_rs_can_brake_force_off();
	
	return TIMEOUT_Q_COMPLETE ; 
}
//--------------------------------------------------------------------------//
