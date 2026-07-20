/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: obd_can_ign.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void reset_data(void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void obd_can_ign_on(void * arg)
{    
  #define DATA_REQ(z)  obd_can_vars.schedule_info.can_tx_msg->data._8_[z]
  #define DATA_OD(z)   obd_can_vars.on_demand_info.can_tx_msg->data._8_[z]
  
  UInt8 i;

  if(!obd_can_vars.flg_ign)
    {  
  
    #if DBG_OBD_CAN == 1
      dbg_tx_text("*** obd_can_ign_on()\r\n");
    #endif
    
      // If we were deactivated because of disconnected bus, allow reactivation 
      // unless the situation has occurred twice in succession
      if (obd_can_vars.disconnected_bus_cnt > 0)
      {
    #if DBG_OBD_CAN == 1
      dbg_tx_text("*** Bus was disconnected\r\n");
    #endif
    	  if (obd_can_vars.disconnected_bus_cnt >= BEFORE_DISCONNECT_MAX) return;
    #if DBG_OBD_CAN == 1
      dbg_tx_text("*** Reactivating\r\n");
    #endif
    	  obd_can_activate() ;
      }
    
      // If a scan tool was detected during last ignition cycle, re-activate OBD CAN
      if(obd_can_vars.scan_tool_detected)
        {
    #if DBG_OBD_CAN == 1
      dbg_tx_text("*** Scantool was detected, reactivating\r\n");
    #endif
    	  obd_can_vars.scan_tool_detected = FALSE ;
    	  obd_can_activate() ;
    	}
    
      // New ignition cycle
      if(!obd_can_vars.flg_ign)
        {
          
              obd_can_vars.flg_ign  = TRUE  ;	
        
              DATA_REQ(0)               = 0 ;  
              DATA_REQ(1)               = 0 ;  
              DATA_REQ(2)               = 0 ;
              DATA_OD(0)                = 0 ;  
              DATA_OD(1)                = 0 ;  
              DATA_OD(2)                = 0 ;
            
           
          timeout_update(obd_can_vars.ign_timeout , OBD_CAN_IGN_DELAY * 1000) ; 
          timeout_start(obd_can_vars.ign_timeout) ;
	      obd_can_init_ecu(DEFAULT_ECUS) ;                      // restart request on highest priority ID    

      	  // Configure all requests as per configuration
      	  for (i=0; i<sizeof_array(obd_can_cfg); i++)
      	  {
        	if (obd_can_cfg[i].is_enabled)
    		{
       			obd_can_schedule[i].status = OBD_CAN_INITIAL_REQUEST_NOT_DONE;
    	  		obd_can_config_internal ( (Obd_Can_Req_Enum)i, obd_can_cfg[i].time_based_period, obd_can_cfg[i].event_based_cmd, obd_can_cfg[i].event_based_delay, FALSE);
    		}
      	  }
    
        }
  }	

  #undef DATA 
}

//--------------------------------------------------------------------------// 

void obd_can_ign_off (void * arg)
{
  if(obd_can_vars.flg_ign && !key.bit.ign)
    {     
    #if DBG_OBD_CAN == 1
      dbg_tx_text("*** obd_can_ign_off()\r\n");
    #endif
    
      obd_can_vars.flg_ign = FALSE ;
      obd_can_vars.flg_request = FALSE ;

      timeout_stop(obd_can_vars.ign_timeout) ;
      reset_data () ;                       //reset just RPM, SPEED and Engine Running
    }
}

//--------------------------------------------------------------------------// 

static void reset_data (void)  //reset just RPM, SPEED and Engine Running
{
  UInt8 i ;

  for(i = 0 ; i < sizeof_array(obd_can_cfg) ; i++)
    {
      if(obd_can_is_req_enabled((Obd_Can_Req_Enum)i))
        {
         
              switch(i)
                {
                  case OBD_CAN_RPM          :  	
				  								#if STATUS_RPM_ENABLE == 1 
				  								rpm.word = 0 ;
				  								#endif
				  								break ;
                  case OBD_CAN_SPEED        :  
				  								#if STATUS_SPEED_ENABLE == 1 
				  									speed.bit.is_detected = FALSE ; speed.byte = 0;
				  								#endif
				  								break ;
                  case OBD_CAN_ENG_RUN      :  
				  							   #if STATUS_ENGINE_ENABLE == 1 
				  									engine.run_time = 0 ; 
											   #endif
											   break ;
                }
            
        }
    }
}


