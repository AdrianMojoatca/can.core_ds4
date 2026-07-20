/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: obd_can_rx_dtc.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "obd_can_private.h"
#include <string.h>
#include "d2d.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define MAX_DTC_VALUE  15	 //limited by status variable size and define by D2D protocol

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void dtc_update_check (void) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean obd_can_rx_dtc (Obd_Can_Rx_Msg * obd_can_rx_msg)
{
  #define DATA(z) obd_can_rx_msg->data[z]

  #if STATUS_DTC_ENABLE == 1
	dtc.count = DATA(0);
	
	if(dtc.count > MAX_DTC_VALUE)
	{
	dtc.count = MAX_DTC_VALUE ;
	}
	
	memset(&dtc.code._8_[0], 0 ,  sizeof_array(dtc.code._8_) ) ;
	
	memcpy(&dtc.code._8_[0] , &DATA(1) , dtc.count * 2) ;
	
	dtc_update_check () ;

  #endif

#if DBG_OBD_CAN == 1
  dbg_tx_text("*** obd_can_rx_dtc count = ");
  dbg_tx_h8(dtc.count);
  dbg_tx_text("\r\n");
#endif

  return TRUE ;

  #undef DATA
}
//--------------------------------------------------------------------------//

Boolean obd_can_rx_dtc_clr (Obd_Can_Rx_Msg * obd_can_rx_msg)
{
  
#if DBG_OBD_CAN == 1
  dbg_tx_text("*** obd_can_rx_dtc_clr()\r\n");
#endif

    obd_can_rx_dtc_clr_tx_d2d(DTC_CLR_SUCCESS);

    return FALSE ;
}
//--------------------------------------------------------------------------//

void obd_can_rx_dtc_clr_tx_d2d  (UInt8 status) //DTC_CLR_SUCCESS or DTC_CLR_FAIL
{
  UInt8 data[2] = {SET_DTC_CLR,0};
    
  #if DBG_OBD_CAN == 1
    dbg_tx_text("*** obd_can_rx_dtc_clr_tx_d2d(");
    dbg_tx_h4(status);
    dbg_tx_text(")\r\n");
  #endif

  
    data[1] = status;
  
  d2d_tx_frame (&data[0] , sizeof_array(data)) ;

}


//--------------------------------------------------------------------------//

void dtc_update_check (void)
{
static Boolean flg_dtc_set  = FALSE ;
static UInt8 prev_dtc_code [OBD_CAN_MAX_RESPONSE_DATA] ;

  #if STATUS_DTC_ENABLE == 1
	  if(dtc.count)
		  {
		    flg_dtc_set = TRUE ;
	
			if(&dtc.code._8_[(dtc.count * 2)])  //if last entry != 0 then all code have been received
			  {
	            if((memcmp(&dtc.code._8_[0],&prev_dtc_code[0], dtc.count * 2)) !=0)  //not the same
	              {
	             	cmd_q_insert ((Cmd_List)(INPUT_DTC_ALERT | INPUT_ON), 500) ;				
	              }
	
	 		    memcpy(&prev_dtc_code[0],&dtc.code._8_[0],dtc.count * 2) ;		   	 
	          }
		  }
	  else if(flg_dtc_set && !dtc.count)
	    {
		   flg_dtc_set  = FALSE ;
		   cmd_q_insert ((Cmd_List)(INPUT_DTC_ALERT), 500) ;	   
		}
  #endif
}
