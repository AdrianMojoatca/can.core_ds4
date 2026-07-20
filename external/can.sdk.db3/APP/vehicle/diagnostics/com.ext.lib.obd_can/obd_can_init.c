/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: obd_can_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "obd_can_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void init_data				  ( void ) ;
static void init_obd_can_vars ( void ) ;
static void init_request_info(Obd_Can_Req_Info * request_info) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Boolean do_init = TRUE ;
static Boolean all_ecus = TRUE;

static const Cmd_Array1 cmd_array[] = 
{
    {INPUT_REQ_CLR_DTC           , obd_can_clr_dtc } , 
    {INPUT_IGNITION   | INPUT_ON , obd_can_ign_on  } ,
    {INPUT_IGNITION              , obd_can_ign_off } ,            
};
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void obd_can_init (void)
{
  UInt8 i;

  do_init = FALSE ;
  
  if(!obd_can_init_nvfs())
  {
    #if DBG_OBD_CAN == 1
      dbg_tx_text("\n*** obd_can_init(): cannot initialize nvfs!\n");
    #endif

    return ;
  }

  // Initialize some runtime variables
  obd_can_vars.is_connected = FALSE ;
  obd_can_vars.flg_ign = FALSE ;
  obd_can_vars.disconnected_bus_cnt = 0;
  obd_can_vars.scan_tool_detected = FALSE ;
  obd_can_vars.is_active = FALSE ;
  init_data() ;

  // Zero the request schedule - it will be automatically rebuilt
  // from the configuration at the next IGN cycle
  for (i=0; i<sizeof_array(obd_can_schedule); i++)
  {
     obd_can_schedule[i].when = 0; 
    obd_can_schedule[i].status = OBD_CAN_INITIAL_REQUEST_NOT_DONE;
  }

  // Initialize the ECU map
  obd_can_init_ecu( obd_can_vars.is_ext ? ALL_ECUS : SPECIFIC_ECUS ) ;
  
  // Special case for DTC_CLR: send request to all ECUs
   obd_can_ecu_map[OBD_CAN_DTC_CLR].ecu = OBD_CAN_REQ_ALL_ECUS; 

  // Initialize the request retries
  init_obd_can_vars() ;

  // Initialize RX timeout
  obd_can_vars.schedule_info.rx_timeout  = timeout_f_create (OBD_CAN_P2_TIMEOUT, obd_can_rx_timeout_schedule) ;
  obd_can_vars.on_demand_info.rx_timeout = timeout_f_create (OBD_CAN_P2_TIMEOUT, obd_can_rx_timeout_on_demand) ;
  // Initialize IGN delay timeout
  obd_can_vars.ign_timeout = timeout_f_create (OBD_CAN_IGN_DELAY * 1000 + 10, obd_can_ign_timeout) ;

  

  // Initialize our tasks

  obd_can_vars.rx_tsk = os_tsk_create (can_v_usr_rx_task , TASK_CAN_V_USR_RX_PRI , TASK_CAN_V_USR_RX_STK , 0 , "Obd Can Rx") ;
  os_tsk_start (obd_can_vars.rx_tsk , &obd_can_vars.can_v_usr) ;

  obd_can_vars.ctrl_tsk = os_tsk_create (obd_can_ctrl_task , TASK_OBD_CTRL_PRI , TASK_OBD_CTRL_STK , 0 , TASK_OBD_CTRL_NAME) ;
  os_tsk_start (obd_can_vars.ctrl_tsk , NULL) ;

  // Register to ignition because OBD CAN request can only happen during an ignition cycle  

  CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);


}

//--------------------------------------------------------------------------// 

static void init_data(void)
{
  UInt8 i ;
  
  for(i = 0 ; i < sizeof_array(obd_can_cfg) ; i++)
    {
      if(obd_can_is_req_enabled((Obd_Can_Req_Enum)i))
        {
          
              switch(i)
                {
                  case OBD_CAN_VIN          :  ; break ;
                  case OBD_CAN_RPM          :  
				  								#if STATUS_RPM_ENABLE == 1 
				  								rpm.word = 0 ;
				  								#endif
				  								break ;
                  case OBD_CAN_SPEED        :  
				  								#if STATUS_SPEED_ENABLE > 0
				  									speed.bit.is_detected = FALSE ; speed.byte = 0;
				  								#endif
				  								break ;
                  case OBD_CAN_ENG_RUN      :  
				  							   #if STATUS_ENGINE_ENABLE == 1 
				  									engine.run_time = 0 ; 
											   #endif
											   break ;
                  case OBD_CAN_FUEL_LEVEL   :  
				  							   #if STATUS_ENGINE_ENABLE > 0 
				  									engine.fuel_level = 0 ; 
											   #endif 
											   break ;
                  case OBD_CAN_ENG_COOLANT  :  
				  							   #if STATUS_TEMPERATURE_ENABLE == 1
				  									 temperature.coolant_ena = FALSE ; temperature.eng_coolant = 0; 
											   #endif 
											   break ;
                  case OBD_CAN_AIR_AMB      :  
				  								#if STATUS_TEMPERATURE_ENABLE == 1
													temperature.cabin_ena   = FALSE ; temperature.cabin       = 0;
												#endif
												break ;
                  case OBD_CAN_INTAKE_TEMP  :  
				  								#if STATUS_TEMPERATURE_ENABLE == 1
				  									temperature.in_ena      = FALSE ; temperature.in          = 0;
												#endif
												break ;
                  case OBD_CAN_BAT_VOLT     :  
				  							   #if STATUS_ENGINE_ENABLE > 0 
				  									engine.bat_volt = 0 ;
											   #endif 
											   break ;
                  case OBD_CAN_DTC_CODE     :                                                               ; break ;
                  case OBD_CAN_DTC_CLR      :                                                               ; break ;
                }
            
        }
    }
}

//--------------------------------------------------------------------------//

void obd_can_init_ecu(All_Ecu_State state)
{
  if(do_init)
    { 
      obd_can_init() ; 	  
    }
  if(state == ALL_ECUS)
  {
	  all_ecus = TRUE;
  }
  else if(state == SPECIFIC_ECUS)
  {
	  all_ecus = FALSE;
  }
    #if DBG_OBD_CAN == 1
        dbg_tx_text("\n*** obd_can_init_ecu() :");
        dbg_tx_text(all_ecus ? "All ECUs" : "Specific ECUs");
        dbg_tx_text("\r\n");
    #endif  

  all_ecus ? init_ecu_map(OBD_CAN_REQ_ALL_ECUS) : init_ecu_map(OBD_CAN_REQ_FIRST_ECU);	
}

//--------------------------------------------------------------------------//

void init_ecu_map(UInt32 first_potential_ecu)
{
  UInt8 i;
  
	// Initialize the ECU map
  for (i=0; i<OBD_CAN_REQ_COUNT; i++)
  {
    
	  // ECU not known yet - the transmission will automatically detect the ECU starting with 'potential_ecu'
      obd_can_ecu_map[i].ecu = OBD_CAN_REQ_NO_ECU;
      obd_can_ecu_map[i].potential_ecu = first_potential_ecu;
      // Sets the highest value for the answer_map
      obd_can_ecu_ans_map[i].ecu = OBD_CAN_REQ_NO_ECU;
	
  }
}

//--------------------------------------------------------------------------//

static void init_obd_can_vars( void )
{
  obd_can_vars.req_retries = OBD_CAN_REQ_RETRIES;
  init_request_info(&obd_can_vars.schedule_info);
  init_request_info(&obd_can_vars.on_demand_info);
}

//--------------------------------------------------------------------------//

static void init_request_info(Obd_Can_Req_Info * request_info)
{
  
  #if DBG_OBD_CAN == 1
    dbg_tx_text("--- init_request_info():\r\n");
  #endif
  request_info->req_retry_cnt = 0;
}

