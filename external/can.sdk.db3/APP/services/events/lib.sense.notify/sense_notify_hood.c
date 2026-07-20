/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_notify_hood.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sense_notify_private.h"
#include "status.h"
#include "cmd.h"
#include "prog.h"
#include "rf.h"

void gpio_iface_hood_open(void);
void gpio_iface_hood_closed(void);

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
void sense_notify_hood_open(void)
{
  if(!door.bit.hood)
    {
      door.bit.hood = TRUE ;

      cmd_q_insert(INPUT_HOOD_STATUS_OPEN,COMMAND_INSERT_TIMEOUT);
    
      if(RF_IS_DIS)
        {
          gpio_iface_hood_open();
        }
        
      rf_icon_set (ICON_HOOD, FALSE) ;
    
      #if DBG_SENSE == 1
       
        sense_notify_dbg_print("\nHood Open" , NULL , NULL);
      
      #endif

    }
}

//--------------------------------------------------------------------------------

void sense_notify_hood_close(void)
{
  if(door.bit.hood)
    {
      door.bit.hood = FALSE ;

      cmd_q_insert(INPUT_HOOD_STATUS_CLOSE,COMMAND_INSERT_TIMEOUT);
      
      if(RF_IS_DIS)
        {
          gpio_iface_hood_closed();
        }  
      
      rf_icon_clr (ICON_HOOD, FALSE) ; 
      
      #if DBG_SENSE == 1
       
        sense_notify_dbg_print("\nHood Close" , NULL , NULL);
      
      #endif      
  }
}
