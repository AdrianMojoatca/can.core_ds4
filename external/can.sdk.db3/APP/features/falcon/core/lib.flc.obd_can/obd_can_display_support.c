/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: obd_can_display_support.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

#if DBG_OBD_CAN == 1

void obd_can_display_support(void)
{
  typedef struct
    {
      char * text ;
      Obd_Can_Req_Enum num ;

    }Obd_Support_Def ;

  const Obd_Support_Def obd_def[] =
  {
    {"\r\nVin         " , OBD_CAN_VIN         } ,
    {"\r\nEng_coolant " , OBD_CAN_ENG_COOLANT } ,
    {"\r\nRpm         " , OBD_CAN_RPM         } ,
    {"\r\nSpeed       " , OBD_CAN_SPEED       } ,
    {"\r\nEng run     " , OBD_CAN_ENG_RUN     } ,
    {"\r\nFuel level  " , OBD_CAN_FUEL_LEVEL  } ,
    {"\r\nAir amb     " , OBD_CAN_AIR_AMB     } ,  
    {"\r\nIntake temp " , OBD_CAN_INTAKE_TEMP } ,  
    {"\r\nBat volt    " , OBD_CAN_BAT_VOLT    } ,  
    {"\r\nDtc code    " , OBD_CAN_DTC_CODE    } ,  
    {"\r\nDtc clear   " , OBD_CAN_DTC_CLR     } ,  
  } ;

  UInt8 i ;

  for(i = 0 ; i < sizeof_array(obd_def) ; i++)
    {
      dbg_tx_text(obd_def[i].text) ;

      dbg_tx_text(obd_can_is_req_enabled(obd_def[i].num)? "Supported" : "NOT Supported" ) ;
    }
  dbg_tx_crlf() ;
}

#endif

