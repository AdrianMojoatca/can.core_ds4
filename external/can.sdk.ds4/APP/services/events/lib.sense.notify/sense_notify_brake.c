/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_notify_brake.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sense_notify_private.h"
#include "status.h"
#include "cmd.h"
#include "prog.h"
#include "config_gpio.h"

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

void sense_notify_brake_on (void) 
{
  if(!sense_notify_brake_skip_flag)
  {
    if(!brake.bit.brake)
    {
      brake.bit.brake = TRUE ;
  
      cmd_q_insert(INPUT_BRAKE_STATUS_PRESS, COMMAND_INSERT_TIMEOUT);
      
      #ifdef BRAKE_SENSE_SET
        if(RF_IS_DIS)
        {
          BRAKE_SENSE_SET ;
        }
      #else
      #warning BRAKE_SENSE_SET not defined
      #endif

      #if DBG_SENSE == 1
          sense_notify_dbg_print("\nSense Brake On" , NULL , NULL);
      #endif
    }
  }
  else if(!brake.bit.brake)
  {
    #if DBG_SENSE == 1
        sense_notify_dbg_print("\nSense Brake On Skipped" , NULL , NULL);
    #endif  
  }
}

//--------------------------------------------------------------------------------

void sense_notify_brake_off (void) 
{
  if(!sense_notify_brake_skip_flag)
  {
    if(brake.bit.brake) 
    {
        brake.bit.brake = FALSE ;
        
        cmd_q_insert(INPUT_BRAKE_STATUS_RELEASE, COMMAND_INSERT_TIMEOUT);
                    
        #ifdef BRAKE_SENSE_CLR	  
        if(RF_IS_DIS)
        {
          BRAKE_SENSE_CLR ;
        }
        #else
        #warning BRAKE_SENSE_CLR not defined
        #endif

        #if DBG_SENSE == 1
            sense_notify_dbg_print("\nSense Brake Off" , NULL , NULL);
        #endif
    }
  }
  else if(brake.bit.brake) 
  {
    #if DBG_SENSE == 1
        sense_notify_dbg_print("\nSense Brake Off Skipped" , NULL , NULL);
    #endif      
  }
}

