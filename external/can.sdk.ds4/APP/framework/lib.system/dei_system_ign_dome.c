/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_ign_dome.c 19287 2013-02-18 12:08:05Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_System_Ign_Dome
{
    Vrtc    vrtc;
    Boolean dome_status;

}Dei_System_Ign_Dome;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_system_ign_dome_control        ( void );
static void dei_system_ign_dome_unlock_control ( void );
static void dei_system_ign_dome_reset          ( void );
static void dei_system_ign_dome_timeout        ( void );
static void dei_system_ign_dome_status_on	   ( void );
static void dei_system_ign_dome_status_off	   ( void );
static void dei_system_rs_ign_dome_control     ( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Dei_System_Ign_Dome dei_system_ign_dome_vars;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_system_ign_dome_init( void )
{
	dei_system_ign_dome_vars.vrtc = vrtc_f_create( (Exec_Func)dei_system_ign_dome_timeout );
				
	cmd_register:
    {
        static const Cmd_Array1 cmd_array[] = {
          { INPUT_IGN_STATUS_ON 				, (Exec_Func)dei_system_ign_dome_control },
					{ INPUT_RS_IGNITION_ON 				, (Exec_Func)dei_system_rs_ign_dome_control },
					{ INPUT_RS_IGNITION_OFF				, (Exec_Func)dei_system_rs_ign_dome_control },
        	{ INPUT_OEM_LOCK      				, (Exec_Func)dei_system_ign_dome_reset },
          { INPUT_OEM_ARM       				, (Exec_Func)dei_system_ign_dome_reset },
        	{ INPUT_RF_LOCK       				, (Exec_Func)dei_system_ign_dome_reset },
        	{ INPUT_RF_UNLOCK     				, (Exec_Func)dei_system_ign_dome_unlock_control },
        	{ INPUT_RF_SILENT_LOCK    		, (Exec_Func)dei_system_ign_dome_reset },
        	{ INPUT_RF_SILENT_UNLOCK  		, (Exec_Func)dei_system_ign_dome_unlock_control },
        	{ INPUT_RF_SENSOR_SILENT_ARM	, (Exec_Func)dei_system_ign_dome_reset },
        	{ INPUT_RF_FULL_SILENT_ARM 		, (Exec_Func)dei_system_ign_dome_reset }
        };
        
        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
    }

	dei_system_ign_dome_vars.dome_status = FALSE ; 
			
	if( dei_feature_get(DEI_FEAT_IGN_CONTROLLED_DOMELIGHT) == 2 ) //feature is enable
	{
		cmd_lut_insert( INPUT_IGN_STATUS_OFF, (Exec_Func)dei_system_ign_dome_control , EXEC_MODE_NORMAL );		
	}
}

//--------------------------------------------------------------------------//

void dei_system_ign_dome_unlock_control( void )
{
	if( (!get_rs_gwr() && get_ign_status())  || get_ss_panic()  ) return ;	  //not available when car is not remote started and ignition is on
  
	vrtc_stop  ( dei_system_ign_dome_vars.vrtc );
		
	vrtc_update( dei_system_ign_dome_vars.vrtc , DEI_SYSTEM_VRTC_TIMEOUT_DOME );
		
	vrtc_start ( dei_system_ign_dome_vars.vrtc );

    os_tsk_wait(200);

	dei_system_ign_dome_status_on();
}

//--------------------------------------------------------------------------//

void dei_system_ign_dome_control( void )
{		
	if( get_rs_gwr() || get_ss_lock() ) return ;//not available in remote start and when the car is locked
  
  	if( (!get_ign_status()) && (!get_rs_ign_off_status()) )
	{
		vrtc_stop  ( dei_system_ign_dome_vars.vrtc );
		
		vrtc_update( dei_system_ign_dome_vars.vrtc , DEI_SYSTEM_VRTC_TIMEOUT_DOME );
		
		vrtc_start ( dei_system_ign_dome_vars.vrtc );

		dei_system_ign_dome_status_on();
	}
	else
	{
		dei_system_ign_dome_reset();
	}
}

//--------------------------------------------------------------------------//

void dei_system_rs_ign_dome_control( void )
{		
  if( (get_ign_status() && get_rs_gwr()) && (!get_rs_ign_off_status()) )
	{
		vrtc_stop  ( dei_system_ign_dome_vars.vrtc );
		
		vrtc_update( dei_system_ign_dome_vars.vrtc , DEI_SYSTEM_VRTC_TIMEOUT_DOME );
		
		vrtc_start ( dei_system_ign_dome_vars.vrtc );

		dei_system_ign_dome_status_on();
	}
	else
	{
		dei_system_ign_dome_reset();
	}
}


//--------------------------------------------------------------------------//

void dei_system_ign_dome_reset( void )
{
	vrtc_stop( dei_system_ign_dome_vars.vrtc );

	dei_system_ign_dome_status_off();
}

//--------------------------------------------------------------------------//

void dei_system_ign_dome_timeout( void )
{
	dei_system_ign_dome_status_off();
}

//--------------------------------------------------------------------------//

void dei_system_ign_dome_status_on( void )
{
	if( !dei_system_ign_dome_vars.dome_status )
	{
		ATOMIC( dei_system_ign_dome_vars.dome_status = TRUE );
		
		cmd_q_rs_insert( INPUT_DOME_ON  , 100 , 0 ); 
	}
}

//--------------------------------------------------------------------------//

void dei_system_ign_dome_status_off( void )
{
	if( dei_system_ign_dome_vars.dome_status )
	{
		ATOMIC( dei_system_ign_dome_vars.dome_status = FALSE );
		
		cmd_q_rs_insert( INPUT_DOME_OFF  , 100 , 0 ); 
	}
}

//--------------------------------------------------------------------------//

