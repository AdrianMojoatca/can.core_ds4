/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_bus_task.c 34443 2015-10-19 14:58:23Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "hobp_bus_private.h"
#include "wake.h"

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
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
void hobp_bus_task (void)
{  
  #if DBG_HOBP_BUS == 1
     hobp_bus_dbg_print("\nHobp Bus Start" , NULL , NULL) ;
  #endif
  
  for(;;)
    {
      Hobp_Bus_Rx_Q_Msg * rx_q_msg = (Hobp_Bus_Rx_Q_Msg *) q_remove (hobp_bus_vars.rx_q_pending , NULL);

      if(rx_q_msg)
        {          
          
          Hobp_Bus_Entry * curr ;

          int data ;

          wake_enter() ;

          data = rx_q_msg->data ;
          
          q_return ((Q_Msg *) rx_q_msg);
          
          for (curr = hobp_bus_vars.list ; curr ; curr = curr->next)
            {
              Hobp_Bus_Rx_Q_Msg * q_msg = (Hobp_Bus_Rx_Q_Msg *) q_remove_c (curr->q_free);

              if (q_msg)
                {   
                    q_msg->data = data ;
                    
                    q_insert (curr->q_pending , (Q_Msg *)q_msg) ;
                }
              else
                {
                  #if DBG_HOBP_BUS == 1
                    hobp_bus_dbg_print("\n!!!!!Fail to insert!!!!!" , NULL , NULL) ;
                  #endif  
                }
            }

          if(!hobp_bus_vars.is_connected)
            {
              hobp_bus_vars.is_connected = !!((data != HOBP_BUS_TIMEOUT) && (data != NULL)) ;

              #if DBG_HOBP_BUS == 1
                if(hobp_bus_vars.is_connected)
                  {
                    hobp_bus_dbg_print("\nHobp Bus Is Connected" , NULL , NULL) ;
                  }
                
              #endif  

            }
          
          

     #if DBG_HOBP_BUS == 1
        switch(data)
          {
            case    HOBP_BUS_TIMEOUT     : hobp_bus_dbg_print("\nTimeout" , NULL , NULL) ; break ;
            default                     : 
              {
                UInt8 u8data = data & 0xFF  ;

                hobp_bus_dbg_print("\n"  , &u8data , 1) ;
                
                break ;
              }
          }           
      #endif

          wake_leave() ;
        }  
    }
}
