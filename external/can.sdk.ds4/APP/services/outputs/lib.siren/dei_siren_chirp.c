/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_siren_chirp.c 20820 2013-05-16 12:38:20Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_siren_private.h"
#include "intl_port.h"
#include "prg_out.h"
#include "status_system.h"
#include "dei_ss.h"
#include "stdbool.h"


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#if DEI_SIREN_USE_SYSTEM_TIMER == 1
#define DEI_SIREN_RESET()					    ATOMIC( out_q_remove( dei_siren_out_q , (Out_Func)dei_siren_control , TRUE ) );
#define DEI_SIREN(t_offset,t_on,t_off,pulse)    ATOMIC( out_q_insert( dei_siren_out_q , (Out_Func)dei_siren_control , 2*t_offset , 2*t_on , 2*t_off , pulse )) ;
#else
#define DEI_SIREN_RESET()					    PER_EXEC_ATOMIC( out_q_remove( dei_siren_out_q , (Out_Func)dei_siren_control , TRUE ) );
#define DEI_SIREN(t_offset,t_on,t_off,pulse)    PER_EXEC_ATOMIC( out_q_insert( dei_siren_out_q , (Out_Func)dei_siren_control , t_offset , t_on , t_off , pulse )) ;
#endif


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void dei_siren_trace_chirp( Dei_Siren_Chirp chirp );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
UInt8 reset_delay;

//--------------------------------------------------------------------------//

Boolean dei_siren_chirp( Dei_Siren_Chirp chirp )
{		
	//UInt8 siren_order, reset_delay;
	dei_siren_init();
	DEI_SIREN_RESET();
	
	 if((chirp == DEI_SIREN_TRIGGER)
		   || (chirp == DEI_SIREN_CHIRP_THEN_TRIGGER))
		 reset_delay = 30;
	 else
		 reset_delay = 5;
	 
    prg_out_siren(chirp);
	 	 	
		if( (chirp == DEI_SIREN_ARM) || (chirp == DEI_SIREN_ARM_NOTIFICATION) || (chirp == DEI_SIREN_ZONE_BYPASS) || (chirp == DEI_SIREN_FULL_SILENT_ARM) || (chirp == DEI_SIREN_SECOND_ARM) || (chirp == DEI_SIREN_SENSOR_WARNING))
		{
			DEI_SIREN( 20 , 3 , 20 , 1 ) ;		
		}


		if(( chirp == DEI_SIREN_DISARM_NO_EVENT ) || (chirp == DEI_SIREN_SENSOR_TRIGGER))
		{
			DEI_SIREN( 20 , 3 , 20 , 2 ) ;		
		}


		if( chirp == DEI_SIREN_DISARM_LOW_BATT )
		{
			DEI_SIREN( 20 , 3 , 20 , 1 ) ;		
		}

		if( chirp == DEI_SIREN_DISARM_TRIGGER_ON )
		{
			DEI_SIREN( 20 , 3 , 20 , 4 ) ;		
		}

		if( chirp == DEI_SIREN_DISARM_NPC_ON )
		{
			DEI_SIREN( 20 , 3 , 20 , 5 ) ;		
		}
		
		if( chirp == DEI_SIREN_CHIRP_THEN_TRIGGER )
		{
			DEI_SIREN( 50  , 3    , 20 , 10);				
			DEI_SIREN( 280 ,18000 , 1  , 0 );				
		}

		if( chirp == DEI_SIREN_TRIGGER )
		{	
			DEI_SIREN( 50 , 18000  , 1  , 0);
		}

		if( chirp == DEI_SIREN_WARNING )
		{
			DEI_SIREN( 50 , 3 , 20 , 10 ) ;		
		}
		
		if( chirp == DEI_SIREN_CAR_FINDER )
		{		
			DEI_SIREN(1  , 50 , 1  , 1 ) ;		
		}
		
		if( chirp == DEI_SIREN_REMOTE_PROGRAMMING_ENTER )
		{
			DEI_SIREN(50 , 3  , 20 , 1 ) ;		
		}
					
		
		if( chirp == DEI_SIREN_REMOTE_DELETE_ENTER )
		{
			DEI_SIREN(50 , 3  , 20 , 2 ) ;		
		}
		
		if( chirp == DEI_SIREN_ZAP_ENTER )
		{
			DEI_SIREN(50 , 3  , 20 , 3 ) ;
		}
		
		
		if( chirp == DEI_SIREN_TACHO_DELETE_ENTER )
		{
			DEI_SIREN(50 , 3  , 20 , 4 ) ;
		}
		

		if( chirp == DEI_SIREN_REMOTE_PROGRAMMING_DONE || chirp == DEI_SIREN_REMOTE_LEARNED)
		{
			DEI_SIREN(50 , 50  , 20 , 1 ) ;
		}

		
		if( chirp == DEI_SIREN_PROGRAMMING_EXIT )
		{
			DEI_SIREN(50 , 3  , 20 , 1 ) ;
			DEI_SIREN(73 , 50 , 1  , 1 ) ;
		}
		
		if( chirp == DEI_SIREN_SENSOR_MAX_LIMIT )
		{
			DEI_SIREN(50 , 3  , 20 , 2 ) ;
			DEI_SIREN(93 , 50 , 1  , 1 ) ;
		}

		if( chirp == DEI_SIREN_SENSOR_MIN_LIMIT )
		{
			DEI_SIREN(50 , 3  , 20 , 1 ) ;
			DEI_SIREN(73 , 50 , 1  , 1 ) ;
		}

		if( chirp == DEI_SIREN_SENSOR_LEVEL_UP )
		{
			DEI_SIREN(50 , 3  , 20 , 2 ) ;
		}

		if( chirp == DEI_SIREN_SENSOR_LEVEL_DOWN )
		{
			DEI_SIREN(50 , 3  , 20 , 1 ) ;
		}
		
		if( chirp == DEI_SIREN_SENSOR_DEFAULT_SETTING )
		{
			DEI_SIREN( 50 , 3  , 20 , 3 ) ;
		}

		if( chirp == DEI_SIREN_SENSOR_ADJUST_ENTER )
		{		
			DEI_SIREN(1  , 50 , 1  , 1 ) ;		
		}
		
		if( (chirp == DEI_SIREN_VALET_MODE_ON) ||(chirp == DEI_SIREN_COMBO_VALET_ON) )
		{
			DEI_SIREN(50 , 50  , 20 , 2 ) ;
		}
		if( (chirp == DEI_SIREN_VALET_MODE_OFF) ||(chirp == DEI_SIREN_COMBO_VALET_OFF) )
		{
			DEI_SIREN(50 , 50  , 20 , 1 ) ;
		}
		if( chirp == DEI_SIREN_SENSOR_PROGRAMMING_EXIT )
		{
			DEI_SIREN(50 , 3  , 20 , 3 ) ;
		}


    TRACE_ZONE( dei_siren_trace_chirp( chirp ) );
	
	return cmd_q_rs_insert( INPUT_CHIRP , 100 , (void*)chirp );
}


//--------------------------------------------------------------------------//
void dei_siren_trace_chirp( Dei_Siren_Chirp chirp )
{		
#if DEBUG_TEXT == 1
	if( chirp == DEI_SIREN_ARM                     ){ TRACE_DEI_SIREN("S01") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_ARM_NOTIFICATION        ){ TRACE_DEI_SIREN("S02") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_ZONE_BYPASS             ){ TRACE_DEI_SIREN("S03") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_DISARM_NO_EVENT         ){ TRACE_DEI_SIREN("S04") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_DISARM_TRIGGER_ON       ){ TRACE_DEI_SIREN("S05") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_DISARM_NPC_ON           ){ TRACE_DEI_SIREN("S06") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_CHIRP_THEN_TRIGGER      ){ TRACE_DEI_SIREN("S07") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_TRIGGER                 ){ TRACE_DEI_SIREN("S08") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_WARNING                 ){ TRACE_DEI_SIREN("S09") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_CAR_FINDER              ){ TRACE_DEI_SIREN("S10") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_DISARM_LOW_BATT         ){ TRACE_DEI_SIREN("S11") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_REMOTE_PROGRAMMING_ENTER){ TRACE_DEI_SIREN("S12") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_REMOTE_PROGRAMMING_DONE ){ TRACE_DEI_SIREN("S13") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_REMOTE_DELETE_ENTER     ){ TRACE_DEI_SIREN("S14") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_TACHO_DELETE_ENTER      ){ TRACE_DEI_SIREN("S15") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_PROGRAMMING_EXIT        ){ TRACE_DEI_SIREN("S16") ; os_tsk_wait( 10 ); };	
	if( chirp == DEI_SIREN_FULL_SILENT_ARM         ){ TRACE_DEI_SIREN("S17") ; os_tsk_wait( 10 ); };		
#endif
}

void dei_siren_send_zone(void)
{
	bool is_other = true;
	
	if((get_door_status() && (dei_feature_get(DEI_FEAT_DOOR_TRIGGER_ERROR_CHIRP)==2)) || get_inst_trig_wire_status())
	{
		is_other = false;
		prg_out_siren( DEI_SIREN_ZONE_DOOR );
	}
	if(get_trunk_status())
	{
		is_other = false;
		prg_out_siren( DEI_SIREN_ZONE_TRUNK );
	}
	if(get_hood_status())
	{
		is_other = false;
		prg_out_siren( DEI_SIREN_ZONE_HOOD );
	}
	
	if(is_other)
		prg_out_siren( DEI_SIREN_ZONE_OTHER );
}
//--------------------------------------------------------------------------//
void dei_siren_send_zone_trigger(void)//calls recent trigger output 
{
	UInt8 x;
	UInt8 pattern;

	Dei_Siren_Chirp chirp_patt[] = 
	{
		DEI_SIREN_ZONE_TRUNK  , 
		DEI_SIREN_ZONE_SHOCK ,
		DEI_SIREN_ZONE_DOOR   ,
		DEI_SIREN_ZONE_IGNITION    ,
		DEI_SIREN_ZONE_HOOD   ,
		DEI_SIREN_ZONE_TILT,
		DEI_SIREN_ZONE_OTHER,
	};

	if(get_ss_last_trigger())
		prg_out_siren(chirp_patt[get_ss_last_trigger() - 1]);

}
void dei_siren_send_zone_npc(void)
{
	if(get_ss_sensor_npc(1))
		prg_out_siren( DEI_SIREN_ZONE_SHOCK );
	if(get_ss_sensor_npc(2))
		prg_out_siren( DEI_SIREN_ZONE_TILT );
	if(get_ss_sensor_npc(0))
		prg_out_siren( DEI_SIREN_ZONE_OTHER );
}
//--------------------------------------------------------------------------//

#undef DEI_SIREN_RESET
#undef DEI_SIREN
