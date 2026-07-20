/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_logic_resume.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
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

void dei_ss_logic_resume( void )
{
	Dei_Ss_Status dei_ss_status  ;

	dei_ss_status_init( &dei_ss_status );

    if( dei_ss_trigger_enable == FALSE )
    {
        dei_ss_status.trigger_active = FALSE ;
    }    
	
	set_ss_memory_trigger_event( dei_ss_status.event_memory[0] );		
	set_ss_memory_trigger_event( dei_ss_status.event_memory[1] );
	set_ss_memory_trigger_event( dei_ss_status.event_memory[2] );
	set_ss_memory_trigger_event( dei_ss_status.event_memory[3] );
	
	if( dei_ss_status.valet )
	{
		set_ss_lock( !!dei_ss_status.arm );		
		set_ss_arm ( !!dei_ss_status.arm );
		            
        cmd_q_rs_insert( INPUT_RF_VALET_TOOGLE , 100 , 0 );

		TRACE_DEI_SS("\n\r[DEI_RS_LOGIC_RESUME : VALET ON ]\n\r");

		return ;
	}


	if( dei_ss_status.arm && dei_ss_status.trigger_active )
	{
		
		set_ss_lock( TRUE );		
		set_ss_arm ( TRUE );
		
		cmd_q_rs_insert( INPUT_OEM_LOCK , 100 , (void *)DEI_SS_SILENT ) ;

		os_tsk_wait( 1000 );

		dei_ss_trigger_start( (Dei_Ss_Trigger_Source) dei_ss_status.trigger_source );

		TRACE_DEI_SS("\n\r[DEI_RS_LOGIC_RESUME : ARM AND TRIGGER ]\n\r");
		
		return ;
	}		
	
	TRACE_DEI_SS("\n\r[DEI_RS_LOGIC_RESUME : %s ]\n\r" , dei_ss_status.arm  ? "ARMED" : "DISARMED" );
	
	if( dei_ss_status.arm )
	{		
		set_ss_lock( TRUE );		
		set_ss_arm ( TRUE );
		
		dei_ss_2nd_disarm = FALSE ; // INPUT_OEM_LOCK extra command inserted after reset

#ifdef PLATFORM_CM800
		cmd_q_rs_insert( INPUT_OEM_LOCK , 100 , (void *)DEI_SS_SILENT ) ;
#else
		cmd_q_rs_insert( INPUT_OEM_LOCK , 100 , (void *)DEI_SS_SILENT_LOGIC_RESUME ) ;
#endif
	}
	else
	{		
		set_ss_lock( FALSE );		
		set_ss_arm ( FALSE );
		
		dei_ss_2nd_disarm = FALSE ; // INPUT_OEM_UNLOCK extra command inserted after reset
#ifdef PLATFORM_CM800
		cmd_q_rs_insert( INPUT_OEM_UNLOCK , 100 , (void *)DEI_SS_SILENT ) ;
#else
		cmd_q_rs_insert( INPUT_OEM_UNLOCK , 100 , (void *)DEI_SS_SILENT_LOGIC_RESUME ) ;
#endif
	}	
}
	
//--------------------------------------------------------------------------//		


