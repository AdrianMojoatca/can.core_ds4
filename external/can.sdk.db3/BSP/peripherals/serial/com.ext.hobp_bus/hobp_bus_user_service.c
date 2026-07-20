/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: hobp_bus_user_service.c 34443 2015-10-19 14:58:23Z martin.bouchard $
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
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void hobp_bus_user_service (void)
{ 
  if(!!(hobp_bus_vars.flag & (HOBP_BUS_RX_DATA)))
    {      
      hobp_bus_vars.flag &= ~(HOBP_BUS_RX_DATA)  ;

      wake_enter      () ;

      /*process_msg:*/
        {
            Hobp_Bus_Rx_Q_Msg * q_msg = (Hobp_Bus_Rx_Q_Msg *) q_remove_c (hobp_bus_vars.rx_q_free);
                
            if (q_msg)
            {   
                q_msg->data = hobp_bus_vars.fiq_rx_data ;

                hobp_bus_vars.fiq_rx_data = NULL ;
                
                q_insert (hobp_bus_vars.rx_q_pending , (Q_Msg *)q_msg) ;
            }
        }


      wake_leave() ;
    }

   if( !tmr_is_ena (hobp_bus_vars.tmr_tx)) // if tmr is enable, we are already trying to send a msg
     {
       if(!hobp_bus_vars.fiq_tx_msg.size) // if tx complete, load new message
         {
           if((hobp_bus_vars.flag & (HOBP_BUS_TX_BYTE_DONE)))
             {
               Hobp_Bus_Tx_Q_Msg * tx_q_msg = (Hobp_Bus_Tx_Q_Msg *) q_remove_c (hobp_bus_vars.tx_q_pending);
               
               hobp_bus_vars.flag &= ~(HOBP_BUS_TX_BYTE_DONE)  ;

               if(tx_q_msg)
                 {
                   hobp_bus_rx_edge_dis() ; 
                   
                   hobp_bus_vars.fiq_tx_msg = tx_q_msg->msg ;

                   tmr_start (hobp_bus_vars.tmr_tx, (HOBP_BUS_TX_SPEED), FALSE) ;
                 }
               else
                 {
                   hobp_bus_rx_edge_ena() ;
                 }
             }
         }
     }
}


