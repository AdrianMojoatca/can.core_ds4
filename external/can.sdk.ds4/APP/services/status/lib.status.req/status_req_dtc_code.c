/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: status_req_dtc_code.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "status_req_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define	DTC_MSG_SIZE 2 	 			// DTC_MSG_SIZE should be defined elsewhere
#define D2D_MULTI_BYTE_TIMEOUT	5000

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void status_req_dtc_code (void)
{	
  #define LSB(x) (x>>8)
  	
	UInt8 data[4]  = {SET_DTC_CODE,0xFF,0xFF,0xFF};

	SInt16 dtc_index = d2d_rx (D2D_MULTI_BYTE_TIMEOUT) ;

	if(dtc_index < 0)
    {
	  	status_req_err_add(STATUS_REQ_D2D_TIMEOUT) ;
	  return ;
	}

  #if STATUS_DTC_ENABLE == 1
    if(obd_can_is_active())
    {
      	if(dtc.count < dtc_index && !dtc_index)
    	{
    		data[1] = dtc.count ;
        	data[2] = 0 ;
    		data[3] = 0 ;
    	}  
       	else
    	{
        	UInt16 dtc_code = status_get_dtc((Status_Dtc)(dtc_index-1)) ;
    		data[1]	= (dtc_index << 4) | dtc.count ;
    		data[2] = dtc_code ;	  // DTC Stored as DATA[0] = LSB ,,, DATA[1] = MSB
        	data[3] = LSB(dtc_code) ;
    	}
    }
  #endif

	d2d_tx_frame (&data[0] , sizeof_array(data)) ;  

  #undef LSB
}

