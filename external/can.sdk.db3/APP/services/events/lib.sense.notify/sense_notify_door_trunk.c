/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_notify_door_trunk.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sense_notify_private.h"
#include "status.h"
#include "cmd.h"
#include "prog.h"
#include "rf.h"

void gpio_iface_doors_open (void);
void gpio_iface_doors_closed (void);


/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define MASK_DOOR_TRUNK  0x10
#define MASK_OTHER_DOOR  0x0F
#define MASK_ALL_DOOR    (MASK_OTHER_DOOR | MASK_DOOR_TRUNK)

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void sense_notify_door_trunk_update (void)
{
  static UInt8 temp     = 0 ;
         UInt8 cur_door     ;


  cur_door = door.byte & MASK_OTHER_DOOR; 

  if((temp & MASK_OTHER_DOOR) != cur_door)
    {
      if(!(temp & MASK_OTHER_DOOR))
        {
            cmd_q_insert(INPUT_ALL_DOOR_STATUS_OPEN,COMMAND_INSERT_TIMEOUT);
          
          if(cur_door & (MASK_OTHER_DOOR))
            {
              rf_icon_set (ICON_DOOR, FALSE) ;
            }
    
          #if DBG_SENSE == 1
            sense_notify_dbg_print("\nSense Door Set" , NULL , NULL);
          #endif
        }
      else if(!cur_door)
        {
            cmd_q_insert(INPUT_ALL_DOOR_STATUS_CLOSE,COMMAND_INSERT_TIMEOUT);
    
          if(!(cur_door & MASK_OTHER_DOOR))
            {
              rf_icon_clr (ICON_DOOR, FALSE) ;
            }
    
          #if DBG_SENSE == 1
            sense_notify_dbg_print("\nSense Door Clr" , NULL , NULL);
          #endif

        }
    }
    
  cur_door = door.byte & MASK_ALL_DOOR; 
    
  if((temp & MASK_ALL_DOOR) != cur_door)
    {
      if(!(temp & MASK_ALL_DOOR))  
        {
          #if DBG_SENSE == 1
            sense_notify_dbg_print("\nDOOR_OUTPUT_SET" , NULL , NULL);
          #endif 
            
          if(RF_IS_DIS)
            {
              gpio_iface_doors_open();
            }
        }   
      else if(!cur_door)
        {  
          #if DBG_SENSE == 1
            sense_notify_dbg_print("\nDOOR_OUTPUT_CLR" , NULL , NULL);
          #endif 
            
          if(RF_IS_DIS)
            {
              gpio_iface_doors_closed();
            }
        }
    }
 
  temp = door.byte & MASK_ALL_DOOR ;

}
