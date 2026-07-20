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
#define     SENSE_RS_CAN_RPM_FAKE       200
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void rpm_update( UInt16 new_rpm );
static void rpm_trace ( UInt16 new_rpm );
static void update_rpm_with_fake_value(UInt16 value);
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Boolean last_rpm_zero_state;
static Boolean do_rpm_fake        ;
static UInt16  rpm_can_value      ;  
static UInt16  rpm_fake_value     ;  
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_rs_can_rpm_init( void )
{
    last_rpm_zero_state  = 0; 
	do_rpm_fake          = 0; 
	rpm_can_value        = 0;    
	rpm_fake_value       = 0;
}
	
//--------------------------------------------------------------------------//

void sense_rs_rpm_can( UInt16 new_rpm )
{
	if( get_ign_status() )
	{
		if(rpm_can_value != new_rpm)
		{
				//TRACE_SENSE_RS_CAN("[RPM CAN VALUE = %d]\n\r",new_rpm);
		}
		
		rpm_can_value = new_rpm ; 
		
		if(get_rs_start() && !do_rpm_fake && (rpm_can_value == 0))
		{
				do_rpm_fake    = TRUE  ;
			  rpm_fake_value = SENSE_RS_CAN_RPM_FAKE ; 
			
			  TRACE_SENSE_RS_CAN("[ENTER_IN_RPM_FAKE_MODE]\n\r");
		}
				
		
		if(do_rpm_fake && (rpm_can_value >= rpm_fake_value))
		{
				ATOMIC(do_rpm_fake = FALSE);
			
			  TRACE_SENSE_RS_CAN("[EXIT_FROM_RPM_FAKE_MODE]\n\r");
		}
			
		ATOMIC(rpm.word = do_rpm_fake ? rpm_fake_value : new_rpm) ; 		
	}
	else
	{
		rpm.word      =0;
		rpm_can_value =0;
        do_rpm_fake   =FALSE;
	}
	
	rpm_update(rpm.word);	
	rpm_trace (rpm.word);
}

//--------------------------------------------------------------------------//

void sense_rs_can_rpm_fake_enter( UInt16 rpm_fake )
{
	ATOMIC
	( 
		do_rpm_fake    = TRUE     ;
		rpm_fake_value = rpm_fake ; 
		rpm.word       = rpm_fake ;
		rpm_update(rpm.word);		
	)	
}

//--------------------------------------------------------------------------//

void sense_rs_can_rpm_fake_leave( void )
{
	ATOMIC
	( 
		do_rpm_fake    = FALSE ;
		rpm_fake_value = 0     ; 
		rpm.word       = rpm_can_value ;
		rpm_update(rpm.word);		
	)	

}

//--------------------------------------------------------------------------//

UInt16 sense_rs_can_get_real_rpm( void )
{
	return rpm_can_value ; 
}


//--------------------------------------------------------------------------//

void rpm_update( UInt16 new_rpm )
{
	set_rpm_data( new_rpm );	

	#ifndef PLATFORM_CM800
    {
    	tach_update((new_rpm>250) || (new_rpm==0)  ? new_rpm : 251 );

		if( last_rpm_zero_state != !!new_rpm )
		{					
        	last_rpm_zero_state = !!new_rpm ;	

			//cmd_q_rs_insert( last_rpm_zero_state ? INPUT_RPM_SET : INPUT_RPM_CLR , SENSE_RS_CAN_TIMEOUT_CMD_Q , 0 );            
			cmd_q_insert( last_rpm_zero_state ? INPUT_RPM_SET : INPUT_RPM_CLR , SENSE_RS_CAN_TIMEOUT_CMD_Q);            
			
			TRACE_SENSE_RS_CAN("\n\r[%s]\n\r" , last_rpm_zero_state ? "INPUT_RPM_SET" : "INPUT_RPM_CLR" );
		}		
	}
    #endif	
}

//--------------------------------------------------------------------------//

void rpm_trace( UInt16 new_rpm )
{
	#define TIMEOUT_5SEC	( (tmr > T1TC) ? ((tmr-T1TC)>5000*1000*72) : ((T1TC-tmr)>5000*1000*72) ) 
	
	TRACE_ZONE
	( 
		static UInt32 tmr     = 0 ;
		static UInt16 cur_rpm = 0 ;

		if( TIMEOUT_5SEC )
		{
			tmr = T1TC ; 
				
			if( cur_rpm != new_rpm )
			{
				TRACE_SENSE_RS_CAN("\n\r[RPM : %d]\n\r" , new_rpm );			
			}

			cur_rpm = new_rpm ;
		}
	)	

	#undef TIMEOUT_5SEC
}

//--------------------------------------------------------------------------//
