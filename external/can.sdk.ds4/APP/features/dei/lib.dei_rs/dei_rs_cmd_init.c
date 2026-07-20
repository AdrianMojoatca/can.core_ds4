/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rs_cmd_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rs_private.h"
#include "fob_ctrl.h"
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
static void dei_rs_keypad_unlock(void);
//--------------------------------------------------------------------------//

void dei_rs_cmd_init( void )
{
	static const Cmd_Array1 cmd_array[] = 
    {    
        { INPUT_RF_START_STOP_TOOGLE           , (Exec_Func)dei_rs_rf_start_stop             },  
    	{ INPUT_RF_RUNTIME_RESET               , (Exec_Func)dei_rs_rf_runtime_reset          },       
    	{ INPUT_RF_RUNTIME_CHECK               , (Exec_Func)dei_rs_rf_runtime_check          },       
        { INPUT_RF_REAR_DEFOGGER               , (Exec_Func)dei_rs_rf_defogger_toogle        },       
        { INPUT_RF_SMART_START_TOOGLE          , (Exec_Func)dei_rs_rf_smart_toogle           },   	
    	{ INPUT_RF_TEMP_START_ADJUST           , (Exec_Func)dei_rs_rf_smart_prg_enter        },       
    	{ INPUT_RF_PUT_NEW_TEMP                , (Exec_Func)dei_rs_rf_smart_prg_set          },
    	{ INPUT_RF_TIMER_MODE_TOOGLE           , (Exec_Func)dei_rs_rf_timer_toogle           }, 
    	{ INPUT_RF_START_TIMES_ADJUST          , (Exec_Func)dei_rs_rf_timer_prg_enter        },   
        { INPUT_RF_PUT_NEW_START_TIMES         , (Exec_Func)dei_rs_rf_timer_prg_adjust       },       
    	{ INPUT_ALL_DOOR_STATUS_OPEN		   , (Exec_Func)dei_rs_trigger_door              }, 	
    	{ INPUT_TRUNK_STATUS_OPEN			   , (Exec_Func)dei_rs_trigger_trunk             }, 	
    	{ INPUT_TRIGGER_ON                     , (Exec_Func)dei_rs_trigger_generic           }, 				
    	{ INPUT_IGN_STATUS_ON				   , (Exec_Func)dei_rs_key                       }, 		
    	{ INPUT_BRAKE_STATUS_PRESS 			   , (Exec_Func)dei_rs_brake                     }, 
    	{ INPUT_PRK_NEUTRAL_STATUS_OFF         , (Exec_Func)dei_rs_neutral                   }, 	
    	{ INPUT_HANDBRAKE_STATUS_OFF           , (Exec_Func)dei_rs_handbrake                 }, 				
    	{ INPUT_HOOD_STATUS_OPEN			   , (Exec_Func)dei_rs_hood                      }, 	
    	{ INPUT_ALL_DOOR_STATUS_OPEN		   , (Exec_Func)dei_rs_door                      },
#ifdef PLATFORM_933      
      { INPUT_RF_START_STOP_TOOGLE | INPUT_RF_EXT  , (Exec_Func)as_rs_extend_runtime_cmd   },
      { INPUT_ALL_DOOR_STATUS_OPEN		   , (Exec_Func)dei_rs_door_open                     },
			{ INPUT_ALL_DOOR_STATUS_CLOSE			 , (Exec_Func)dei_rs_door_closed									},
      { INPUT_RF_UNLOCK           		   , (Exec_Func)dei_rs_unlock_cmd                    },
      { INPUT_OEM_UNLOCK           		   , (Exec_Func)dei_rs_unlock_cmd                    },
#endif      

         #ifndef TRUNK_OPEN_DONT_DISABLE_MTS   	
        { INPUT_TRUNK_STATUS_OPEN			   , (Exec_Func)dei_rs_door                      }, 	
         #else
         #if TRUNK_OPEN_DONT_DISABLE_MTS == 0
         { INPUT_TRUNK_STATUS_OPEN			   , (Exec_Func)dei_rs_door                      }, 	
         #else
            #warning "The Manual Transmission Mode is not disabled by trunk"
         #endif
         #endif

//    	{ INPUT_GWR_ON          		   ,   (Exec_Func)dei_rs_takeover_door_allow                       }, 	
    	{ INPUT_IGN_STATUS_ON				   , (Exec_Func)dei_rs_key                       }, 	
    	{ INPUT_GLOWPLUG_STATUS_OFF            , (Exec_Func)dei_rs_glowplug                  },	
    	{ INPUT_START | INPUT_ON               , (Exec_Func)fob_ctrl_dei_rs_toggle }, //was dei_rs_start_stop_from_smartstart
    	{ INPUT_STOP  | INPUT_ON               , (Exec_Func)dei_rs_stop_only                 },
    	{ INPUT_REQ_RUNTIME                    , (Exec_Func)dei_rs_rf_runtime_check          },
#ifdef PLATFORM_933
			{ INPUT_RS_VALET									, 	(Exec_Func)dei_rs_push_valet			},
			{ INPUT_RF_HOMEVALET						, 	(Exec_Func)dei_rs_rf_valet			},
			{ INPUT_RF_START_VALET						, 	(Exec_Func)dei_rs_rf_valet			},
			{ INPUT_RF_COMBO_VALET						, 	(Exec_Func)dei_rs_rf_combo_valet			},
			{ INPUT_RS_IGNITION_ON            ,   (Exec_Func)dei_rs_keypad_unlock }
#endif


    };

    CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
}

#if 0
//--------------------------------------------------------------------------//
void dei_rs_takeover_door_allow(void)
{
		if( (get_gearbox_type() == GEARBOX_AUTOMATIC)
					 && (dei_feature_get(DEI_FEAT_TAKEOVER) == DEI_TAKEOVER_DISABLE_W_DOOR_OPEN))
			door_open_allow_once = TRUE;
		else
			door_open_allow_once = FALSE;
}
#endif

#ifdef PLATFORM_933	
void dei_rs_door_closed(void) {

	door_open_allow_once = FALSE;	
}

void dei_rs_keypad_unlock(void)
{
	if(dei_ign_before_rs)
	{
	  if( dei_feature_get( DEI_FEAT_RF_KEYBOARD_UNLOCK ) == 2 ) //feature is enable
	  {		
		  notify_send( NOTIFY_KEYBOARD_UNLOCK ) ; 				
	  }
	}
}
#endif

//--------------------------------------------------------------------------//
