/*==========================================================================*/
// $Id: status_req_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "status_req_private.h"
#include "cmd.h"

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

void status_req_init (void)
{

	status_req_init_nvfs();

    status_req_vars.smartstart_is_connect = FALSE ;
    status_req_vars.obd_can_check_ena     = FALSE ;

    status_req_vars.rpm_timeout = timeout_f_create(RPM_MIN_INTERVAL , status_req_rpm_blocked) ;

    cmd_register :
    {
        static const Cmd_Array1 cmd_array[] = 
        {
	        {INPUT_REQ_INPUT_STATUS					, status_req_input_status					},
	        {INPUT_REQ_RPM							, status_req_rpm							},
	        {INPUT_REQ_SPEED						, status_req_d2d_speed				},
	        {INPUT_REQ_ENG_TEMP						, status_req_eng_temp						},
	        {INPUT_REQ_EXTERIOR_TEMP				, status_req_intake_temp					},
	        {INPUT_REQ_DTC			 				, status_req_dtc							},
	        {INPUT_REQ_CLR_DTC						, status_req_clr_dtc						},
	        {INPUT_REQ_FUEL_LEVEL					, status_req_fuel_level						},
	        {INPUT_REQ_ODO							, status_req_odo							},
	        {INPUT_REQ_BAT_VOLT						, status_req_bat_volt						},
	        {INPUT_REQ_TPMS							, status_req_tpms							},
	        {INPUT_REQ_VIN_1						, status_req_vin_1							},
	        {INPUT_REQ_VIN_2						, status_req_vin_2							},
	        {INPUT_REQ_VIN_3						, status_req_vin_3							},
	        {INPUT_REQ_VIN_4						, status_req_vin_4							},
	        {INPUT_REQ_VIN_5						, status_req_vin_5							},
	        {INPUT_REQ_VIN_6						, status_req_vin_6							},
	        {INPUT_REQ_VIN							, status_req_vin							},
	        {INPUT_REQ_PROTOCOL_VER					, status_req_protocol_ver					},
	        {INPUT_AL_QUERY							, status_req_al_query						},
	        {INPUT_AL_ALERT_CLEAR				    , status_req_al_alert_clear					},
	        {INPUT_REQ_MODULE_INFO				    , status_req_module_info					},
	        {INPUT_REQ_EXTENDED_STATUS_SUPPORT	    , status_req_extended_status_support		},
	        {INPUT_REQ_EXTENDED_STATUS_SUPPORT_SYNC	, status_req_extended_status_support_sync	},
	        {INPUT_REQ_RUNTIME						, status_req_runtime						},
	        {INPUT_REQ_2WAY_STATUS					, status_req_2way_status					},
	        {INPUT_REQ_EXTENDED_STATUS				, status_req_extended_status				},
					{INPUT_REQ_SENS_TEMP            , status_req_sens_temp },
        };

        CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL | EXEC_MODE_PROG);
    } 

	d2d_lut_insert (REQ_DTC_CODE						, status_req_dtc_code) ;
}

//--------------------------------------------------------------------------//
