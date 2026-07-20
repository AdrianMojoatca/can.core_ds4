/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_common_events.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
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

void dei_system_common_events( void )
{
	static const Cmd_Array1 cmd_array[] = 
    {    
        { INPUT_RF_LOCK        | INPUT_RF_EXT, (Exec_Func)dei_system_rf_panic          }, 
        { INPUT_RF_SILENT_LOCK | INPUT_RF_EXT, (Exec_Func)dei_system_rf_panic          },         
        { INPUT_RF_PANIC                     , (Exec_Func)dei_system_rf_panic          },	
        { INPUT_RF_CAR_FINDER                , (Exec_Func)dei_system_rf_car_finder     },             	    
        { INPUT_OEM_LOCK                     , (Exec_Func)dei_system_oem_lock          },
    	{ INPUT_OEM_UNLOCK                   , (Exec_Func)dei_system_oem_unlock        },
        { INPUT_IGN_STATUS_OFF               , (Exec_Func)dei_system_rf_keyboard_unlock},	
    	{ INPUT_RF_LOCK                      , (Exec_Func)dei_system_rf_lock    	   }, 
        { INPUT_RF_SILENT_LOCK               , (Exec_Func)dei_system_rf_lock    	   }, 
        { INPUT_RF_UNLOCK                    , (Exec_Func)dei_system_rf_unlock 	 	   }, 
        { INPUT_RF_SILENT_UNLOCK             , (Exec_Func)dei_system_rf_unlock  	   },         
    	{ INPUT_RF_TRUNK                     , (Exec_Func)dei_system_rf_trunk	       }, 
    	{ INPUT_RF_TRUNK | INPUT_RF_EXT	   , (Exec_Func)dei_system_rf_trunk_ext        },   
    	{ INPUT_RF_TRUNK | INPUT_RF_RELEASE  , (Exec_Func)dei_system_rf_trunk_release  },   		    
    	{ INPUT_RF_AUX3                      , (Exec_Func)dei_system_rf_ch3_on  	   },
    	{ INPUT_RF_AUX3 | INPUT_RF_RELEASE   , (Exec_Func)dei_system_rf_ch3_off 	   },
    	{ INPUT_IGN_STATUS_OFF               , (Exec_Func)dei_system_ch3_reset         },
    	{ INPUT_GWR_ON                       , (Exec_Func)dei_system_ch3_link_rs_on    },
    	{ INPUT_GWR_OFF                      , (Exec_Func)dei_system_ch3_link_rs_off   },		
    	{ INPUT_RF_AUX4                      , (Exec_Func)dei_system_rf_ch4_on  	   },
    	{ INPUT_RF_AUX4 | INPUT_RF_RELEASE   , (Exec_Func)dei_system_rf_ch4_off 	   },
    	{ INPUT_IGN_STATUS_OFF               , (Exec_Func)dei_system_ch4_reset         },
    	{ INPUT_GWR_ON                       , (Exec_Func)dei_system_ch4_link_rs_on    },
    	{ INPUT_GWR_OFF                      , (Exec_Func)dei_system_ch4_link_rs_off   },    
    	{ INPUT_RF_AUX5                      , (Exec_Func)dei_system_rf_ch5_on  	   },
    	{ INPUT_RF_AUX5 | INPUT_RF_RELEASE   , (Exec_Func)dei_system_rf_ch5_off 	   },
    	{ INPUT_IGN_STATUS_OFF               , (Exec_Func)dei_system_ch5_reset         },
    	{ INPUT_GWR_ON                       , (Exec_Func)dei_system_ch5_link_rs_on    },
    	{ INPUT_GWR_OFF                      , (Exec_Func)dei_system_ch5_link_rs_off   },
    	{ INPUT_RF_AUX6                      , (Exec_Func)dei_system_rf_ch6_on  	   },
    	{ INPUT_RF_AUX6 | INPUT_RF_RELEASE   , (Exec_Func)dei_system_rf_ch6_off 	   },
    	{ INPUT_IGN_STATUS_OFF               , (Exec_Func)dei_system_ch6_reset         },
    	{ INPUT_GWR_ON                       , (Exec_Func)dei_system_ch6_link_rs_on    },
    	{ INPUT_GWR_OFF                      , (Exec_Func)dei_system_ch6_link_rs_off   },    
    	{ INPUT_RF_TEMP_REPORT_REQ           , (Exec_Func)dei_system_rf_temp           }
    };

    CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);		
}	

//--------------------------------------------------------------------------//
