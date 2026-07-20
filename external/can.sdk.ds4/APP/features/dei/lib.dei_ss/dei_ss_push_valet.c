/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_push_valet.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Ss_Push_Valet
{
    Vrtc 	vrtc_timeout_operation;
    Vrtc 	vrtc_ign_check_timeout;
    Boolean is_enable     ;
    Boolean push_is_press ;
    Boolean ext_push_pressed;
	  Boolean push_btn_valet_mode;

}Dei_Ss_Push_Valet;
    
#define NGMM_SS_VRTC_TAP_CHECK    (1*VRTC_1SEC)
#define NGMM_SS_VRTC_PV_EXT_RELEASE (8*VRTC_1SEC)
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_ss_push_valet_ignition_on              ( void );
static void dei_ss_push_valet_ignition_off             ( void );
static void dei_ss_push_valet_push_status_off          ( void );
static void dei_ss_push_valet_push_status_on           ( void );
static void dei_ss_push_valet_timeout_operation_expired( void );
static void dei_ss_push_valet_timeout_ign_check_expired( void );
static void dei_ss_push_valet_begin_gwr_off_ign_check  ( void );
static void ngmm_ss_push_valet(void);
static void ngmm_ss_push_valet_ign_off( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Dei_Ss_Push_Valet dei_ss_pv;
Vrtc   vrtc_check_tap_release;
Vrtc  vrtc_ext_press_release;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_ss_push_valet_init( void )
{
	cmd_register:
    {
        static const Cmd_Array1 cmd_array[] = 
        {
#ifdef DEV_LPC_236x
            { INPUT_IGN_STATUS_OFF 	  , (Exec_Func)dei_ss_push_valet_ignition_off     },
            { INPUT_IGN_STATUS_ON	  , (Exec_Func)dei_ss_push_valet_ignition_on      },
            { INPUT_PUSH_VALET_RELEASE, (Exec_Func)dei_ss_push_valet_push_status_off  },
						{ INPUT_PUSH_VALET_PRESS  , (Exec_Func)dei_ss_push_valet_push_status_on   },
            { INPUT_GWR_OFF           , (Exec_Func)dei_ss_push_valet_begin_gwr_off_ign_check}
#else
            { INPUT_ALARM_VALET           , (Exec_Func)ngmm_ss_push_valet},
            { INPUT_IGN_STATUS_OFF 	  , (Exec_Func)ngmm_ss_push_valet_ign_off     }
#endif
						
        };

        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
    }
        	
#ifdef DEV_LPC_236x

	dei_ss_pv.vrtc_timeout_operation = vrtc_create  ( (Vrtc_Func)dei_ss_push_valet_timeout_operation_expired );
    dei_ss_pv.vrtc_ign_check_timeout = vrtc_f_create( (Exec_Func)dei_ss_push_valet_timeout_ign_check_expired );

	dei_ss_pv.is_enable = FALSE ; 
	dei_ss_pv.ext_push_pressed = FALSE;
	dei_ss_pv.push_btn_valet_mode = FALSE;
#endif
}

//--------------------------------------------------------------------------//

void dei_ss_push_valet_ignition_on( void )
{  
	if( ! get_ss_arm() && !get_rs_gwr() )
	{
		vrtc_stop ( dei_ss_pv.vrtc_timeout_operation ) ;
		
    dei_ss_pv.is_enable = FALSE ;
		
		dei_led_flash( DEI_LED_RESET , TRUE );
		dei_ss_pv.push_btn_valet_mode = TRUE;

	}
}

//--------------------------------------------------------------------------//

void dei_ss_push_valet_ignition_off( void )
{
	if(dei_ss_pv.ext_push_pressed)
  {
      return;   //not finish yet on checking RS err history
  }
		
	if( !get_ss_arm() && !get_rs_gwr() && !dei_ss_pv.push_is_press && dei_ss_pv.push_btn_valet_mode )
	{
		vrtc_stop( dei_ss_pv.vrtc_timeout_operation ); 

		vrtc_update( dei_ss_pv.vrtc_timeout_operation , DEI_SS_VRTC_TIMEOUT_PUSH_VALET_TOGGLE ) ; 

		dei_ss_pv.is_enable = TRUE ; 
		dei_ss_pv.push_btn_valet_mode = FALSE;

		vrtc_start( dei_ss_pv.vrtc_timeout_operation );
			  
		dei_led_flash  (  get_ss_valet() ? DEI_LED_VALET_ON : DEI_LED_DISARM , LED_ENABLE ) ; 

	}
}

//--------------------------------------------------------------------------//

void dei_ss_push_valet_push_status_off( void )
{	

	if( dei_ss_pv.is_enable )
	{	  
		if( !get_ss_arm() )
		{
			dei_ss_rf_valet( NULL );
		}
	}
	
	vrtc_stop( dei_ss_pv.vrtc_timeout_operation );
	
	dei_ss_pv.is_enable = FALSE ;  

	dei_ss_pv.push_is_press = FALSE ; 
}

//--------------------------------------------------------------------------//

void dei_ss_push_valet_push_status_on( void )
{
	dei_ss_pv.push_is_press = TRUE ; 
}

//--------------------------------------------------------------------------//

void dei_ss_push_valet_timeout_operation_expired( void )
{
	dei_ss_pv.is_enable = FALSE ;
}

//--------------------------------------------------------------------------//

void dei_ss_push_valet_begin_gwr_off_ign_check( void )
{
    vrtc_stop( dei_ss_pv.vrtc_ign_check_timeout ); 

    vrtc_update( dei_ss_pv.vrtc_ign_check_timeout , DEI_SS_VRTC_TIMEOUT_VALET_IGN_CHECK ) ; 

    vrtc_start( dei_ss_pv.vrtc_ign_check_timeout );
}
    
//--------------------------------------------------------------------------//

void dei_ss_push_valet_timeout_ign_check_expired( void )
{
    if( !get_ss_arm() && get_ign_status() && !get_rs_gwr() )
    {
        dei_led_flash( DEI_LED_RESET , TRUE );
    }    
}

//--------------------------------------------------------------------------//

void ngmm_ss_push_valet( void )
{	
	if( !get_ss_arm() )
	{
		dei_ss_rf_valet( NULL );
		cmd_q_rs_insert(INPUT_PRG_MODE_EXIT, 100);
	}
}

//--------------------------------------------------------------------------//

void ngmm_ss_push_valet_ign_off( void )
{
	if(get_ss_valet())
		dei_led_flash( DEI_LED_VALET_ON , LED_ENABLE);
}
