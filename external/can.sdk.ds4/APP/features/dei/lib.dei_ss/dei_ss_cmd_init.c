/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_ss_cmd_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
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

void dei_ss_cmd_init( void )
{	
	static const Cmd_Array1 cmd_array[] = 
    {
        { INPUT_OEM_LOCK                     , (Exec_Func)dei_ss_lock_oem                       },
        { INPUT_OEM_ARM                      , (Exec_Func)dei_ss_arm_oem                        },
    	{ INPUT_RF_LOCK                      , (Exec_Func)dei_ss_arm 	                        },
    	{ INPUT_RF_SILENT_LOCK               , (Exec_Func)dei_ss_arm_silent                     },	 
        { INPUT_OEM_DISARM                   , (Exec_Func)dei_ss_disarm_oem                     },        
    	{ INPUT_OEM_UNLOCK                   , (Exec_Func)dei_ss_disarm_oem                     },
    	{ INPUT_RF_UNLOCK                    , (Exec_Func)dei_ss_disarm                         }, 
    	{ INPUT_RF_SILENT_UNLOCK             , (Exec_Func)dei_ss_disarm_silent                  }, 
    	{ INPUT_RF_SENSOR_BYPASS_LEVEL       , (Exec_Func)dei_ss_sensor_bypass                  }, 
    	{ INPUT_RF_SENSOR_SILENT_ARM         , (Exec_Func)dei_ss_sensor_silent_arm              }, 
    	{ INPUT_RF_FULL_SILENT_ARM           , (Exec_Func)dei_ss_full_silent_arm                }, 
    	{ INPUT_RF_SENSOR1_ADJUST            , (Exec_Func)dei_ss_sensor_adjust_enter            }, 
    	{ INPUT_RF_PUT_NEW_SENSOR1_VALUE     , (Exec_Func)dei_ss_sensor_adjust_put              },	
    	{ INPUT_RF_EXIT_PROGRAMMING_MODE     , (Exec_Func)dei_ss_sensor_adjust_exit              },	
    	{ INPUT_RF_TRIGGER_HISTORY_REQUEST   , (Exec_Func)dei_ss_rf_trg_history                 },		
    	{ INPUT_RF_VALET_TOOGLE              , (Exec_Func)dei_ss_rf_valet                       },
    	{ INPUT_VALET_TOGGLE                 , (Exec_Func)dei_ss_rf_valet                       },
    	{ INPUT_ALL_DOOR_STATUS_OPEN		 , (Exec_Func)dei_ss_door_open                      },
    	{ INPUT_ALL_DOOR_STATUS_CLOSE        , (Exec_Func)dei_ss_door_close                     },
    	{ INPUT_HOOD_STATUS_OPEN  		     , (Exec_Func)dei_ss_hood_open                      },
    	{ INPUT_HOOD_STATUS_CLOSE            , (Exec_Func)dei_ss_hood_close                     },	
    	{ INPUT_TRUNK_STATUS_OPEN 		     , (Exec_Func)dei_ss_trunk_open                     },
    	{ INPUT_TRUNK_STATUS_CLOSE           , (Exec_Func)dei_ss_trunk_close                    },
    	{ INPUT_IGN_STATUS_ON 			     , (Exec_Func)dei_ss_ignition_on                    },	
    	{ INPUT_IGN_STATUS_OFF 			     , (Exec_Func)dei_ss_ignition_off                    },	
    	{ INPUT_SENSOR_TRIGGER  			 , (Exec_Func)dei_ss_sensor_trigger                 },
    	{ INPUT_SENSOR_WARNING               , (Exec_Func)dei_ss_sensor_warning                 },
        { INPUT_GWR_OFF                      , (Exec_Func)dei_ss_sensor_bp_rs_done              },
        { INPUT_GWR_OFF                      , (Exec_Func)dei_ss_ignition_checking_after_gwr_off},
        { INPUT_DOOR_SKIP_SET                , (Exec_Func)dei_ss_door_skip_set					},
        { INPUT_DOOR_SKIP_CLR                , (Exec_Func)dei_ss_door_skip_clr					},
        //{ INPUT_BRAKE_STATUS_PRESS 			 , (Exec_Func)dei_ss_brake_press 			        },
		
		#ifdef PLATFORM_933
		{INPUT_BUTTON_TAP1					 , (Exec_Func)dei_ss_sensor_tap1_count			},
		{INPUT_BUTTON_TAP2					 , (Exec_Func)dei_ss_sensor_tap2_count			},
		{INPUT_BUTTON_TAP3					 , (Exec_Func)dei_ss_sensor_tap3_count			},
		{INPUT_BUTTON_TAP4					 , (Exec_Func)dei_ss_sensor_tap4_count			},
		{INPUT_BUTTON_TAP5					 , (Exec_Func)dei_ss_sensor_tap5_count			},
		{INPUT_IGN_STATUS_ON				 , (Exec_Func)dei_ss_sensor_mode_exit_ign_on	},
		{INPUT_ARM										, (Exec_Func)dei_ss_sensor_mode_exit_ign_on	},
		{INPUT_RF_VALET_TOOGLE				, (Exec_Func)dei_ss_sensor_mode_exit_ign_on	},
		{INPUT_VALET_TOGGLE					 	, (Exec_Func)dei_ss_sensor_mode_exit_ign_on	},
		{INPUT_GWR_ON									, (Exec_Func)dei_ss_sensor_mode_exit_ign_on	},
		{INPUT_INST_TRIG_STATUS_ON				, (Exec_Func)dei_ss_instant_alarm_trig	},
		
		#endif
    };

    CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
	
	dei_ss_2nd_arm = FALSE;
	
	dei_ss_trg_history_timeout = vrtc_f_create( (Exec_Func) dei_ss_ign_on_for_trig_history );
	dei_ss_door_skip_vrtc_timer = vrtc_f_create( (Exec_Func)dei_ss_door_skip_timeout_handler);
	dei_ss_door_is_skip = FALSE;
}

//--------------------------------------------------------------------------//
void dei_ss_silent_arm_updating(Boolean armed)
{
	if(armed)
	  dei_ss_arm_actions(DEI_SS_SILENT, NULL);
	else
		dei_ss_disarm_actions(DEI_SS_SILENT, NULL);
}

