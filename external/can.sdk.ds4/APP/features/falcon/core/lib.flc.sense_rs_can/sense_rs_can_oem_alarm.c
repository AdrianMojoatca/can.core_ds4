/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_can_oem_alarm.c 1106 2010-04-27 13:30:39Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_can_private.h"
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


//--------------------------------------------------------------------------//
void sense_rs_can_oem_alarm_disarm (void)
{ 
 if(!alarm.bit.disarmed)
    {      
      ATOMIC
        (
          alarm.bit.arming    = 0 ;
          alarm.bit.armed     = 0 ;      
          alarm.bit.triggered = 0 ;  
          alarm.bit.disarmed  = 1 ;
        ) 
    
      cmd_q_insert(INPUT_OEM_DISARM, COMMAND_INSERT_TIMEOUT);

      rf_icon_clr  (ICON_HORN     , FALSE) ;
      rf_icon_clr  (ICON_ARMED    , TRUE)  ;
     

      #if DBG_SENSE == 1
              
        TRACE_SENSE_RS_CAN("\r\nDisarm");
      
      #endif

    } 
}

//--------------------------------------------------------------------------//

void sense_rs_can_oem_alarm_arm (void)
{
  if(!alarm.bit.armed)
    {
      ATOMIC                             
        (                                
          alarm.bit.equipped  = 1 ;
          alarm.bit.arming    = 0 ;
          alarm.bit.armed     = 1 ;      
          alarm.bit.triggered = 0 ;
          alarm.bit.disarmed  = 0 ;
        )                                
                                           
      cmd_q_insert(INPUT_OEM_ARM, COMMAND_INSERT_TIMEOUT);

      rf_icon_clr  (ICON_HORN  , FALSE) ;
      rf_icon_set  (ICON_ARMED , TRUE)  ;

      #if DBG_SENSE == 1
              
        TRACE_SENSE_RS_CAN("\r\nArm");
      
      #endif
    } 
}

//--------------------------------------------------------------------------//
void sense_rs_can_oem_alarm_arming (void)
{
  if(!alarm.bit.arming)
    {
      ATOMIC                             
        (                                
          alarm.bit.equipped  = 1 ;
          alarm.bit.arming    = 1 ;
          alarm.bit.armed     = 0 ;      
          alarm.bit.triggered = 0 ;   
          alarm.bit.disarmed  = 0 ;   
        )

      #if DBG_SENSE == 1
              
        TRACE_SENSE_RS_CAN("\r\nArming");
      
      #endif
    } 
}

//--------------------------------------------------------------------------//
void sense_rs_can_oem_alarm_trigger (void)
{
  if(!alarm.bit.triggered)
    {
      ATOMIC
        (
          alarm.bit.equipped  = 1 ;
          alarm.bit.triggered = 1 ;
        )  
                                    
      rf_icon_set  (ICON_HORN , TRUE)  ;

      #if DBG_SENSE == 1
              
        TRACE_SENSE_RS_CAN("\r\nTriggered");
      
      #endif
    }  
}

//--------------------------------------------------------------------------//
