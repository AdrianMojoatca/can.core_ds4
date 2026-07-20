/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_notify_door.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sense_notify_private.h"
#include "status.h"
#include "cmd.h"
#include "prog.h"
#include "config_gpio.h"
#include "rf.h"


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

void sense_notify_door_update (void)
{
  if(!sense_notify_door_skip_flag)
  {
    static UInt8 temp     = 0 ;
           UInt8 cur_door     ;
    
    cur_door = door.byte & 0x0F; 
    
    if(cur_door != (temp & 0x0F))
      {
         if(!(temp & 0x0F))
           {
                cmd_q_insert(INPUT_ALL_DOOR_STATUS_OPEN,COMMAND_INSERT_TIMEOUT);
    
                #ifdef DOOR_SENSE_SET
                if(RF_IS_DIS)
                  {
                    DOOR_SENSE_SET   ;
                  }
                #else
                #warning DOOR_SENSE_SET not defined
                #endif
                     
                rf_icon_set (ICON_DOOR, FALSE) ;
    
                #if DBG_SENSE == 1
                 
                  sense_notify_dbg_print("\nSense Door Open" , NULL , NULL);
                
                #endif
    
          }
        else if(!cur_door)
          {       
            cmd_q_insert(INPUT_ALL_DOOR_STATUS_CLOSE,COMMAND_INSERT_TIMEOUT);
    
            #ifdef DOOR_SENSE_CLR
            if(RF_IS_DIS)
              {
                DOOR_SENSE_CLR ;
              }
            #else
            #warning DOOR_SENSE_CLR not defined
            #endif
    
            rf_icon_clr (ICON_DOOR, FALSE) ;
    
            #if DBG_SENSE == 1
             
              sense_notify_dbg_print("\nSense Door Close" , NULL , NULL);
            
            #endif
          }
      }
    temp = (door.byte & 0x0F) ;
  }
  else
  {
    #if DBG_SENSE == 1
     
      sense_notify_dbg_print("\nSense Door Skipped" , NULL , NULL);
    
    #endif  
  }
}

//--------------------------------------------------------------------------//
Boolean sense_notify_set_front_driver( Boolean state )
{
  if(!sense_notify_door_skip_flag)
  {
    Boolean do_update = !!(door.bit.front_driver != state);

    door.bit.front_driver = state ;
    
    return do_update ;
  }
  return FALSE;
}

//--------------------------------------------------------------------------//
Boolean sense_notify_set_front_pass( Boolean state )
{
    if(!sense_notify_door_skip_flag)
    {
      Boolean do_update = !!(door.bit.front_pass != state);
      
      door.bit.front_pass = state ;
   
      return do_update ;
    }
    return FALSE;
}

//--------------------------------------------------------------------------//
Boolean sense_notify_set_rear_driver( Boolean state )
{
    if(!sense_notify_door_skip_flag)
    {
      Boolean do_update = !!(door.bit.rear_driver != state);

      door.bit.rear_driver = state ;

      return do_update ;
    }
    return FALSE;
}

//--------------------------------------------------------------------------//
Boolean sense_notify_set_rear_pass( Boolean state )
{
    if(!sense_notify_door_skip_flag)
    {
      Boolean do_update = !!(door.bit.rear_pass != state);

      door.bit.rear_pass = state ;

      return do_update ;
    }
    return FALSE;
}

//--------------------------------------------------------------------------//
void sense_notify_set_trunk( Boolean state )
{  
  state? sense_notify_trunk_open() : sense_notify_trunk_close() ;

}

//--------------------------------------------------------------------------//
void sense_notify_set_hood( Boolean state )
{
  state ? sense_notify_hood_open() : sense_notify_hood_close() ;
}
