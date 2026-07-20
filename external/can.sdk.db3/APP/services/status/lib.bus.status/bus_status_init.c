/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: bus_status_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/
//
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "bus_status_private.h"
#include "timeout_f.h"

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

void bus_status_init (void)
{
  
	static const Cmd_Array1 cmd_array[] = 
    {
        {INPUT_GWR_ON 						, bus_status_notify_input_gwr_on				}, 
    	{INPUT_GWR_OFF				  		, bus_status_notify_input_gwr_off				}, 
    	{INPUT_BRAKE_STATUS_PRESS 			, bus_status_notify_brake_on  	    			}, 
    	{INPUT_BRAKE_STATUS_RELEASE    		, bus_status_notify_brake_off      				}, 
    	{INPUT_PRK_NEUTRAL_STATUS_ON  		, bus_status_notify_prk_neutral_active    	 	}, 
    	{INPUT_PRK_NEUTRAL_STATUS_OFF	 	, bus_status_notify_prk_neutral_not_active  	}, 
    	{INPUT_HANDBRAKE_STATUS_ON	 		, bus_status_notify_handbrake_on    			}, 
    	{INPUT_HANDBRAKE_STATUS_OFF	 		, bus_status_notify_handbrake_off   			}, 
    	{INPUT_HOOD_STATUS_OPEN	 			, bus_status_notify_hood_open 					}, 
    	{INPUT_HOOD_STATUS_CLOSE	 		, bus_status_notify_hood_close		 			}, 
    	{INPUT_TRUNK_STATUS_OPEN	 		, bus_status_notify_trunk_open		 			}, 
    	{INPUT_TRUNK_STATUS_CLOSE	 		, bus_status_notify_trunk_close 	 			}, 
    	{INPUT_ALL_DOOR_STATUS_OPEN 		, bus_status_notify_other_door_open 			}, 
     	{INPUT_ALL_DOOR_STATUS_CLOSE 	    , bus_status_notify_other_door_close			}, 
    	{INPUT_GLOWPLUG_STATUS_ON 			, bus_status_notify_glowplug_on				  	}, 
    	{INPUT_GLOWPLUG_STATUS_OFF 			, bus_status_notify_glowplug_off		  		}, 
    	{INPUT_KEY_IN_STATUS_ON 			, bus_status_notify_key_in				  		}, 
    	{INPUT_KEY_IN_STATUS_OFF 			, bus_status_notify_key_out	  					}, 
    	{INPUT_IGNITION 					, bus_status_notify_input_ignition_off	  		}, 
    	{INPUT_IGNITION_ON 		            , bus_status_notify_input_ignition_on			}, 
    	{INPUT_LOCK1 			| INPUT_ON  , bus_status_notify_input_lock					}, 
    	{INPUT_LOCK2 			| INPUT_ON  , bus_status_notify_input_lock					}, 
    	{INPUT_LOCK3 			| INPUT_ON  , bus_status_notify_input_lock					}, 
    	{INPUT_UNLOCK_DRIVER1	| INPUT_ON 	, bus_status_notify_input_unlock	    		}, 
    	{INPUT_UNLOCK_DRIVER2	| INPUT_ON 	, bus_status_notify_input_unlock	    		}, 
    	{INPUT_UNLOCK_DRIVER3	| INPUT_ON 	, bus_status_notify_input_unlock	    		}, 
    	{INPUT_UNLOCK_ALL1 	| INPUT_ON 		, bus_status_notify_input_unlock	    		}, 
    	{INPUT_UNLOCK_ALL2	| INPUT_ON 		, bus_status_notify_input_unlock	    		}, 
    	{INPUT_UNLOCK_ALL3	| INPUT_ON 		, bus_status_notify_input_unlock	    		}, 
    	{INPUT_OEM_LOCK 					, bus_status_notify_input_keyless_lock			}, 
    	{INPUT_OEM_UNLOCK 					, bus_status_notify_input_keyless_unlock		}, 
    	{INPUT_OEM_ARM 						, bus_status_notify_input_keyless_arm			}, 
    	{INPUT_OEM_DISARM 					, bus_status_notify_input_keyless_disarm		}, 
    	{INPUT_OEM_TRUNK 					, bus_status_notify_input_keyless_trunk	  		}, 
    	{INPUT_OEM_PANIC 					, bus_status_notify_input_keyless_panic	  		}, 
    	{INPUT_OEM_AUX1 					, bus_status_notify_input_keyless_aux1	  		}, 
    	{INPUT_OEM_AUX2 					, bus_status_notify_input_keyless_aux2	  		}, 
    	{INPUT_OEM_AUX3 					, bus_status_notify_input_keyless_aux3	  		}, 
    	{INPUT_OEM_AUX4 					, bus_status_notify_input_keyless_aux4	  		}, 
    	{INPUT_OEM_RS_ON 					, bus_status_notify_input_keyless_start			}, 
    	{INPUT_OEM_RS_OFF					, bus_status_notify_input_keyless_stop  	  	}, 
    	{INPUT_EIPS_ALERT					, bus_status_notify_eips_alert					}, 
    	{INPUT_EIPS_SHUTDOWN         	    , bus_status_notify_eips_shutdown    			}, 
    	{INPUT_EIPS_SHUTDOWN_FAIL_ON       	, bus_status_notify_eips_shutdown_fail			},
    	{ INPUT_HEADLIGHT_STATUS_ON         , bus_status_notify_headlight_on                },
		{ INPUT_HEADLIGHT_STATUS_OFF        , bus_status_notify_headlight_off               }

    };                                                                                       

    CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL|EXEC_MODE_PROG);

    d2d_delay_timeout = timeout_f_create(D2D_TIMEOUT , bus_status_notify_brake_d2d_delay) ;

}


