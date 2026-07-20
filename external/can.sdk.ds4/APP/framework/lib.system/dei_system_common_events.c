/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_common_events.c 22345 2013-08-26 10:17:04Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
#include "prg_out.h"
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
void dei_system_pop_trunk_timeout( void);
//--------------------------------------------------------------------------//

#include "nvfs.h"
#include "bus_status_private.h"
#include "intl_port.h"

extern UInt32 htonl(UInt32);

const NVFS_Entry_Name nvfs_tamper_file  = "TAMPER" ;

void dei_system_tamper_event(void)
{
#define TAMPERFILE_ENTRIES 4	
#define TAMPERFILE_SIZE (TAMPERFILE_ENTRIES*sizeof(UInt32))
static UInt32 stamps[TAMPERFILE_ENTRIES];
UInt32 nnow;
UInt8 min,i;


	if(!bus_status_tampered())
	{// is this the first tamper_open event this arm/disarm cycle?
		bus_status_tamper(1); // remember that first time has occurred
		
		memset(stamps,0,TAMPERFILE_SIZE);
		nvfs_rd_s(nvfs_tamper_file,&stamps[0],TAMPERFILE_SIZE);

		// find lowest timestamp
		min = 0;
		for(i=1;i<TAMPERFILE_ENTRIES;i++)
		{
			if((SInt32)htonl(stamps[i]) < (SInt32)htonl(stamps[min]))
				min = i;
		}
		
		nnow = htonl(timeserv_get());
		if(	stamps[min] != nnow )
		{
			stamps[min] = nnow;
			nvfs_wr_create(nvfs_tamper_file,stamps,TAMPERFILE_SIZE);
		}		
	}
}

void dei_system_brake_pressed(void)
{
	prg_out_execute(1+PRG_OUT_STS_BRAKE,1); // tell anyone who is interested
}

void dei_system_brake_released(void)
{
	prg_out_execute(1+PRG_OUT_STS_BRAKE,0); // tell anyone who is interested	
}


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
        { INPUT_OEM_ARM                      , (Exec_Func)dei_system_oem_lock          },
    	{ INPUT_OEM_UNLOCK                   , (Exec_Func)dei_system_oem_unlock        },
        { INPUT_OEM_TRUNK                    , (Exec_Func)dei_system_oem_trunk         },
        { INPUT_IGN_STATUS_OFF               , (Exec_Func)dei_system_rf_keyboard_unlock},	
    	{ INPUT_RF_LOCK                      , (Exec_Func)dei_system_rf_lock    	   }, 
        { INPUT_RF_SILENT_LOCK               , (Exec_Func)dei_system_rf_lock    	   }, 
        { INPUT_RF_UNLOCK                    , (Exec_Func)dei_system_rf_unlock 	 	   }, 
        { INPUT_RF_SILENT_UNLOCK             , (Exec_Func)dei_system_rf_unlock  	   },         
    	{ INPUT_RF_TRUNK                     , (Exec_Func)dei_system_rf_trunk	       }, 
    	{ INPUT_RF_TRUNK | INPUT_RF_EXT	     , (Exec_Func)dei_system_rf_trunk_ext      },   
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
    	{ INPUT_RF_TEMP_REPORT_REQ           , (Exec_Func)dei_system_rf_temp           },
    	{ INPUT_CONVENIENCE_ALERT           , (Exec_Func)dei_system_convenience_alert  },
    	{ INPUT_HEADLIGHT_STATUS_ON         , (Exec_Func)dei_system_headlight_begin  },
    	{ INPUT_HEADLIGHT_STATUS_OFF        , (Exec_Func)dei_system_headlight_begin  },
			{ INPUT_TRUNK_STATUS_CLOSE         , (Exec_Func)dei_system_trunk_close  				},
			{ INPUT_TRUNK_STATUS_OPEN         , (Exec_Func)dei_system_trunk_open  				},
			{ INPUT_RF_UNLOCK        | INPUT_RF_EXT, (Exec_Func)as_system_rf_panic          }, 
			{ INPUT_RF_SILENT_UNLOCK | INPUT_RF_EXT, (Exec_Func)as_system_rf_panic          },
      { INPUT_ALL_DOOR_STATUS_OPEN		   , (Exec_Func)dei_system_rf_poptrunk_zone_open  },
			{ INPUT_HOOD_STATUS_OPEN					 , (Exec_Func)dei_system_rf_poptrunk_zone_open },
			{ INPUT_IGN_STATUS_ON							 , (Exec_Func)dei_system_rf_poptrunk_zone_open },
			{ INPUT_RF_UNLOCK									 , (Exec_Func)dei_system_rf_trunk_disarm },
			{ INPUT_TAMPER | INPUT_ON          , (Exec_Func)dei_system_tamper_event },
			{ INPUT_BRAKE_STATUS_PRESS         , (Exec_Func)dei_system_brake_pressed },
			{ INPUT_BRAKE_STATUS_RELEASE       , (Exec_Func)dei_system_brake_released },
			{ INPUT_PTIM_STOP | INPUT_ON       , (Exec_Func)dei_system_ptim_stop_on },
			{ INPUT_PTIM_STOP                  , (Exec_Func)dei_system_ptim_stop_off },
			{ INPUT_EXT_OPEN | INPUT_ON        , (Exec_Func)dei_system_ext_open_trigger },
			{ INPUT_EXT_CLOSE | INPUT_ON       , (Exec_Func)dei_system_ext_close_trigger },
			{ INPUT_EXT_TRUNK | INPUT_ON       , (Exec_Func)dei_system_ext_trunk_trigger },
			
			{ INPUT_OEM_UNLOCK                 , (Exec_Func)dei_system_prox_oem_disarm },
			{ INPUT_OEM_LOCK                   , (Exec_Func)dei_system_prox_oem_arm },
			{ INPUT_RF_LOCK                    , (Exec_Func)dei_system_prox_rf_lock },
			{ INPUT_RF_UNLOCK                  , (Exec_Func)dei_system_prox_rf_unlock },
				
    };

    CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);		
		
		// VRTCs
		
		dei_system_headlight_alert_vrtc = vrtc_f_create( (Exec_Func)dei_system_headlight_process ) ; 
		dei_system_pop_trunk_vrtc = vrtc_f_create( (Exec_Func)dei_system_pop_trunk_timeout );  // VRTC Timer for pop trunk state machine

}	

//--------------------------------------------------------------------------//
