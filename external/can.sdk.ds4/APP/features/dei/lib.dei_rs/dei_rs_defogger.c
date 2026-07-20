/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_defogger.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
#include "vrtc_f.h"

#ifdef PLATFORM_933		// (BUILD_NGMM == 1)
	#include "temperature.h"
#endif
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Rs_Defogger
{
    Vrtc vrtc;
}Dei_Rs_Defogger;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void    dei_rs_defogger_timeout          ( void );
static Boolean dei_rs_defogger_out_is_programmed( void );
static void dei_rs_defogger_turn_off( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Dei_Rs_Defogger  dei_rs_defogger_vars; 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_rs_defogger_init( void )
{
    dei_rs_defogger_vars.vrtc = vrtc_f_create( (Exec_Func)dei_rs_defogger_timeout ) ; 

    cmd_lut_insert( INPUT_RS_IGNITION_OFF , (Exec_Func)dei_rs_defogger_turn_off  , EXEC_MODE_NORMAL ) ;
}

//--------------------------------------------------------------------------//

void dei_rs_rf_defogger_toogle( void *remote )
{		
	if( !dei_rs_state )
	{
		notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_FEATURE_NA ) ; 
	
		return ;
	}
	
	if( dei_rs_defogger_out_is_programmed()  )
	{			
		set_rs_defogger( TRUE );
		  
		if( get_rs_defogger() )
	    {
	        vrtc_stop( dei_rs_defogger_vars.vrtc ) ; 
	
	        vrtc_update( dei_rs_defogger_vars.vrtc , DEI_RS_VRTC_TIMEOUT_DEFOGGER ) ;//10 minutes
	
	        vrtc_start( dei_rs_defogger_vars.vrtc );
	    }
		
		notify_send( NOTIFY_DEFOGGER_ON );
		
		dei_prk_flash( DEI_PRK_MTS_ON );	
		
		cmd_q_rs_insert( INPUT_DEFOGGER_ON , 100 );
	}
	else
	{
		notify_send( NOTIFY_ERROR , NOTIFY_PLD_ERROR_FEATURE_NA ) ; 
	}
		
}
	

//--------------------------------------------------------------------------//

void dei_rs_auto_defogger( void )
{  
	#ifdef PLATFORM_933		// (BUILD_NGMM == 1)
    if(dei_rs_defogger_out_is_programmed())
    {
		temperature_update();
		os_tsk_wait(360);
    }
	#endif
	
	if( get_temp_sensor_present() && dei_rs_defogger_out_is_programmed())
	{
		if( (get_temp_sensor_value()  < DEI_RS_TEMP_DEFOGGER_TRIGGER) && !get_rs_defogger() )
		{
			vrtc_stop( dei_rs_defogger_vars.vrtc ) ; 

			vrtc_update( dei_rs_defogger_vars.vrtc , DEI_RS_VRTC_TIMEOUT_DEFOGGER_ON ) ;//10 seconds to turn ON the defogger

			vrtc_start( dei_rs_defogger_vars.vrtc );

		}
	}
}

//--------------------------------------------------------------------------//

void dei_rs_defogger_timeout( void )
{    
	if( get_rs_defogger() )
    {
        vrtc_stop( dei_rs_defogger_vars.vrtc ); 
        
        set_rs_defogger( FALSE );
        
        cmd_q_rs_insert( get_rs_defogger() ? INPUT_DEFOGGER_ON : INPUT_DEFOGGER_OFF , 100 );
    }
		else
		{
			vrtc_stop( dei_rs_defogger_vars.vrtc ) ; 

			vrtc_update( dei_rs_defogger_vars.vrtc , DEI_RS_VRTC_TIMEOUT_DEFOGGER ) ;//10 minutes

			vrtc_start( dei_rs_defogger_vars.vrtc );

			set_rs_defogger( TRUE );
    		
			cmd_q_rs_insert( INPUT_DEFOGGER_ON , 100 );		
		}
}

//--------------------------------------------------------------------------//

Boolean dei_rs_defogger_out_is_programmed( void )
{
	UInt8 i ; 

	for( i = 0 ; i < DEI_PROG_OUT_COUNT ; i ++ )
	{
#ifdef PLATFORM_CM800
		if( dei_feature_get( DEI_FEAT_OUTPUT1 + i ) == DEI_OUT_PRG_REAR_DEFOGGER )
#else
		if( (dei_feature_get( (Dei_Feat)(DEI_FEAT_OUTPUT1 + i) ) == DEI_OUT_PRG_DEFROSTER_LATCH) || ( dei_feature_get( (Dei_Feat)(DEI_FEAT_OUTPUT1 + i) ) == DEI_OUT_PRG_DEFROSTER_PULSED ) )
#endif
		{
		 	return TRUE ; 
		}
	}

	return FALSE ; 
}

//--------------------------------------------------------------------------//

void dei_rs_defogger_turn_off( void )
{
		if(!get_rs_ign_disarm_oem())
		{
			vrtc_stop( dei_rs_defogger_vars.vrtc ); 
		
			set_rs_defogger( FALSE );
			
			cmd_q_rs_insert( INPUT_DEFOGGER_OFF, 100 );
		}

}
