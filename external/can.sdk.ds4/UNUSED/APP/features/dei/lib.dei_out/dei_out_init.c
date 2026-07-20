/*==========================================================================*/
// $Id: dei_out_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_out_private.h"
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
void dei_out_init( void )
{
	prg_out_init();	
	
	cmd_register:
    {
        static const Cmd_Array1 cmd_array[] =
        {    
            { INPUT_GWR_OFF 			      , (Exec_Func)dei_out_ground_when_running_status_off},
        	{ INPUT_GWR_ON  			      , (Exec_Func)dei_out_ground_when_running_status_on },	        		
        	{ INPUT_RS_ACCESSORY_OFF 	      , (Exec_Func)dei_out_acc_off 						 },
        	{ INPUT_RS_ACCESSORY_ON  	      , (Exec_Func)dei_out_acc_on 						 },
        	{ INPUT_RS_IGNITION_OFF  	      , (Exec_Func)dei_out_ign_off 						 },
        	{ INPUT_RS_IGNITION_ON   	      , (Exec_Func)dei_out_ign_on 						 },
        	{ INPUT_RS_STARTER_OFF  	      , (Exec_Func)dei_out_start_off					 },
        	{ INPUT_RS_STARTER_ON   	      , (Exec_Func)dei_out_start_on						 },
        	{ INPUT_GWR_ON  			      , (Exec_Func)dei_out_ground_when_running_5sec   	 },	
        	{ INPUT_GWR_ON  			      , (Exec_Func)dei_out_pulse_before_start         	 },		        	
        	{ INPUT_GWR_OFF       	          , (Exec_Func)dei_out_smartkey_control           	 },        	
        	{ INPUT_RS_STARTER_OFF   	      , (Exec_Func)dei_out_pulse_after_start          	 },
        	{ INPUT_DEFOGGER_ON               , (Exec_Func)dei_out_rear_defogger_on              },
        	{ INPUT_DEFOGGER_OFF              , (Exec_Func)dei_out_rear_defogger_off             },
        	{ INPUT_LOCK1         |INPUT_ON   , (Exec_Func)dei_out_unlock_user2                  },
        	{ INPUT_LOCK2         |INPUT_ON   , (Exec_Func)dei_out_unlock_user2                  },
        	{ INPUT_LOCK3         |INPUT_ON   , (Exec_Func)dei_out_unlock_user2                  },
        	{ INPUT_OEM_LOCK      		      , (Exec_Func)dei_out_unlock_user2                  },
            { INPUT_OEM_ARM       		      , (Exec_Func)dei_out_unlock_user2                  },	
            { INPUT_UNLOCK_DRIVER1|INPUT_ON   , (Exec_Func)dei_out_unlock_user2                  },
        	{ INPUT_UNLOCK_DRIVER2|INPUT_ON   , (Exec_Func)dei_out_unlock_user2                  },
        	{ INPUT_UNLOCK_DRIVER3|INPUT_ON   , (Exec_Func)dei_out_unlock_user2                  },	
            { INPUT_UNLOCK_ALL1   |INPUT_ON   , (Exec_Func)dei_out_unlock_user2                  },
        	{ INPUT_UNLOCK_ALL2   |INPUT_ON   , (Exec_Func)dei_out_unlock_user2                  },
        	{ INPUT_UNLOCK_ALL3   |INPUT_ON   , (Exec_Func)dei_out_unlock_user2                  },		
        	{ INPUT_RF_LOCK                   , (Exec_Func)dei_out_ground_when_arm               },
        	{ INPUT_RF_SILENT_LOCK            , (Exec_Func)dei_out_ground_when_arm               },
        	{ INPUT_RF_UNLOCK                 , (Exec_Func)dei_out_ground_when_arm               },
        	{ INPUT_RF_SILENT_UNLOCK          , (Exec_Func)dei_out_ground_when_arm               },
        	{ INPUT_OEM_LOCK				  , (Exec_Func)dei_out_ground_when_arm               },
        	{ INPUT_OEM_UNLOCK			      , (Exec_Func)dei_out_ground_when_arm               },
            { INPUT_OEM_ARM				      , (Exec_Func)dei_out_ground_when_arm               },
        	{ INPUT_OEM_DISARM			      , (Exec_Func)dei_out_ground_when_arm               },
        	{ INPUT_LOCK1         |INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
        	{ INPUT_LOCK2         |INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
        	{ INPUT_LOCK3         |INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
        	{ INPUT_UNLOCK_DRIVER1|INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
        	{ INPUT_UNLOCK_DRIVER2|INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
        	{ INPUT_UNLOCK_DRIVER3|INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
        	{ INPUT_UNLOCK_ALL1   |INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
        	{ INPUT_UNLOCK_ALL2   |INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
        	{ INPUT_UNLOCK_ALL3   |INPUT_ON   , (Exec_Func)dei_out_ground_when_locked            },
        	{ INPUT_OEM_LOCK				  , (Exec_Func)dei_out_ground_when_locked            },
        	{ INPUT_OEM_UNLOCK			      , (Exec_Func)dei_out_ground_when_locked            },
            { INPUT_OEM_ARM				      , (Exec_Func)dei_out_ground_when_locked            },
        	{ INPUT_OEM_DISARM			      , (Exec_Func)dei_out_ground_when_locked            },		
        	{ INPUT_LOCK1         |INPUT_ON   , (Exec_Func)dei_out_lock                          },
        	{ INPUT_LOCK2         |INPUT_ON   , (Exec_Func)dei_out_lock                          },
        	{ INPUT_LOCK3         |INPUT_ON   , (Exec_Func)dei_out_lock                          },	
            { INPUT_UNLOCK_DRIVER1|INPUT_ON   , (Exec_Func)dei_out_unlock_driver	             },
        	{ INPUT_UNLOCK_DRIVER2|INPUT_ON   , (Exec_Func)dei_out_unlock_driver	             },
        	{ INPUT_UNLOCK_DRIVER3|INPUT_ON   , (Exec_Func)dei_out_unlock_driver	             },	
            { INPUT_UNLOCK_ALL1   |INPUT_ON   , (Exec_Func)dei_out_unlock_all	            	 },
        	{ INPUT_UNLOCK_ALL2   |INPUT_ON   , (Exec_Func)dei_out_unlock_all	            	 },
        	{ INPUT_UNLOCK_ALL3   |INPUT_ON   , (Exec_Func)dei_out_unlock_all	            	 },	    
            { INPUT_UNLOCK_ALL1   |INPUT_ON   , (Exec_Func)dei_out_second_unlock                 },
        	{ INPUT_UNLOCK_ALL2   |INPUT_ON   , (Exec_Func)dei_out_second_unlock                 },
        	{ INPUT_UNLOCK_ALL3   |INPUT_ON   , (Exec_Func)dei_out_second_unlock                 },	
        	{ INPUT_TRUNK         |INPUT_ON   , (Exec_Func)dei_out_trunk_release_on              },
        	{ INPUT_TRUNK         		      , (Exec_Func)dei_out_trunk_release_off             },		
        	{ INPUT_ARM                       , (Exec_Func)dei_out_factory_alarm_arm             },
        	{ INPUT_DISARM                    , (Exec_Func)dei_out_factory_alarm_disarm          },
        	{ INPUT_TRIGGER_ON                , (Exec_Func)dei_out_ground_when_triggered_on      },
        	{ INPUT_TRIGGER_OFF               , (Exec_Func)dei_out_ground_when_triggered_off     },
        	{ INPUT_CHIRP                     , (Exec_Func)dei_out_horn                          },	
        	{ INPUT_PARKING_ON                , (Exec_Func)dei_out_parking_light_on              },
        	{ INPUT_PARKING_OFF               , (Exec_Func)dei_out_parking_light_off             },
        	{ INPUT_DOME_ON          	      , (Exec_Func)dei_out_domelight_on  	  	         },
        	{ INPUT_DOME_OFF         	      , (Exec_Func)dei_out_domelight_off	             },
        	{ INPUT_IGN_STATUS_OFF 	          , (Exec_Func)dei_out_pulse_after_ign_off  		 },
        	{ INPUT_IGN_STATUS_ON  	          , (Exec_Func)dei_out_pulse_after_ign_off_reset  	 },
        };

        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
    }
    
    cmd_lut_insert(INPUT_CHIRP,(Exec_Func)dei_out_horn,EXEC_MODE_PROG);
	
	dei_out_aux1_init();
	dei_out_aux2_init();
	dei_out_aux3_init();
	dei_out_aux4_init();
}

//--------------------------------------------------------------------------//

