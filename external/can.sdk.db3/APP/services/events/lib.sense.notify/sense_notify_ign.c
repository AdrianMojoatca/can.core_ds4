/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_notify_ign.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sense_notify_private.h"
#include "status.h"
#include "cmd.h"
#include "prog.h"

void gpio_iface_ignition_on(void);
void gpio_iface_ignition_off(void);

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

void sense_notify_ign_on(void) 
{
  if(!sense_notify_ignition_skip_flag)
  {
      if(!key.bit.ign)
      {      
          key.bit.ign = TRUE ; 
      
          cmd_q_insert((Cmd_List)(INPUT_IGNITION|INPUT_ON),COMMAND_INSERT_TIMEOUT);
      
	      rf_icon_set (ICON_IGNITION, FALSE) ;
          
          if(RF_IS_DIS)
            {
              gpio_iface_ignition_on();
            }
          #if DBG_SENSE == 1           
            sense_notify_dbg_print("\nSense Ign On" , NULL , NULL);         
          #endif
      }
  }
}

//--------------------------------------------------------------------------//

void sense_notify_ign_off(void) 
{
  if(!sense_notify_ignition_skip_flag)
  {
    if(key.bit.ign)
      {
        key.bit.ign = FALSE ;
    
        cmd_q_insert(INPUT_IGNITION,COMMAND_INSERT_TIMEOUT);
        
	    rf_icon_clr (ICON_IGNITION, FALSE) ;
        
        if(RF_IS_DIS)
          {
            gpio_iface_ignition_off();
          }
        #if DBG_SENSE == 1
          sense_notify_dbg_print("\nSense Ign Off" , NULL , NULL);
        #endif
      } 
  }
}

