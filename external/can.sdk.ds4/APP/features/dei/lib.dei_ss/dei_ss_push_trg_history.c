/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_push_trg_history.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_ss_private.h"
#include "dei_sc.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Ss_Trigger_History
{
    Vrtc 	 vrtc_timeout ;
    Vrtc 	 led_timeout  ;
    Boolean  is_enable    ;
    Irq      irq ;     

}Dei_Ss_Trigger_History;

#define  NGMM_VRTC_TRG_HIST_CHECK   (VRTC_1SEC)
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_ss_trigger_history_ignition_changed  	 ( void );
static void dei_ss_trigger_history_push_off     	  	 ( void );
static void dei_ss_trigger_history_push_on      	  	 ( void );
static void dei_ss_trigger_history_timeout_enable_expired( void );
static void dei_ss_trigger_history_display               ( void );
static void dei_ss_trigger_history_timeout_led_expired   ( void );
static void ngmm_ss_trigger_history(void);
static void ngmm_ss_trigger_history_cleared( void );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Dei_Ss_Trigger_History dei_ss_th;
Vrtc   vrtc_ngmm_trg_hist_check;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_ss_push_trg_history_init( void )
{
	dei_ss_th.vrtc_timeout= vrtc_create  ( (Vrtc_Func)dei_ss_trigger_history_timeout_enable_expired );
    dei_ss_th.led_timeout = vrtc_f_create( (Exec_Func)dei_ss_trigger_history_timeout_led_expired    );

	cmd_register:
    {
        static const Cmd_Array1 cmd_array[] = 
        {    
#ifdef DEV_LPC_236x
            { INPUT_IGN_STATUS_OFF        , (Exec_Func)dei_ss_trigger_history_ignition_changed },
            { INPUT_IGN_STATUS_ON 	      , (Exec_Func)dei_ss_trigger_history_ignition_changed },
        	{ INPUT_PUSH_VALET_RELEASE    , (Exec_Func)dei_ss_trigger_history_push_off      },
        	{ INPUT_EXT_PUSH_VALET_RELEASE, (Exec_Func)dei_ss_trigger_history_push_off     	},
        	{ INPUT_PUSH_VALET_PRESS      , (Exec_Func)dei_ss_trigger_history_push_on      	},
#else
          { INPUT_IGN_STATUS_OFF        , (Exec_Func)ngmm_ss_trigger_history_cleared },
					{ INPUT_ALARM_TRIG_HISTORY           , (Exec_Func)ngmm_ss_trigger_history      	}
#endif
        };

        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
    }
	
	dei_ss_th.is_enable = FALSE ; 
	dei_ss_alarm_hist_report_on = FALSE;

#ifdef DEV_LPC_236x
	dei_ss_th.irq = os_irq_create();
#endif
}

//--------------------------------------------------------------------------//

void dei_ss_trigger_history_ignition_changed( void )
{	
    dei_ss_th.is_enable = FALSE ;
    
    vrtc_stop( dei_ss_th.vrtc_timeout );

    vrtc_stop( dei_ss_th.led_timeout ) ;	
}

//--------------------------------------------------------------------------//


void dei_ss_trigger_history_push_on( void )
{  
	if( !get_ign_status() && !get_rs_gwr() )
    {        
        wake_enter_flagged(WAKE_PRG_MENU);
	    {
			if( os_irq_wait( dei_ss_th.irq , TRUE , 10000 ))
	        {            
	            os_tsk_wait( 10 ) ; 
							  
				if( get_ign_status() )
				{
					vrtc_stop( dei_ss_th.vrtc_timeout );
	
					dei_ss_th.is_enable = TRUE  ; 
					
					vrtc_update( dei_ss_th.vrtc_timeout , DEI_SS_VRTC_TIMEOUT_TRIGGER_HISTORY_PUSH_RELEASE  ); //5 seconds dei_ss_trigger_history_timeout_enable_expired
	
					vrtc_start( dei_ss_th.vrtc_timeout );            	
				}
	       }
	   }	   	   
	   wake_leave_flagged(WAKE_PRG_MENU);                
    }	
}

//--------------------------------------------------------------------------//

void dei_ss_trigger_history_push_off( void )
{	
	vrtc_stop( dei_ss_th.vrtc_timeout ) ;
	
	if( dei_ss_th.is_enable )
    {
        dei_ss_th.is_enable = FALSE ; 
	  
		if( !get_ss_arm() && get_ign_status() && !get_rs_gwr())
		{
			dei_ss_trigger_history_display();
		}
		        
    }

	os_irq_set( dei_ss_th.irq );	
}

//--------------------------------------------------------------------------//

void dei_ss_trigger_history_timeout_enable_expired( void )
{
	dei_ss_th.is_enable = FALSE ; 	
}

//--------------------------------------------------------------------------//

void dei_ss_trigger_history_display( void )
{
	Dei_Led_Pattern trigger = DEI_LED_NONE ;
	
#ifdef PLATFORM_CM800
	static const Dei_Led_Pattern pattern[] = 
	{		
		DEI_LED_TRUNK_TRIGGERED     ,
		DEI_LED_SENSOR_TRIGGERED    ,
		DEI_LED_DOOR_TRIGGERED      ,
    	DEI_LED_IGN_TRIGGERED       ,
		DEI_LED_HOOD_TRIGGERED
	};											 
	
#else
	static const Dei_Led_Pattern pattern[] = 
	{		
		DEI_LED_TRUNK_TRIGGERED     ,
		DEI_LED_SHOCK_SENSOR_TRIGGERED    ,
		DEI_LED_DOOR_TRIGGERED      ,
    	DEI_LED_IGN_TRIGGERED       ,
		DEI_LED_HOOD_TRIGGERED    ,
		DEI_LED_TILT_SENSOR_TRIGGERED		,
		DEI_LED_UNKNOWN_SENSOR_TRIGGERED
	};											 
#endif
	if( get_ss_memory_trigger_event(0) )
	{
		trigger |= pattern[	get_ss_memory_trigger_event(0) - 1] ; 
	}
		
	if( get_ss_memory_trigger_event(1) )
	{	  
		trigger |= pattern[	get_ss_memory_trigger_event(1) - 1];  
	}
	
	if( get_ss_memory_trigger_event(2) )
	{
		trigger |= pattern[	get_ss_memory_trigger_event(2) - 1] ; 
	}

	if( get_ss_memory_trigger_event(3) )
	{
		trigger |= pattern[	get_ss_memory_trigger_event(3) - 1] ; 
	}

	if( trigger != DEI_SS_NO_TRIGGER )
	{	
		dei_sc_set_mode(DEI_SC_MODE_ALARM_REPORT );
		
		dei_led_flash( trigger , TRUE );

		vrtc_stop( dei_ss_th.led_timeout ) ;

		vrtc_update( dei_ss_th.led_timeout , DEI_SS_VRTC_TIMEOUT_TRIGGER_HISTORY_DISPLAY_RESET ); 

		vrtc_start( dei_ss_th.led_timeout ); 
	}
}

//--------------------------------------------------------------------------//

void dei_ss_trigger_history_timeout_led_expired( void )
{
    dei_led_flash( DEI_LED_RESET , TRUE ) ; 
	  if(dei_sc_get_mode() != DEI_SC_MODE_NORMAL)
		{
			dei_sc_set_mode(DEI_SC_MODE_NORMAL);
		}
}

//--------------------------------------------------------------------------//

void ngmm_ss_trigger_history( void )
{	
	if( !get_ss_arm() && get_ign_status() && !get_rs_gwr())
	{
		dei_ss_alarm_hist_report_on = TRUE;

		dei_ss_trigger_history_display();
		cmd_q_rs_insert(INPUT_PRG_MODE_EXIT, 100);
	}
}

//--------------------------------------------------------------------------//

void ngmm_ss_trigger_history_cleared( void )
{
	if( dei_ss_alarm_hist_report_on )
	{
		dei_ss_alarm_hist_report_on = FALSE;
		dei_led_flash( DEI_LED_RESET , TRUE) ; 
	}

}

//--------------------------------------------------------------------------//
