/*==========================================================================*/
// $Id: dei_sc_notify_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_sc_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static const Notify_Array dei_sc_notify_table[] = 
{
	{ NOTIFY_LOCK                    ,  (Notify_Func)dei_sc_icon_lock				  		 } , 
	{ NOTIFY_UNLOCK                  ,  (Notify_Func)dei_sc_icon_unlock				  		 } , 
	{ NOTIFY_VALET_ON                ,	(Notify_Func)dei_sc_icon_valet_on			  		 } , 
	{ NOTIFY_VALET_OFF               ,	(Notify_Func)dei_sc_icon_valet_off			  		 } , 
	{ NOTIFY_TRUNK_RELEASE_OPEN      ,  (Notify_Func)dei_sc_icon_trunk_release_on	  		 } , 
	{ NOTIFY_TRUNK_RELEASE_CLOSE     ,  (Notify_Func)dei_sc_icon_trunk_release_off	  		 } , 
	{ NOTIFY_AUX_CH3_ON              ,	(Notify_Func)dei_sc_icon_aux_ch3_on			  		 } , 
	{ NOTIFY_AUX_CH3_OFF             ,	(Notify_Func)dei_sc_icon_aux_ch3_off		  		 } , 
	{ NOTIFY_AUX_CH4_ON              ,	(Notify_Func)dei_sc_icon_aux_ch4_on			  		 } , 
	{ NOTIFY_AUX_CH4_OFF             ,	(Notify_Func)dei_sc_icon_aux_ch4_off		  		 } , 
	{ NOTIFY_AUX_CH5_ON              ,  (Notify_Func)dei_sc_icon_aux_ch5_on			  		 } , 
	{ NOTIFY_AUX_CH5_OFF             ,  (Notify_Func)dei_sc_icon_aux_ch5_off		  		 } , 
	{ NOTIFY_AUX_CH6_ON              ,  (Notify_Func)dei_sc_icon_aux_ch6_on			  		 } , 
	{ NOTIFY_AUX_CH6_OFF             , 	(Notify_Func)dei_sc_icon_aux_ch6_off		  		 } , 
	{ NOTIFY_AUX_CH2_TYPE            ,  (Notify_Func)dei_sc_icon_aux2_type			  		 } , 
	{ NOTIFY_AUX_CH3_TYPE            ,  (Notify_Func)dei_sc_icon_aux3_type			  		 } , 
	{ NOTIFY_AUX_CH4_TYPE            ,  (Notify_Func)dei_sc_icon_aux4_type			  		 } , 
	{ NOTIFY_AUX_CH5_TYPE            ,  (Notify_Func)dei_sc_icon_aux5_type			  		 } , 
	{ NOTIFY_SENSOR1_TYPE            ,  (Notify_Func)dei_sc_icon_sensor1_type		  		 } , 
	{ NOTIFY_SENSOR2_TYPE            ,  (Notify_Func)dei_sc_icon_sensor2_type		  		 } , 
	{ NOTIFY_SENSOR3_TYPE            ,  (Notify_Func)dei_sc_icon_sensor3_type		 	 	 } , 
	{ NOTIFY_PANIC                   ,  (Notify_Func)dei_sc_icon_panic				  		 } , 
	{ NOTIFY_RANGE_CHECK             ,  (Notify_Func)dei_sc_icon_range_check		  		 } , 
	{ NOTIFY_CAR_FINDER              ,  (Notify_Func)dei_sc_icon_car_finder			  		 } , 
	{ NOTIFY_SENSOR1_ADJUST          ,  (Notify_Func)dei_sc_icon_sensor1_adjust		  		 } , 
	{ NOTIFY_SENSOR_BYPASS_WARN      ,  (Notify_Func)dei_sc_icon_sensor_bypass_warning		 } , 
	{ NOTIFY_SENSOR_BYPASS_SHOCK     ,  (Notify_Func)dei_sc_icon_sensor_bypass_shock  		 } , 
	{ NOTIFY_SENSOR_BYPASS_OPT       ,  (Notify_Func)dei_sc_icon_sensor_bypass_opt	  		 } , 
	{ NOTIFY_SENSOR_BYPASS_FULL      ,  (Notify_Func)dei_sc_icon_sensor_bypass_full	  		 } , 
	{ NOTIFY_SENSOR_BYPASS_OFF       ,  (Notify_Func)dei_sc_icon_sensor_bypass_off	  		 } , 
	{ NOTIFY_SENSOR_SILENT_ARM       ,  (Notify_Func)dei_sc_icon_sensor_silent_arm	  		 } , 
	{ NOTIFY_FULL_SILENT_ARM         ,  (Notify_Func)dei_sc_icon_full_silent_arm	  		 } , 
	{ NOTIFY_LAST_TRIGGER            ,  (Notify_Func)dei_sc_icon_trigger_zone_report  		 } , 
	{ NOTIFY_ERROR                   ,  (Notify_Func)dei_sc_icon_error				  		 } , 
	{ NOTIFY_TRIGGER                 ,  (Notify_Func)dei_sc_icon_trigger			  		 } , 
	{ NOTIFY_WARNING                 ,	(Notify_Func)dei_sc_icon_warning			  		 } , 
	{ NOTIFY_RS_START                ,  (Notify_Func)dei_sc_icon_start				  		 } , 
	{ NOTIFY_RS_STOP                 ,  (Notify_Func)dei_sc_icon_stop				  		 } , 
	{ NOTIFY_TIMER_MODE_ON           ,  (Notify_Func)dei_sc_icon_timer_mode_on		  		 } , 
	{ NOTIFY_TIMER_MODE_OFF          ,  (Notify_Func)dei_sc_icon_timer_mode_off		  		 } , 
	{ NOTIFY_SMART_MODE_ON           ,  (Notify_Func)dei_sc_icon_smart_mode_on		  		 } , 
	{ NOTIFY_SMART_MODE_OFF          ,  (Notify_Func)dei_sc_icon_smart_mode_off		  		 } , 
	{ NOTIFY_TIMER_ADJUST_ENTER      ,  (Notify_Func)dei_sc_icon_timer_mode_adjust_enter	 } , 
	{ NOTIFY_TIMER_SET_START_COUNT	 ,	(Notify_Func)dei_sc_icon_timer_mode_adjust_count	 } , 
	{ NOTIFY_TIMER_SET_TIME_START    ,  (Notify_Func)dei_sc_icon_timer_mode_adjust_time		 } , 
	{ NOTIFY_SHOW_TEMPERATURE        ,  (Notify_Func)dei_sc_icon_temp					     } , 
	{ NOTIFY_TEMP_ADJUST_ENTER		 ,  (Notify_Func)dei_sc_icon_smart_mode_temp_adjust_enter} , 
	{ NOTIFY_LOW_TEMP_UPDATE         ,  (Notify_Func)dei_sc_icon_smart_mode_temp_adjust_low	 } , 
	{ NOTIFY_HIGH_TEMP_UPDATE        ,  (Notify_Func)dei_sc_icon_smart_mode_temp_adjust_high } , 
	{ NOTIFY_RUNTIME_PROGRAMMED      ,  (Notify_Func)dei_sc_icon_runtime_programmed			 } , 
	{ NOTIFY_RUNTIME_REMAINING       ,  (Notify_Func)dei_sc_icon_runtime_remaining			 } , 
	{ NOTIFY_RUNTIME_RESET           ,  (Notify_Func)dei_sc_icon_runtime_reset				 } , 
	{ NOTIFY_RUNTIME_LOW             ,  (Notify_Func)dei_sc_icon_runtime_low_alert   		 } , 
	{ NOTIFY_RUNTIME_CHECK           ,  (Notify_Func)dei_sc_icon_runtime_check				 } , 
	{ NOTIFY_RUNTIME_UPDATE          ,  (Notify_Func)dei_sc_icon_runtime_update			 } , 
	{ NOTIFY_DEFOGGER_ON             ,  (Notify_Func)dei_sc_icon_defogger_on				 } , 
	{ NOTIFY_DEFOGGER_OFF            ,  (Notify_Func)dei_sc_icon_defogger_off				 } , 
	{ NOTIFY_KEYBOARD_UNLOCK         ,  (Notify_Func)dei_sc_icon_keyboard_unlock			 } , 
	{ NOTIFY_PAGE_UPDATE	         ,  (Notify_Func)dei_sc_icon_update_page				 } , 
	{ NOTIFY_MTS_ENABLE              ,  (Notify_Func)dei_sc_icon_mts_enable					 } , 
	{ NOTIFY_MTS_DISABLE             ,  (Notify_Func)dei_sc_icon_mts_disable    			 } , 	
	{ NOTIFY_PROGRAMMING_ENTER       ,	(Notify_Func)dei_sc_icon_programming_enter		  	 } , 
    { NOTIFY_REMOTE_PROGRAMMED_ACK   ,	(Notify_Func)dei_sc_icon_remote_programmed_ack	  	 } , 
	{ NOTIFY_PROGRAMMING_LEAVE       ,	(Notify_Func)dei_sc_icon_programming_leave		  	 } , 
	{ NOTIFY_REMOTE_DELETE		     ,	(Notify_Func)dei_sc_delete				  			 } ,
	{ NOTIFY_REMOTE_DELETE_ENTER     ,	(Notify_Func)dei_sc_delete_enter		  			 } ,
	{ NOTIFY_REMOTE_ZAP_ENTER     				,	(Notify_Func)dei_sc_zap_enter		  			 } ,
	{ NOTIFY_REMOTE_TACHO_DELETE     ,	(Notify_Func)dei_sc_tacho_delete_enter		  			 } ,
    { NOTIFY_ACTIVE_ZONE             ,  (Notify_Func)dei_sc_icon_active_zone                 } ,
		{ NOTIFY_LED_FLASH             ,  (Notify_Func)dei_sc_led_flash                 } ,
		{NOTIFY_SENSOR1_LEVEL_UPDATE		,	(Notify_Func)dei_sc_icon_sensor1_level_update		} ,
		{NOTIFY_SENSOR_PROG_MODE_EXIT	,	(Notify_Func)dei_sc_icon_sensor_prog_mode_exit} ,
		{NOTIFY_SENSOR1_LEVEL_INCREASE, (Notify_Func) dei_sc_icon_sensor_level_increase  },
		{NOTIFY_SENSOR1_LEVEL_DECREASE, (Notify_Func) dei_sc_icon_sensor_level_decrease  },
		{NOTIFY_RS_VALET_ON              ,	(Notify_Func) dei_sc_icon_rs_valet_on		}	,
		{NOTIFY_RS_VALET_OFF             ,	(Notify_Func) dei_sc_icon_rs_valet_off	}	,
	{NOTIFY_COMBO_VALET_ON              ,	(Notify_Func) dei_sc_icon_combo_valet_on		}	,
	{NOTIFY_COMBO_VALET_OFF             ,	(Notify_Func) dei_sc_icon_combo_valet_off	}	,
		{NOTIFY_CONVENIENCE_ALERT     , (Notify_Func) dei_sc_icon_convenience_alert  },
		  { NOTIFY_STANDBY                  ,   (Notify_Func)dei_sc_icon_standby }

};																					  

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_sc_notify_init( void )
{
	NOTIFY_ADD(dei_sc_notify_table);
}

//--------------------------------------------------------------------------//
Boolean             dei_sc_icon_remote_programmed_ack       ( void )
{
	dei_sc_tx(DEI_SC_ACK_LOCK , DEI_SC_PROGRAMMING_TX , 100);  // was NULL
	return TRUE;
}
