/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/


/*==========================================================================*/
// $Id: obd_can_rx.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "obd_can_private.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef Boolean (* Func)(Obd_Can_Rx_Msg * obd_can_rx_msg) ;

typedef struct
{
  UInt8 pid  ;
  Func  func  ;

}Obd_Can_Pid_Def;  

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void process_mode1    (Obd_Can_Rx_Msg * obd_can_rx_msg);
static void process_mode2    (Obd_Can_Rx_Msg * obd_can_rx_msg);
static void process_mode3    (Obd_Can_Rx_Msg * obd_can_rx_msg);
static void process_mode4    (Obd_Can_Rx_Msg * obd_can_rx_msg);
static void process_mode9    (Obd_Can_Rx_Msg * obd_can_rx_msg);
static void process_neg_resp (Obd_Can_Rx_Msg * obd_can_rx_msg, Obd_Can_Req_Enum request) ;
static void process_neg_resp_info (Obd_Can_Rx_Msg * obd_can_rx_msg, Obd_Can_Req_Info * request_info) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static const Obd_Can_Pid_Def pid_mode1_table[] =
{
   {0x05 , obd_can_rx_eng_coolant } ,// Engine coolant temperature
   {0x0C , obd_can_rx_rpm         } ,// Engine RPM
   {0x0D , obd_can_rx_speed       } ,// Vehicle speed
   {0x0F , obd_can_rx_intake_temp } ,// Intake air temperature
   {0x1F , obd_can_rx_eng_run     } ,// Run time since engine start
   {0x2F , obd_can_rx_fuel_level  } ,// Fuel Level Input
   {0x42 , obd_can_rx_bat_volt    } ,// Control module voltage
   {0x46 , obd_can_rx_air_ambiant } ,// Ambient air temperature

//   {0x00 ,                      } ,// PIDs supported [01 - 20]
//   {0x01 ,                      } ,// Monitor status since DTCs cleared. (Includes malfunction indicator lamp (MIL) status and number of DTCs.)
//   {0x02 ,                      } ,// Freeze DTC
//   {0x03 ,                      } ,// Fuel system status
//   {0x04 ,                      } ,// Calculated engine load value
//   {0x06 ,                      } ,// Short term fuel % trim—Bank 1
//   {0x07 ,                      } ,// Long term fuel % trim—Bank 1
//   {0x08 ,                      } ,// Short term fuel % trim—Bank 2
//   {0x09 ,                      } ,// Long term fuel % trim—Bank 2
//   {0x0A ,                      } ,// Fuel pressure
//   {0x0B ,                      } ,// Intake manifold absolute pressure
//   {0x0E ,                      } ,// Timing advance
//   {0x10 ,                      } ,// MAF air flow rate
//   {0x11 ,                      } ,// Throttle position
//   {0x12 ,                      } ,// Commanded secondary air status
//   {0x13 ,                      } ,// Oxygen sensors present
//   {0x14 ,                      } ,// Bank 1, Sensor 1:     Oxygen sensor voltage,      Short term fuel trim
//   {0x15 ,                      } ,// Bank 1, Sensor 2:     Oxygen sensor voltage,      Short term fuel trim
//   {0x16 ,                      } ,// Bank 1, Sensor 3:     Oxygen sensor voltage,      Short term fuel trim
//   {0x17 ,                      } ,// Bank 1, Sensor 4:     Oxygen sensor voltage,      Short term fuel trim
//   {0x18 ,                      } ,// Bank 2, Sensor 1:     Oxygen sensor voltage,      Short term fuel trim
//   {0x19 ,                      } ,// Bank 2, Sensor 2:         Oxygen sensor voltage,      Short term fuel trim
//   {0x1A ,                      } ,// Bank 2, Sensor 3:     Oxygen sensor voltage,      Short term fuel trim
//   {0x1B ,                      } ,// Bank 2, Sensor 4:     Oxygen sensor voltage,      Short term fuel trim
//   {0x1C ,                      } ,// OBD standards this vehicle conforms to
//   {0x1D ,                      } ,// Oxygen sensors present
//   {0x1E ,                      } ,// Auxiliary input status        
//   {0x20 ,                      } ,// PIDs supported 21-40
//   {0x21 ,                      } ,// Distance traveled with malfunction indicator lamp (MIL) on
//   {0x22 ,                      } ,// Fuel Rail Pressure (relative to manifold vacuum)
//   {0x23 ,                      } ,// Fuel Rail Pressure (diesel)
//   {0x24 ,                      } ,// O2S1_WR_lambda(1):        Equivalence Ratio       Voltage
//   {0x25 ,                      } ,// O2S2_WR_lambda(1):        Equivalence Ratio       Voltage
//   {0x26 ,                      } ,// O2S3_WR_lambda(1):        Equivalence Ratio       Voltage
//   {0x27 ,                      } ,// O2S4_WR_lambda(1):        Equivalence Ratio       Voltage
//   {0x28 ,                      } ,// O2S5_WR_lambda(1):        Equivalence Ratio       Voltage
//   {0x29 ,                      } ,// O2S6_WR_lambda(1):        Equivalence Ratio       Voltage
//   {0x2A ,                      } ,// O2S7_WR_lambda(1):        Equivalence Ratio       Voltage
//   {0x2B ,                      } ,// O2S8_WR_lambda(1):        Equivalence Ratio       Voltage
//   {0x2C ,                      } ,// Commanded EGR
//   {0x2D ,                      } ,// EGR Error
//   {0x2E ,                      } ,// Commanded evaporative purge
//   {0x30 ,                      } ,// # of warm-ups since codes cleared
//   {0x31 ,                      } ,// Distance traveled since codes cleared
//   {0x32 ,                      } ,// Evap. System Vapor Pressure
//   {0x33 ,                      } ,// Barometric pressure
//   {0x34 ,                      } ,// O2S1_WR_lambda(1):        Equivalence Ratio       Current
//   {0x35 ,                      } ,// O2S2_WR_lambda(1):        Equivalence Ratio       Current
//   {0x36 ,                      } ,// O2S3_WR_lambda(1):        Equivalence Ratio       Current
//   {0x37 ,                      } ,// O2S4_WR_lambda(1):        Equivalence Ratio       Current
//   {0x38 ,                      } ,// O2S5_WR_lambda(1):        Equivalence Ratio       Current
//   {0x39 ,                      } ,// O2S6_WR_lambda(1):        Equivalence Ratio       Current
//   {0x3A ,                      } ,// O2S7_WR_lambda(1):        Equivalence Ratio       Current
//   {0x3B ,                      } ,// O2S8_WR_lambda(1):        Equivalence Ratio       Current
//   {0x3C ,                      } ,// Catalyst Temperature      Bank 1, Sensor 1
//   {0x3D ,                      } ,// Catalyst Temperature      Bank 2, Sensor 1
//   {0x3E ,                      } ,// Catalyst Temperature      Bank 1, Sensor 2
//   {0x3F ,                      } ,// Catalyst Temperature      Bank 2, Sensor 2
//   {0x40 ,                      } ,// PIDs supported 41-60
//   {0x41 ,                      } ,// Monitor status this drive cycle
//   {0x43 ,                      } ,// Absolute load value
//   {0x44 ,                      } ,// Command equivalence ratio
//   {0x45 ,                      } ,// Relative throttle position
//   {0x47 ,                      } ,// Absolute throttle position B
//   {0x48 ,                      } ,// Absolute throttle position C
//   {0x49 ,                      } ,// Accelerator pedal position D
//   {0x4A ,                      } ,// Accelerator pedal position E
//   {0x4B ,                      } ,// Accelerator pedal position F
//   {0x4C ,                      } ,// Commanded throttle actuator
//   {0x4D ,                      } ,// Time run with MIL on
//   {0x4E ,                      } ,// Time since trouble codes cleared
//   {0x51 ,                      } ,// Fuel Type
//   {0x52 ,                      } ,// Ethanol fuel %
} ; 

//--------------------------------------------------------------------------// 

static const Obd_Can_Pid_Def pid_mode9_table[] =
{
   {0x02 , obd_can_rx_vin } ,		// Vehicle identification number (VIN)
};
static const Pid_to_Req_Enum pid_to_req_enum[] = {
                                      { 0x05, OBD_CAN_ENG_COOLANT } , { 0x0C, OBD_CAN_RPM }       , { 0x0D, OBD_CAN_SPEED } ,
                                      { 0x0F, OBD_CAN_INTAKE_TEMP } , { 0x1F, OBD_CAN_ENG_RUN }   , { 0x2F, OBD_CAN_FUEL_LEVEL }  ,
                                      { 0x42, OBD_CAN_BAT_VOLT }  , { 0x46, OBD_CAN_AIR_AMB }
                                    } ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void obd_can_rx (void * q_msg)
{
  #define CAN_RX_Q_MSG    ((Can_Rx_Q_Msg *) q_msg)
  #define CAN_MSG         CAN_RX_Q_MSG->can_msg
  #define RXDATA(z)       CAN_MSG.data._8_[z]
    
  #define ID			        CAN_MSG.id

  Boolean                 done = FALSE ;
  UInt8                   process_mode;
  static Obd_Can_Rx_Msg   obd_can_msg ;
  
  Obd_Can_Req_Enum  request = (Obd_Can_Req_Enum)1;
  UInt8 i;


  obd_can_vars.is_connected = TRUE ;
  
  // --------------------------------
  // Single-frame responses
  // --------------------------------  
  
  if(RXDATA(0) < 8)
  {
      obd_can_msg.size   = RXDATA(0) ;
      obd_can_msg.sidpr  = RXDATA(1) ;

		  memset(obd_can_msg.data,0,sizeof(obd_can_msg));
		  if(obd_can_msg.size) // NCC-DIRE002-011
				memcpy(&obd_can_msg.data[0] , &RXDATA(2) , obd_can_msg.size - 1) ; // sub the sidpr

      done = TRUE ;
  }

  // ----------------------------------------------------------------------------------------
  // Multi-frame responses (not to be confused with multiple responses from different ECUs)
  // Support for VIN and DTC CODES - if other requests use multi-frame responses, the code
  // below (unchanged from original obd can lib) may not be sufficient.
  // ----------------------------------------------------------------------------------------

	else
  {
    if(RXDATA(0) == 0x10)
    {         
      
        obd_can_msg.size  = RXDATA(1) ;
        obd_can_msg.sidpr = RXDATA(2) ;

        memcpy(&obd_can_msg.data[0] , &RXDATA(3) , 5) ; // sub the sidpr
      

      if(obd_can_vars.is_active)
      {
        if(!obd_can_vars.is_ext)
        {
            obd_can_req_std_msg_req_next.id |= (CAN_MSG.id & 0x07) ;

            can_v_usr_tx (obd_can_vars.dev_id , &obd_can_req_std_msg_req_next , 10) ;
        }
        else
        {
          obd_can_od_ext_msg_req_next.id = (CAN_MSG.id & 0xFFFF0000) | ( (CAN_MSG.id & 0x00FF) << 8 ) | ( (CAN_MSG.id & 0xFF00) >> 8 );
          can_v_usr_tx (obd_can_vars.dev_id , &obd_can_od_ext_msg_req_next , 10) ;
        }
      }
    }
		else if (RXDATA(0) == 0x21)
    {
      
        SInt16 cur_size = (obd_can_msg.size - 6 - 7) ;
        memcpy(&obd_can_msg.data[0 + 5] , &RXDATA(1) , 7) ; // 
        done = (cur_size <= 0) ;
			
		}
    else if (RXDATA(0) == 0x22)
    {
      
        SInt16 cur_size = (obd_can_msg.size - 6 - 7 - 7) ;
        memcpy(&obd_can_msg.data[5 + 7] , &RXDATA(1) , 7) ; // 
        done = (cur_size <= 0) ;
        request = OBD_CAN_VIN;
      
    }
  }

  // ------------------------------------------
  // Received a full response, now process it
  // ------------------------------------------

  if(done)
  {
    process_mode = obd_can_msg.sidpr & 0x0F;
    if(request != OBD_CAN_VIN)
    {
      if(process_mode == OBD_CAN_MODE3)
      {
        request = OBD_CAN_DTC_CODE ;
      }
      else if(process_mode == OBD_CAN_MODE4)
      {
        request = OBD_CAN_DTC_CLR ;
      }
      else
      {
        Boolean found = FALSE;
        for(i = 0 ; i < sizeof_array(pid_to_req_enum) ; i++)
        {
          if(pid_to_req_enum[i].pid == RXDATA(2))
          {
            request = pid_to_req_enum[i].request ;
            found = TRUE;
            break;
          }
        }
        if(!found)
        {
          return;
        }
      }
    }
    if(obd_can_msg.sidpr == OBD_CAN_NEGATIVE_RESPONSE)
    {      
      process_neg_resp(&obd_can_msg, request) ;
      obd_can_vars.is_connected = TRUE ;
    }
    else
    {
        // We have a response, clear error conditions
        obd_can_vars.disconnected_bus_cnt = 0;
        obd_can_schedule[request].status = OBD_CAN_SUCCESS;
        obd_can_vars.is_connected = TRUE;

        // We normally talk to specific ECUs, so we expect 1 response - however, we also support multiple responses (ex. DTC_CLR is sent to all ECUs).
        // For safety, let's wait until P2 before giving up on more responses.
        if(obd_can_vars.schedule_info.current_req == request)
        {
          timeout_update(obd_can_vars.schedule_info.rx_timeout,OBD_CAN_P2_TIMEOUT) ;
          timeout_start(obd_can_vars.schedule_info.rx_timeout) ;
        }
        if(obd_can_vars.on_demand_info.current_req == request)
        {
          timeout_update(obd_can_vars.on_demand_info.rx_timeout,OBD_CAN_P2_TIMEOUT) ;
          timeout_start(obd_can_vars.on_demand_info.rx_timeout) ;
        }
    
         
            // Remember the ECU that responded for the next request
            if (obd_can_ecu_map[request].ecu == OBD_CAN_REQ_NO_ECU)			
            {
            obd_can_ecu_map[request].ecu = obd_can_ecu_map[request].potential_ecu;

            #if DBG_OBD_CAN == 1
              dbg_tx_text("*** obd_can_rx(): ecu found = ");
              dbg_tx_h32(obd_can_ecu_map[request].ecu); 
              dbg_tx_text("\r\n");
            #endif
            }
        
        
        
        if(process_mode == OBD_CAN_MODE4)
        {
          //do something special for DTC codes
          process_mode4(&obd_can_msg) ;
        }
        else if (ID <= obd_can_ecu_ans_map[request].ecu )			
        {
          obd_can_ecu_ans_map[request].ecu = ID;
    
          // Process the response
          switch(process_mode)
            {
              case OBD_CAN_MODE1 : process_mode1(&obd_can_msg) ; break ;
              case OBD_CAN_MODE2 : process_mode2(&obd_can_msg) ; break ;
              case OBD_CAN_MODE3 : process_mode3(&obd_can_msg) ; break ;
              //case OBD_CAN_MODE4 : process_mode4(&obd_can_msg) ; break ;
              case OBD_CAN_MODE9 : process_mode9(&obd_can_msg) ; break ;
              default : 
              {
                if (obd_can_vars.flg_ign) obd_can_err_add ( OBD_CAN_ERR_UNSUPPORTED_MODE );
                break ;
              }
            }
        }
        else
        {
          #if DBG_OBD_CAN == 1
            dbg_tx_text("*** Higher ID has been ignored. ID kept: ");
            dbg_tx_h32(obd_can_ecu_ans_map[request].ecu);
            dbg_tx_text(" ID ignored: ");
            dbg_tx_h32(ID); 
            dbg_tx_text("\r\n");
          #endif 
        }  
    }
    if(obd_can_vars.schedule_info.current_req == request)
    {
      obd_can_tx_reset_sidrq(&obd_can_vars.schedule_info);
    }
    if(obd_can_vars.on_demand_info.current_req == request)
    {
      obd_can_tx_reset_sidrq(&obd_can_vars.on_demand_info);
    }
      
  }

  #undef CAN_MSG
  #undef CAN_RX_Q_MSG
  #undef RXDATA
  #undef ID
}

//--------------------------------------------------------------------------// 

Boolean obd_can_rx_value_is_valid(void * value_to_validate, UInt8 number_of_bytes)
{
	 int i;
	 int FF_count = 0;
   UInt32* value = value_to_validate;
	 for(i = 0; i < number_of_bytes; i++)
	 {
		 if( (*value >> (8 * i) & 0xFF) == 0xFF)
		 {
			 FF_count++;
		 }
	 }
	 if(FF_count == number_of_bytes)
		 return FALSE;
	 else
		return TRUE;
}

//--------------------------------------------------------------------------// 

// Process responses from MODE1 requests
// - Engine coolant temperature
// - Engine RPM
// - Vehicle speed
// - Intake air temperature
// - Run time since engine start
// - Fuel Level Input
// - Control module voltage
// - Ambient air temperature

static void process_mode1 (Obd_Can_Rx_Msg * obd_can_rx_msg)
{
  #define DATA(z) obd_can_rx_msg->data[z]
  UInt8 i ;

  for(i = 0 ; i < sizeof_array(pid_mode1_table) ; i++)
  {
    if(pid_mode1_table[i].pid == DATA(0))
      {
        pid_mode1_table[i].func(obd_can_rx_msg) ;
      }
  }

  #undef DATA
}

//--------------------------------------------------------------------------//

// Process responses from MODE2 requests
// - Not supported yet

static void process_mode2 (Obd_Can_Rx_Msg * obd_can_rx_msg)
{
}

//--------------------------------------------------------------------------//

// Process responses from MODE3 requests
// - Read DTC codes

static void process_mode3 (Obd_Can_Rx_Msg * obd_can_rx_msg)
{
  // #warning :must be sent last
  #define DATA(z) obd_can_rx_msg->data[z]  

  obd_can_rx_dtc(obd_can_rx_msg) ;	

  #undef DATA
}

//--------------------------------------------------------------------------//

// Process responses from MODE4 requests
// - Clear DTC codes

static void process_mode4 (Obd_Can_Rx_Msg * obd_can_rx_msg)
{
  obd_can_rx_dtc_clr(obd_can_rx_msg) ;
}

//--------------------------------------------------------------------------//

// Process responses from MODE9 requests
// - VIN

static void process_mode9 (Obd_Can_Rx_Msg * obd_can_rx_msg)
{
  #define DATA(z) obd_can_rx_msg->data[z]
  
  UInt8 i ;

  for(i = 0 ; i < sizeof_array(pid_mode9_table) ; i++)
  {
    if(pid_mode9_table[i].pid == DATA(0))
      {
        pid_mode9_table[i].func(obd_can_rx_msg) ;
      }
  }
  #undef DATA
}

//--------------------------------------------------------------------------//

// Process negative responses from MODE1 and MODE4 requests
// - VIN

static void process_neg_resp (Obd_Can_Rx_Msg * obd_can_rx_msg, Obd_Can_Req_Enum request)
{
  if(obd_can_vars.schedule_info.current_req == request)
  {
    process_neg_resp_info(obd_can_rx_msg, &obd_can_vars.schedule_info) ;
  }
  if(obd_can_vars.on_demand_info.current_req == request)
  {
    process_neg_resp_info(obd_can_rx_msg, &obd_can_vars.on_demand_info) ;
  }
}

static void process_neg_resp_info (Obd_Can_Rx_Msg * obd_can_rx_msg, Obd_Can_Req_Info * request_info)
{
  #define DATA(z)       obd_can_rx_msg->data[z]

#if DBG_OBD_CAN == 1
  dbg_tx_text("*** obd_can_rx(): negative response, DATA[2]=");
  dbg_tx_h8(DATA(2));
  dbg_tx_text("\r\n");
#endif
  
  if(TX_STD_SIDRQ == OBD_CAN_MODE4 && DATA(0) == OBD_CAN_MODE4)
  {
      obd_can_rx_dtc_clr_tx_d2d(DTC_CLR_SUCCESS);
  }
  else if (DATA(2) == OBD_CAN_NEGATIVE_PENDING)
  {
      //There can be a negative response subcode $78 that indicates request pending
      //Let's not log an error unless we have expired all retries
      if (request_info->req_retry_cnt >= obd_can_vars.req_retries-1)
      {
          obd_can_err_add(OBD_CAN_ERR_PENDING);
      }
      else return;
  }
  else
  {
      obd_can_err_add(OBD_CAN_ERR_NOT_AVAILABLE);
  }

  obd_can_schedule[request_info->current_req].status = OBD_CAN_NOT_SUPPORTED;
  timeout_stop(request_info->rx_timeout) ;

  #undef DATA
  #undef STD_SIZE
  #undef STD_SIDRQ
  #undef STD_INFTYP
}

