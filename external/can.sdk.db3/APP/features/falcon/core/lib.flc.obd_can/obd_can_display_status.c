/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: obd_can_display_status.c 33257 2015-08-19 12:45:49Z martin.bouchard $
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

void obd_can_display_status(void)
{
  typedef enum
  {
    STR ,
    DEC ,
    HEX ,

  }Display_Type ;
  
  
  typedef struct
    {
      const char * text   ;
      
      Obd_Can_Req_Enum num         ;
      UInt8            size        ;
      UInt8          * data        ;
      Display_Type     type        ;
    
    }Obd_Status_Def ;
          
  Obd_Status_Def obd_def[] = 
  {

    {"\r\nVin         = " , OBD_CAN_VIN         , sizeof(vin)                     , &vin[0]                   , STR } ,
    {"\r\nEng_coolant = " , OBD_CAN_ENG_COOLANT , sizeof(temperature.eng_coolant) , &temperature.eng_coolant  , DEC } ,
    {"\r\nRpm         = " , OBD_CAN_RPM         , sizeof(rpm.byte)                , (UInt8 *)&rpm.word        , DEC } ,
    {"\r\nSpeed       = " , OBD_CAN_SPEED       , sizeof(speed.byte)              , &speed.byte               , DEC } ,
    {"\r\nEng run     = " , OBD_CAN_ENG_RUN     , sizeof(engine.run_time)         , (UInt8 *)&engine.run_time , DEC   } ,
    {"\r\nFuel level  = " , OBD_CAN_FUEL_LEVEL  , sizeof(engine.fuel_level)       , &engine.fuel_level        , DEC } ,
    {"\r\nAir amb     = " , OBD_CAN_AIR_AMB     , sizeof(temperature.cabin)       , &temperature.cabin        , DEC } ,  
    {"\r\nIntake temp = " , OBD_CAN_INTAKE_TEMP , sizeof(temperature.in)          , &temperature.in           , DEC } ,  
    {"\r\nBat volt    = " , OBD_CAN_BAT_VOLT    , sizeof(engine.bat_volt)         , &engine.bat_volt          , DEC } ,  
    {"\r\nDtc count   = " , OBD_CAN_DTC_CODE    , sizeof(dtc.count)               , &dtc.count                , DEC } ,  
    {"\r\nDtc code    = " , OBD_CAN_DTC_CODE    , sizeof(dtc.code)                , &dtc.code._8_[0]          , HEX } ,  

  } ;

  UInt8 x ;

  dbg_tx_text("*** status:");

  for(x = 0 ; x < sizeof_array(obd_def) ; x++)
    {
      dbg_tx_text(obd_def[x].text) ;

     // if(obd_can_is_req_supported(obd_def[x].request))
        {         
          if((obd_def[x].type == STR) || (obd_def[x].type == HEX))
            {
              UInt8 i ;

              for(i = 0 ; i < obd_def[x].size ; i++ )
                {
                  obd_def[x].type == STR ? dbg_tx(obd_def[x].data[i]) : dbg_tx_h8(obd_def[x].data[i]) ;
                }
            }
          else if (obd_def[x].type == DEC)
            {
              UInt32 data = obd_def[x].data[obd_def[x].size - 1] ;

              while(--obd_def[x].size)
                {
                  data <<= 8 ;
                  data  |= obd_def[x].data[obd_def[x].size - 1] ;
                }              
              
              if(data <  10)
                {
                  dbg_tx_u(data , 1) ;
                }
              else if(data <  100)
                {
                  dbg_tx_u(data , 2) ;
                }
              else if(data < 1000)
                {
                  dbg_tx_u(data , 3) ;
                }
              else if(data < 10000)
                {
                  dbg_tx_u(data , 4) ;
                }
            }          
        }
     // else
     //   {
     //     dbg_tx_text("Not Support") ;
     //   }
    }

  dbg_tx_crlf() ;
}

#endif

