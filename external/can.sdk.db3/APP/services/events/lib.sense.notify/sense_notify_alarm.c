/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_notify_alarm.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sense_notify_private.h"
#include "rf.h"
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

void sense_notify_disarm (void)
{ 
 if(!alarm.bit.disarmed)
    {      
     
          alarm.bit.arming    = 0 ;
          alarm.bit.armed     = 0 ;      
          alarm.bit.triggered = 0 ;  
          alarm.bit.disarmed  = 1 ;
         
    
      cmd_q_insert(INPUT_OEM_DISARM, COMMAND_INSERT_TIMEOUT);

      rf_icon_clr  (ICON_HORN     , FALSE) ;
      rf_icon_clr  (ICON_ARMED    , TRUE)  ;
     

      #if DBG_SENSE == 1              
        sense_notify_dbg_print("\nSense Disarm" , NULL , NULL);      
      #endif

    } 
}

//--------------------------------------------------------------------------//

void sense_notify_arm (void)
{
  if(!alarm.bit.armed)
    {
                                      
          alarm.bit.equipped  = 1 ;
          alarm.bit.arming    = 0 ;
          alarm.bit.armed     = 1 ;      
          alarm.bit.triggered = 0 ;
          alarm.bit.disarmed  = 0 ;
                                        
                                           
      cmd_q_insert(INPUT_OEM_ARM, COMMAND_INSERT_TIMEOUT);

      rf_icon_clr  (ICON_HORN  , FALSE) ;
      rf_icon_set  (ICON_ARMED , TRUE)  ;

      #if DBG_SENSE == 1              
        sense_notify_dbg_print("\nSense Arm" , NULL , NULL);      
      #endif
    } 
}

//--------------------------------------------------------------------------//
void sense_notify_arming (void)
{
  if(!alarm.bit.arming)
    {
                                      
          alarm.bit.equipped  = 1 ;
          alarm.bit.arming    = 1 ;
          alarm.bit.armed     = 0 ;      
          alarm.bit.triggered = 0 ;   
          alarm.bit.disarmed  = 0 ;   
        

      cmd_q_insert(INPUT_OEM_ARMING, COMMAND_INSERT_TIMEOUT);

      #if DBG_SENSE == 1              
        sense_notify_dbg_print("\nSense Arming" , NULL , NULL);      
      #endif
    } 
}

//--------------------------------------------------------------------------//
void sense_notify_trigger (void)
{
  if(!alarm.bit.triggered)
    {
      
          alarm.bit.equipped  = 1 ;
          alarm.bit.triggered = 1 ;
         
                                    
      rf_icon_set  (ICON_HORN , TRUE)  ;

      #if DBG_SENSE == 1              
        sense_notify_dbg_print("\nSense Triggered", NULL, NULL);      
      #endif
    }  
}
