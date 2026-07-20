/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_shutdown_history.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
#include "cmd.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Rs_Shutdown_History
{
    Boolean on_enable ;
    Boolean off_enable;
    Boolean push_status;
    Boolean is_active;
    Vrtc    vrtc_timeout ;

}Dei_Rs_Shutdown_History;
    

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_rs_shutdown_history_ign_on       ( void);
static void dei_rs_shutdown_history_ign_off      ( void );
static void dei_rs_shutdown_history_push_on      ( void );
static void dei_rs_shutdown_history_push_off     ( void );
static void dei_rs_shutdown_history_ext_push_off ( void );
static void dei_rs_shutdown_history_timeout      ( void );
static void dei_rs_shutdown_history_display_reset( void );
static void dei_rs_shutdown_history_display      ( void );
static void ngmm_rs_shutdown_history_ign_on				( void );
static void ngmm_rs_shutdown_history_ign_off				( void );
static void ngmm_ext_push_pressed_before_ign			( void );
static void ngmm_rs_shutdown_history_display_for_single_tap			( void );
static void ngmm_rs_shutdown_history_display_for_double_tap 			( void );
static void ngmm_rs_shutdown_history_push_on ( void );



/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Dei_Rs_Shutdown_History     dei_rs_sh_vars;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_rs_shutdown_history_init( void )
{
	dei_rs_sh_vars.vrtc_timeout = vrtc_f_create( (Exec_Func)dei_rs_shutdown_history_timeout );

	dei_rs_sh_vars.on_enable   = FALSE ; 
	dei_rs_sh_vars.off_enable  = FALSE ;
	dei_rs_sh_vars.push_status = FALSE ; 
	dei_rs_sh_vars.is_active   = FALSE ;
	
	cmd_register:
    {
        static const Cmd_Array1 cmd_array[] =
        {
					#ifdef DEV_LPC_236x
            { INPUT_IGN_STATUS_ON 		   , (Exec_Func)dei_rs_shutdown_history_ign_on        },
        	{ INPUT_IGN_STATUS_OFF         , (Exec_Func)dei_rs_shutdown_history_ign_off       },
        	{ INPUT_PUSH_VALET_PRESS	   , (Exec_Func)dei_rs_shutdown_history_push_on       },
            //{ INPUT_BUTTON_TAP1            , (Exec_Func)dei_rs_shutdown_history_push_on       },
            { INPUT_PUSH_VALET_RELEASE     , (Exec_Func)dei_rs_shutdown_history_push_off      },
            //{ INPUT_PUSH_VALET_RELEASE_    , (Exec_Func)dei_rs_shutdown_history_ext_push_off  },
        	{ INPUT_EXT_PUSH_VALET_RELEASE , (Exec_Func)dei_rs_shutdown_history_push_off      },
					#endif
        	{ INPUT_OEM_LOCK               , (Exec_Func)dei_rs_shutdown_history_display_reset },
            { INPUT_OEM_ARM                , (Exec_Func)dei_rs_shutdown_history_display_reset },
        	{ INPUT_RF_LOCK                , (Exec_Func)dei_rs_shutdown_history_display_reset },
        	{ INPUT_RF_SILENT_LOCK         , (Exec_Func)dei_rs_shutdown_history_display_reset },
        	{ INPUT_OEM_UNLOCK             , (Exec_Func)dei_rs_shutdown_history_display_reset },
            { INPUT_OEM_DISARM             , (Exec_Func)dei_rs_shutdown_history_display_reset },
        	{ INPUT_RF_UNLOCK              , (Exec_Func)dei_rs_shutdown_history_display_reset },
        	{ INPUT_RF_SILENT_UNLOCK       , (Exec_Func)dei_rs_shutdown_history_display_reset },
					
					#ifdef DEV_LPC_176x
					// NGMM
            { INPUT_IGN_STATUS_ON 		   , (Exec_Func)ngmm_rs_shutdown_history_ign_on        },
					{ INPUT_RS_SHUTDOWN_HISTORY         , (Exec_Func)ngmm_rs_shutdown_history_push_on },
					
					#endif
        };

        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
    }
}

//--------------------------------------------------------------------------//

void dei_rs_shutdown_history_ign_on( void )
{
	
	dei_rs_shutdown_history_display_reset(); //order is important
	
  //if( dei_rs_sh_vars.push_status && !ss.locked     && !dei_rs_state )	
	if( dei_rs_sh_vars.push_status && !get_ss_lock() && !dei_rs_state )	
	{
		vrtc_stop( dei_rs_sh_vars.vrtc_timeout );

		dei_rs_sh_vars.on_enable = TRUE ; 
		
		vrtc_update( dei_rs_sh_vars.vrtc_timeout , DEI_RS_VRTC_TIMEOUT_SHUTDOWN_HISTORY_PUSH_RELEASE ) ; 

		vrtc_start( dei_rs_sh_vars.vrtc_timeout );
	}	
}

//--------------------------------------------------------------------------//

void dei_rs_shutdown_history_ign_off( void )
{
	if( dei_rs_sh_vars.push_status && dei_rs_sh_vars.on_enable )
	{
		vrtc_stop( dei_rs_sh_vars.vrtc_timeout );

		dei_rs_sh_vars.off_enable = TRUE ; 
		
		vrtc_update( dei_rs_sh_vars.vrtc_timeout , DEI_RS_VRTC_TIMEOUT_SHUTDOWN_HISTORY_PUSH_PRESS ) ; 

		vrtc_start( dei_rs_sh_vars.vrtc_timeout );
	}
}

//--------------------------------------------------------------------------//

void dei_rs_shutdown_history_push_on ( void )
{
	dei_rs_sh_vars.push_status = TRUE ; 	

	if( dei_rs_sh_vars.on_enable && dei_rs_sh_vars.off_enable && !dei_rs_sh_vars.is_active )
	{
		dei_rs_shutdown_history_display();

		vrtc_update( dei_rs_sh_vars.vrtc_timeout , DEI_RS_VRTC_TIMEOUT_SHUTDOWN_HISTORY_DISPLAY_RESET ) ; //dei_rs_shutdown_history_timeout for dei_rs_shutdown_history_display reset
		
		vrtc_start( dei_rs_sh_vars.vrtc_timeout );
	}
}

//--------------------------------------------------------------------------//

void dei_rs_shutdown_history_push_off( void )
{
	dei_rs_sh_vars.push_status = FALSE  ;	
}

//--------------------------------------------------------------------------//
void dei_rs_shutdown_history_ext_push_off ( void )
{
		vrtc_stop( dei_rs_sh_vars.vrtc_timeout );

		vrtc_update( dei_rs_sh_vars.vrtc_timeout , DEI_RS_VRTC_TIMEOUT_SHUTDOWN_HISTORY_PUSH_PRESS ) ; 

		vrtc_start( dei_rs_sh_vars.vrtc_timeout );
}

//--------------------------------------------------------------------------//

void dei_rs_shutdown_history_display_reset( void )
{
	dei_rs_sh_vars.on_enable  = FALSE ; 
	dei_rs_sh_vars.off_enable = FALSE ;	
	
	if( dei_rs_sh_vars.is_active )
	{
		dei_led_flash( DEI_LED_RESET , TRUE);

		dei_rs_sh_vars.is_active = FALSE ; 
	}
}

//--------------------------------------------------------------------------//

void dei_rs_shutdown_history_timeout( void )
{
	dei_rs_sh_vars.on_enable  = FALSE ; 
	dei_rs_sh_vars.off_enable = FALSE ;	
	dei_rs_sh_vars.push_status = FALSE ; 	
 

	if( dei_rs_sh_vars.is_active )
	{
		dei_rs_sh_vars.is_active = FALSE ; 
		
	   //dei_led_flash( ss.valet      ? DEI_LED_VALET_ON : DEI_LED_DISARM );
		dei_led_flash( get_ss_valet() ? DEI_LED_VALET_ON : DEI_LED_DISARM, LED_ENABLE );
	}		
}

//--------------------------------------------------------------------------//

void dei_rs_shutdown_history_display( void )
{
	static const Dei_Led_Pattern pattern[] = 
	{		
		DEI_LED_RS_ERROR_RUNTIME    ,
		DEI_LED_RS_ERROR_OVERREV    ,
		DEI_LED_RS_ERROR_LOW_RPM    ,
		DEI_LED_RS_ERROR_TRANSMITTER,
		DEI_LED_RS_ERROR_BRAKE      ,
		DEI_LED_RS_ERROR_HOOD       ,
		DEI_LED_RS_ERROR_MTS        ,
		DEI_LED_RS_ERROR_NEUTRAL    ,
		DEI_LED_RS_ERROR_LOW_BATT   ,
		DEI_LED_RS_ERROR_TRIGGER    ,
		DEI_LED_RS_ERROR_GLOWPLUG   
	};											 

	if( !get_rs_error_shutdown() )return ; //nothing to dei_rs_shutdown_history_display

	dei_rs_sh_vars.is_active = TRUE  ;


	dei_led_flash( pattern[	get_rs_error_shutdown() - 1] , TRUE);
}


//--------------------------------------------------------------------------//

void ngmm_rs_shutdown_history_ign_on( void )
{
	
	dei_rs_shutdown_history_display_reset(); //order is important
	
  //if( dei_rs_sh_vars.push_status && !ss.locked     && !dei_rs_state )	
	if( !get_ss_lock() && !dei_rs_state )	
	{
		vrtc_stop( dei_rs_sh_vars.vrtc_timeout );

		dei_rs_sh_vars.on_enable = TRUE ; 
		
		vrtc_update( dei_rs_sh_vars.vrtc_timeout , DEI_RS_VRTC_TIMEOUT_SHUTDOWN_HISTORY_PUSH_RELEASE ) ; 

		vrtc_start( dei_rs_sh_vars.vrtc_timeout );
	}	
}

//--------------------------------------------------------------------------//

void ngmm_rs_shutdown_history_push_on ( void )
{
	dei_rs_sh_vars.push_status = TRUE ; 	

	if( !dei_rs_sh_vars.is_active )
	{
		dei_rs_shutdown_history_display();
		cmd_q_rs_insert( INPUT_PRG_MODE_EXIT, 100 );

		vrtc_update( dei_rs_sh_vars.vrtc_timeout , DEI_RS_VRTC_TIMEOUT_SHUTDOWN_HISTORY_DISPLAY_RESET ) ; //dei_rs_shutdown_history_timeout for dei_rs_shutdown_history_display reset
		
		vrtc_start( dei_rs_sh_vars.vrtc_timeout );
	}
}


//--------------------------------------------------------------------------//
