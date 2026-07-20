/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: obd_can_init_can.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

static void init_std (void) ; 
static void init_ext (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void obd_can_init_can (Can_Dev_Id can_dev_id, Boolean is_std , Boolean do_init_can)
{  
  obd_can_vars.dev_id  = can_dev_id ; 

  // The CAN bus may be already initialized because it is shared - only initialize it if requested  
  if(do_init_can)
    {
      static Can_V_Init_Options options ;
#if OBD_CAN_BAUDRATE != 500 && OBD_CAN_BAUDRATE != 250
  #error OBD_CAN_BAUDRATE must be defined at either 250 or 500
#endif
      
#if OBD_CAN_BAUDRATE == 500    
      static const Can_Bit_Def can_bit_def_500_000   = { CCLK_DIV4 , 2 , 12 , 5 , 1 } ;
#else
      static const Can_Bit_Def can_bit_def_250_000   = { CCLK_DIV4 , 4 , 12 , 5 , 1 } ;
#endif      
       
      options.can_dev_id  = obd_can_vars.dev_id  ;
    
#if OBD_CAN_BAUDRATE == 500
      options.can_bit_def = &can_bit_def_500_000 ;	 
#else
      options.can_bit_def = &can_bit_def_250_000 ;
#endif
    
      options.rx_size     = 10  ;
      options.tx_size     = 5   ;
    
      can_v_usr_init_opt (&options, FALSE) ;
    }

  // Initialize CAN messages and filters depending on standard (11-bits) of extended mode (29-bits) 
  is_std ? init_std() : init_ext() ;

  // Create a virtual CAN received 
  obd_can_vars.can_v_usr.can_v_rx = can_v_rx_create (&obd_can_vars.can_v_usr.can_lut_sw , 2) ;
}


//--------------------------------------------------------------------------//

static void init_std (void)
{
  UInt16 id ;
    
  obd_can_vars.is_ext = FALSE ;	

  // Basic CAN tx message 
  obd_can_vars.schedule_info.can_tx_msg = &obd_can_req_std_msg ;
  obd_can_vars.on_demand_info.can_tx_msg  = &obd_can_od_std_msg ;
  
  // Create and initialize the CAN lookup table
  obd_can_vars.can_v_usr.can_lut_sw_table = obd_can_std_lut_table ;
  can_lut_sw_create (&obd_can_vars.can_v_usr.can_lut_sw , obd_can_vars.can_v_usr.can_lut_sw_table , OBD_CAN_STD_ID_COUNT) ;

  // Requests to the ECUs - if we detect this, it means a scan tool is connected - we must enter silent mode
  can_lut_sw_insert_id_std  (&obd_can_vars.can_v_usr.can_lut_sw , obd_can_vars.dev_id , OBD_CAN_REQ_ALL_ECUS , obd_can_scan_tool_detected ) ;
  for(id = OBD_CAN_REQ_FIRST_ECU ; id <= OBD_CAN_REQ_LAST_ECU ; id++)
    {
      can_lut_sw_insert_id_std  (&obd_can_vars.can_v_usr.can_lut_sw , obd_can_vars.dev_id , id , obd_can_scan_tool_detected ) ;
    }				 

  // Responses from the ECUs
  for(id = OBD_CAN_RESP_FIRST_ECU ; id <= OBD_CAN_RESP_LAST_ECU ; id++)
    {
  	  can_lut_sw_insert_id_std  (&obd_can_vars.can_v_usr.can_lut_sw , obd_can_vars.dev_id , id , obd_can_rx ) ;
    }

}

//--------------------------------------------------------------------------//

static void init_ext (void)
{
  UInt32 id ;

  obd_can_vars.is_ext = TRUE ;

  // Extended CAN tx message 
  obd_can_vars.schedule_info.can_tx_msg = &obd_can_req_ext_msg ;
  obd_can_vars.on_demand_info.can_tx_msg  = &obd_can_od_ext_msg ;
  // Create and initialize the CAN lookup table
  obd_can_vars.can_v_usr.can_lut_sw_table = obd_can_ext_lut_table ;
  can_lut_sw_create (& obd_can_vars.can_v_usr.can_lut_sw , obd_can_vars.can_v_usr.can_lut_sw_table , OBD_CAN_EXT_ID_COUNT) ;

  // Requests to the ECUs - if we detect this, it means a scan tool is connected - we must enter silent mode
  can_lut_sw_insert_id_ext  (&obd_can_vars.can_v_usr.can_lut_sw , obd_can_vars.dev_id , OBD_CAN_REQ_ALL_ECUS , obd_can_scan_tool_detected ) ;
  for(id = OBD_CAN_REQ_FIRST_ECU ; id <= OBD_CAN_REQ_LAST_ECU ; id+=0x0100)
    {
      can_lut_sw_insert_id_ext  (&obd_can_vars.can_v_usr.can_lut_sw , obd_can_vars.dev_id , id , obd_can_scan_tool_detected ) ;
    }				 

  // Responses from the ECUs
  for(id = OBD_CAN_RESP_FIRST_ECU ; id <= OBD_CAN_RESP_LAST_ECU ; id++)
    {
  	  can_lut_sw_insert_id_ext  (&obd_can_vars.can_v_usr.can_lut_sw , obd_can_vars.dev_id , id , obd_can_rx ) ;
    }

}
